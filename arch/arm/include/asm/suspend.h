#ifndef __ASM_ARM_SUSPEND_H
#define __ASM_ARM_SUSPEND_H

<<<<<<< HEAD
extern void cpu_resume(void);
=======
struct sleep_save_sp {
	u32 *save_ptr_stash;
	u32 save_ptr_stash_phys;
};

extern void cpu_resume(void);
extern void cpu_resume_no_hyp(void);
extern void cpu_resume_arm(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int cpu_suspend(unsigned long, int (*)(unsigned long));

#endif
