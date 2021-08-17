/*
 * linux/sound/arm/pxa2xx-pcm.h -- ALSA PCM interface for the Intel PXA2xx chip
 *
 * Author:	Nicolas Pitre
 * Created:	Nov 30, 2004
 * Copyright:	MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
#include <mach/dma.h>

struct pxa2xx_runtime_data {
	int dma_ch;
	struct pxa2xx_pcm_dma_params *params;
	pxa_dma_desc *dma_desc_array;
	dma_addr_t dma_desc_array_phys;
};

struct pxa2xx_pcm_client {
	struct pxa2xx_pcm_dma_params *playback_params;
	struct pxa2xx_pcm_dma_params *capture_params;
=======

struct pxa2xx_runtime_data {
	int dma_ch;
	struct snd_dmaengine_dai_dma_data *params;
};

struct pxa2xx_pcm_client {
	struct snd_dmaengine_dai_dma_data *playback_params;
	struct snd_dmaengine_dai_dma_data *capture_params;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int (*startup)(struct snd_pcm_substream *);
	void (*shutdown)(struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *);
};

extern int pxa2xx_pcm_new(struct snd_card *, struct pxa2xx_pcm_client *, struct snd_pcm **);

