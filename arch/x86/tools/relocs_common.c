#include "relocs.h"

void die(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

static void usage(void)
{
<<<<<<< HEAD
	die("relocs [--abs-syms|--abs-relocs|--text|--realmode] vmlinux\n");
=======
	die("relocs [--abs-syms|--abs-relocs|--reloc-info|--text|--realmode]" \
	    " vmlinux\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	int show_absolute_syms, show_absolute_relocs;
=======
	int show_absolute_syms, show_absolute_relocs, show_reloc_info;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int as_text, use_real_mode;
	const char *fname;
	FILE *fp;
	int i;
	unsigned char e_ident[EI_NIDENT];

	show_absolute_syms = 0;
	show_absolute_relocs = 0;
<<<<<<< HEAD
=======
	show_reloc_info = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	as_text = 0;
	use_real_mode = 0;
	fname = NULL;
	for (i = 1; i < argc; i++) {
		char *arg = argv[i];
		if (*arg == '-') {
			if (strcmp(arg, "--abs-syms") == 0) {
				show_absolute_syms = 1;
				continue;
			}
			if (strcmp(arg, "--abs-relocs") == 0) {
				show_absolute_relocs = 1;
				continue;
			}
<<<<<<< HEAD
=======
			if (strcmp(arg, "--reloc-info") == 0) {
				show_reloc_info = 1;
				continue;
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (strcmp(arg, "--text") == 0) {
				as_text = 1;
				continue;
			}
			if (strcmp(arg, "--realmode") == 0) {
				use_real_mode = 1;
				continue;
			}
		}
		else if (!fname) {
			fname = arg;
			continue;
		}
		usage();
	}
	if (!fname) {
		usage();
	}
	fp = fopen(fname, "r");
	if (!fp) {
		die("Cannot open %s: %s\n", fname, strerror(errno));
	}
	if (fread(&e_ident, 1, EI_NIDENT, fp) != EI_NIDENT) {
		die("Cannot read %s: %s", fname, strerror(errno));
	}
	rewind(fp);
	if (e_ident[EI_CLASS] == ELFCLASS64)
		process_64(fp, use_real_mode, as_text,
<<<<<<< HEAD
			   show_absolute_syms, show_absolute_relocs);
	else
		process_32(fp, use_real_mode, as_text,
			   show_absolute_syms, show_absolute_relocs);
=======
			   show_absolute_syms, show_absolute_relocs,
			   show_reloc_info);
	else
		process_32(fp, use_real_mode, as_text,
			   show_absolute_syms, show_absolute_relocs,
			   show_reloc_info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	fclose(fp);
	return 0;
}
