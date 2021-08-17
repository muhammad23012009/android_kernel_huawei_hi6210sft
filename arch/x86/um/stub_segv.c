/*
 * Copyright (C) 2004 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <sysdep/stub.h>
#include <sysdep/faultinfo.h>
#include <sysdep/mcontext.h>
<<<<<<< HEAD
=======
#include <sys/ucontext.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void __attribute__ ((__section__ (".__syscall_stub")))
stub_segv_handler(int sig, siginfo_t *info, void *p)
{
<<<<<<< HEAD
	struct ucontext *uc = p;
=======
	ucontext_t *uc = p;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	GET_FAULTINFO_FROM_MC(*((struct faultinfo *) STUB_DATA),
			      &uc->uc_mcontext);
	trap_myself();
}

