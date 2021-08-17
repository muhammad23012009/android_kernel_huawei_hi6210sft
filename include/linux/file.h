/*
 * Wrapper functions for accessing the file_struct fd array.
 */

#ifndef __LINUX_FILE_H
#define __LINUX_FILE_H

#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/posix_types.h>

struct file;

extern void fput(struct file *);

struct file_operations;
struct vfsmount;
struct dentry;
struct path;
extern struct file *alloc_file(struct path *, fmode_t mode,
	const struct file_operations *fop);

static inline void fput_light(struct file *file, int fput_needed)
{
	if (fput_needed)
		fput(file);
}

struct fd {
	struct file *file;
<<<<<<< HEAD
	int need_put;
};

static inline void fdput(struct fd fd)
{
	if (fd.need_put)
=======
	unsigned int flags;
};
#define FDPUT_FPUT       1
#define FDPUT_POS_UNLOCK 2

static inline void fdput(struct fd fd)
{
	if (fd.flags & FDPUT_FPUT)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		fput(fd.file);
}

extern struct file *fget(unsigned int fd);
<<<<<<< HEAD
extern struct file *fget_light(unsigned int fd, int *fput_needed);

static inline struct fd fdget(unsigned int fd)
{
	int b;
	struct file *f = fget_light(fd, &b);
	return (struct fd){f,b};
}

extern struct file *fget_raw(unsigned int fd);
extern struct file *fget_raw_light(unsigned int fd, int *fput_needed);

static inline struct fd fdget_raw(unsigned int fd)
{
	int b;
	struct file *f = fget_raw_light(fd, &b);
	return (struct fd){f,b};
=======
extern struct file *fget_raw(unsigned int fd);
extern unsigned long __fdget(unsigned int fd);
extern unsigned long __fdget_raw(unsigned int fd);
extern unsigned long __fdget_pos(unsigned int fd);
extern void __f_unlock_pos(struct file *);

static inline struct fd __to_fd(unsigned long v)
{
	return (struct fd){(struct file *)(v & ~3),v & 3};
}

static inline struct fd fdget(unsigned int fd)
{
	return __to_fd(__fdget(fd));
}

static inline struct fd fdget_raw(unsigned int fd)
{
	return __to_fd(__fdget_raw(fd));
}

static inline struct fd fdget_pos(int fd)
{
	return __to_fd(__fdget_pos(fd));
}

static inline void fdput_pos(struct fd f)
{
	if (f.flags & FDPUT_POS_UNLOCK)
		__f_unlock_pos(f.file);
	fdput(f);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

extern int f_dupfd(unsigned int from, struct file *file, unsigned flags);
extern int replace_fd(unsigned fd, struct file *file, unsigned flags);
extern void set_close_on_exec(unsigned int fd, int flag);
extern bool get_close_on_exec(unsigned int fd);
extern void put_filp(struct file *);
extern int get_unused_fd_flags(unsigned flags);
<<<<<<< HEAD
#define get_unused_fd() get_unused_fd_flags(0)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void put_unused_fd(unsigned int fd);

extern void fd_install(unsigned int fd, struct file *file);

extern void flush_delayed_fput(void);
extern void __fput_sync(struct file *);

#endif /* __LINUX_FILE_H */
