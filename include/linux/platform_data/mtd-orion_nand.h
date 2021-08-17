/*
<<<<<<< HEAD
 * arch/arm/plat-orion/include/plat/orion_nand.h
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#ifndef __PLAT_ORION_NAND_H
#define __PLAT_ORION_NAND_H
=======
#ifndef __MTD_ORION_NAND_H
#define __MTD_ORION_NAND_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Device bus NAND private data
 */
struct orion_nand_data {
	struct mtd_partition *parts;
	int (*dev_ready)(struct mtd_info *mtd);
	u32 nr_parts;
	u8 ale;		/* address line number connected to ALE */
	u8 cle;		/* address line number connected to CLE */
	u8 width;	/* buswidth */
	u8 chip_delay;
};


#endif
