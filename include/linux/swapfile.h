#ifndef _LINUX_SWAPFILE_H
#define _LINUX_SWAPFILE_H

/*
 * these were static in swapfile.c but frontswap.c needs them and we don't
 * want to expose them to the dozens of source files that include swap.h
 */
extern spinlock_t swap_lock;
<<<<<<< HEAD
extern struct swap_list_t swap_list;
extern struct swap_info_struct *swap_info[];
extern int try_to_unuse(unsigned int, bool, unsigned long);
=======
extern struct plist_head swap_active_head;
extern struct swap_info_struct *swap_info[];
extern int try_to_unuse(unsigned int, bool, unsigned long);
extern unsigned long generic_max_swapfile_size(void);
extern unsigned long max_swapfile_size(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _LINUX_SWAPFILE_H */
