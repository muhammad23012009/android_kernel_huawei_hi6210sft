#ifndef __BCM47XXNFLASH_H
#define __BCM47XXNFLASH_H

#ifndef pr_fmt
#define pr_fmt(fmt)		KBUILD_MODNAME ": " fmt
#endif

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

struct bcm47xxnflash {
	struct bcma_drv_cc *cc;

	struct nand_chip nand_chip;
<<<<<<< HEAD
	struct mtd_info mtd;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	unsigned curr_command;
	int curr_page_addr;
	int curr_column;

	u8 id_data[8];
};

int bcm47xxnflash_ops_bcm4706_init(struct bcm47xxnflash *b47n);

#endif /* BCM47XXNFLASH */
