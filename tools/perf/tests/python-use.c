/*
 * Just test if we can load the python binding.
 */

#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
=======
#include <linux/compiler.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "tests.h"

extern int verbose;

<<<<<<< HEAD
int test__python_use(void)
=======
int test__python_use(int subtest __maybe_unused)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	char *cmd;
	int ret;

	if (asprintf(&cmd, "echo \"import sys ; sys.path.append('%s'); import perf\" | %s %s",
		     PYTHONPATH, PYTHON, verbose ? "" : "2> /dev/null") < 0)
		return -1;

	ret = system(cmd) ? -1 : 0;
	free(cmd);
	return ret;
}
