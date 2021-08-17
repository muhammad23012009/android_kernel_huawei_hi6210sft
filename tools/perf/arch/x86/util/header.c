#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../util/header.h"

static inline void
cpuid(unsigned int op, unsigned int *a, unsigned int *b, unsigned int *c,
      unsigned int *d)
{
	__asm__ __volatile__ (".byte 0x53\n\tcpuid\n\t"
			      "movl %%ebx, %%esi\n\t.byte 0x5b"
			: "=a" (*a),
			"=S" (*b),
			"=c" (*c),
			"=d" (*d)
			: "a" (op));
}

<<<<<<< HEAD
int
get_cpuid(char *buffer, size_t sz)
=======
static int
__get_cpuid(char *buffer, size_t sz, const char *fmt)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned int a, b, c, d, lvl;
	int family = -1, model = -1, step = -1;
	int nb;
	char vendor[16];

	cpuid(0, &lvl, &b, &c, &d);
	strncpy(&vendor[0], (char *)(&b), 4);
	strncpy(&vendor[4], (char *)(&d), 4);
	strncpy(&vendor[8], (char *)(&c), 4);
	vendor[12] = '\0';

	if (lvl >= 1) {
		cpuid(1, &a, &b, &c, &d);

		family = (a >> 8) & 0xf;  /* bits 11 - 8 */
		model  = (a >> 4) & 0xf;  /* Bits  7 - 4 */
		step   = a & 0xf;

		/* extended family */
		if (family == 0xf)
			family += (a >> 20) & 0xff;

		/* extended model */
		if (family >= 0x6)
			model += ((a >> 16) & 0xf) << 4;
	}
<<<<<<< HEAD
	nb = scnprintf(buffer, sz, "%s,%u,%u,%u$", vendor, family, model, step);
=======
	nb = scnprintf(buffer, sz, fmt, vendor, family, model, step);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* look for end marker to ensure the entire data fit */
	if (strchr(buffer, '$')) {
		buffer[nb-1] = '\0';
		return 0;
	}
	return -1;
}
<<<<<<< HEAD
=======

int
get_cpuid(char *buffer, size_t sz)
{
	return __get_cpuid(buffer, sz, "%s,%u,%u,%u$");
}

char *
get_cpuid_str(void)
{
	char *buf = malloc(128);

	if (buf && __get_cpuid(buf, 128, "%s-%u-%X$") < 0) {
		free(buf);
		return NULL;
	}
	return buf;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
