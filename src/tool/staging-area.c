/******************************************************************************
 * Copyright (c) 2017, NXP Semiconductors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include "internal.h"
/* From libsja1105 */
#include <lib/include/static-config.h>
#include <lib/include/staging-area.h>
#include <lib/include/gtable.h>
#include <lib/include/spi.h>
#include <lib/include/ptp.h>
#include <lib/include/status.h>
#include <lib/include/reset.h>
#include <lib/include/clock.h>
#include <common.h>

static int reliable_write(int fd, char *buf, int len)
{
	int bytes = 0;
	int rc;

	do {
		rc = write(fd, buf + bytes, len - bytes);
		if (rc < 0) {
			loge("could not write to file");
			goto out;
		}
		bytes += rc;
	} while (rc);
out:
	return rc;
}

static int reliable_read(int fd, char *buf, int len)
{
	int bytes = 0;
	int rc;

	do {
		rc = read(fd, buf + bytes, len - bytes);
		if (rc < 0) {
			loge("read failed");
			goto out;
		}
		bytes += rc;
	} while (rc);
out:
	return rc;
}

int
staging_area_hexdump(const char *staging_area_file)
{
	struct stat stat;
	unsigned int len;
	char *buf;
	char *ptp_buf;
	int fd;
	int rc;

	fd = open(staging_area_file, O_RDONLY);
	if (fd < 0) {
		loge("Staging area %s does not exist!", staging_area_file);
		rc = fd;
		goto out_1;
	}
	rc = fstat(fd, &stat);
	if (rc < 0) {
		loge("could not read file size");
		goto out_2;
	}
	len = stat.st_size;
	buf = (char*) malloc(len * sizeof(char));
	if (!buf) {
		loge("malloc failed");
		goto out_2;
	}
	rc = reliable_read(fd, buf, len);
	if (rc < 0) {
		goto out_3;
	}
	printf("Static configuration:\n");
	/* Returns number of bytes dumped */
	rc = sja1105_static_config_hexdump(buf);
	if (rc < 0) {
		loge("error while interpreting config");
		goto out_3;
	}
	logi("static config: dumped %d bytes", rc);
	/* There is a final table header which is not being dumped */
	ptp_buf = buf + rc + SIZE_TABLE_HEADER;
	printf("PTP configuration:\n");
	gtable_hexdump(ptp_buf, SIZE_PTP_CONFIG);
	rc = 0;
out_3:
	free(buf);
out_2:
	close(fd);
out_1:
	return rc;
}

int
staging_area_load(const char *staging_area_file,
                  struct sja1105_staging_area *staging_area)
{
	struct sja1105_static_config *static_config;
	struct sja1105_ptp_config    *ptp_config;
	struct stat stat;
	unsigned int staging_area_len;
	unsigned int static_config_len;
	char *buf;
	int fd;
	int rc;

	static_config = &staging_area->static_config;
	ptp_config    = &staging_area->ptp_config;

	fd = open(staging_area_file, O_RDONLY);
	if (fd < 0) {
		loge("Staging area %s does not exist!", staging_area_file);
		rc = fd;
		goto out_1;
	}
	rc = fstat(fd, &stat);
	if (rc < 0) {
		loge("could not read file size");
		goto out_2;
	}
	staging_area_len = stat.st_size;
	buf = (char*) malloc(staging_area_len * sizeof(char));
	if (!buf) {
		loge("malloc failed");
		goto out_2;
	}
	rc = reliable_read(fd, buf, staging_area_len);
	if (rc < 0) {
		goto out_3;
	}
	/* Static config */
	rc = sja1105_static_config_unpack(buf, static_config);
	if (rc < 0) {
		loge("error while interpreting config");
		goto out_3;
	}
	static_config_len = sja1105_static_config_get_length(static_config);
	/* PTP config */
	if (staging_area_len - static_config_len < SIZE_PTP_CONFIG) {
		loge("PTP config not present in staging area!");
		goto out_3;
	}
	sja1105_ptp_config_unpack(buf + static_config_len, ptp_config);
	rc = 0;
out_3:
	free(buf);
out_2:
	close(fd);
out_1:
	return rc;
}

int
staging_area_save(const char *staging_area_file,
                  struct sja1105_staging_area *staging_area)
{
	struct sja1105_static_config *static_config;
	struct sja1105_ptp_config    *ptp_config;
	int   rc = 0;
	char *buf;
	int   static_config_len;
	int   staging_area_len;
	int   fd;

	static_config     = &staging_area->static_config;
	ptp_config        = &staging_area->ptp_config;
	static_config_len = sja1105_static_config_get_length(static_config);
	staging_area_len  = static_config_len + SIZE_PTP_CONFIG;

	buf = (char*) malloc(staging_area_len * sizeof(char));
	if (!buf) {
		loge("malloc failed");
		goto out_1;
	}
	logv("saving static config... %d bytes", static_config_len);
	sja1105_static_config_pack(buf, static_config);
	logv("saving ptp config... %d bytes", SIZE_PTP_CONFIG);
	sja1105_ptp_config_pack(buf + static_config_len, ptp_config);

	logv("total staging area size: %d bytes", staging_area_len);
	fd = open(staging_area_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		loge("could not open %s for write", staging_area_file);
		rc = fd;
		goto out_2;
	}

	rc = reliable_write(fd, buf, staging_area_len);
	if (rc < 0) {
		goto out_2;
	}
	logv("done");

	close(fd);
out_2:
	free(buf);
out_1:
	return rc;
}

static int
static_config_upload(struct sja1105_spi_setup *spi_setup,
                     struct sja1105_static_config *config)
{
	struct   sja1105_table_header final_header;
	char    *final_header_ptr;
	/* XXX: Maybe 100 is not the best number of chunks here */
	struct sja1105_spi_chunk chunks[100];
	int    chunk_count;
	char   tx_buf[SIZE_SPI_MSG_HEADER + SIZE_SPI_MSG_MAXLEN];
	char   rx_buf[SIZE_SPI_MSG_MAXLEN + SIZE_SPI_MSG_HEADER];
	char  *config_buf;
	int    config_buf_len;
	int    crc_len;
	int    rc;
	int    i;

	config_buf_len = sja1105_static_config_get_length(config) +
	                 SIZE_SJA1105_DEVICE_ID;
	config_buf = (char*) malloc(config_buf_len * sizeof(char));
	if (!config_buf) {
		loge("malloc failed");
		rc = -errno;
		goto out;
	}
	/* Write Device ID to first 4 bytes of config_buf */
	rc = gtable_pack(config_buf, &spi_setup->device_id, 31, 0,
	                 SIZE_SJA1105_DEVICE_ID);
	if (rc < 0) {
		loge("failed to write device id to buffer");
		goto out_free;
	}
	/* Write config tables to config_buf */
	sja1105_static_config_pack(config_buf + SIZE_SJA1105_DEVICE_ID,
	                           config);
	/* Recalculate CRC of the last header */
	/* Don't include the CRC field itself */
	crc_len = config_buf_len - 4;
	/* Read the whole table header */
	final_header_ptr = config_buf + config_buf_len - SIZE_TABLE_HEADER;
	sja1105_table_header_unpack(final_header_ptr, &final_header);
	/* Modify */
	final_header.crc = ether_crc32_le(config_buf, crc_len);
	/* Rewrite */
	sja1105_table_header_pack(final_header_ptr, &final_header);

	/* Fill chunks array with chunk_count pointers */
	spi_get_chunks(config_buf, config_buf_len, chunks, &chunk_count);

	for (i = 0; i < chunk_count; i++) {
		/* Combine chunks[i].msg and chunks[i].buf into tx_buf */
		spi_message_aggregate(tx_buf, &chunks[i].msg, chunks[i].buf,
		                      chunks[i].size);
		/* Send it out */
		rc = sja1105_spi_transfer(spi_setup, tx_buf, rx_buf,
		                          SIZE_SPI_MSG_HEADER + chunks[i].size);
		if (rc < 0) {
			loge("sja1105_spi_transfer failed");
			goto out_free;
		}
	}
out_free:
	free(config_buf);
out:
	return rc;
}

int static_config_flush(struct sja1105_spi_setup *spi_setup,
                        struct sja1105_static_config *config)
{
	struct sja1105_reset_ctrl     reset = {.rst_ctrl = RGU_COLD};
	struct sja1105_general_status status;
	uint64_t expected_device_id = spi_setup->device_id;
	int rc;

	/* Check that we are talking with the right device over SPI */
	rc = sja1105_general_status_get(spi_setup, &status);
	if (rc < 0) {
		goto out;
	}
	if (spi_setup->dry_run == 0) {
		/* These checks simply cannot pass (and do not even
		 * make sense to have) if we are in dry run mode */
		if (status.device_id != expected_device_id) {
			loge("read device id %" PRIx64 ", expected %" PRIx64,
			     status.device_id, expected_device_id);
			goto out;
		}
	}
	rc = sja1105_static_config_check_valid(config);
	if (rc < 0) {
		loge("cannot upload config, because it is not valid");
		goto out;
	}
	rc = sja1105_reset(spi_setup, &reset);
	if (rc < 0) {
		loge("sja1105_reset failed");
		goto out;
	}
	rc = static_config_upload(spi_setup, config);
	if (rc < 0) {
		loge("static_config_upload failed");
		goto out;
	}
	rc = sja1105_clocking_setup(spi_setup, &config->xmii_params[0],
	                           &config->mac_config[0]);
	if (rc < 0) {
		loge("sja1105_clocking_setup failed");
		goto out;
	}
	/* Check that SJA1105 responded well to the config upload */
	if (spi_setup->dry_run == 0) {
		/* These checks simply cannot pass (and do not even
		 * make sense to have) if we are in dry run mode */
		rc = sja1105_general_status_get(spi_setup, &status);
		if (rc < 0) {
			goto out;
		}
		if (status.ids == 1) {
			loge("not responding to configured device id");
			goto out;
		}
		if (status.crcchkl == 1) {
			loge("local crc failed while uploading config");
			goto out;
		}
		if (status.crcchkg == 1) {
			loge("global crc failed while uploading config");
			goto out;
		}
		if (status.configs == 0) {
			loge("configuration is invalid");
		}
	}
out:
	return rc;
}

int
staging_area_flush(struct sja1105_spi_setup *spi_setup,
                   struct sja1105_staging_area *staging_area)
{
	int rc;

	rc = static_config_flush(spi_setup, &staging_area->static_config);
	if (rc < 0) {
		loge("static_config_flush failed");
		goto out;
	}
	rc = sja1105_ptp_configure(spi_setup, &staging_area->ptp_config);
	if (rc < 0) {
		loge("ptp_init failed");
		goto out;
	}
out:
	return rc;
}

