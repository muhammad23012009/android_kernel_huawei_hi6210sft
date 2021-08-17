/*
 *    Copyright IBM Corp. 2006
 *    Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#ifndef _ASM_S390_RESET_H
#define _ASM_S390_RESET_H

#include <linux/list.h>

struct reset_call {
	struct list_head list;
	void (*fn)(void);
};

extern void register_reset_call(struct reset_call *reset);
extern void unregister_reset_call(struct reset_call *reset);
<<<<<<< HEAD
extern void s390_reset_system(void (*func)(void *), void *data);
=======
extern void s390_reset_system(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _ASM_S390_RESET_H */
