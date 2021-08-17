#ifndef LINUX_KERNEL_PAGE_FLAGS_H
#define LINUX_KERNEL_PAGE_FLAGS_H

#include <uapi/linux/kernel-page-flags.h>


/* kernel hacking assistances
 * WARNING: subject to change, never rely on them!
 */
#define KPF_RESERVED		32
#define KPF_MLOCKED		33
#define KPF_MAPPEDTODISK	34
#define KPF_PRIVATE		35
#define KPF_PRIVATE_2		36
#define KPF_OWNER_PRIVATE	37
#define KPF_ARCH		38
#define KPF_UNCACHED		39
<<<<<<< HEAD
=======
#define KPF_SOFTDIRTY		40
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* LINUX_KERNEL_PAGE_FLAGS_H */
