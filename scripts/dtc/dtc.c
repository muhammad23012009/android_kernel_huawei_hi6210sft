/*
 * (C) Copyright David Gibson <dwg@au1.ibm.com>, IBM Corporation.  2005.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *                                                                   USA
 */

<<<<<<< HEAD
#include "dtc.h"
#include "srcpos.h"

#include "version_gen.h"

=======
#include <sys/stat.h>

#include "dtc.h"
#include "srcpos.h"

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Command line options
 */
int quiet;		/* Level of quietness */
int reservenum;		/* Number of memory reservation slots */
int minsize;		/* Minimum blob size */
int padsize;		/* Additional padding to blob */
int phandle_format = PHANDLE_BOTH;	/* Use linux,phandle or phandle properties */

static void fill_fullpaths(struct node *tree, const char *prefix)
{
	struct node *child;
	const char *unit;

	tree->fullpath = join_path(prefix, tree->name);

	unit = strchr(tree->name, '@');
	if (unit)
		tree->basenamelen = unit - tree->name;
	else
		tree->basenamelen = strlen(tree->name);

	for_each_child(tree, child)
		fill_fullpaths(child, tree->fullpath);
}

<<<<<<< HEAD
static void  __attribute__ ((noreturn)) usage(void)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "\tdtc [options] <input file>\n");
	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, "\t-h\n");
	fprintf(stderr, "\t\tThis help text\n");
	fprintf(stderr, "\t-q\n");
	fprintf(stderr, "\t\tQuiet: -q suppress warnings, -qq errors, -qqq all\n");
	fprintf(stderr, "\t-I <input format>\n");
	fprintf(stderr, "\t\tInput formats are:\n");
	fprintf(stderr, "\t\t\tdts - device tree source text\n");
	fprintf(stderr, "\t\t\tdtb - device tree blob\n");
	fprintf(stderr, "\t\t\tfs - /proc/device-tree style directory\n");
	fprintf(stderr, "\t-o <output file>\n");
	fprintf(stderr, "\t-O <output format>\n");
	fprintf(stderr, "\t\tOutput formats are:\n");
	fprintf(stderr, "\t\t\tdts - device tree source text\n");
	fprintf(stderr, "\t\t\tdtb - device tree blob\n");
	fprintf(stderr, "\t\t\tasm - assembler source\n");
	fprintf(stderr, "\t-V <output version>\n");
	fprintf(stderr, "\t\tBlob version to produce, defaults to %d (relevant for dtb\n\t\tand asm output only)\n", DEFAULT_FDT_VERSION);
	fprintf(stderr, "\t-d <output dependency file>\n");
	fprintf(stderr, "\t-R <number>\n");
	fprintf(stderr, "\t\tMake space for <number> reserve map entries (relevant for \n\t\tdtb and asm output only)\n");
	fprintf(stderr, "\t-S <bytes>\n");
	fprintf(stderr, "\t\tMake the blob at least <bytes> long (extra space)\n");
	fprintf(stderr, "\t-p <bytes>\n");
	fprintf(stderr, "\t\tAdd padding to the blob of <bytes> long (extra space)\n");
	fprintf(stderr, "\t-b <number>\n");
	fprintf(stderr, "\t\tSet the physical boot cpu\n");
	fprintf(stderr, "\t-f\n");
	fprintf(stderr, "\t\tForce - try to produce output even if the input tree has errors\n");
	fprintf(stderr, "\t-i\n");
	fprintf(stderr, "\t\tAdd a path to search for include files\n");
	fprintf(stderr, "\t-s\n");
	fprintf(stderr, "\t\tSort nodes and properties before outputting (only useful for\n\t\tcomparing trees)\n");
	fprintf(stderr, "\t-v\n");
	fprintf(stderr, "\t\tPrint DTC version and exit\n");
	fprintf(stderr, "\t-H <phandle format>\n");
	fprintf(stderr, "\t\tphandle formats are:\n");
	fprintf(stderr, "\t\t\tlegacy - \"linux,phandle\" properties only\n");
	fprintf(stderr, "\t\t\tepapr - \"phandle\" properties only\n");
	fprintf(stderr, "\t\t\tboth - Both \"linux,phandle\" and \"phandle\" properties\n");
	fprintf(stderr, "\t-W [no-]<checkname>\n");
	fprintf(stderr, "\t-E [no-]<checkname>\n");
	fprintf(stderr, "\t\t\tenable or disable warnings and errors\n");
	exit(3);
=======
/* Usage related data. */
#define FDT_VERSION(version)	_FDT_VERSION(version)
#define _FDT_VERSION(version)	#version
static const char usage_synopsis[] = "dtc [options] <input file>";
static const char usage_short_opts[] = "qI:O:o:V:d:R:S:p:fb:i:H:sW:E:hv";
static struct option const usage_long_opts[] = {
	{"quiet",            no_argument, NULL, 'q'},
	{"in-format",         a_argument, NULL, 'I'},
	{"out",               a_argument, NULL, 'o'},
	{"out-format",        a_argument, NULL, 'O'},
	{"out-version",       a_argument, NULL, 'V'},
	{"out-dependency",    a_argument, NULL, 'd'},
	{"reserve",           a_argument, NULL, 'R'},
	{"space",             a_argument, NULL, 'S'},
	{"pad",               a_argument, NULL, 'p'},
	{"boot-cpu",          a_argument, NULL, 'b'},
	{"force",            no_argument, NULL, 'f'},
	{"include",           a_argument, NULL, 'i'},
	{"sort",             no_argument, NULL, 's'},
	{"phandle",           a_argument, NULL, 'H'},
	{"warning",           a_argument, NULL, 'W'},
	{"error",             a_argument, NULL, 'E'},
	{"help",             no_argument, NULL, 'h'},
	{"version",          no_argument, NULL, 'v'},
	{NULL,               no_argument, NULL, 0x0},
};
static const char * const usage_opts_help[] = {
	"\n\tQuiet: -q suppress warnings, -qq errors, -qqq all",
	"\n\tInput formats are:\n"
	 "\t\tdts - device tree source text\n"
	 "\t\tdtb - device tree blob\n"
	 "\t\tfs  - /proc/device-tree style directory",
	"\n\tOutput file",
	"\n\tOutput formats are:\n"
	 "\t\tdts - device tree source text\n"
	 "\t\tdtb - device tree blob\n"
	 "\t\tasm - assembler source",
	"\n\tBlob version to produce, defaults to "FDT_VERSION(DEFAULT_FDT_VERSION)" (for dtb and asm output)",
	"\n\tOutput dependency file",
	"\n\tMake space for <number> reserve map entries (for dtb and asm output)",
	"\n\tMake the blob at least <bytes> long (extra space)",
	"\n\tAdd padding to the blob of <bytes> long (extra space)",
	"\n\tSet the physical boot cpu",
	"\n\tTry to produce output even if the input tree has errors",
	"\n\tAdd a path to search for include files",
	"\n\tSort nodes and properties before outputting (useful for comparing trees)",
	"\n\tValid phandle formats are:\n"
	 "\t\tlegacy - \"linux,phandle\" properties only\n"
	 "\t\tepapr  - \"phandle\" properties only\n"
	 "\t\tboth   - Both \"linux,phandle\" and \"phandle\" properties",
	"\n\tEnable/disable warnings (prefix with \"no-\")",
	"\n\tEnable/disable errors (prefix with \"no-\")",
	"\n\tPrint this help and exit",
	"\n\tPrint version and exit",
	NULL,
};

static const char *guess_type_by_name(const char *fname, const char *fallback)
{
	const char *s;

	s = strrchr(fname, '.');
	if (s == NULL)
		return fallback;
	if (!strcasecmp(s, ".dts"))
		return "dts";
	if (!strcasecmp(s, ".dtb"))
		return "dtb";
	return fallback;
}

static const char *guess_input_format(const char *fname, const char *fallback)
{
	struct stat statbuf;
	uint32_t magic;
	FILE *f;

	if (stat(fname, &statbuf) != 0)
		return fallback;

	if (S_ISDIR(statbuf.st_mode))
		return "fs";

	if (!S_ISREG(statbuf.st_mode))
		return fallback;

	f = fopen(fname, "r");
	if (f == NULL)
		return fallback;
	if (fread(&magic, 4, 1, f) != 1) {
		fclose(f);
		return fallback;
	}
	fclose(f);

	magic = fdt32_to_cpu(magic);
	if (magic == FDT_MAGIC)
		return "dtb";

	return guess_type_by_name(fname, fallback);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int main(int argc, char *argv[])
{
	struct boot_info *bi;
<<<<<<< HEAD
	const char *inform = "dts";
	const char *outform = "dts";
	const char *outname = "-";
	const char *depname = NULL;
	int force = 0, sort = 0;
=======
	const char *inform = NULL;
	const char *outform = NULL;
	const char *outname = "-";
	const char *depname = NULL;
	bool force = false, sort = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char *arg;
	int opt;
	FILE *outf = NULL;
	int outversion = DEFAULT_FDT_VERSION;
	long long cmdline_boot_cpuid = -1;

	quiet      = 0;
	reservenum = 0;
	minsize    = 0;
	padsize    = 0;

<<<<<<< HEAD
	while ((opt = getopt(argc, argv, "hI:O:o:V:d:R:S:p:fqb:i:vH:sW:E:"))
			!= EOF) {
=======
	while ((opt = util_getopt_long()) != EOF) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		switch (opt) {
		case 'I':
			inform = optarg;
			break;
		case 'O':
			outform = optarg;
			break;
		case 'o':
			outname = optarg;
			break;
		case 'V':
			outversion = strtol(optarg, NULL, 0);
			break;
		case 'd':
			depname = optarg;
			break;
		case 'R':
			reservenum = strtol(optarg, NULL, 0);
			break;
		case 'S':
			minsize = strtol(optarg, NULL, 0);
			break;
		case 'p':
			padsize = strtol(optarg, NULL, 0);
			break;
		case 'f':
<<<<<<< HEAD
			force = 1;
=======
			force = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;
		case 'q':
			quiet++;
			break;
		case 'b':
			cmdline_boot_cpuid = strtoll(optarg, NULL, 0);
			break;
		case 'i':
			srcfile_add_search_path(optarg);
			break;
		case 'v':
<<<<<<< HEAD
			printf("Version: %s\n", DTC_VERSION);
			exit(0);
=======
			util_version();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case 'H':
			if (streq(optarg, "legacy"))
				phandle_format = PHANDLE_LEGACY;
			else if (streq(optarg, "epapr"))
				phandle_format = PHANDLE_EPAPR;
			else if (streq(optarg, "both"))
				phandle_format = PHANDLE_BOTH;
			else
				die("Invalid argument \"%s\" to -H option\n",
				    optarg);
			break;

		case 's':
<<<<<<< HEAD
			sort = 1;
=======
			sort = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			break;

		case 'W':
			parse_checks_option(true, false, optarg);
			break;

		case 'E':
			parse_checks_option(false, true, optarg);
			break;

		case 'h':
<<<<<<< HEAD
		default:
			usage();
=======
			usage(NULL);
		default:
			usage("unknown option");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	if (argc > (optind+1))
<<<<<<< HEAD
		usage();
=======
		usage("missing files");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else if (argc < (optind+1))
		arg = "-";
	else
		arg = argv[optind];

	/* minsize and padsize are mutually exclusive */
	if (minsize && padsize)
		die("Can't set both -p and -S\n");

<<<<<<< HEAD
	if (minsize)
		fprintf(stderr, "DTC: Use of \"-S\" is deprecated; it will be removed soon, use \"-p\" instead\n");

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (depname) {
		depfile = fopen(depname, "w");
		if (!depfile)
			die("Couldn't open dependency file %s: %s\n", depname,
			    strerror(errno));
		fprintf(depfile, "%s:", outname);
	}

<<<<<<< HEAD
=======
	if (inform == NULL)
		inform = guess_input_format(arg, "dts");
	if (outform == NULL) {
		outform = guess_type_by_name(outname, NULL);
		if (outform == NULL) {
			if (streq(inform, "dts"))
				outform = "dtb";
			else
				outform = "dts";
		}
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (streq(inform, "dts"))
		bi = dt_from_source(arg);
	else if (streq(inform, "fs"))
		bi = dt_from_fs(arg);
	else if(streq(inform, "dtb"))
		bi = dt_from_blob(arg);
	else
		die("Unknown input format \"%s\"\n", inform);

	if (depfile) {
		fputc('\n', depfile);
		fclose(depfile);
	}

	if (cmdline_boot_cpuid != -1)
		bi->boot_cpuid_phys = cmdline_boot_cpuid;

	fill_fullpaths(bi->dt, "");
	process_checks(force, bi);

	if (sort)
		sort_tree(bi);

	if (streq(outname, "-")) {
		outf = stdout;
	} else {
<<<<<<< HEAD
		outf = fopen(outname, "w");
=======
		outf = fopen(outname, "wb");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (! outf)
			die("Couldn't open output file %s: %s\n",
			    outname, strerror(errno));
	}

	if (streq(outform, "dts")) {
		dt_to_source(outf, bi);
	} else if (streq(outform, "dtb")) {
		dt_to_blob(outf, bi, outversion);
	} else if (streq(outform, "asm")) {
		dt_to_asm(outf, bi, outversion);
	} else if (streq(outform, "null")) {
		/* do nothing */
	} else {
		die("Unknown output format \"%s\"\n", outform);
	}

	exit(0);
}
