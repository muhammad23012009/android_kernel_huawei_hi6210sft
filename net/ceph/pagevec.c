#include <linux/ceph/ceph_debug.h>

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/namei.h>
#include <linux/writeback.h>

#include <linux/ceph/libceph.h>

/*
 * build a vector of user pages
 */
struct page **ceph_get_direct_page_vector(const void __user *data,
					  int num_pages, bool write_page)
{
	struct page **pages;
	int got = 0;
	int rc = 0;

	pages = kmalloc(sizeof(*pages) * num_pages, GFP_NOFS);
	if (!pages)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	down_read(&current->mm->mmap_sem);
	while (got < num_pages) {
		rc = get_user_pages(current, current->mm,
		    (unsigned long)data + ((unsigned long)got * PAGE_SIZE),
		    num_pages - got, write_page, 0, pages + got, NULL);
=======
	while (got < num_pages) {
		rc = get_user_pages_unlocked(
		    (unsigned long)data + ((unsigned long)got * PAGE_SIZE),
		    num_pages - got, pages + got, write_page ? FOLL_WRITE : 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (rc < 0)
			break;
		BUG_ON(rc == 0);
		got += rc;
	}
<<<<<<< HEAD
	up_read(&current->mm->mmap_sem);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (rc < 0)
		goto fail;
	return pages;

fail:
	ceph_put_page_vector(pages, got, false);
	return ERR_PTR(rc);
}
EXPORT_SYMBOL(ceph_get_direct_page_vector);

void ceph_put_page_vector(struct page **pages, int num_pages, bool dirty)
{
	int i;

	for (i = 0; i < num_pages; i++) {
		if (dirty)
			set_page_dirty_lock(pages[i]);
		put_page(pages[i]);
	}
<<<<<<< HEAD
	kfree(pages);
=======
	kvfree(pages);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(ceph_put_page_vector);

void ceph_release_page_vector(struct page **pages, int num_pages)
{
	int i;

	for (i = 0; i < num_pages; i++)
		__free_pages(pages[i], 0);
	kfree(pages);
}
EXPORT_SYMBOL(ceph_release_page_vector);

/*
 * allocate a vector new pages
 */
struct page **ceph_alloc_page_vector(int num_pages, gfp_t flags)
{
	struct page **pages;
	int i;

	pages = kmalloc(sizeof(*pages) * num_pages, flags);
	if (!pages)
		return ERR_PTR(-ENOMEM);
	for (i = 0; i < num_pages; i++) {
		pages[i] = __page_cache_alloc(flags);
		if (pages[i] == NULL) {
			ceph_release_page_vector(pages, i);
			return ERR_PTR(-ENOMEM);
		}
	}
	return pages;
}
EXPORT_SYMBOL(ceph_alloc_page_vector);

/*
 * copy user data into a page vector
 */
int ceph_copy_user_to_page_vector(struct page **pages,
					 const void __user *data,
					 loff_t off, size_t len)
{
	int i = 0;
<<<<<<< HEAD
	int po = off & ~PAGE_CACHE_MASK;
=======
	int po = off & ~PAGE_MASK;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int left = len;
	int l, bad;

	while (left > 0) {
<<<<<<< HEAD
		l = min_t(int, PAGE_CACHE_SIZE-po, left);
=======
		l = min_t(int, PAGE_SIZE-po, left);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		bad = copy_from_user(page_address(pages[i]) + po, data, l);
		if (bad == l)
			return -EFAULT;
		data += l - bad;
		left -= l - bad;
		po += l - bad;
<<<<<<< HEAD
		if (po == PAGE_CACHE_SIZE) {
=======
		if (po == PAGE_SIZE) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			po = 0;
			i++;
		}
	}
	return len;
}
EXPORT_SYMBOL(ceph_copy_user_to_page_vector);

void ceph_copy_to_page_vector(struct page **pages,
				    const void *data,
				    loff_t off, size_t len)
{
	int i = 0;
<<<<<<< HEAD
	size_t po = off & ~PAGE_CACHE_MASK;
	size_t left = len;

	while (left > 0) {
		size_t l = min_t(size_t, PAGE_CACHE_SIZE-po, left);
=======
	size_t po = off & ~PAGE_MASK;
	size_t left = len;

	while (left > 0) {
		size_t l = min_t(size_t, PAGE_SIZE-po, left);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		memcpy(page_address(pages[i]) + po, data, l);
		data += l;
		left -= l;
		po += l;
<<<<<<< HEAD
		if (po == PAGE_CACHE_SIZE) {
=======
		if (po == PAGE_SIZE) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			po = 0;
			i++;
		}
	}
}
EXPORT_SYMBOL(ceph_copy_to_page_vector);

void ceph_copy_from_page_vector(struct page **pages,
				    void *data,
				    loff_t off, size_t len)
{
	int i = 0;
<<<<<<< HEAD
	size_t po = off & ~PAGE_CACHE_MASK;
	size_t left = len;

	while (left > 0) {
		size_t l = min_t(size_t, PAGE_CACHE_SIZE-po, left);
=======
	size_t po = off & ~PAGE_MASK;
	size_t left = len;

	while (left > 0) {
		size_t l = min_t(size_t, PAGE_SIZE-po, left);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		memcpy(data, page_address(pages[i]) + po, l);
		data += l;
		left -= l;
		po += l;
<<<<<<< HEAD
		if (po == PAGE_CACHE_SIZE) {
=======
		if (po == PAGE_SIZE) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			po = 0;
			i++;
		}
	}
}
EXPORT_SYMBOL(ceph_copy_from_page_vector);

/*
<<<<<<< HEAD
 * copy user data from a page vector into a user pointer
 */
int ceph_copy_page_vector_to_user(struct page **pages,
					 void __user *data,
					 loff_t off, size_t len)
{
	int i = 0;
	int po = off & ~PAGE_CACHE_MASK;
	int left = len;
	int l, bad;

	while (left > 0) {
		l = min_t(int, left, PAGE_CACHE_SIZE-po);
		bad = copy_to_user(data, page_address(pages[i]) + po, l);
		if (bad == l)
			return -EFAULT;
		data += l - bad;
		left -= l - bad;
		if (po) {
			po += l - bad;
			if (po == PAGE_CACHE_SIZE)
				po = 0;
		}
		i++;
	}
	return len;
}
EXPORT_SYMBOL(ceph_copy_page_vector_to_user);

/*
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Zero an extent within a page vector.  Offset is relative to the
 * start of the first page.
 */
void ceph_zero_page_vector_range(int off, int len, struct page **pages)
{
<<<<<<< HEAD
	int i = off >> PAGE_CACHE_SHIFT;

	off &= ~PAGE_CACHE_MASK;
=======
	int i = off >> PAGE_SHIFT;

	off &= ~PAGE_MASK;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	dout("zero_page_vector_page %u~%u\n", off, len);

	/* leading partial page? */
	if (off) {
<<<<<<< HEAD
		int end = min((int)PAGE_CACHE_SIZE, off + len);
=======
		int end = min((int)PAGE_SIZE, off + len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dout("zeroing %d %p head from %d\n", i, pages[i],
		     (int)off);
		zero_user_segment(pages[i], off, end);
		len -= (end - off);
		i++;
	}
<<<<<<< HEAD
	while (len >= PAGE_CACHE_SIZE) {
		dout("zeroing %d %p len=%d\n", i, pages[i], len);
		zero_user_segment(pages[i], 0, PAGE_CACHE_SIZE);
		len -= PAGE_CACHE_SIZE;
=======
	while (len >= PAGE_SIZE) {
		dout("zeroing %d %p len=%d\n", i, pages[i], len);
		zero_user_segment(pages[i], 0, PAGE_SIZE);
		len -= PAGE_SIZE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		i++;
	}
	/* trailing partial page? */
	if (len) {
		dout("zeroing %d %p tail to %d\n", i, pages[i], (int)len);
		zero_user_segment(pages[i], 0, len);
	}
}
EXPORT_SYMBOL(ceph_zero_page_vector_range);

