#ifndef __LINUX_KBUILD_H
#define __LINUX_KBUILD_H

#define DEFINE(sym, val) \
<<<<<<< HEAD
        asm volatile("\n->" #sym " %0 " #val : : "i" (val))

#define BLANK() asm volatile("\n->" : : )
=======
	asm volatile("\n.ascii \"->" #sym " %0 " #val "\"" : : "i" (val))

#define BLANK() asm volatile("\n.ascii \"->\"" : : )
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define OFFSET(sym, str, mem) \
	DEFINE(sym, offsetof(struct str, mem))

#define COMMENT(x) \
<<<<<<< HEAD
	asm volatile("\n->#" x)
=======
	asm volatile("\n.ascii \"->#" x "\"")
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
