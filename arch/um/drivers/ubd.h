/* 
 * Copyright (C) 2000 Jeff Dike (jdike@karaya.com)
 * Copyright (C) 2001 RidgeRun, Inc (glonnon@ridgerun.com)
 * Licensed under the GPL
 */

#ifndef __UM_UBD_USER_H
#define __UM_UBD_USER_H

<<<<<<< HEAD
extern void ignore_sigwinch_sig(void);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int start_io_thread(unsigned long sp, int *fds_out);
extern int io_thread(void *arg);
extern int kernel_fd;

#endif

