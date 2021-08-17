/*
 * Generate definitions needed by assembly language modules.
 * This code generates raw asm output which is post-processed
 * to extract and format the required data.
 */

#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/thread_info.h>
#include <linux/kbuild.h>

void foo(void)
{
	OFFSET(TI_task, thread_info, task);
<<<<<<< HEAD
	OFFSET(TI_exec_domain, thread_info, exec_domain);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OFFSET(TI_flags, thread_info, flags);
	OFFSET(TI_cpu, thread_info, cpu);
	OFFSET(TI_preempt_count, thread_info, preempt_count);
	OFFSET(TI_rar_saved, thread_info, rar_saved);
	OFFSET(TI_rsr_saved, thread_info, rsr_saved);
<<<<<<< HEAD
	OFFSET(TI_restart_block, thread_info, restart_block);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	BLANK();
	OFFSET(TSK_active_mm, task_struct, active_mm);
	BLANK();
	OFFSET(MM_pgd, mm_struct, pgd);
}
