<<<<<<< HEAD
#ifndef BOOT_ISDIGIT_H

#define BOOT_ISDIGIT_H
=======
#ifndef BOOT_CTYPE_H
#define BOOT_CTYPE_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline int isdigit(int ch)
{
	return (ch >= '0') && (ch <= '9');
}

static inline int isxdigit(int ch)
{
	if (isdigit(ch))
		return true;

	if ((ch >= 'a') && (ch <= 'f'))
		return true;

	return (ch >= 'A') && (ch <= 'F');
}

#endif
