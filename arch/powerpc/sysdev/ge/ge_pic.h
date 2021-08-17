#ifndef __GEF_PIC_H__
#define __GEF_PIC_H__

<<<<<<< HEAD
#include <linux/init.h>

void gef_pic_cascade(unsigned int, struct irq_desc *);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
unsigned int gef_pic_get_irq(void);
void gef_pic_init(struct device_node *);

#endif /* __GEF_PIC_H__ */

