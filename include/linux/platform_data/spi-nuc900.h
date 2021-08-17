/*
<<<<<<< HEAD
 * arch/arm/mach-w90x900/include/mach/nuc900_spi.h
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (c) 2009 Nuvoton technology corporation.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

<<<<<<< HEAD
#ifndef __ASM_ARCH_SPI_H
#define __ASM_ARCH_SPI_H
=======
#ifndef __SPI_NUC900_H
#define __SPI_NUC900_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern void mfp_set_groupg(struct device *dev, const char *subname);

struct nuc900_spi_info {
	unsigned int num_cs;
	unsigned int lsb;
	unsigned int txneg;
	unsigned int rxneg;
	unsigned int divider;
	unsigned int sleep;
	unsigned int txnum;
	unsigned int txbitlen;
	int bus_num;
};

struct nuc900_spi_chip {
	unsigned char bits_per_word;
};

<<<<<<< HEAD
#endif /* __ASM_ARCH_SPI_H */
=======
#endif /* __SPI_NUC900_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
