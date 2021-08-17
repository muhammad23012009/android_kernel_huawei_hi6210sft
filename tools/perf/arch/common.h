#ifndef ARCH_PERF_COMMON_H
#define ARCH_PERF_COMMON_H

<<<<<<< HEAD
#include "../util/session.h"

extern const char *objdump_path;

int perf_session_env__lookup_objdump(struct perf_session_env *env);
=======
#include "../util/env.h"

extern const char *objdump_path;

int perf_env__lookup_objdump(struct perf_env *env);
const char *normalize_arch(char *arch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* ARCH_PERF_COMMON_H */
