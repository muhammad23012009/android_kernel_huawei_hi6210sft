#ifndef _PARISC_MSGBUF_H
#define _PARISC_MSGBUF_H

<<<<<<< HEAD
=======
#include <asm/bitsperlong.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* 
 * The msqid64_ds structure for parisc architecture, copied from sparc.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 64-bit time_t to solve y2038 problem
 * - 2 miscellaneous 32-bit values
 */

struct msqid64_ds {
	struct ipc64_perm msg_perm;
<<<<<<< HEAD
#ifndef CONFIG_64BIT
	unsigned int   __pad1;
#endif
	__kernel_time_t msg_stime;	/* last msgsnd time */
#ifndef CONFIG_64BIT
	unsigned int   __pad2;
#endif
	__kernel_time_t msg_rtime;	/* last msgrcv time */
#ifndef CONFIG_64BIT
	unsigned int   __pad3;
#endif
	__kernel_time_t msg_ctime;	/* last change time */
	unsigned int  msg_cbytes;	/* current number of bytes on queue */
	unsigned int  msg_qnum;	/* number of messages in queue */
	unsigned int  msg_qbytes;	/* max number of bytes on queue */
	__kernel_pid_t msg_lspid;	/* pid of last msgsnd */
	__kernel_pid_t msg_lrpid;	/* last receive pid */
	unsigned int  __unused1;
	unsigned int  __unused2;
=======
#if __BITS_PER_LONG != 64
	unsigned int   __pad1;
#endif
	__kernel_time_t msg_stime;	/* last msgsnd time */
#if __BITS_PER_LONG != 64
	unsigned int   __pad2;
#endif
	__kernel_time_t msg_rtime;	/* last msgrcv time */
#if __BITS_PER_LONG != 64
	unsigned int   __pad3;
#endif
	__kernel_time_t msg_ctime;	/* last change time */
	unsigned long msg_cbytes;	/* current number of bytes on queue */
	unsigned long msg_qnum;		/* number of messages in queue */
	unsigned long msg_qbytes;	/* max number of bytes on queue */
	__kernel_pid_t msg_lspid;	/* pid of last msgsnd */
	__kernel_pid_t msg_lrpid;	/* last receive pid */
	unsigned long __unused1;
	unsigned long __unused2;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* _PARISC_MSGBUF_H */
