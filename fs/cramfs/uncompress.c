/*
 * uncompress.c
 *
 * (C) Copyright 1999 Linus Torvalds
 *
 * cramfs interfaces to the uncompression library. There's really just
 * three entrypoints:
 *
 *  - cramfs_uncompress_init() - called to initialize the thing.
 *  - cramfs_uncompress_exit() - tell me when you're done
 *  - cramfs_uncompress_block() - uncompress a block.
 *
 * NOTE NOTE NOTE! The uncompression is entirely single-threaded. We
 * only have one stream, and we'll initialize it only once even if it
 * then is used by multiple filesystems.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/vmalloc.h>
#include <linux/zlib.h>
<<<<<<< HEAD
#include <linux/cramfs_fs.h>
=======
#include "internal.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static z_stream stream;
static int initialized;

/* Returns length of decompressed data. */
int cramfs_uncompress_block(void *dst, int dstlen, void *src, int srclen)
{
	int err;

	stream.next_in = src;
	stream.avail_in = srclen;

	stream.next_out = dst;
	stream.avail_out = dstlen;

	err = zlib_inflateReset(&stream);
	if (err != Z_OK) {
<<<<<<< HEAD
		printk("zlib_inflateReset error %d\n", err);
=======
		pr_err("zlib_inflateReset error %d\n", err);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		zlib_inflateEnd(&stream);
		zlib_inflateInit(&stream);
	}

	err = zlib_inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END)
		goto err;
	return stream.total_out;

err:
<<<<<<< HEAD
	printk("Error %d while decompressing!\n", err);
	printk("%p(%d)->%p(%d)\n", src, srclen, dst, dstlen);
=======
	pr_err("Error %d while decompressing!\n", err);
	pr_err("%p(%d)->%p(%d)\n", src, srclen, dst, dstlen);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return -EIO;
}

int cramfs_uncompress_init(void)
{
	if (!initialized++) {
		stream.workspace = vmalloc(zlib_inflate_workspacesize());
<<<<<<< HEAD
		if ( !stream.workspace ) {
=======
		if (!stream.workspace) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			initialized = 0;
			return -ENOMEM;
		}
		stream.next_in = NULL;
		stream.avail_in = 0;
		zlib_inflateInit(&stream);
	}
	return 0;
}

void cramfs_uncompress_exit(void)
{
	if (!--initialized) {
		zlib_inflateEnd(&stream);
		vfree(stream.workspace);
	}
}
