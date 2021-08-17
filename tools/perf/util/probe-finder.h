#ifndef _PROBE_FINDER_H
#define _PROBE_FINDER_H

#include <stdbool.h>
#include "util.h"
<<<<<<< HEAD
=======
#include "intlist.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "probe-event.h"

#define MAX_PROBE_BUFFER	1024
#define MAX_PROBES		 128
<<<<<<< HEAD
=======
#define MAX_PROBE_ARGS		 128

#define PROBE_ARG_VARS		"$vars"
#define PROBE_ARG_PARAMS	"$params"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline int is_c_varname(const char *name)
{
	/* TODO */
	return isalpha(name[0]) || name[0] == '_';
}

<<<<<<< HEAD
#ifdef DWARF_SUPPORT
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "dwarf-aux.h"

/* TODO: export debuginfo data structure even if no dwarf support */

/* debug information structure */
struct debuginfo {
	Dwarf		*dbg;
<<<<<<< HEAD
=======
	Dwfl_Module	*mod;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	Dwfl		*dwfl;
	Dwarf_Addr	bias;
};

<<<<<<< HEAD
extern struct debuginfo *debuginfo__new(const char *path);
extern struct debuginfo *debuginfo__new_online_kernel(unsigned long addr);
extern void debuginfo__delete(struct debuginfo *self);

/* Find probe_trace_events specified by perf_probe_event from debuginfo */
extern int debuginfo__find_trace_events(struct debuginfo *self,
					struct perf_probe_event *pev,
					struct probe_trace_event **tevs,
					int max_tevs);

/* Find a perf_probe_point from debuginfo */
extern int debuginfo__find_probe_point(struct debuginfo *self,
				       unsigned long addr,
				       struct perf_probe_point *ppt);

/* Find a line range */
extern int debuginfo__find_line_range(struct debuginfo *self,
				      struct line_range *lr);

/* Find available variables */
extern int debuginfo__find_available_vars_at(struct debuginfo *self,
					     struct perf_probe_event *pev,
					     struct variable_list **vls,
					     int max_points, bool externs);
=======
/* This also tries to open distro debuginfo */
struct debuginfo *debuginfo__new(const char *path);
void debuginfo__delete(struct debuginfo *dbg);

/* Find probe_trace_events specified by perf_probe_event from debuginfo */
int debuginfo__find_trace_events(struct debuginfo *dbg,
				 struct perf_probe_event *pev,
				 struct probe_trace_event **tevs);

/* Find a perf_probe_point from debuginfo */
int debuginfo__find_probe_point(struct debuginfo *dbg, unsigned long addr,
				struct perf_probe_point *ppt);

int debuginfo__get_text_offset(struct debuginfo *dbg, Dwarf_Addr *offs,
			       bool adjust_offset);

/* Find a line range */
int debuginfo__find_line_range(struct debuginfo *dbg, struct line_range *lr);

/* Find available variables */
int debuginfo__find_available_vars_at(struct debuginfo *dbg,
				      struct perf_probe_event *pev,
				      struct variable_list **vls);

/* Find a src file from a DWARF tag path */
int get_real_path(const char *raw_path, const char *comp_dir,
			 char **new_path);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct probe_finder {
	struct perf_probe_event	*pev;		/* Target probe event */

	/* Callback when a probe point is found */
	int (*callback)(Dwarf_Die *sc_die, struct probe_finder *pf);

	/* For function searching */
	int			lno;		/* Line number */
	Dwarf_Addr		addr;		/* Address */
	const char		*fname;		/* Real file name */
	Dwarf_Die		cu_die;		/* Current CU */
	Dwarf_Die		sp_die;
<<<<<<< HEAD
	struct list_head	lcache;		/* Line cache for lazy match */

	/* For variable searching */
#if _ELFUTILS_PREREQ(0, 142)
	Dwarf_CFI		*cfi;		/* Call Frame Information */
#endif
	Dwarf_Op		*fb_ops;	/* Frame base attribute */
=======
	struct intlist		*lcache;	/* Line cache for lazy match */

	/* For variable searching */
#if _ELFUTILS_PREREQ(0, 142)
	/* Call Frame Information from .eh_frame */
	Dwarf_CFI		*cfi_eh;
	/* Call Frame Information from .debug_frame */
	Dwarf_CFI		*cfi_dbg;
#endif
	Dwarf_Op		*fb_ops;	/* Frame base attribute */
	unsigned int		machine;	/* Target machine arch */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct perf_probe_arg	*pvar;		/* Current target variable */
	struct probe_trace_arg	*tvar;		/* Current result variable */
};

struct trace_event_finder {
	struct probe_finder	pf;
<<<<<<< HEAD
=======
	Dwfl_Module		*mod;		/* For solving symbols */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct probe_trace_event *tevs;		/* Found trace events */
	int			ntevs;		/* Number of trace events */
	int			max_tevs;	/* Max number of trace events */
};

struct available_var_finder {
	struct probe_finder	pf;
<<<<<<< HEAD
	struct variable_list	*vls;		/* Found variable lists */
	int			nvls;		/* Number of variable lists */
	int			max_vls;	/* Max no. of variable lists */
	bool			externs;	/* Find external vars too */
=======
	Dwfl_Module		*mod;		/* For solving symbols */
	struct variable_list	*vls;		/* Found variable lists */
	int			nvls;		/* Number of variable lists */
	int			max_vls;	/* Max no. of variable lists */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool			child;		/* Search child scopes */
};

struct line_finder {
	struct line_range	*lr;		/* Target line range */

	const char		*fname;		/* File name */
	int			lno_s;		/* Start line number */
	int			lno_e;		/* End line number */
	Dwarf_Die		cu_die;		/* Current CU */
	Dwarf_Die		sp_die;
	int			found;
};

<<<<<<< HEAD
#endif /* DWARF_SUPPORT */
=======
#endif /* HAVE_DWARF_SUPPORT */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /*_PROBE_FINDER_H */
