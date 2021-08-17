#ifndef DECOMPRESS_UNLZMA_H
#define DECOMPRESS_UNLZMA_H

<<<<<<< HEAD
int unlzma(unsigned char *, int,
	   int(*fill)(void*, unsigned int),
	   int(*flush)(void*, unsigned int),
	   unsigned char *output,
	   int *posp,
=======
int unlzma(unsigned char *, long,
	   long (*fill)(void*, unsigned long),
	   long (*flush)(void*, unsigned long),
	   unsigned char *output,
	   long *posp,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	   void(*error)(char *x)
	);

#endif
