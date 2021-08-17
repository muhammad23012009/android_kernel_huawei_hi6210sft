<<<<<<< HEAD
#ifndef __CAN_PLATFORM_MCP251X_H__
#define __CAN_PLATFORM_MCP251X_H__
=======
#ifndef _CAN_PLATFORM_MCP251X_H
#define _CAN_PLATFORM_MCP251X_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 *
 * CAN bus driver for Microchip 251x CAN Controller with SPI Interface
 *
 */

#include <linux/spi/spi.h>

<<<<<<< HEAD
/**
 * struct mcp251x_platform_data - MCP251X SPI CAN controller platform data
 * @oscillator_frequency:       - oscillator frequency in Hz
 * @irq_flags:                  - IRQF configuration flags
 * @board_specific_setup:       - called before probing the chip (power,reset)
 * @transceiver_enable:         - called to power on/off the transceiver
 * @power_enable:               - called to power on/off the mcp *and* the
 *                                transceiver
 *
 * Please note that you should define power_enable or transceiver_enable or
 * none of them. Defining both of them is no use.
 *
=======
/*
 * struct mcp251x_platform_data - MCP251X SPI CAN controller platform data
 * @oscillator_frequency:       - oscillator frequency in Hz
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

struct mcp251x_platform_data {
	unsigned long oscillator_frequency;
<<<<<<< HEAD
	unsigned long irq_flags;
	int (*board_specific_setup)(struct spi_device *spi);
	int (*transceiver_enable)(int enable);
	int (*power_enable) (int enable);
};

#endif /* __CAN_PLATFORM_MCP251X_H__ */
=======
};

#endif /* !_CAN_PLATFORM_MCP251X_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
