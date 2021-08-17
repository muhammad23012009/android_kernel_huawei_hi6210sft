#ifndef _ASM_ALPHA_PARAM_H
#define _ASM_ALPHA_PARAM_H

#include <uapi/asm/param.h>

<<<<<<< HEAD
#define HZ		CONFIG_HZ
#define USER_HZ		HZ
# define CLOCKS_PER_SEC	HZ	/* frequency at which times() counts */
=======
# undef HZ
# define HZ		CONFIG_HZ
# define USER_HZ	1024
# define CLOCKS_PER_SEC	USER_HZ	/* frequency at which times() counts */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _ASM_ALPHA_PARAM_H */
