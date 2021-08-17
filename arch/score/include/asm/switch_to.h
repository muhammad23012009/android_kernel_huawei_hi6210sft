#ifndef _ASM_SCORE_SWITCH_TO_H
#define _ASM_SCORE_SWITCH_TO_H

extern void *resume(void *last, void *next, void *next_ti);

#define switch_to(prev, next, last)				\
do {								\
	(last) = resume(prev, next, task_thread_info(next));	\
} while (0)

<<<<<<< HEAD
#define finish_arch_switch(prev)	do {} while (0)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _ASM_SCORE_SWITCH_TO_H */
