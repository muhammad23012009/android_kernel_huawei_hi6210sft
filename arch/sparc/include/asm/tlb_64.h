#ifndef _SPARC64_TLB_H
#define _SPARC64_TLB_H

#include <linux/swap.h>
#include <linux/pagemap.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#include <asm/mmu_context.h>

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void smp_flush_tlb_pending(struct mm_struct *,
=======
void smp_flush_tlb_pending(struct mm_struct *,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				  unsigned long, unsigned long *);
#endif

#ifdef CONFIG_SMP
<<<<<<< HEAD
extern void smp_flush_tlb_mm(struct mm_struct *mm);
=======
void smp_flush_tlb_mm(struct mm_struct *mm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define do_flush_tlb_mm(mm) smp_flush_tlb_mm(mm)
#else
#define do_flush_tlb_mm(mm) __flush_tlb_mm(CTX_HWBITS(mm->context), SECONDARY_CONTEXT)
#endif

<<<<<<< HEAD
extern void __flush_tlb_pending(unsigned long, unsigned long, unsigned long *);
extern void flush_tlb_pending(void);
=======
void __flush_tlb_pending(unsigned long, unsigned long, unsigned long *);
void flush_tlb_pending(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define tlb_start_vma(tlb, vma) do { } while (0)
#define tlb_end_vma(tlb, vma)	do { } while (0)
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)
#define tlb_flush(tlb)	flush_tlb_pending()

#include <asm-generic/tlb.h>

#endif /* _SPARC64_TLB_H */
