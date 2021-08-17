/*
 * file.c
 *
 * Copyright (c) 1999 Al Smith
 *
 * Portions derived from work (c) 1995,1996 Christian Vogelgsang.
 */

#include <linux/buffer_head.h>
#include "efs.h"

int efs_get_block(struct inode *inode, sector_t iblock,
		  struct buffer_head *bh_result, int create)
{
	int error = -EROFS;
	long phys;

	if (create)
		return error;
	if (iblock >= inode->i_blocks) {
#ifdef DEBUG
		/*
		 * i have no idea why this happens as often as it does
		 */
<<<<<<< HEAD
		printk(KERN_WARNING "EFS: bmap(): block %d >= %ld (filesize %ld)\n",
			block,
			inode->i_blocks,
			inode->i_size);
=======
		pr_warn("%s(): block %d >= %ld (filesize %ld)\n",
			__func__, block, inode->i_blocks, inode->i_size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
		return 0;
	}
	phys = efs_map_block(inode, iblock);
	if (phys)
		map_bh(bh_result, inode->i_sb, phys);
	return 0;
}

int efs_bmap(struct inode *inode, efs_block_t block) {

	if (block < 0) {
<<<<<<< HEAD
		printk(KERN_WARNING "EFS: bmap(): block < 0\n");
=======
		pr_warn("%s(): block < 0\n", __func__);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;
	}

	/* are we about to read past the end of a file ? */
	if (!(block < inode->i_blocks)) {
#ifdef DEBUG
		/*
		 * i have no idea why this happens as often as it does
		 */
<<<<<<< HEAD
		printk(KERN_WARNING "EFS: bmap(): block %d >= %ld (filesize %ld)\n",
			block,
			inode->i_blocks,
			inode->i_size);
=======
		pr_warn("%s(): block %d >= %ld (filesize %ld)\n",
			__func__, block, inode->i_blocks, inode->i_size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
		return 0;
	}

	return efs_map_block(inode, block);
}
