#ifndef _TDA18271C2DD_H_
#define _TDA18271C2DD_H_

<<<<<<< HEAD
#include <linux/kconfig.h>

#if IS_ENABLED(CONFIG_DVB_TDA18271C2DD)
=======
#if IS_REACHABLE(CONFIG_DVB_TDA18271C2DD)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct dvb_frontend *tda18271c2dd_attach(struct dvb_frontend *fe,
					 struct i2c_adapter *i2c, u8 adr);
#else
static inline struct dvb_frontend *tda18271c2dd_attach(struct dvb_frontend *fe,
					 struct i2c_adapter *i2c, u8 adr)
{
        printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
        return NULL;
}
#endif

#endif
