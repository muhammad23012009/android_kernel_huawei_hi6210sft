/*
 * linux/fs/hfsplus/xattr_user.c
 *
 * Vyacheslav Dubeyko <slava@dubeyko.com>
 *
 * Handler for user extended attributes.
 */

<<<<<<< HEAD
#include "hfsplus_fs.h"
#include "xattr.h"

static int hfsplus_user_getxattr(struct dentry *dentry, const char *name,
					void *buffer, size_t size, int type)
{
	char xattr_name[HFSPLUS_ATTR_MAX_STRLEN + 1] = {0};
	size_t len = strlen(name);

	if (!strcmp(name, ""))
		return -EINVAL;

	if (len + XATTR_USER_PREFIX_LEN > HFSPLUS_ATTR_MAX_STRLEN)
		return -EOPNOTSUPP;

	strcpy(xattr_name, XATTR_USER_PREFIX);
	strcpy(xattr_name + XATTR_USER_PREFIX_LEN, name);

	return hfsplus_getxattr(dentry, xattr_name, buffer, size);
}

static int hfsplus_user_setxattr(struct dentry *dentry, const char *name,
		const void *buffer, size_t size, int flags, int type)
{
	char xattr_name[HFSPLUS_ATTR_MAX_STRLEN + 1] = {0};
	size_t len = strlen(name);

	if (!strcmp(name, ""))
		return -EINVAL;

	if (len + XATTR_USER_PREFIX_LEN > HFSPLUS_ATTR_MAX_STRLEN)
		return -EOPNOTSUPP;

	strcpy(xattr_name, XATTR_USER_PREFIX);
	strcpy(xattr_name + XATTR_USER_PREFIX_LEN, name);

	return hfsplus_setxattr(dentry, xattr_name, buffer, size, flags);
}

static size_t hfsplus_user_listxattr(struct dentry *dentry, char *list,
		size_t list_size, const char *name, size_t name_len, int type)
{
	/*
	 * This method is not used.
	 * It is used hfsplus_listxattr() instead of generic_listxattr().
	 */
	return -EOPNOTSUPP;
=======
#include <linux/nls.h>

#include "hfsplus_fs.h"
#include "xattr.h"

static int hfsplus_user_getxattr(const struct xattr_handler *handler,
				 struct dentry *unused, struct inode *inode,
				 const char *name, void *buffer, size_t size)
{

	return hfsplus_getxattr(inode, name, buffer, size,
				XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN);
}

static int hfsplus_user_setxattr(const struct xattr_handler *handler,
				 struct dentry *unused, struct inode *inode,
				 const char *name, const void *buffer,
				 size_t size, int flags)
{
	return hfsplus_setxattr(inode, name, buffer, size, flags,
				XATTR_USER_PREFIX, XATTR_USER_PREFIX_LEN);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

const struct xattr_handler hfsplus_xattr_user_handler = {
	.prefix	= XATTR_USER_PREFIX,
<<<<<<< HEAD
	.list	= hfsplus_user_listxattr,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.get	= hfsplus_user_getxattr,
	.set	= hfsplus_user_setxattr,
};
