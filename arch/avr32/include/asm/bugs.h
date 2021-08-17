/*
 * This is included by init/main.c to check for architecture-dependent bugs.
 *
 * Needs:
 *      void check_bugs(void);
 */
#ifndef __ASM_AVR32_BUGS_H
#define __ASM_AVR32_BUGS_H

static void __init check_bugs(void)
{
<<<<<<< HEAD
	cpu_data->loops_per_jiffy = loops_per_jiffy;
=======
	boot_cpu_data.loops_per_jiffy = loops_per_jiffy;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* __ASM_AVR32_BUGS_H */
