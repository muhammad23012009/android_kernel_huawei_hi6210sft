#include "misc.h"

<<<<<<< HEAD
#ifdef CONFIG_EARLY_PRINTK
=======
#if CONFIG_EARLY_PRINTK || CONFIG_RANDOMIZE_BASE
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static unsigned long fs;
static inline void set_fs(unsigned long seg)
{
	fs = seg << 4;  /* shift it back */
}
typedef unsigned long addr_t;
static inline char rdfs8(addr_t addr)
{
	return *((char *)(fs + addr));
}
#include "../cmdline.c"
static unsigned long get_cmd_line_ptr(void)
{
<<<<<<< HEAD
	unsigned long cmd_line_ptr = real_mode->hdr.cmd_line_ptr;

	cmd_line_ptr |= (u64)real_mode->ext_cmd_line_ptr << 32;
=======
	unsigned long cmd_line_ptr = boot_params->hdr.cmd_line_ptr;

	cmd_line_ptr |= (u64)boot_params->ext_cmd_line_ptr << 32;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return cmd_line_ptr;
}
int cmdline_find_option(const char *option, char *buffer, int bufsize)
{
	return __cmdline_find_option(get_cmd_line_ptr(), option, buffer, bufsize);
}
int cmdline_find_option_bool(const char *option)
{
	return __cmdline_find_option_bool(get_cmd_line_ptr(), option);
}

#endif
