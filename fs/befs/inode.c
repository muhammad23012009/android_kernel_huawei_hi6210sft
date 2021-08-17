/*
 * inode.c
 * 
 * Copyright (C) 2001 Will Dyson <will_dyson@pobox.com>
 */

#include <linux/fs.h>

#include "befs.h"
#include "inode.h"

/*
	Validates the correctness of the befs inode
	Returns BEFS_OK if the inode should be used, otherwise
	returns BEFS_BAD_INODE
*/
int
befs_check_inode(struct super_block *sb, befs_inode * raw_inode,
		 befs_blocknr_t inode)
{
	u32 magic1 = fs32_to_cpu(sb, raw_inode->magic1);
	befs_inode_addr ino_num = fsrun_to_cpu(sb, raw_inode->inode_num);
	u32 flags = fs32_to_cpu(sb, raw_inode->flags);

	/* check magic header. */
	if (magic1 != BEFS_INODE_MAGIC1) {
		befs_error(sb,
<<<<<<< HEAD
			   "Inode has a bad magic header - inode = %lu", inode);
=======
			   "Inode has a bad magic header - inode = %lu",
			   (unsigned long)inode);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return BEFS_BAD_INODE;
	}

	/*
	 * Sanity check2: inodes store their own block address. Check it.
	 */
	if (inode != iaddr2blockno(sb, &ino_num)) {
		befs_error(sb, "inode blocknr field disagrees with vfs "
<<<<<<< HEAD
			   "VFS: %lu, Inode %lu",
			   inode, iaddr2blockno(sb, &ino_num));
=======
			   "VFS: %lu, Inode %lu", (unsigned long)
			   inode, (unsigned long)iaddr2blockno(sb, &ino_num));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return BEFS_BAD_INODE;
	}

	/*
	 * check flag
	 */

	if (!(flags & BEFS_INODE_IN_USE)) {
<<<<<<< HEAD
		befs_error(sb, "inode is not used - inode = %lu", inode);
=======
		befs_error(sb, "inode is not used - inode = %lu",
			   (unsigned long)inode);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return BEFS_BAD_INODE;
	}

	return BEFS_OK;
}
