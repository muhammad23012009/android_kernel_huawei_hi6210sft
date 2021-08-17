/*
 *  (C) 2011 Thomas Renninger <trenn@suse.de>, Novell Inc.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

<<<<<<< HEAD
#include <cpufreq.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "helpers/helpers.h"
#include "helpers/sysfs.h"
#include "helpers/bitmask.h"

static struct option set_opts[] = {
<<<<<<< HEAD
	{ .name = "perf-bias",	.has_arg = required_argument,	.flag = NULL,	.val = 'b'},
	{ .name = "sched-mc",	.has_arg = required_argument,	.flag = NULL,	.val = 'm'},
	{ .name = "sched-smt",	.has_arg = required_argument,	.flag = NULL,	.val = 's'},
=======
	{"perf-bias", required_argument, NULL, 'b'},
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ },
};

static void print_wrong_arg_exit(void)
{
	printf(_("invalid or unknown argument\n"));
	exit(EXIT_FAILURE);
}

int cmd_set(int argc, char **argv)
{
	extern char *optarg;
	extern int optind, opterr, optopt;
	unsigned int cpu;

	union {
		struct {
<<<<<<< HEAD
			int sched_mc:1;
			int sched_smt:1;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			int perf_bias:1;
		};
		int params;
	} params;
<<<<<<< HEAD
	int sched_mc = 0, sched_smt = 0, perf_bias = 0;
=======
	int perf_bias = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret = 0;

	setlocale(LC_ALL, "");
	textdomain(PACKAGE);

	params.params = 0;
	/* parameter parsing */
<<<<<<< HEAD
	while ((ret = getopt_long(argc, argv, "m:s:b:",
=======
	while ((ret = getopt_long(argc, argv, "b:",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						set_opts, NULL)) != -1) {
		switch (ret) {
		case 'b':
			if (params.perf_bias)
				print_wrong_arg_exit();
			perf_bias = atoi(optarg);
			if (perf_bias < 0 || perf_bias > 15) {
				printf(_("--perf-bias param out "
					 "of range [0-%d]\n"), 15);
				print_wrong_arg_exit();
			}
			params.perf_bias = 1;
			break;
<<<<<<< HEAD
		case 'm':
			if (params.sched_mc)
				print_wrong_arg_exit();
			sched_mc = atoi(optarg);
			if (sched_mc < 0 || sched_mc > 2) {
				printf(_("--sched-mc param out "
					 "of range [0-%d]\n"), 2);
				print_wrong_arg_exit();
			}
			params.sched_mc = 1;
			break;
		case 's':
			if (params.sched_smt)
				print_wrong_arg_exit();
			sched_smt = atoi(optarg);
			if (sched_smt < 0 || sched_smt > 2) {
				printf(_("--sched-smt param out "
					 "of range [0-%d]\n"), 2);
				print_wrong_arg_exit();
			}
			params.sched_smt = 1;
			break;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		default:
			print_wrong_arg_exit();
		}
	};

	if (!params.params)
		print_wrong_arg_exit();

<<<<<<< HEAD
	if (params.sched_mc) {
		ret = sysfs_set_sched("mc", sched_mc);
		if (ret)
			fprintf(stderr, _("Error setting sched-mc %s\n"),
				(ret == -ENODEV) ? "not supported" : "");
	}
	if (params.sched_smt) {
		ret = sysfs_set_sched("smt", sched_smt);
		if (ret)
			fprintf(stderr, _("Error setting sched-smt %s\n"),
				(ret == -ENODEV) ? "not supported" : "");
	}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Default is: set all CPUs */
	if (bitmask_isallclear(cpus_chosen))
		bitmask_setall(cpus_chosen);

	/* loop over CPUs */
	for (cpu = bitmask_first(cpus_chosen);
	     cpu <= bitmask_last(cpus_chosen); cpu++) {

<<<<<<< HEAD
		if (!bitmask_isbitset(cpus_chosen, cpu) ||
		    cpufreq_cpu_exists(cpu))
			continue;

=======
		if (!bitmask_isbitset(cpus_chosen, cpu))
			continue;

		if (sysfs_is_cpu_online(cpu) != 1){
			fprintf(stderr, _("Cannot set values on CPU %d:"), cpu);
			fprintf(stderr, _(" *is offline\n"));
			continue;
		}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (params.perf_bias) {
			ret = msr_intel_set_perf_bias(cpu, perf_bias);
			if (ret) {
				fprintf(stderr, _("Error setting perf-bias "
						  "value on CPU %d\n"), cpu);
				break;
			}
		}
	}
	return ret;
}
