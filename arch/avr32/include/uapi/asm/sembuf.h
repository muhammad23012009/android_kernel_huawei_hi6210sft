<<<<<<< HEAD
#ifndef __ASM_AVR32_SEMBUF_H
#define __ASM_AVR32_SEMBUF_H
=======
#ifndef _UAPI__ASM_AVR32_SEMBUF_H
#define _UAPI__ASM_AVR32_SEMBUF_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
* The semid64_ds structure for AVR32 architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 64-bit time_t to solve y2038 problem
 * - 2 miscellaneous 32-bit values
 */

struct semid64_ds {
        struct ipc64_perm sem_perm;             /* permissions .. see ipc.h */
        __kernel_time_t sem_otime;              /* last semop time */
        unsigned long   __unused1;
        __kernel_time_t sem_ctime;              /* last change time */
        unsigned long   __unused2;
        unsigned long   sem_nsems;              /* no. of semaphores in array */
        unsigned long   __unused3;
        unsigned long   __unused4;
};

<<<<<<< HEAD
#endif /* __ASM_AVR32_SEMBUF_H */
=======
#endif /* _UAPI__ASM_AVR32_SEMBUF_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
