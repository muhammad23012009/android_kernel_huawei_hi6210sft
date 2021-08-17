/*
<<<<<<< HEAD
 *  dma.h --
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  ALSA PCM interface for the Samsung SoC
 */

<<<<<<< HEAD
#ifndef _S3C_AUDIO_H
#define _S3C_AUDIO_H

struct s3c_dma_params {
	struct s3c2410_dma_client *client;	/* stream identifier */
	int channel;				/* Channel ID */
	dma_addr_t dma_addr;
	int dma_size;			/* Size of the DMA transfer */
	unsigned ch;
	struct samsung_dma_ops *ops;
	char *ch_name;
};

int asoc_dma_platform_register(struct device *dev);
void asoc_dma_platform_unregister(struct device *dev);

#endif
=======
#ifndef _SAMSUNG_DMA_H
#define _SAMSUNG_DMA_H

#include <sound/dmaengine_pcm.h>

/*
 * @tx, @rx arguments can be NULL if the DMA channel names are "tx", "rx",
 * otherwise actual DMA channel names must be passed to this function.
 */
int samsung_asoc_dma_platform_register(struct device *dev, dma_filter_fn filter,
				       const char *tx, const char *rx);
#endif /* _SAMSUNG_DMA_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
