/*
 *  ncp_fs_i.h
 *
 *  Copyright (C) 1995 Volker Lendecke
 *
 */

#ifndef _LINUX_NCP_FS_I
#define _LINUX_NCP_FS_I

/*
 * This is the ncpfs part of the inode structure. This must contain
 * all the information we need to work with an inode after creation.
 */
struct ncp_inode_info {
	__le32	dirEntNum;
	__le32	DosDirNum;
	__u8	volNumber;
	__le32	nwattr;
	struct mutex open_mutex;
	atomic_t	opened;
	int	access;
	int	flags;
#define NCPI_KLUDGE_SYMLINK	0x0001
<<<<<<< HEAD
=======
#define NCPI_DIR_CACHE		0x0002
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__u8	file_handle[6];
	struct inode vfs_inode;
};

#endif	/* _LINUX_NCP_FS_I */
