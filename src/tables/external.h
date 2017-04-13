/******************************************************************************
 * Copyright (c) 2016, NXP Semiconductors
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
#ifndef _TABLES_EXTERNAL_H
#define _TABLES_EXTERNAL_H

#include "agu-tables.h"
#include "cgu-tables.h"
#include "rgu-tables.h"
#include "status-tables.h"
#include "config-tables.h"

int generic_table_field_get(void*, uint64_t*, int, int, int);
int generic_table_field_set(void*, uint64_t*, int, int, int);
void generic_table_hexdump(void*, int);
uint32_t ether_crc32_le(void*, unsigned int);
void sja1105_table_write_crc(char*, char*);

#define CONFIG_ADDR 0x20000

#define SPI_READ  0
#define SPI_WRITE 1

/* Core */
#define CORE_ADDR   0x000000

void sja1105_l2_forwarding_params_table_show(struct sja1105_l2_forwarding_params_table*);
void sja1105_l2_forwarding_params_table_fmt_show(char*, char*, struct sja1105_l2_forwarding_params_table*);
void sja1105_l2_forwarding_params_table_get(void*, struct sja1105_l2_forwarding_params_table*);
void sja1105_l2_forwarding_params_table_set(void*, struct sja1105_l2_forwarding_params_table*);
void sja1105_l2_forwarding_entry_set(void*, struct sja1105_l2_forwarding_entry*);
void sja1105_l2_forwarding_entry_get(void*, struct sja1105_l2_forwarding_entry*);
void sja1105_l2_forwarding_entry_show(struct sja1105_l2_forwarding_entry*);
void sja1105_l2_forwarding_entry_fmt_show(char*, char*, struct sja1105_l2_forwarding_entry*);
void sja1105_l2_lookup_params_table_set(void*, struct sja1105_l2_lookup_params_table*);
void sja1105_l2_lookup_params_table_get(void*, struct sja1105_l2_lookup_params_table*);
void sja1105_l2_lookup_params_table_show(struct sja1105_l2_lookup_params_table*);
void sja1105_l2_lookup_params_table_fmt_show(char*, char*, struct sja1105_l2_lookup_params_table*);
void sja1105_l2_lookup_entry_set(void*, struct sja1105_l2_lookup_entry*);
void sja1105_l2_lookup_entry_get(void*, struct sja1105_l2_lookup_entry*);
void sja1105_l2_lookup_entry_show(struct sja1105_l2_lookup_entry*);
void sja1105_l2_lookup_entry_fmt_show(char*, char*, struct sja1105_l2_lookup_entry*);
void sja1105_l2_policing_entry_set(void*, struct sja1105_l2_policing_entry*);
void sja1105_l2_policing_entry_get(void*, struct sja1105_l2_policing_entry*);
void sja1105_l2_policing_entry_show(struct sja1105_l2_policing_entry*);
void sja1105_l2_policing_entry_fmt_show(char*, char*, struct sja1105_l2_policing_entry*);
void sja1105_mac_config_entry_set(void*, struct sja1105_mac_config_entry*);
void sja1105_mac_config_entry_get(void*, struct sja1105_mac_config_entry*);
void sja1105_mac_config_entry_show(struct sja1105_mac_config_entry*);
void sja1105_mac_config_entry_fmt_show(char*, char*, struct sja1105_mac_config_entry*);
void sja1105_schedule_entry_points_params_set(void*, struct sja1105_schedule_entry_points_params*);
void sja1105_schedule_entry_points_params_get(void*, struct sja1105_schedule_entry_points_params*);
void sja1105_schedule_entry_points_params_show(struct sja1105_schedule_entry_points_params*);
void sja1105_schedule_entry_points_params_fmt_show(char*, char*, struct sja1105_schedule_entry_points_params*);
void sja1105_schedule_entry_points_entry_set(void*, struct sja1105_schedule_entry_points_entry*);
void sja1105_schedule_entry_points_entry_get(void*, struct sja1105_schedule_entry_points_entry*);
void sja1105_schedule_entry_points_entry_show(struct sja1105_schedule_entry_points_entry*);
void sja1105_schedule_entry_points_entry_fmt_show(char*, char*, struct sja1105_schedule_entry_points_entry*);
void sja1105_schedule_params_entry_set(void*, struct sja1105_schedule_params_entry*);
void sja1105_schedule_params_entry_get(void*, struct sja1105_schedule_params_entry*);
void sja1105_schedule_params_entry_show(struct sja1105_schedule_params_entry*);
void sja1105_schedule_params_entry_fmt_show(char*, char*, struct sja1105_schedule_params_entry*);
void sja1105_schedule_entry_get(void*, struct sja1105_schedule_entry*);
void sja1105_schedule_entry_set(void*, struct sja1105_schedule_entry*);
void sja1105_schedule_entry_show(struct sja1105_schedule_entry*);
void sja1105_schedule_entry_fmt_show(char*, char*, struct sja1105_schedule_entry*);
void sja1105_table_header_get(void*, struct sja1105_table_header*);
void sja1105_table_header_set(void*, struct sja1105_table_header *hdr);
void sja1105_table_header_set_with_crc(void*, struct sja1105_table_header *hdr);
void sja1105_table_header_show(struct sja1105_table_header *hdr);
void sja1105_vlan_lookup_entry_set(void*, struct sja1105_vlan_lookup_entry*);
void sja1105_vlan_lookup_entry_get(void*, struct sja1105_vlan_lookup_entry*);
void sja1105_vlan_lookup_entry_show(struct sja1105_vlan_lookup_entry*);
void sja1105_vlan_lookup_entry_fmt_show(char*, char*, struct sja1105_vlan_lookup_entry*);
void sja1105_xmii_params_table_set(void*, struct sja1105_xmii_params_table*);
void sja1105_xmii_params_table_get(void*, struct sja1105_xmii_params_table*);
void sja1105_xmii_params_table_show(struct sja1105_xmii_params_table*);
void sja1105_xmii_params_table_fmt_show(char*, char*, struct sja1105_xmii_params_table*);
void sja1105_general_params_table_set(void*, struct sja1105_general_params_table*);
void sja1105_general_params_table_get(void*, struct sja1105_general_params_table*);
void sja1105_general_params_table_show(struct sja1105_general_params_table*);
void sja1105_general_params_table_fmt_show(char*, char*, struct sja1105_general_params_table*);
void sja1105_avb_params_table_set(void*, struct sja1105_avb_params_table*);
void sja1105_avb_params_table_get(void*, struct sja1105_avb_params_table*);
void sja1105_avb_params_table_show(struct sja1105_avb_params_table*);
void sja1105_avb_params_table_fmt_show(char*, char*, struct sja1105_avb_params_table*);
void sja1105_vl_forwarding_params_table_set(void*, struct sja1105_vl_forwarding_params_table*);
void sja1105_vl_forwarding_params_table_get(void*, struct sja1105_vl_forwarding_params_table*);
void sja1105_vl_forwarding_params_table_show(struct sja1105_vl_forwarding_params_table*);
void sja1105_vl_forwarding_params_table_fmt_show(char*, char*, struct sja1105_vl_forwarding_params_table*);
void sja1105_vl_forwarding_entry_set(void*, struct sja1105_vl_forwarding_entry*);
void sja1105_vl_forwarding_entry_get(void*, struct sja1105_vl_forwarding_entry*);
void sja1105_vl_forwarding_entry_show(struct sja1105_vl_forwarding_entry*);
void sja1105_vl_forwarding_entry_fmt_show(char*, char*, struct sja1105_vl_forwarding_entry*);
void sja1105_vl_policing_entry_set(void*, struct sja1105_vl_policing_entry*);
void sja1105_vl_policing_entry_get(void*, struct sja1105_vl_policing_entry*);
void sja1105_vl_policing_entry_show(struct sja1105_vl_policing_entry*);
void sja1105_vl_policing_entry_fmt_show(char*, char*, struct sja1105_vl_policing_entry*);
void sja1105_vl_lookup_entry_set(void*, struct sja1105_vl_lookup_entry*);
void sja1105_vl_lookup_entry_get(void*, struct sja1105_vl_lookup_entry*);
void sja1105_vl_lookup_entry_show(struct sja1105_vl_lookup_entry*);
void sja1105_vl_lookup_entry_fmt_show(char*, char*, struct sja1105_vl_lookup_entry*);

#endif
