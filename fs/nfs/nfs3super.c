/*
 * Copyright (c) 2012 Netapp, Inc. All rights reserved.
 */
#include <linux/module.h>
#include <linux/nfs_fs.h>
#include "internal.h"
<<<<<<< HEAD
#include "nfs.h"

static struct nfs_subversion nfs_v3 = {
=======
#include "nfs3_fs.h"
#include "nfs.h"

struct nfs_subversion nfs_v3 = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.owner = THIS_MODULE,
	.nfs_fs   = &nfs_fs_type,
	.rpc_vers = &nfs_version3,
	.rpc_ops  = &nfs_v3_clientops,
	.sops     = &nfs_sops,
<<<<<<< HEAD
=======
#ifdef CONFIG_NFS_V3_ACL
	.xattr    = nfs3_xattr_handlers,
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int __init init_nfs_v3(void)
{
	register_nfs_version(&nfs_v3);
	return 0;
}

static void __exit exit_nfs_v3(void)
{
	unregister_nfs_version(&nfs_v3);
}

MODULE_LICENSE("GPL");

module_init(init_nfs_v3);
module_exit(exit_nfs_v3);
