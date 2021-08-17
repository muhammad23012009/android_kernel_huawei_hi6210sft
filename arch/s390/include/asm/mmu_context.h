/*
 *  S390 version
 *
 *  Derived from "include/asm-i386/mmu_context.h"
 */

#ifndef __S390_MMU_CONTEXT_H
#define __S390_MMU_CONTEXT_H

#include <asm/pgalloc.h>
#include <asm/uaccess.h>
#include <asm/tlbflush.h>
#include <asm/ctl_reg.h>

static inline int init_new_context(struct task_struct *tsk,
				   struct mm_struct *mm)
{
<<<<<<< HEAD
	atomic_set(&mm->context.attach_count, 0);
	mm->context.flush_mm = 0;
	mm->context.asce_bits = _ASCE_TABLE_LENGTH | _ASCE_USER_BITS;
#ifdef CONFIG_64BIT
	mm->context.asce_bits |= _ASCE_TYPE_REGION3;
#endif
	if (current->mm && current->mm->context.alloc_pgste) {
		/*
		 * alloc_pgste indicates, that any NEW context will be created
		 * with extended page tables. The old context is unchanged. The
		 * page table allocation and the page table operations will
		 * look at has_pgste to distinguish normal and extended page
		 * tables. The only way to create extended page tables is to
		 * set alloc_pgste and then create a new context (e.g. dup_mm).
		 * The page table allocation is called after init_new_context
		 * and if has_pgste is set, it will create extended page
		 * tables.
		 */
		mm->context.has_pgste = 1;
		mm->context.alloc_pgste = 1;
	} else {
		mm->context.has_pgste = 0;
		mm->context.alloc_pgste = 0;
	}
	mm->context.asce_limit = STACK_TOP_MAX;
=======
	spin_lock_init(&mm->context.lock);
	spin_lock_init(&mm->context.pgtable_lock);
	INIT_LIST_HEAD(&mm->context.pgtable_list);
	spin_lock_init(&mm->context.gmap_lock);
	INIT_LIST_HEAD(&mm->context.gmap_list);
	cpumask_clear(&mm->context.cpu_attach_mask);
	atomic_set(&mm->context.flush_count, 0);
	mm->context.gmap_asce = 0;
	mm->context.flush_mm = 0;
#ifdef CONFIG_PGSTE
	mm->context.alloc_pgste = page_table_allocate_pgste;
	mm->context.has_pgste = 0;
	mm->context.use_skey = 0;
#endif
	switch (mm->context.asce_limit) {
	case 1UL << 42:
		/*
		 * forked 3-level task, fall through to set new asce with new
		 * mm->pgd
		 */
	case 0:
		/* context created by exec, set asce limit to 4TB */
		mm->context.asce_limit = STACK_TOP_MAX;
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
				   _ASCE_USER_BITS | _ASCE_TYPE_REGION3;
		break;
	case 1UL << 53:
		/* forked 4-level task, set new asce with new mm->pgd */
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
				   _ASCE_USER_BITS | _ASCE_TYPE_REGION2;
		break;
	case 1UL << 31:
		/* forked 2-level compat task, set new asce with new mm->pgd */
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
				   _ASCE_USER_BITS | _ASCE_TYPE_SEGMENT;
		/* pgd_alloc() did not increase mm->nr_pmds */
		mm_inc_nr_pmds(mm);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	crst_table_init((unsigned long *) mm->pgd, pgd_entry_type(mm));
	return 0;
}

#define destroy_context(mm)             do { } while (0)

<<<<<<< HEAD
#ifndef CONFIG_64BIT
#define LCTL_OPCODE "lctl"
#else
#define LCTL_OPCODE "lctlg"
#endif

static inline void update_mm(struct mm_struct *mm, struct task_struct *tsk)
{
	pgd_t *pgd = mm->pgd;

	S390_lowcore.user_asce = mm->context.asce_bits | __pa(pgd);
	if (s390_user_mode != HOME_SPACE_MODE) {
		/* Load primary space page table origin. */
		asm volatile(LCTL_OPCODE" 1,1,%0\n"
			     : : "m" (S390_lowcore.user_asce) );
	} else
		/* Load home space page table origin. */
		asm volatile(LCTL_OPCODE" 13,13,%0"
			     : : "m" (S390_lowcore.user_asce) );
	set_fs(current->thread.mm_segment);
=======
static inline void set_user_asce(struct mm_struct *mm)
{
	S390_lowcore.user_asce = mm->context.asce;
	if (current->thread.mm_segment.ar4)
		__ctl_load(S390_lowcore.user_asce, 7, 7);
	set_cpu_flag(CIF_ASCE);
}

static inline void clear_user_asce(void)
{
	S390_lowcore.user_asce = S390_lowcore.kernel_asce;

	__ctl_load(S390_lowcore.user_asce, 1, 1);
	__ctl_load(S390_lowcore.user_asce, 7, 7);
}

static inline void load_kernel_asce(void)
{
	unsigned long asce;

	__ctl_store(asce, 1, 1);
	if (asce != S390_lowcore.kernel_asce)
		__ctl_load(S390_lowcore.kernel_asce, 1, 1);
	set_cpu_flag(CIF_ASCE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next,
			     struct task_struct *tsk)
{
<<<<<<< HEAD
	cpumask_set_cpu(smp_processor_id(), mm_cpumask(next));
	update_mm(next, tsk);
	atomic_dec(&prev->context.attach_count);
	WARN_ON(atomic_read(&prev->context.attach_count) < 0);
	atomic_inc(&next->context.attach_count);
	/* Check for TLBs not flushed yet */
	if (next->context.flush_mm)
		__tlb_flush_mm(next);
=======
	int cpu = smp_processor_id();

	S390_lowcore.user_asce = next->context.asce;
	if (prev == next)
		return;
	cpumask_set_cpu(cpu, &next->context.cpu_attach_mask);
	/* Clear old ASCE by loading the kernel ASCE. */
	__ctl_load(S390_lowcore.kernel_asce, 1, 1);
	__ctl_load(S390_lowcore.kernel_asce, 7, 7);
	cpumask_clear_cpu(cpu, &prev->context.cpu_attach_mask);
}

#define finish_arch_post_lock_switch finish_arch_post_lock_switch
static inline void finish_arch_post_lock_switch(void)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;

	load_kernel_asce();
	if (mm) {
		preempt_disable();
		while (atomic_read(&mm->context.flush_count))
			cpu_relax();
		cpumask_set_cpu(smp_processor_id(), mm_cpumask(mm));
		__tlb_flush_mm_lazy(mm);
		preempt_enable();
	}
	set_fs(current->thread.mm_segment);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#define enter_lazy_tlb(mm,tsk)	do { } while (0)
#define deactivate_mm(tsk,mm)	do { } while (0)

static inline void activate_mm(struct mm_struct *prev,
                               struct mm_struct *next)
{
<<<<<<< HEAD
        switch_mm(prev, next, current);
=======
	switch_mm(prev, next, current);
	cpumask_set_cpu(smp_processor_id(), mm_cpumask(next));
	set_user_asce(next);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void arch_dup_mmap(struct mm_struct *oldmm,
				 struct mm_struct *mm)
{
<<<<<<< HEAD
#ifdef CONFIG_64BIT
	if (oldmm->context.asce_limit < mm->context.asce_limit)
		crst_table_downgrade(mm, oldmm->context.asce_limit);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void arch_exit_mmap(struct mm_struct *mm)
{
}

<<<<<<< HEAD
=======
static inline void arch_unmap(struct mm_struct *mm,
			struct vm_area_struct *vma,
			unsigned long start, unsigned long end)
{
}

static inline void arch_bprm_mm_init(struct mm_struct *mm,
				     struct vm_area_struct *vma)
{
}

static inline bool arch_vma_access_permitted(struct vm_area_struct *vma,
		bool write, bool execute, bool foreign)
{
	/* by default, allow everything */
	return true;
}

static inline bool arch_pte_access_permitted(pte_t pte, bool write)
{
	/* by default, allow everything */
	return true;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __S390_MMU_CONTEXT_H */
