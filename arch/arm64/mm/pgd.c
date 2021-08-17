/*
 * PGD allocation/freeing
 *
 * Copyright (C) 2012 ARM Ltd.
 * Author: Catalin Marinas <catalin.marinas@arm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/slab.h>

#include <asm/pgalloc.h>
#include <asm/page.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
#include "mm.h"

#define PGD_SIZE	(PTRS_PER_PGD * sizeof(pgd_t))
=======
static struct kmem_cache *pgd_cache;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

pgd_t *pgd_alloc(struct mm_struct *mm)
{
	if (PGD_SIZE == PAGE_SIZE)
<<<<<<< HEAD
		return (pgd_t *)get_zeroed_page(GFP_KERNEL);
	else
		return kzalloc(PGD_SIZE, GFP_KERNEL);
=======
		return (pgd_t *)__get_free_page(PGALLOC_GFP);
	else
		return kmem_cache_alloc(pgd_cache, PGALLOC_GFP);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	if (PGD_SIZE == PAGE_SIZE)
		free_page((unsigned long)pgd);
	else
<<<<<<< HEAD
		kfree(pgd);
=======
		kmem_cache_free(pgd_cache, pgd);
}

void __init pgd_cache_init(void)
{
	if (PGD_SIZE == PAGE_SIZE)
		return;

	/*
	 * Naturally aligned pgds required by the architecture.
	 */
	pgd_cache = kmem_cache_create("pgd_cache", PGD_SIZE, PGD_SIZE,
				      SLAB_PANIC, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
