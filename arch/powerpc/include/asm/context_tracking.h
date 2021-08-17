#ifndef _ASM_POWERPC_CONTEXT_TRACKING_H
#define _ASM_POWERPC_CONTEXT_TRACKING_H

#ifdef CONFIG_CONTEXT_TRACKING
<<<<<<< HEAD
#define SCHEDULE_USER bl	.schedule_user
#else
#define SCHEDULE_USER bl	.schedule
=======
#define SCHEDULE_USER bl	schedule_user
#else
#define SCHEDULE_USER bl	schedule
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#endif
