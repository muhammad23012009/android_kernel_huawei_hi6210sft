;;; abs.S

#include <asm/linkage.h>

<<<<<<< HEAD
#if defined(__H8300H__) 
	.h8300h
#endif
#if defined(__H8300S__) 
	.h8300s
#endif
	.text
.global SYMBOL_NAME(abs)

;;; int abs(int n)
SYMBOL_NAME_LABEL(abs)
=======
#if defined(CONFIG_CPU_H8300H)
	.h8300h
#endif
#if defined(CONFIG_CPU_H8S)
	.h8300s
#endif
	.text
.global _abs

;;; int abs(int n)
_abs:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	mov.l	er0,er0
	bpl	1f
	neg.l	er0
1:
	rts
<<<<<<< HEAD
	
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
