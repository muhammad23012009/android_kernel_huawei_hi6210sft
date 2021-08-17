#ifndef _S390_TLBFLUSH_H
#define _S390_TLBFLUSH_H

#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/processor.h>
#include <asm/pgalloc.h>
<<<<<<< HEAD

/*
 * Flush all tlb entries on the local cpu.
=======
#include <asm/pgtable.h>

/*
 * Flush all TLB entries on the local CPU.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static inline void __tlb_flush_local(void)
{
	asm volatile("ptlb" : : : "memory");
}

<<<<<<< HEAD
#ifdef CONFIG_SMP
/*
 * Flush all tlb entries on all cpus.
 */
void smp_ptlb_all(void);

static inline void __tlb_flush_global(void)
{
	register unsigned long reg2 asm("2");
	register unsigned long reg3 asm("3");
	register unsigned long reg4 asm("4");
	long dummy;

#ifndef CONFIG_64BIT
	if (!MACHINE_HAS_CSP) {
		smp_ptlb_all();
		return;
	}
#endif /* CONFIG_64BIT */

	dummy = 0;
	reg2 = reg3 = 0;
	reg4 = ((unsigned long) &dummy) + 1;
	asm volatile(
		"	csp	%0,%2"
		: : "d" (reg2), "d" (reg3), "d" (reg4), "m" (dummy) : "cc" );
}

static inline void __tlb_flush_full(struct mm_struct *mm)
{
	cpumask_t local_cpumask;

	preempt_disable();
	/*
	 * If the process only ran on the local cpu, do a local flush.
	 */
	cpumask_copy(&local_cpumask, cpumask_of(smp_processor_id()));
	if (cpumask_equal(mm_cpumask(mm), &local_cpumask))
		__tlb_flush_local();
	else
		__tlb_flush_global();
	preempt_enable();
}
#else
#define __tlb_flush_full(mm)	__tlb_flush_local()
#define __tlb_flush_global()	__tlb_flush_local()
#endif

/*
 * Flush all tlb entries of a page table on all cpus.
 */
static inline void __tlb_flush_idte(unsigned long asce)
{
	asm volatile(
		"	.insn	rrf,0xb98e0000,0,%0,%1,0"
		: : "a" (2048), "a" (asce) : "cc" );
}

static inline void __tlb_flush_mm(struct mm_struct * mm)
{
	/*
	 * If the machine has IDTE we prefer to do a per mm flush
	 * on all cpus instead of doing a local flush if the mm
	 * only ran on the local cpu.
	 */
	if (MACHINE_HAS_IDTE && list_empty(&mm->context.gmap_list))
		__tlb_flush_idte((unsigned long) mm->pgd |
				 mm->context.asce_bits);
	else
		__tlb_flush_full(mm);
}

static inline void __tlb_flush_mm_cond(struct mm_struct * mm)
{
	if (mm->context.flush_mm) {
		__tlb_flush_mm(mm);
		mm->context.flush_mm = 0;
	}
=======
/*
 * Flush TLB entries for a specific ASCE on all CPUs
 */
static inline void __tlb_flush_idte(unsigned long asce)
{
	/* Global TLB flush for the mm */
	asm volatile(
		"	.insn	rrf,0xb98e0000,0,%0,%1,0"
		: : "a" (2048), "a" (asce) : "cc");
}

#ifdef CONFIG_SMP
void smp_ptlb_all(void);

/*
 * Flush all TLB entries on all CPUs.
 */
static inline void __tlb_flush_global(void)
{
	unsigned int dummy = 0;

	csp(&dummy, 0, 0);
}

/*
 * Flush TLB entries for a specific mm on all CPUs (in case gmap is used
 * this implicates multiple ASCEs!).
 */
static inline void __tlb_flush_mm(struct mm_struct *mm)
{
	unsigned long gmap_asce;

	/*
	 * If the machine has IDTE we prefer to do a per mm flush
	 * on all cpus instead of doing a local flush if the mm
	 * only ran on the local cpu.
	 */
	preempt_disable();
	atomic_inc(&mm->context.flush_count);
	/* Reset TLB flush mask */
	cpumask_copy(mm_cpumask(mm), &mm->context.cpu_attach_mask);
	barrier();
	gmap_asce = READ_ONCE(mm->context.gmap_asce);
	if (MACHINE_HAS_IDTE && gmap_asce != -1UL) {
		if (gmap_asce)
			__tlb_flush_idte(gmap_asce);
		__tlb_flush_idte(mm->context.asce);
	} else {
		/* Global TLB flush */
		__tlb_flush_global();
	}
	atomic_dec(&mm->context.flush_count);
	preempt_enable();
}

static inline void __tlb_flush_kernel(void)
{
	if (MACHINE_HAS_IDTE)
		__tlb_flush_idte(init_mm.context.asce);
	else
		__tlb_flush_global();
}
#else
#define __tlb_flush_global()	__tlb_flush_local()

/*
 * Flush TLB entries for a specific ASCE on all CPUs.
 */
static inline void __tlb_flush_mm(struct mm_struct *mm)
{
	__tlb_flush_local();
}

static inline void __tlb_flush_kernel(void)
{
	__tlb_flush_local();
}
#endif

static inline void __tlb_flush_mm_lazy(struct mm_struct * mm)
{
	spin_lock(&mm->context.lock);
	if (mm->context.flush_mm) {
		mm->context.flush_mm = 0;
		__tlb_flush_mm(mm);
	}
	spin_unlock(&mm->context.lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * TLB flushing:
 *  flush_tlb() - flushes the current mm struct TLBs
 *  flush_tlb_all() - flushes all processes TLBs
 *  flush_tlb_mm(mm) - flushes the specified mm context TLB's
 *  flush_tlb_page(vma, vmaddr) - flushes one page
 *  flush_tlb_range(vma, start, end) - flushes a range of pages
 *  flush_tlb_kernel_range(start, end) - flushes a range of kernel pages
 */

/*
 * flush_tlb_mm goes together with ptep_set_wrprotect for the
 * copy_page_range operation and flush_tlb_range is related to
 * ptep_get_and_clear for change_protection. ptep_set_wrprotect and
 * ptep_get_and_clear do not flush the TLBs directly if the mm has
 * only one user. At the end of the update the flush_tlb_mm and
 * flush_tlb_range functions need to do the flush.
 */
#define flush_tlb()				do { } while (0)
#define flush_tlb_all()				do { } while (0)
#define flush_tlb_page(vma, addr)		do { } while (0)

static inline void flush_tlb_mm(struct mm_struct *mm)
{
<<<<<<< HEAD
	__tlb_flush_mm_cond(mm);
=======
	__tlb_flush_mm_lazy(mm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void flush_tlb_range(struct vm_area_struct *vma,
				   unsigned long start, unsigned long end)
{
<<<<<<< HEAD
	__tlb_flush_mm_cond(vma->vm_mm);
=======
	__tlb_flush_mm_lazy(vma->vm_mm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void flush_tlb_kernel_range(unsigned long start,
					  unsigned long end)
{
<<<<<<< HEAD
	__tlb_flush_mm(&init_mm);
=======
	__tlb_flush_kernel();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* _S390_TLBFLUSH_H */
