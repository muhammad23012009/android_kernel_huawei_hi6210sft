/*
 *  linux/fs/ext4/symlink.c
 *
 * Only fast symlinks left here - the rest is done by generic code. AV, 1999
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/fs/minix/symlink.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  ext4 symlink handling code
 */

#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/jbd2.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/namei.h>
#include "ext4.h"
#include "xattr.h"

<<<<<<< HEAD
static void *ext4_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct ext4_inode_info *ei = EXT4_I(dentry->d_inode);
	nd_set_link(nd, (char *) ei->i_data);
	return NULL;
}

const struct inode_operations ext4_symlink_inode_operations = {
	.readlink	= generic_readlink,
	.follow_link	= page_follow_link_light,
	.put_link	= page_put_link,
	.setattr	= ext4_setattr,
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= ext4_listxattr,
	.removexattr	= generic_removexattr,
=======
static const char *ext4_encrypted_get_link(struct dentry *dentry,
					   struct inode *inode,
					   struct delayed_call *done)
{
	struct page *cpage = NULL;
	char *caddr, *paddr = NULL;
	struct fscrypt_str cstr, pstr;
	struct fscrypt_symlink_data *sd;
	int res;
	u32 max_size = inode->i_sb->s_blocksize;

	if (!dentry)
		return ERR_PTR(-ECHILD);

	res = fscrypt_get_encryption_info(inode);
	if (res)
		return ERR_PTR(res);

	if (ext4_inode_is_fast_symlink(inode)) {
		caddr = (char *) EXT4_I(inode)->i_data;
		max_size = sizeof(EXT4_I(inode)->i_data);
	} else {
		cpage = read_mapping_page(inode->i_mapping, 0, NULL);
		if (IS_ERR(cpage))
			return ERR_CAST(cpage);
		caddr = page_address(cpage);
	}

	/* Symlink is encrypted */
	sd = (struct fscrypt_symlink_data *)caddr;
	cstr.name = sd->encrypted_path;
	cstr.len  = le16_to_cpu(sd->len);
	if ((cstr.len + sizeof(struct fscrypt_symlink_data) - 1) > max_size) {
		/* Symlink data on the disk is corrupted */
		res = -EFSCORRUPTED;
		goto errout;
	}

	res = fscrypt_fname_alloc_buffer(inode, cstr.len, &pstr);
	if (res)
		goto errout;
	paddr = pstr.name;

	res = fscrypt_fname_disk_to_usr(inode, 0, 0, &cstr, &pstr);
	if (res)
		goto errout;

	/* Null-terminate the name */
	paddr[pstr.len] = '\0';
	if (cpage)
		put_page(cpage);
	set_delayed_call(done, kfree_link, paddr);
	return paddr;
errout:
	if (cpage)
		put_page(cpage);
	kfree(paddr);
	return ERR_PTR(res);
}

const struct inode_operations ext4_encrypted_symlink_inode_operations = {
	.readlink	= generic_readlink,
	.get_link	= ext4_encrypted_get_link,
	.setattr	= ext4_setattr,
	.listxattr	= ext4_listxattr,
};

const struct inode_operations ext4_symlink_inode_operations = {
	.readlink	= generic_readlink,
	.get_link	= page_get_link,
	.setattr	= ext4_setattr,
	.listxattr	= ext4_listxattr,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

const struct inode_operations ext4_fast_symlink_inode_operations = {
	.readlink	= generic_readlink,
<<<<<<< HEAD
	.follow_link	= ext4_follow_link,
	.setattr	= ext4_setattr,
	.setxattr	= generic_setxattr,
	.getxattr	= generic_getxattr,
	.listxattr	= ext4_listxattr,
	.removexattr	= generic_removexattr,
=======
	.get_link	= simple_get_link,
	.setattr	= ext4_setattr,
	.listxattr	= ext4_listxattr,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};
