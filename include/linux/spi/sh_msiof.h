#ifndef __SPI_SH_MSIOF_H__
#define __SPI_SH_MSIOF_H__

struct sh_msiof_spi_info {
	int tx_fifo_override;
	int rx_fifo_override;
	u16 num_chipselect;
<<<<<<< HEAD
=======
	unsigned int dma_tx_id;
	unsigned int dma_rx_id;
	u32 dtdl;
	u32 syncdl;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* __SPI_SH_MSIOF_H__ */
