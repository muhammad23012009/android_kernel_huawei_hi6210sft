<<<<<<< HEAD
#ifndef INFLATE_H
#define INFLATE_H

int gunzip(unsigned char *inbuf, int len,
	   int(*fill)(void*, unsigned int),
	   int(*flush)(void*, unsigned int),
	   unsigned char *output,
	   int *pos,
=======
#ifndef LINUX_DECOMPRESS_INFLATE_H
#define LINUX_DECOMPRESS_INFLATE_H

int gunzip(unsigned char *inbuf, long len,
	   long (*fill)(void*, unsigned long),
	   long (*flush)(void*, unsigned long),
	   unsigned char *output,
	   long *pos,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	   void(*error_fn)(char *x));
#endif
