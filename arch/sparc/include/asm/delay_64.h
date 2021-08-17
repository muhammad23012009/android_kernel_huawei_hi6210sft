/* delay.h: Linux delay routines on sparc64.
 *
 * Copyright (C) 1996, 2004, 2007 David S. Miller (davem@davemloft.net).
 */

#ifndef _SPARC64_DELAY_H
#define _SPARC64_DELAY_H

#ifndef __ASSEMBLY__

<<<<<<< HEAD
extern void __delay(unsigned long loops);
extern void udelay(unsigned long usecs);
=======
void __delay(unsigned long loops);
void udelay(unsigned long usecs);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define mdelay(n)	udelay((n) * 1000)

#endif /* !__ASSEMBLY__ */

#endif /* _SPARC64_DELAY_H */
