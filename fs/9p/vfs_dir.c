/*
 * linux/fs/9p/vfs_dir.c
 *
 * This file contains vfs directory ops for the 9P2000 protocol.
 *
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/inet.h>
#include <linux/idr.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/uio.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <net/9p/9p.h>
#include <net/9p/client.h>

#include "v9fs.h"
#include "v9fs_vfs.h"
#include "fid.h"

/**
 * struct p9_rdir - readdir accounting
<<<<<<< HEAD
 * @mutex: mutex protecting readdir
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @head: start offset of current dirread buffer
 * @tail: end offset of current dirread buffer
 * @buf: dirread buffer
 *
 * private structure for keeping track of readdir
 * allocated on demand
 */

struct p9_rdir {
	int head;
	int tail;
	uint8_t buf[];
};

/**
 * dt_type - return file type
 * @mistat: mistat structure
 *
 */

static inline int dt_type(struct p9_wstat *mistat)
{
	unsigned long perm = mistat->mode;
	int rettype = DT_REG;

	if (perm & P9_DMDIR)
		rettype = DT_DIR;
	if (perm & P9_DMSYMLINK)
		rettype = DT_LNK;

	return rettype;
}

<<<<<<< HEAD
static void p9stat_init(struct p9_wstat *stbuf)
{
	stbuf->name  = NULL;
	stbuf->uid   = NULL;
	stbuf->gid   = NULL;
	stbuf->muid  = NULL;
	stbuf->extension = NULL;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * v9fs_alloc_rdir_buf - Allocate buffer used for read and readdir
 * @filp: opened file structure
 * @buflen: Length in bytes of buffer to allocate
 *
 */

static struct p9_rdir *v9fs_alloc_rdir_buf(struct file *filp, int buflen)
{
	struct p9_fid *fid = filp->private_data;
	if (!fid->rdir)
		fid->rdir = kzalloc(sizeof(struct p9_rdir) + buflen, GFP_KERNEL);
	return fid->rdir;
}

/**
<<<<<<< HEAD
 * v9fs_dir_readdir - read a directory
 * @filp: opened file structure
 * @dirent: directory structure ???
 * @filldir: function to populate directory structure ???
 *
 */

static int v9fs_dir_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	int over;
=======
 * v9fs_dir_readdir - iterate through a directory
 * @file: opened file structure
 * @ctx: actor we feed the entries to
 *
 */

static int v9fs_dir_readdir(struct file *file, struct dir_context *ctx)
{
	bool over;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct p9_wstat st;
	int err = 0;
	struct p9_fid *fid;
	int buflen;
<<<<<<< HEAD
	int reclen = 0;
	struct p9_rdir *rdir;

	p9_debug(P9_DEBUG_VFS, "name %s\n", filp->f_path.dentry->d_name.name);
	fid = filp->private_data;

	buflen = fid->clnt->msize - P9_IOHDRSZ;

	rdir = v9fs_alloc_rdir_buf(filp, buflen);
	if (!rdir)
		return -ENOMEM;

	while (1) {
		if (rdir->tail == rdir->head) {
			err = v9fs_file_readn(filp, rdir->buf, NULL,
							buflen, filp->f_pos);
			if (err <= 0)
				return err;

			rdir->head = 0;
			rdir->tail = err;
		}
		while (rdir->head < rdir->tail) {
			p9stat_init(&st);
			err = p9stat_read(fid->clnt, rdir->buf + rdir->head,
					  rdir->tail - rdir->head, &st);
			if (err) {
				p9_debug(P9_DEBUG_VFS, "returned %d\n", err);
				p9stat_free(&st);
				return -EIO;
			}
			reclen = st.size+2;

			over = filldir(dirent, st.name, strlen(st.name),
			    filp->f_pos, v9fs_qid2ino(&st.qid), dt_type(&st));

			p9stat_free(&st);

			if (over)
				return 0;

			rdir->head += reclen;
			filp->f_pos += reclen;
=======
	struct p9_rdir *rdir;
	struct kvec kvec;

	p9_debug(P9_DEBUG_VFS, "name %pD\n", file);
	fid = file->private_data;

	buflen = fid->clnt->msize - P9_IOHDRSZ;

	rdir = v9fs_alloc_rdir_buf(file, buflen);
	if (!rdir)
		return -ENOMEM;
	kvec.iov_base = rdir->buf;
	kvec.iov_len = buflen;

	while (1) {
		if (rdir->tail == rdir->head) {
			struct iov_iter to;
			int n;
			iov_iter_kvec(&to, READ | ITER_KVEC, &kvec, 1, buflen);
			n = p9_client_read(file->private_data, ctx->pos, &to,
					   &err);
			if (err)
				return err;
			if (n == 0)
				return 0;

			rdir->head = 0;
			rdir->tail = n;
		}
		while (rdir->head < rdir->tail) {
			err = p9stat_read(fid->clnt, rdir->buf + rdir->head,
					  rdir->tail - rdir->head, &st);
			if (err <= 0) {
				p9_debug(P9_DEBUG_VFS, "returned %d\n", err);
				return -EIO;
			}

			over = !dir_emit(ctx, st.name, strlen(st.name),
					 v9fs_qid2ino(&st.qid), dt_type(&st));
			p9stat_free(&st);
			if (over)
				return 0;

			rdir->head += err;
			ctx->pos += err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
}

/**
<<<<<<< HEAD
 * v9fs_dir_readdir_dotl - read a directory
 * @filp: opened file structure
 * @dirent: buffer to fill dirent structures
 * @filldir: function to populate dirent structures
 *
 */
static int v9fs_dir_readdir_dotl(struct file *filp, void *dirent,
						filldir_t filldir)
{
	int over;
=======
 * v9fs_dir_readdir_dotl - iterate through a directory
 * @file: opened file structure
 * @ctx: actor we feed the entries to
 *
 */
static int v9fs_dir_readdir_dotl(struct file *file, struct dir_context *ctx)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int err = 0;
	struct p9_fid *fid;
	int buflen;
	struct p9_rdir *rdir;
	struct p9_dirent curdirent;
<<<<<<< HEAD
	u64 oldoffset = 0;

	p9_debug(P9_DEBUG_VFS, "name %s\n", filp->f_path.dentry->d_name.name);
	fid = filp->private_data;

	buflen = fid->clnt->msize - P9_READDIRHDRSZ;

	rdir = v9fs_alloc_rdir_buf(filp, buflen);
=======

	p9_debug(P9_DEBUG_VFS, "name %pD\n", file);
	fid = file->private_data;

	buflen = fid->clnt->msize - P9_READDIRHDRSZ;

	rdir = v9fs_alloc_rdir_buf(file, buflen);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!rdir)
		return -ENOMEM;

	while (1) {
		if (rdir->tail == rdir->head) {
			err = p9_client_readdir(fid, rdir->buf, buflen,
<<<<<<< HEAD
						filp->f_pos);
=======
						ctx->pos);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (err <= 0)
				return err;

			rdir->head = 0;
			rdir->tail = err;
		}

		while (rdir->head < rdir->tail) {

			err = p9dirent_read(fid->clnt, rdir->buf + rdir->head,
					    rdir->tail - rdir->head,
					    &curdirent);
			if (err < 0) {
				p9_debug(P9_DEBUG_VFS, "returned %d\n", err);
				return -EIO;
			}

<<<<<<< HEAD
			/* d_off in dirent structure tracks the offset into
			 * the next dirent in the dir. However, filldir()
			 * expects offset into the current dirent. Hence
			 * while calling filldir send the offset from the
			 * previous dirent structure.
			 */
			over = filldir(dirent, curdirent.d_name,
					strlen(curdirent.d_name),
					oldoffset, v9fs_qid2ino(&curdirent.qid),
					curdirent.d_type);
			oldoffset = curdirent.d_off;

			if (over)
				return 0;

			filp->f_pos = curdirent.d_off;
=======
			if (!dir_emit(ctx, curdirent.d_name,
				      strlen(curdirent.d_name),
				      v9fs_qid2ino(&curdirent.qid),
				      curdirent.d_type))
				return 0;

			ctx->pos = curdirent.d_off;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rdir->head += err;
		}
	}
}


/**
 * v9fs_dir_release - close a directory
 * @inode: inode of the directory
 * @filp: file pointer to a directory
 *
 */

int v9fs_dir_release(struct inode *inode, struct file *filp)
{
	struct p9_fid *fid;

	fid = filp->private_data;
	p9_debug(P9_DEBUG_VFS, "inode: %p filp: %p fid: %d\n",
		 inode, filp, fid ? fid->fid : -1);
	if (fid)
		p9_client_clunk(fid);
	return 0;
}

const struct file_operations v9fs_dir_operations = {
	.read = generic_read_dir,
	.llseek = generic_file_llseek,
<<<<<<< HEAD
	.readdir = v9fs_dir_readdir,
=======
	.iterate_shared = v9fs_dir_readdir,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
};

const struct file_operations v9fs_dir_operations_dotl = {
	.read = generic_read_dir,
	.llseek = generic_file_llseek,
<<<<<<< HEAD
	.readdir = v9fs_dir_readdir_dotl,
=======
	.iterate_shared = v9fs_dir_readdir_dotl,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.open = v9fs_file_open,
	.release = v9fs_dir_release,
        .fsync = v9fs_file_fsync_dotl,
};
