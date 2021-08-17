/*
 *    Hypervisor filesystem for Linux on s390.
 *
 *    Copyright IBM Corp. 2006
 *    Author(s): Michael Holzheu <holzheu@de.ibm.com>
 */

#ifndef _HYPFS_H_
#define _HYPFS_H_

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/workqueue.h>
#include <linux/kref.h>
<<<<<<< HEAD
=======
#include <asm/hypfs.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define REG_FILE_MODE    0440
#define UPDATE_FILE_MODE 0220
#define DIR_MODE         0550

<<<<<<< HEAD
extern struct dentry *hypfs_mkdir(struct super_block *sb, struct dentry *parent,
				  const char *name);

extern struct dentry *hypfs_create_u64(struct super_block *sb,
				       struct dentry *dir, const char *name,
				       __u64 value);

extern struct dentry *hypfs_create_str(struct super_block *sb,
				       struct dentry *dir, const char *name,
=======
extern struct dentry *hypfs_mkdir(struct dentry *parent, const char *name);

extern struct dentry *hypfs_create_u64(struct dentry *dir, const char *name,
				       __u64 value);

extern struct dentry *hypfs_create_str(struct dentry *dir, const char *name,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				       char *string);

/* LPAR Hypervisor */
extern int hypfs_diag_init(void);
extern void hypfs_diag_exit(void);
<<<<<<< HEAD
extern int hypfs_diag_create_files(struct super_block *sb, struct dentry *root);
=======
extern int hypfs_diag_create_files(struct dentry *root);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* VM Hypervisor */
extern int hypfs_vm_init(void);
extern void hypfs_vm_exit(void);
<<<<<<< HEAD
extern int hypfs_vm_create_files(struct super_block *sb, struct dentry *root);
=======
extern int hypfs_vm_create_files(struct dentry *root);

/* VM diagnose 0c */
int hypfs_diag0c_init(void);
void hypfs_diag0c_exit(void);

/* Set Partition-Resource Parameter */
int hypfs_sprp_init(void);
void hypfs_sprp_exit(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* debugfs interface */
struct hypfs_dbfs_file;

struct hypfs_dbfs_data {
	void			*buf;
	void			*buf_free_ptr;
	size_t			size;
	struct hypfs_dbfs_file	*dbfs_file;
<<<<<<< HEAD
	struct kref		kref;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct hypfs_dbfs_file {
	const char	*name;
	int		(*data_create)(void **data, void **data_free_ptr,
				       size_t *size);
	void		(*data_free)(const void *buf_free_ptr);
<<<<<<< HEAD

	/* Private data for hypfs_dbfs.c */
	struct hypfs_dbfs_data	*data;
	struct delayed_work	data_free_work;
=======
	long		(*unlocked_ioctl) (struct file *, unsigned int,
					   unsigned long);

	/* Private data for hypfs_dbfs.c */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct mutex		lock;
	struct dentry		*dentry;
};

extern int hypfs_dbfs_init(void);
extern void hypfs_dbfs_exit(void);
extern int hypfs_dbfs_create_file(struct hypfs_dbfs_file *df);
extern void hypfs_dbfs_remove_file(struct hypfs_dbfs_file *df);

#endif /* _HYPFS_H_ */
