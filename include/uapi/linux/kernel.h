#ifndef _UAPI_LINUX_KERNEL_H
#define _UAPI_LINUX_KERNEL_H

#include <linux/sysinfo.h>
<<<<<<< HEAD

/*
 * 'kernel.h' contains some often-used function prototypes etc
 */
#define __ALIGN_KERNEL(x, a)		__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))

=======
#include <linux/const.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _UAPI_LINUX_KERNEL_H */
