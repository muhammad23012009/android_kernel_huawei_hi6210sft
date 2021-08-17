#ifndef _ADDI_WATCHDOG_H
#define _ADDI_WATCHDOG_H

<<<<<<< HEAD
#include "../comedidev.h"
=======
struct comedi_subdevice;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void addi_watchdog_reset(unsigned long iobase);
int addi_watchdog_init(struct comedi_subdevice *, unsigned long iobase);

#endif
