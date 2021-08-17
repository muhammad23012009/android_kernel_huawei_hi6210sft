/*
 * Copyright (C) 2009 IBM Corporation
 * Author: Mimi Zohar <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 */

#ifndef _LINUX_INTEGRITY_H
#define _LINUX_INTEGRITY_H

#include <linux/fs.h>

enum integrity_status {
	INTEGRITY_PASS = 0,
<<<<<<< HEAD
=======
	INTEGRITY_PASS_IMMUTABLE,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	INTEGRITY_FAIL,
	INTEGRITY_NOLABEL,
	INTEGRITY_NOXATTRS,
	INTEGRITY_UNKNOWN,
};

/* List of EVM protected security xattrs */
#ifdef CONFIG_INTEGRITY
extern struct integrity_iint_cache *integrity_inode_get(struct inode *inode);
extern void integrity_inode_free(struct inode *inode);
<<<<<<< HEAD
=======
extern void __init integrity_load_keys(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#else
static inline struct integrity_iint_cache *
				integrity_inode_get(struct inode *inode)
{
	return NULL;
}

static inline void integrity_inode_free(struct inode *inode)
{
	return;
}
<<<<<<< HEAD
#endif /* CONFIG_INTEGRITY */
=======

static inline void integrity_load_keys(void)
{
}
#endif /* CONFIG_INTEGRITY */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _LINUX_INTEGRITY_H */
