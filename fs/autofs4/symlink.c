<<<<<<< HEAD
/* -*- c -*- --------------------------------------------------------------- *
 *
 * linux/fs/autofs/symlink.c
 *
 *  Copyright 1997-1998 Transmeta Corporation -- All Rights Reserved
=======
/*
 * Copyright 1997-1998 Transmeta Corporation -- All Rights Reserved
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
<<<<<<< HEAD
 *
 * ------------------------------------------------------------------------- */

#include "autofs_i.h"

static void *autofs4_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	nd_set_link(nd, dentry->d_inode->i_private);
	return NULL;
=======
 */

#include "autofs_i.h"

static const char *autofs4_get_link(struct dentry *dentry,
				    struct inode *inode,
				    struct delayed_call *done)
{
	struct autofs_sb_info *sbi;
	struct autofs_info *ino;

	if (!dentry)
		return ERR_PTR(-ECHILD);
	sbi = autofs4_sbi(dentry->d_sb);
	ino = autofs4_dentry_ino(dentry);
	if (ino && !autofs4_oz_mode(sbi))
		ino->last_used = jiffies;
	return d_inode(dentry)->i_private;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

const struct inode_operations autofs4_symlink_inode_operations = {
	.readlink	= generic_readlink,
<<<<<<< HEAD
	.follow_link	= autofs4_follow_link
=======
	.get_link	= autofs4_get_link
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
