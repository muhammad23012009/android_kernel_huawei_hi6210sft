/*
 * User address space access functions.
 *
 *  For licencing details see kernel-base/COPYING
 */

#include <linux/highmem.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/word-at-a-time.h>
#include <linux/sched.h>

/*
<<<<<<< HEAD
 * best effort, GUP based copy_from_user() that is NMI-safe
=======
 * We rely on the nested NMI work to allow atomic faults from the NMI path; the
 * nested NMI paths are careful to preserve CR2.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
unsigned long
copy_from_user_nmi(void *to, const void __user *from, unsigned long n)
{
<<<<<<< HEAD
	unsigned long offset, addr = (unsigned long)from;
	unsigned long size, len = 0;
	struct page *page;
	void *map;
	int ret;

	if (__range_not_ok(from, n, TASK_SIZE))
		return len;

	do {
		ret = __get_user_pages_fast(addr, 1, 0, &page);
		if (!ret)
			break;

		offset = addr & (PAGE_SIZE - 1);
		size = min(PAGE_SIZE - offset, n - len);

		map = kmap_atomic(page);
		memcpy(to, map+offset, size);
		kunmap_atomic(map);
		put_page(page);

		len  += size;
		to   += size;
		addr += size;

	} while (len < n);

	return len;
=======
	unsigned long ret;

	if (__range_not_ok(from, n, TASK_SIZE))
		return n;

	/*
	 * Even though this function is typically called from NMI/IRQ context
	 * disable pagefaults so that its behaviour is consistent even when
	 * called form other contexts.
	 */
	pagefault_disable();
	ret = __copy_from_user_inatomic(to, from, n);
	pagefault_enable();

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(copy_from_user_nmi);
