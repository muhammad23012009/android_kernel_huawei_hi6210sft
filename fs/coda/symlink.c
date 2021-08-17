/*
 * Symlink inode operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/pagemap.h>

#include <linux/coda.h>
#include <linux/coda_psdev.h>

#include "coda_linux.h"

static int coda_symlink_filler(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
	int error;
	struct coda_inode_info *cii;
	unsigned int len = PAGE_SIZE;
<<<<<<< HEAD
	char *p = kmap(page);
=======
	char *p = page_address(page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cii = ITOC(inode);

	error = venus_readlink(inode->i_sb, &cii->c_fid, p, &len);
	if (error)
		goto fail;
	SetPageUptodate(page);
<<<<<<< HEAD
	kunmap(page);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unlock_page(page);
	return 0;

fail:
	SetPageError(page);
<<<<<<< HEAD
	kunmap(page);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unlock_page(page);
	return error;
}

const struct address_space_operations coda_symlink_aops = {
	.readpage	= coda_symlink_filler,
};
