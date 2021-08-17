int pxa2xx_drv_pcmcia_add_one(struct soc_pcmcia_socket *skt);
void pxa2xx_drv_pcmcia_ops(struct pcmcia_low_level *ops);
<<<<<<< HEAD
void pxa2xx_configure_sockets(struct device *dev);
=======
void pxa2xx_configure_sockets(struct device *dev, struct pcmcia_low_level *ops);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

