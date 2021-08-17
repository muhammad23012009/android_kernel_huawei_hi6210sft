#include <stdio.h>
#include <sys/utsname.h>
#include "common.h"
<<<<<<< HEAD
=======
#include "../util/util.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "../util/debug.h"

const char *const arm_triplets[] = {
	"arm-eabi-",
	"arm-linux-androideabi-",
	"arm-unknown-linux-",
	"arm-unknown-linux-gnu-",
	"arm-unknown-linux-gnueabi-",
<<<<<<< HEAD
=======
	"arm-linux-gnu-",
	"arm-linux-gnueabihf-",
	"arm-none-eabi-",
	NULL
};

const char *const arm64_triplets[] = {
	"aarch64-linux-android-",
	"aarch64-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

const char *const powerpc_triplets[] = {
	"powerpc-unknown-linux-gnu-",
	"powerpc64-unknown-linux-gnu-",
<<<<<<< HEAD
=======
	"powerpc64-linux-gnu-",
	"powerpc64le-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

const char *const s390_triplets[] = {
	"s390-ibm-linux-",
<<<<<<< HEAD
=======
	"s390x-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

const char *const sh_triplets[] = {
	"sh-unknown-linux-gnu-",
	"sh64-unknown-linux-gnu-",
<<<<<<< HEAD
=======
	"sh-linux-gnu-",
	"sh64-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

const char *const sparc_triplets[] = {
	"sparc-unknown-linux-gnu-",
	"sparc64-unknown-linux-gnu-",
<<<<<<< HEAD
=======
	"sparc64-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

const char *const x86_triplets[] = {
	"x86_64-pc-linux-gnu-",
	"x86_64-unknown-linux-gnu-",
	"i686-pc-linux-gnu-",
	"i586-pc-linux-gnu-",
	"i486-pc-linux-gnu-",
	"i386-pc-linux-gnu-",
	"i686-linux-android-",
	"i686-android-linux-",
<<<<<<< HEAD
=======
	"x86_64-linux-gnu-",
	"i586-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

const char *const mips_triplets[] = {
	"mips-unknown-linux-gnu-",
	"mipsel-linux-android-",
<<<<<<< HEAD
=======
	"mips-linux-gnu-",
	"mips64-linux-gnu-",
	"mips64el-linux-gnuabi64-",
	"mips64-linux-gnuabi64-",
	"mipsel-linux-gnu-",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	NULL
};

static bool lookup_path(char *name)
{
	bool found = false;
<<<<<<< HEAD
	char *path, *tmp;
=======
	char *path, *tmp = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char buf[PATH_MAX];
	char *env = getenv("PATH");

	if (!env)
		return false;

	env = strdup(env);
	if (!env)
		return false;

	path = strtok_r(env, ":", &tmp);
	while (path) {
		scnprintf(buf, sizeof(buf), "%s/%s", path, name);
		if (access(buf, F_OK) == 0) {
			found = true;
			break;
		}
		path = strtok_r(NULL, ":", &tmp);
	}
	free(env);
	return found;
}

static int lookup_triplets(const char *const *triplets, const char *name)
{
	int i;
	char buf[PATH_MAX];

	for (i = 0; triplets[i] != NULL; i++) {
		scnprintf(buf, sizeof(buf), "%s%s", triplets[i], name);
		if (lookup_path(buf))
			return i;
	}
	return -1;
}

/*
 * Return architecture name in a normalized form.
 * The conversion logic comes from the Makefile.
 */
<<<<<<< HEAD
static const char *normalize_arch(char *arch)
=======
const char *normalize_arch(char *arch)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (!strcmp(arch, "x86_64"))
		return "x86";
	if (arch[0] == 'i' && arch[2] == '8' && arch[3] == '6')
		return "x86";
	if (!strcmp(arch, "sun4u") || !strncmp(arch, "sparc", 5))
		return "sparc";
<<<<<<< HEAD
=======
	if (!strcmp(arch, "aarch64") || !strcmp(arch, "arm64"))
		return "arm64";
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!strncmp(arch, "arm", 3) || !strcmp(arch, "sa110"))
		return "arm";
	if (!strncmp(arch, "s390", 4))
		return "s390";
	if (!strncmp(arch, "parisc", 6))
		return "parisc";
	if (!strncmp(arch, "powerpc", 7) || !strncmp(arch, "ppc", 3))
		return "powerpc";
	if (!strncmp(arch, "mips", 4))
		return "mips";
	if (!strncmp(arch, "sh", 2) && isdigit(arch[2]))
		return "sh";

	return arch;
}

<<<<<<< HEAD
static int perf_session_env__lookup_binutils_path(struct perf_session_env *env,
						  const char *name,
						  const char **path)
=======
static int perf_env__lookup_binutils_path(struct perf_env *env,
					  const char *name, const char **path)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int idx;
	const char *arch, *cross_env;
	struct utsname uts;
	const char *const *path_list;
	char *buf = NULL;

	arch = normalize_arch(env->arch);

	if (uname(&uts) < 0)
		goto out;

	/*
	 * We don't need to try to find objdump path for native system.
	 * Just use default binutils path (e.g.: "objdump").
	 */
	if (!strcmp(normalize_arch(uts.machine), arch))
		goto out;

	cross_env = getenv("CROSS_COMPILE");
	if (cross_env) {
		if (asprintf(&buf, "%s%s", cross_env, name) < 0)
			goto out_error;
		if (buf[0] == '/') {
			if (access(buf, F_OK) == 0)
				goto out;
			goto out_error;
		}
		if (lookup_path(buf))
			goto out;
<<<<<<< HEAD
		free(buf);
		buf = NULL;
=======
		zfree(&buf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (!strcmp(arch, "arm"))
		path_list = arm_triplets;
<<<<<<< HEAD
=======
	else if (!strcmp(arch, "arm64"))
		path_list = arm64_triplets;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else if (!strcmp(arch, "powerpc"))
		path_list = powerpc_triplets;
	else if (!strcmp(arch, "sh"))
		path_list = sh_triplets;
	else if (!strcmp(arch, "s390"))
		path_list = s390_triplets;
	else if (!strcmp(arch, "sparc"))
		path_list = sparc_triplets;
	else if (!strcmp(arch, "x86"))
		path_list = x86_triplets;
	else if (!strcmp(arch, "mips"))
		path_list = mips_triplets;
	else {
		ui__error("binutils for %s not supported.\n", arch);
		goto out_error;
	}

	idx = lookup_triplets(path_list, name);
	if (idx < 0) {
		ui__error("Please install %s for %s.\n"
			  "You can add it to PATH, set CROSS_COMPILE or "
			  "override the default using --%s.\n",
			  name, arch, name);
		goto out_error;
	}

	if (asprintf(&buf, "%s%s", path_list[idx], name) < 0)
		goto out_error;

out:
	*path = buf;
	return 0;
out_error:
	free(buf);
	*path = NULL;
	return -1;
}

<<<<<<< HEAD
int perf_session_env__lookup_objdump(struct perf_session_env *env)
=======
int perf_env__lookup_objdump(struct perf_env *env)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/*
	 * For live mode, env->arch will be NULL and we can use
	 * the native objdump tool.
	 */
	if (env->arch == NULL)
		return 0;

<<<<<<< HEAD
	return perf_session_env__lookup_binutils_path(env, "objdump",
						      &objdump_path);
=======
	return perf_env__lookup_binutils_path(env, "objdump", &objdump_path);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
