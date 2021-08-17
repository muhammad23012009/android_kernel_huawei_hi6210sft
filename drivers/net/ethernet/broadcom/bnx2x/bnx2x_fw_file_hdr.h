/* bnx2x_fw_file_hdr.h: FW binary file header structure.
 *
 * Copyright (c) 2007-2013 Broadcom Corporation
<<<<<<< HEAD
=======
 * Copyright (c) 2014 QLogic Corporation
 * All rights reserved
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
<<<<<<< HEAD
 * Maintained by: Eilon Greenstein <eilong@broadcom.com>
 * Written by: Vladislav Zolotarov <vladz@broadcom.com>
=======
 * Maintained by: Ariel Elior <ariel.elior@qlogic.com>
 * Written by: Vladislav Zolotarov
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Based on the original idea of John Wright <john.wright@hp.com>.
 */

#ifndef BNX2X_INIT_FILE_HDR_H
#define BNX2X_INIT_FILE_HDR_H

struct bnx2x_fw_file_section {
	__be32 len;
	__be32 offset;
};

struct bnx2x_fw_file_hdr {
	struct bnx2x_fw_file_section init_ops;
	struct bnx2x_fw_file_section init_ops_offsets;
	struct bnx2x_fw_file_section init_data;
	struct bnx2x_fw_file_section tsem_int_table_data;
	struct bnx2x_fw_file_section tsem_pram_data;
	struct bnx2x_fw_file_section usem_int_table_data;
	struct bnx2x_fw_file_section usem_pram_data;
	struct bnx2x_fw_file_section csem_int_table_data;
	struct bnx2x_fw_file_section csem_pram_data;
	struct bnx2x_fw_file_section xsem_int_table_data;
	struct bnx2x_fw_file_section xsem_pram_data;
	struct bnx2x_fw_file_section iro_arr;
	struct bnx2x_fw_file_section fw_version;
};

#endif /* BNX2X_INIT_FILE_HDR_H */
