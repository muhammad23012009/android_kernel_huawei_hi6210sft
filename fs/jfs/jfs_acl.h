/*
 *   Copyright (C) International Business Machines  Corp., 2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef _H_JFS_ACL
#define _H_JFS_ACL

#ifdef CONFIG_JFS_POSIX_ACL

struct posix_acl *jfs_get_acl(struct inode *inode, int type);
<<<<<<< HEAD
int jfs_init_acl(tid_t, struct inode *, struct inode *);
int jfs_acl_chmod(struct inode *inode);
=======
int jfs_set_acl(struct inode *inode, struct posix_acl *acl, int type);
int jfs_init_acl(tid_t, struct inode *, struct inode *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#else

static inline int jfs_init_acl(tid_t tid, struct inode *inode,
			       struct inode *dir)
{
	return 0;
}

<<<<<<< HEAD
static inline int jfs_acl_chmod(struct inode *inode)
{
	return 0;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
#endif		/* _H_JFS_ACL */
