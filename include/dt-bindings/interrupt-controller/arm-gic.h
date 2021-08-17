/*
 * This header provides constants for the ARM GIC.
 */

#ifndef _DT_BINDINGS_INTERRUPT_CONTROLLER_ARM_GIC_H
#define _DT_BINDINGS_INTERRUPT_CONTROLLER_ARM_GIC_H

#include <dt-bindings/interrupt-controller/irq.h>

<<<<<<< HEAD
/* interrupt specific cell 0 */
=======
/* interrupt specifier cell 0 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define GIC_SPI 0
#define GIC_PPI 1

/*
 * Interrupt specifier cell 2.
<<<<<<< HEAD
 * The flaggs in irq.h are valid, plus those below.
=======
 * The flags in irq.h are valid, plus those below.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
#define GIC_CPU_MASK_RAW(x) ((x) << 8)
#define GIC_CPU_MASK_SIMPLE(num) GIC_CPU_MASK_RAW((1 << (num)) - 1)

#endif
