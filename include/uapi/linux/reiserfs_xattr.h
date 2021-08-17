/*
  File: linux/reiserfs_xattr.h
*/

#ifndef _LINUX_REISERFS_XATTR_H
#define _LINUX_REISERFS_XATTR_H

#include <linux/types.h>

/* Magic value in header */
#define REISERFS_XATTR_MAGIC 0x52465841	/* "RFXA" */

struct reiserfs_xattr_header {
	__le32 h_magic;		/* magic number for identification */
	__le32 h_hash;		/* hash of the value */
};

struct reiserfs_security_handle {
<<<<<<< HEAD
	char *name;
=======
	const char *name;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void *value;
	size_t length;
};

#endif  /*  _LINUX_REISERFS_XATTR_H  */
