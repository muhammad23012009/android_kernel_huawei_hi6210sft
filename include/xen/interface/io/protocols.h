#ifndef __XEN_PROTOCOLS_H__
#define __XEN_PROTOCOLS_H__

#define XEN_IO_PROTO_ABI_X86_32     "x86_32-abi"
#define XEN_IO_PROTO_ABI_X86_64     "x86_64-abi"
<<<<<<< HEAD
#define XEN_IO_PROTO_ABI_IA64       "ia64-abi"
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define XEN_IO_PROTO_ABI_POWERPC64  "powerpc64-abi"
#define XEN_IO_PROTO_ABI_ARM        "arm-abi"

#if defined(__i386__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_X86_32
#elif defined(__x86_64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_X86_64
<<<<<<< HEAD
#elif defined(__ia64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_IA64
#elif defined(__powerpc64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_POWERPC64
#elif defined(__arm__)
=======
#elif defined(__powerpc64__)
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_POWERPC64
#elif defined(__arm__) || defined(__aarch64__)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
# define XEN_IO_PROTO_ABI_NATIVE XEN_IO_PROTO_ABI_ARM
#else
# error arch fixup needed here
#endif

#endif
