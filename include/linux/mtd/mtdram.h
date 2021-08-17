#ifndef __MTD_MTDRAM_H__
#define __MTD_MTDRAM_H__

#include <linux/mtd/mtd.h>
int mtdram_init_device(struct mtd_info *mtd, void *mapped_address,
<<<<<<< HEAD
			unsigned long size, char *name);
=======
			unsigned long size, const char *name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __MTD_MTDRAM_H__ */
