#ifndef _LINUX_IRQRETURN_H
#define _LINUX_IRQRETURN_H

/**
 * enum irqreturn
<<<<<<< HEAD
 * @IRQ_NONE		interrupt was not from this device
=======
 * @IRQ_NONE		interrupt was not from this device or was not handled
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @IRQ_HANDLED		interrupt was handled by this device
 * @IRQ_WAKE_THREAD	handler requests to wake the handler thread
 */
enum irqreturn {
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};

typedef enum irqreturn irqreturn_t;
<<<<<<< HEAD
#define IRQ_RETVAL(x)	((x) != IRQ_NONE)
=======
#define IRQ_RETVAL(x)	((x) ? IRQ_HANDLED : IRQ_NONE)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
