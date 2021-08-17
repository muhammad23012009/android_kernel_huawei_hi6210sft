/*
 * Kprobes-based tracing events
 *
 * Created by Masami Hiramatsu <mhiramat@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/uaccess.h>

#include "trace_probe.h"

#define KPROBE_EVENT_SYSTEM "kprobes"

/**
 * Kprobe event core functions
 */
<<<<<<< HEAD
struct trace_probe {
	struct list_head	list;
	struct kretprobe	rp;	/* Use rp.kp for kprobe use */
	unsigned long 		nhit;
	unsigned int		flags;	/* For TP_FLAG_* */
	const char		*symbol;	/* symbol name */
	struct ftrace_event_class	class;
	struct ftrace_event_call	call;
	struct ftrace_event_file * __rcu *files;
	ssize_t			size;		/* trace entry size */
	unsigned int		nr_args;
	struct probe_arg	args[];
};

#define SIZEOF_TRACE_PROBE(n)			\
	(offsetof(struct trace_probe, args) +	\
	(sizeof(struct probe_arg) * (n)))


static __kprobes bool trace_probe_is_return(struct trace_probe *tp)
{
	return tp->rp.handler != NULL;
}

static __kprobes const char *trace_probe_symbol(struct trace_probe *tp)
{
	return tp->symbol ? tp->symbol : "unknown";
}

static __kprobes unsigned long trace_probe_offset(struct trace_probe *tp)
{
	return tp->rp.kp.offset;
}

static __kprobes bool trace_probe_is_enabled(struct trace_probe *tp)
{
	return !!(tp->flags & (TP_FLAG_TRACE | TP_FLAG_PROFILE));
}

static __kprobes bool trace_probe_is_registered(struct trace_probe *tp)
{
	return !!(tp->flags & TP_FLAG_REGISTERED);
}

static __kprobes bool trace_probe_has_gone(struct trace_probe *tp)
{
	return !!(kprobe_gone(&tp->rp.kp));
}

static __kprobes bool trace_probe_within_module(struct trace_probe *tp,
						struct module *mod)
{
	int len = strlen(mod->name);
	const char *name = trace_probe_symbol(tp);
	return strncmp(mod->name, name, len) == 0 && name[len] == ':';
}

static __kprobes bool trace_probe_is_on_module(struct trace_probe *tp)
{
	return !!strchr(trace_probe_symbol(tp), ':');
}

static int register_probe_event(struct trace_probe *tp);
static int unregister_probe_event(struct trace_probe *tp);
=======
struct trace_kprobe {
	struct list_head	list;
	struct kretprobe	rp;	/* Use rp.kp for kprobe use */
	unsigned long __percpu *nhit;
	const char		*symbol;	/* symbol name */
	struct trace_probe	tp;
};

#define SIZEOF_TRACE_KPROBE(n)				\
	(offsetof(struct trace_kprobe, tp.args) +	\
	(sizeof(struct probe_arg) * (n)))


static nokprobe_inline bool trace_kprobe_is_return(struct trace_kprobe *tk)
{
	return tk->rp.handler != NULL;
}

static nokprobe_inline const char *trace_kprobe_symbol(struct trace_kprobe *tk)
{
	return tk->symbol ? tk->symbol : "unknown";
}

static nokprobe_inline unsigned long trace_kprobe_offset(struct trace_kprobe *tk)
{
	return tk->rp.kp.offset;
}

static nokprobe_inline bool trace_kprobe_has_gone(struct trace_kprobe *tk)
{
	return !!(kprobe_gone(&tk->rp.kp));
}

static nokprobe_inline bool trace_kprobe_within_module(struct trace_kprobe *tk,
						 struct module *mod)
{
	int len = strlen(mod->name);
	const char *name = trace_kprobe_symbol(tk);
	return strncmp(mod->name, name, len) == 0 && name[len] == ':';
}

static nokprobe_inline bool trace_kprobe_is_on_module(struct trace_kprobe *tk)
{
	return !!strchr(trace_kprobe_symbol(tk), ':');
}

static int register_kprobe_event(struct trace_kprobe *tk);
static int unregister_kprobe_event(struct trace_kprobe *tk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static DEFINE_MUTEX(probe_lock);
static LIST_HEAD(probe_list);

static int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs);
static int kretprobe_dispatcher(struct kretprobe_instance *ri,
				struct pt_regs *regs);

<<<<<<< HEAD
/*
 * Allocate new trace_probe and initialize it (including kprobes).
 */
static struct trace_probe *alloc_trace_probe(const char *group,
=======
/* Memory fetching by symbol */
struct symbol_cache {
	char		*symbol;
	long		offset;
	unsigned long	addr;
};

unsigned long update_symbol_cache(struct symbol_cache *sc)
{
	sc->addr = (unsigned long)kallsyms_lookup_name(sc->symbol);

	if (sc->addr)
		sc->addr += sc->offset;

	return sc->addr;
}

void free_symbol_cache(struct symbol_cache *sc)
{
	kfree(sc->symbol);
	kfree(sc);
}

struct symbol_cache *alloc_symbol_cache(const char *sym, long offset)
{
	struct symbol_cache *sc;

	if (!sym || strlen(sym) == 0)
		return NULL;

	sc = kzalloc(sizeof(struct symbol_cache), GFP_KERNEL);
	if (!sc)
		return NULL;

	sc->symbol = kstrdup(sym, GFP_KERNEL);
	if (!sc->symbol) {
		kfree(sc);
		return NULL;
	}
	sc->offset = offset;
	update_symbol_cache(sc);

	return sc;
}

/*
 * Kprobes-specific fetch functions
 */
#define DEFINE_FETCH_stack(type)					\
static void FETCH_FUNC_NAME(stack, type)(struct pt_regs *regs,		\
					  void *offset, void *dest)	\
{									\
	*(type *)dest = (type)regs_get_kernel_stack_nth(regs,		\
				(unsigned int)((unsigned long)offset));	\
}									\
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(stack, type));

DEFINE_BASIC_FETCH_FUNCS(stack)
/* No string on the stack entry */
#define fetch_stack_string	NULL
#define fetch_stack_string_size	NULL

#define DEFINE_FETCH_memory(type)					\
static void FETCH_FUNC_NAME(memory, type)(struct pt_regs *regs,		\
					  void *addr, void *dest)	\
{									\
	type retval;							\
	if (probe_kernel_address(addr, retval))				\
		*(type *)dest = 0;					\
	else								\
		*(type *)dest = retval;					\
}									\
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(memory, type));

DEFINE_BASIC_FETCH_FUNCS(memory)
/*
 * Fetch a null-terminated string. Caller MUST set *(u32 *)dest with max
 * length and relative data location.
 */
static void FETCH_FUNC_NAME(memory, string)(struct pt_regs *regs,
					    void *addr, void *dest)
{
	int maxlen = get_rloc_len(*(u32 *)dest);
	u8 *dst = get_rloc_data(dest);
	long ret;

	if (!maxlen)
		return;

	/*
	 * Try to get string again, since the string can be changed while
	 * probing.
	 */
	ret = strncpy_from_unsafe(dst, addr, maxlen);

	if (ret < 0) {	/* Failed to fetch string */
		dst[0] = '\0';
		*(u32 *)dest = make_data_rloc(0, get_rloc_offs(*(u32 *)dest));
	} else {
		*(u32 *)dest = make_data_rloc(ret, get_rloc_offs(*(u32 *)dest));
	}
}
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(memory, string));

/* Return the length of string -- including null terminal byte */
static void FETCH_FUNC_NAME(memory, string_size)(struct pt_regs *regs,
						 void *addr, void *dest)
{
	mm_segment_t old_fs;
	int ret, len = 0;
	u8 c;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	pagefault_disable();

	do {
		ret = __copy_from_user_inatomic(&c, (u8 *)addr + len, 1);
		len++;
	} while (c && ret == 0 && len < MAX_STRING_SIZE);

	pagefault_enable();
	set_fs(old_fs);

	if (ret < 0)	/* Failed to check the length */
		*(u32 *)dest = 0;
	else
		*(u32 *)dest = len;
}
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(memory, string_size));

#define DEFINE_FETCH_symbol(type)					\
void FETCH_FUNC_NAME(symbol, type)(struct pt_regs *regs, void *data, void *dest)\
{									\
	struct symbol_cache *sc = data;					\
	if (sc->addr)							\
		fetch_memory_##type(regs, (void *)sc->addr, dest);	\
	else								\
		*(type *)dest = 0;					\
}									\
NOKPROBE_SYMBOL(FETCH_FUNC_NAME(symbol, type));

DEFINE_BASIC_FETCH_FUNCS(symbol)
DEFINE_FETCH_symbol(string)
DEFINE_FETCH_symbol(string_size)

/* kprobes don't support file_offset fetch methods */
#define fetch_file_offset_u8		NULL
#define fetch_file_offset_u16		NULL
#define fetch_file_offset_u32		NULL
#define fetch_file_offset_u64		NULL
#define fetch_file_offset_string	NULL
#define fetch_file_offset_string_size	NULL

/* Fetch type information table */
static const struct fetch_type kprobes_fetch_type_table[] = {
	/* Special types */
	[FETCH_TYPE_STRING] = __ASSIGN_FETCH_TYPE("string", string, string,
					sizeof(u32), 1, "__data_loc char[]"),
	[FETCH_TYPE_STRSIZE] = __ASSIGN_FETCH_TYPE("string_size", u32,
					string_size, sizeof(u32), 0, "u32"),
	/* Basic types */
	ASSIGN_FETCH_TYPE(u8,  u8,  0),
	ASSIGN_FETCH_TYPE(u16, u16, 0),
	ASSIGN_FETCH_TYPE(u32, u32, 0),
	ASSIGN_FETCH_TYPE(u64, u64, 0),
	ASSIGN_FETCH_TYPE(s8,  u8,  1),
	ASSIGN_FETCH_TYPE(s16, u16, 1),
	ASSIGN_FETCH_TYPE(s32, u32, 1),
	ASSIGN_FETCH_TYPE(s64, u64, 1),
	ASSIGN_FETCH_TYPE_ALIAS(x8,  u8,  u8,  0),
	ASSIGN_FETCH_TYPE_ALIAS(x16, u16, u16, 0),
	ASSIGN_FETCH_TYPE_ALIAS(x32, u32, u32, 0),
	ASSIGN_FETCH_TYPE_ALIAS(x64, u64, u64, 0),

	ASSIGN_FETCH_TYPE_END
};

/*
 * Allocate new trace_probe and initialize it (including kprobes).
 */
static struct trace_kprobe *alloc_trace_kprobe(const char *group,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     const char *event,
					     void *addr,
					     const char *symbol,
					     unsigned long offs,
					     int nargs, bool is_return)
{
<<<<<<< HEAD
	struct trace_probe *tp;
	int ret = -ENOMEM;

	tp = kzalloc(SIZEOF_TRACE_PROBE(nargs), GFP_KERNEL);
	if (!tp)
		return ERR_PTR(ret);

	if (symbol) {
		tp->symbol = kstrdup(symbol, GFP_KERNEL);
		if (!tp->symbol)
			goto error;
		tp->rp.kp.symbol_name = tp->symbol;
		tp->rp.kp.offset = offs;
	} else
		tp->rp.kp.addr = addr;

	if (is_return)
		tp->rp.handler = kretprobe_dispatcher;
	else
		tp->rp.kp.pre_handler = kprobe_dispatcher;
=======
	struct trace_kprobe *tk;
	int ret = -ENOMEM;

	tk = kzalloc(SIZEOF_TRACE_KPROBE(nargs), GFP_KERNEL);
	if (!tk)
		return ERR_PTR(ret);

	tk->nhit = alloc_percpu(unsigned long);
	if (!tk->nhit)
		goto error;

	if (symbol) {
		tk->symbol = kstrdup(symbol, GFP_KERNEL);
		if (!tk->symbol)
			goto error;
		tk->rp.kp.symbol_name = tk->symbol;
		tk->rp.kp.offset = offs;
	} else
		tk->rp.kp.addr = addr;

	if (is_return)
		tk->rp.handler = kretprobe_dispatcher;
	else
		tk->rp.kp.pre_handler = kprobe_dispatcher;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!event || !is_good_name(event)) {
		ret = -EINVAL;
		goto error;
	}

<<<<<<< HEAD
	tp->call.class = &tp->class;
	tp->call.name = kstrdup(event, GFP_KERNEL);
	if (!tp->call.name)
=======
	tk->tp.call.class = &tk->tp.class;
	tk->tp.call.name = kstrdup(event, GFP_KERNEL);
	if (!tk->tp.call.name)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto error;

	if (!group || !is_good_name(group)) {
		ret = -EINVAL;
		goto error;
	}

<<<<<<< HEAD
	tp->class.system = kstrdup(group, GFP_KERNEL);
	if (!tp->class.system)
		goto error;

	INIT_LIST_HEAD(&tp->list);
	return tp;
error:
	kfree(tp->call.name);
	kfree(tp->symbol);
	kfree(tp);
	return ERR_PTR(ret);
}

static void free_trace_probe(struct trace_probe *tp)
{
	int i;

	for (i = 0; i < tp->nr_args; i++)
		traceprobe_free_probe_arg(&tp->args[i]);

	kfree(tp->call.class->system);
	kfree(tp->call.name);
	kfree(tp->symbol);
	kfree(tp);
}

static struct trace_probe *find_trace_probe(const char *event,
					    const char *group)
{
	struct trace_probe *tp;

	list_for_each_entry(tp, &probe_list, list)
		if (strcmp(tp->call.name, event) == 0 &&
		    strcmp(tp->call.class->system, group) == 0)
			return tp;
	return NULL;
}

static int trace_probe_nr_files(struct trace_probe *tp)
{
	struct ftrace_event_file **file;
	int ret = 0;

	/*
	 * Since all tp->files updater is protected by probe_enable_lock,
	 * we don't need to lock an rcu_read_lock.
	 */
	file = rcu_dereference_raw(tp->files);
	if (file)
		while (*(file++))
			ret++;

	return ret;
}

static DEFINE_MUTEX(probe_enable_lock);

=======
	tk->tp.class.system = kstrdup(group, GFP_KERNEL);
	if (!tk->tp.class.system)
		goto error;

	INIT_LIST_HEAD(&tk->list);
	INIT_LIST_HEAD(&tk->tp.files);
	return tk;
error:
	kfree(tk->tp.call.name);
	kfree(tk->symbol);
	free_percpu(tk->nhit);
	kfree(tk);
	return ERR_PTR(ret);
}

static void free_trace_kprobe(struct trace_kprobe *tk)
{
	int i;

	for (i = 0; i < tk->tp.nr_args; i++)
		traceprobe_free_probe_arg(&tk->tp.args[i]);

	kfree(tk->tp.call.class->system);
	kfree(tk->tp.call.name);
	kfree(tk->symbol);
	free_percpu(tk->nhit);
	kfree(tk);
}

static struct trace_kprobe *find_trace_kprobe(const char *event,
					      const char *group)
{
	struct trace_kprobe *tk;

	list_for_each_entry(tk, &probe_list, list)
		if (strcmp(trace_event_name(&tk->tp.call), event) == 0 &&
		    strcmp(tk->tp.call.class->system, group) == 0)
			return tk;
	return NULL;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Enable trace_probe
 * if the file is NULL, enable "perf" handler, or enable "trace" handler.
 */
static int
<<<<<<< HEAD
enable_trace_probe(struct trace_probe *tp, struct ftrace_event_file *file)
{
	int ret = 0;

	mutex_lock(&probe_enable_lock);

	if (file) {
		struct ftrace_event_file **new, **old;
		int n = trace_probe_nr_files(tp);

		old = rcu_dereference_raw(tp->files);
		/* 1 is for new one and 1 is for stopper */
		new = kzalloc((n + 2) * sizeof(struct ftrace_event_file *),
			      GFP_KERNEL);
		if (!new) {
			ret = -ENOMEM;
			goto out_unlock;
		}
		memcpy(new, old, n * sizeof(struct ftrace_event_file *));
		new[n] = file;
		/* The last one keeps a NULL */

		rcu_assign_pointer(tp->files, new);
		tp->flags |= TP_FLAG_TRACE;

		if (old) {
			/* Make sure the probe is done with old files */
			synchronize_sched();
			kfree(old);
		}
	} else
		tp->flags |= TP_FLAG_PROFILE;

	if (trace_probe_is_enabled(tp) && trace_probe_is_registered(tp) &&
	    !trace_probe_has_gone(tp)) {
		if (trace_probe_is_return(tp))
			ret = enable_kretprobe(&tp->rp);
		else
			ret = enable_kprobe(&tp->rp.kp);
	}

 out_unlock:
	mutex_unlock(&probe_enable_lock);

	return ret;
}

static int
trace_probe_file_index(struct trace_probe *tp, struct ftrace_event_file *file)
{
	struct ftrace_event_file **files;
	int i;

	/*
	 * Since all tp->files updater is protected by probe_enable_lock,
	 * we don't need to lock an rcu_read_lock.
	 */
	files = rcu_dereference_raw(tp->files);
	if (files) {
		for (i = 0; files[i]; i++)
			if (files[i] == file)
				return i;
	}

	return -1;
=======
enable_trace_kprobe(struct trace_kprobe *tk, struct trace_event_file *file)
{
	struct event_file_link *link = NULL;
	int ret = 0;

	if (file) {
		link = kmalloc(sizeof(*link), GFP_KERNEL);
		if (!link) {
			ret = -ENOMEM;
			goto out;
		}

		link->file = file;
		list_add_tail_rcu(&link->list, &tk->tp.files);

		tk->tp.flags |= TP_FLAG_TRACE;
	} else
		tk->tp.flags |= TP_FLAG_PROFILE;

	if (trace_probe_is_registered(&tk->tp) && !trace_kprobe_has_gone(tk)) {
		if (trace_kprobe_is_return(tk))
			ret = enable_kretprobe(&tk->rp);
		else
			ret = enable_kprobe(&tk->rp.kp);
	}

	if (ret) {
		if (file) {
			/* Notice the if is true on not WARN() */
			if (!WARN_ON_ONCE(!link))
				list_del_rcu(&link->list);
			kfree(link);
			tk->tp.flags &= ~TP_FLAG_TRACE;
		} else {
			tk->tp.flags &= ~TP_FLAG_PROFILE;
		}
	}
 out:
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Disable trace_probe
 * if the file is NULL, disable "perf" handler, or disable "trace" handler.
 */
static int
<<<<<<< HEAD
disable_trace_probe(struct trace_probe *tp, struct ftrace_event_file *file)
{
	struct ftrace_event_file **old = NULL;
	int wait = 0;
	int ret = 0;

	mutex_lock(&probe_enable_lock);

	if (file) {
		struct ftrace_event_file **new, **old;
		int n = trace_probe_nr_files(tp);
		int i, j;

		old = rcu_dereference_raw(tp->files);
		if (n == 0 || trace_probe_file_index(tp, file) < 0) {
			ret = -EINVAL;
			goto out_unlock;
		}

		if (n == 1) {	/* Remove the last file */
			tp->flags &= ~TP_FLAG_TRACE;
			new = NULL;
		} else {
			new = kzalloc(n * sizeof(struct ftrace_event_file *),
				      GFP_KERNEL);
			if (!new) {
				ret = -ENOMEM;
				goto out_unlock;
			}

			/* This copy & check loop copies the NULL stopper too */
			for (i = 0, j = 0; j < n && i < n + 1; i++)
				if (old[i] != file)
					new[j++] = old[i];
		}

		rcu_assign_pointer(tp->files, new);
		wait = 1;
	} else
		tp->flags &= ~TP_FLAG_PROFILE;

	if (!trace_probe_is_enabled(tp) && trace_probe_is_registered(tp)) {
		if (trace_probe_is_return(tp))
			disable_kretprobe(&tp->rp);
		else
			disable_kprobe(&tp->rp.kp);
		wait = 1;
	}

 out_unlock:
	mutex_unlock(&probe_enable_lock);

=======
disable_trace_kprobe(struct trace_kprobe *tk, struct trace_event_file *file)
{
	struct event_file_link *link = NULL;
	int wait = 0;
	int ret = 0;

	if (file) {
		link = find_event_file_link(&tk->tp, file);
		if (!link) {
			ret = -EINVAL;
			goto out;
		}

		list_del_rcu(&link->list);
		wait = 1;
		if (!list_empty(&tk->tp.files))
			goto out;

		tk->tp.flags &= ~TP_FLAG_TRACE;
	} else
		tk->tp.flags &= ~TP_FLAG_PROFILE;

	if (!trace_probe_is_enabled(&tk->tp) && trace_probe_is_registered(&tk->tp)) {
		if (trace_kprobe_is_return(tk))
			disable_kretprobe(&tk->rp);
		else
			disable_kprobe(&tk->rp.kp);
		wait = 1;
	}
 out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (wait) {
		/*
		 * Synchronize with kprobe_trace_func/kretprobe_trace_func
		 * to ensure disabled (all running handlers are finished).
		 * This is not only for kfree(), but also the caller,
		 * trace_remove_event_call() supposes it for releasing
		 * event_call related objects, which will be accessed in
		 * the kprobe_trace_func/kretprobe_trace_func.
		 */
		synchronize_sched();
<<<<<<< HEAD
		kfree(old);	/* Ignored if link == NULL */
=======
		kfree(link);	/* Ignored if link == NULL */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return ret;
}

/* Internal register function - just handle k*probes and flags */
<<<<<<< HEAD
static int __register_trace_probe(struct trace_probe *tp)
{
	int i, ret;

	if (trace_probe_is_registered(tp))
		return -EINVAL;

	for (i = 0; i < tp->nr_args; i++)
		traceprobe_update_arg(&tp->args[i]);

	/* Set/clear disabled flag according to tp->flag */
	if (trace_probe_is_enabled(tp))
		tp->rp.kp.flags &= ~KPROBE_FLAG_DISABLED;
	else
		tp->rp.kp.flags |= KPROBE_FLAG_DISABLED;

	if (trace_probe_is_return(tp))
		ret = register_kretprobe(&tp->rp);
	else
		ret = register_kprobe(&tp->rp.kp);

	if (ret == 0)
		tp->flags |= TP_FLAG_REGISTERED;
	else {
		pr_warning("Could not insert probe at %s+%lu: %d\n",
			   trace_probe_symbol(tp), trace_probe_offset(tp), ret);
		if (ret == -ENOENT && trace_probe_is_on_module(tp)) {
			pr_warning("This probe might be able to register after"
				   "target module is loaded. Continue.\n");
			ret = 0;
		} else if (ret == -EILSEQ) {
			pr_warning("Probing address(0x%p) is not an "
				   "instruction boundary.\n",
				   tp->rp.kp.addr);
=======
static int __register_trace_kprobe(struct trace_kprobe *tk)
{
	int i, ret;

	if (trace_probe_is_registered(&tk->tp))
		return -EINVAL;

	for (i = 0; i < tk->tp.nr_args; i++)
		traceprobe_update_arg(&tk->tp.args[i]);

	/* Set/clear disabled flag according to tp->flag */
	if (trace_probe_is_enabled(&tk->tp))
		tk->rp.kp.flags &= ~KPROBE_FLAG_DISABLED;
	else
		tk->rp.kp.flags |= KPROBE_FLAG_DISABLED;

	if (trace_kprobe_is_return(tk))
		ret = register_kretprobe(&tk->rp);
	else
		ret = register_kprobe(&tk->rp.kp);

	if (ret == 0)
		tk->tp.flags |= TP_FLAG_REGISTERED;
	else {
		pr_warn("Could not insert probe at %s+%lu: %d\n",
			trace_kprobe_symbol(tk), trace_kprobe_offset(tk), ret);
		if (ret == -ENOENT && trace_kprobe_is_on_module(tk)) {
			pr_warn("This probe might be able to register after target module is loaded. Continue.\n");
			ret = 0;
		} else if (ret == -EILSEQ) {
			pr_warn("Probing address(0x%p) is not an instruction boundary.\n",
				tk->rp.kp.addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EINVAL;
		}
	}

	return ret;
}

/* Internal unregister function - just handle k*probes and flags */
<<<<<<< HEAD
static void __unregister_trace_probe(struct trace_probe *tp)
{
	if (trace_probe_is_registered(tp)) {
		if (trace_probe_is_return(tp))
			unregister_kretprobe(&tp->rp);
		else
			unregister_kprobe(&tp->rp.kp);
		tp->flags &= ~TP_FLAG_REGISTERED;
		/* Cleanup kprobe for reuse */
		if (tp->rp.kp.symbol_name)
			tp->rp.kp.addr = NULL;
=======
static void __unregister_trace_kprobe(struct trace_kprobe *tk)
{
	if (trace_probe_is_registered(&tk->tp)) {
		if (trace_kprobe_is_return(tk))
			unregister_kretprobe(&tk->rp);
		else
			unregister_kprobe(&tk->rp.kp);
		tk->tp.flags &= ~TP_FLAG_REGISTERED;
		/* Cleanup kprobe for reuse */
		if (tk->rp.kp.symbol_name)
			tk->rp.kp.addr = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/* Unregister a trace_probe and probe_event: call with locking probe_lock */
<<<<<<< HEAD
static int unregister_trace_probe(struct trace_probe *tp)
{
	/* Enabled event can not be unregistered */
	if (trace_probe_is_enabled(tp))
		return -EBUSY;

	/* Will fail if probe is being used by ftrace or perf */
	if (unregister_probe_event(tp))
		return -EBUSY;

	__unregister_trace_probe(tp);
	list_del(&tp->list);
=======
static int unregister_trace_kprobe(struct trace_kprobe *tk)
{
	/* Enabled event can not be unregistered */
	if (trace_probe_is_enabled(&tk->tp))
		return -EBUSY;

	/* Will fail if probe is being used by ftrace or perf */
	if (unregister_kprobe_event(tk))
		return -EBUSY;

	__unregister_trace_kprobe(tk);
	list_del(&tk->list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

/* Register a trace_probe and probe_event */
<<<<<<< HEAD
static int register_trace_probe(struct trace_probe *tp)
{
	struct trace_probe *old_tp;
=======
static int register_trace_kprobe(struct trace_kprobe *tk)
{
	struct trace_kprobe *old_tk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	mutex_lock(&probe_lock);

	/* Delete old (same name) event if exist */
<<<<<<< HEAD
	old_tp = find_trace_probe(tp->call.name, tp->call.class->system);
	if (old_tp) {
		ret = unregister_trace_probe(old_tp);
		if (ret < 0)
			goto end;
		free_trace_probe(old_tp);
	}

	/* Register new event */
	ret = register_probe_event(tp);
	if (ret) {
		pr_warning("Failed to register probe event(%d)\n", ret);
=======
	old_tk = find_trace_kprobe(trace_event_name(&tk->tp.call),
			tk->tp.call.class->system);
	if (old_tk) {
		ret = unregister_trace_kprobe(old_tk);
		if (ret < 0)
			goto end;
		free_trace_kprobe(old_tk);
	}

	/* Register new event */
	ret = register_kprobe_event(tk);
	if (ret) {
		pr_warn("Failed to register probe event(%d)\n", ret);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto end;
	}

	/* Register k*probe */
<<<<<<< HEAD
	ret = __register_trace_probe(tp);
	if (ret < 0)
		unregister_probe_event(tp);
	else
		list_add_tail(&tp->list, &probe_list);
=======
	ret = __register_trace_kprobe(tk);
	if (ret < 0)
		unregister_kprobe_event(tk);
	else
		list_add_tail(&tk->list, &probe_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

end:
	mutex_unlock(&probe_lock);
	return ret;
}

/* Module notifier call back, checking event on the module */
<<<<<<< HEAD
static int trace_probe_module_callback(struct notifier_block *nb,
				       unsigned long val, void *data)
{
	struct module *mod = data;
	struct trace_probe *tp;
=======
static int trace_kprobe_module_callback(struct notifier_block *nb,
				       unsigned long val, void *data)
{
	struct module *mod = data;
	struct trace_kprobe *tk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	if (val != MODULE_STATE_COMING)
		return NOTIFY_DONE;

	/* Update probes on coming module */
	mutex_lock(&probe_lock);
<<<<<<< HEAD
	list_for_each_entry(tp, &probe_list, list) {
		if (trace_probe_within_module(tp, mod)) {
			/* Don't need to check busy - this should have gone. */
			__unregister_trace_probe(tp);
			ret = __register_trace_probe(tp);
			if (ret)
				pr_warning("Failed to re-register probe %s on"
					   "%s: %d\n",
					   tp->call.name, mod->name, ret);
=======
	list_for_each_entry(tk, &probe_list, list) {
		if (trace_kprobe_within_module(tk, mod)) {
			/* Don't need to check busy - this should have gone. */
			__unregister_trace_kprobe(tk);
			ret = __register_trace_kprobe(tk);
			if (ret)
				pr_warn("Failed to re-register probe %s on %s: %d\n",
					trace_event_name(&tk->tp.call),
					mod->name, ret);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
	mutex_unlock(&probe_lock);

	return NOTIFY_DONE;
}

<<<<<<< HEAD
static struct notifier_block trace_probe_module_nb = {
	.notifier_call = trace_probe_module_callback,
	.priority = 1	/* Invoked after kprobe module callback */
};

static int create_trace_probe(int argc, char **argv)
=======
static struct notifier_block trace_kprobe_module_nb = {
	.notifier_call = trace_kprobe_module_callback,
	.priority = 1	/* Invoked after kprobe module callback */
};

static int create_trace_kprobe(int argc, char **argv)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/*
	 * Argument syntax:
	 *  - Add kprobe: p[:[GRP/]EVENT] [MOD:]KSYM[+OFFS]|KADDR [FETCHARGS]
	 *  - Add kretprobe: r[:[GRP/]EVENT] [MOD:]KSYM[+0] [FETCHARGS]
	 * Fetch args:
	 *  $retval	: fetch return value
	 *  $stack	: fetch stack address
	 *  $stackN	: fetch Nth of stack (N:0-)
<<<<<<< HEAD
=======
	 *  $comm       : fetch current task comm
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 *  @ADDR	: fetch memory at ADDR (ADDR should be in kernel)
	 *  @SYM[+|-offs] : fetch memory at SYM +|- offs (SYM is a data symbol)
	 *  %REG	: fetch register REG
	 * Dereferencing memory fetch:
	 *  +|-offs(ARG) : fetch memory at ARG +|- offs address.
	 * Alias name of args:
	 *  NAME=FETCHARG : set NAME as alias of FETCHARG.
	 * Type of args:
	 *  FETCHARG:TYPE : use TYPE instead of unsigned long.
	 */
<<<<<<< HEAD
	struct trace_probe *tp;
=======
	struct trace_kprobe *tk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i, ret = 0;
	bool is_return = false, is_delete = false;
	char *symbol = NULL, *event = NULL, *group = NULL;
	char *arg;
<<<<<<< HEAD
	unsigned long offset = 0;
=======
	long offset = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void *addr = NULL;
	char buf[MAX_EVENT_NAME_LEN];

	/* argc must be >= 1 */
	if (argv[0][0] == 'p')
		is_return = false;
	else if (argv[0][0] == 'r')
		is_return = true;
	else if (argv[0][0] == '-')
		is_delete = true;
	else {
		pr_info("Probe definition must be started with 'p', 'r' or"
			" '-'.\n");
		return -EINVAL;
	}

	if (argv[0][1] == ':') {
		event = &argv[0][2];
		if (strchr(event, '/')) {
			group = event;
			event = strchr(group, '/') + 1;
			event[-1] = '\0';
			if (strlen(group) == 0) {
				pr_info("Group name is not specified\n");
				return -EINVAL;
			}
		}
		if (strlen(event) == 0) {
			pr_info("Event name is not specified\n");
			return -EINVAL;
		}
	}
	if (!group)
		group = KPROBE_EVENT_SYSTEM;

	if (is_delete) {
		if (!event) {
			pr_info("Delete command needs an event name.\n");
			return -EINVAL;
		}
		mutex_lock(&probe_lock);
<<<<<<< HEAD
		tp = find_trace_probe(event, group);
		if (!tp) {
=======
		tk = find_trace_kprobe(event, group);
		if (!tk) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			mutex_unlock(&probe_lock);
			pr_info("Event %s/%s doesn't exist.\n", group, event);
			return -ENOENT;
		}
		/* delete an event */
<<<<<<< HEAD
		ret = unregister_trace_probe(tp);
		if (ret == 0)
			free_trace_probe(tp);
=======
		ret = unregister_trace_kprobe(tk);
		if (ret == 0)
			free_trace_kprobe(tk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		mutex_unlock(&probe_lock);
		return ret;
	}

	if (argc < 2) {
		pr_info("Probe point is not specified.\n");
		return -EINVAL;
	}
<<<<<<< HEAD
	if (isdigit(argv[1][0])) {
		if (is_return) {
			pr_info("Return probe point must be a symbol.\n");
			return -EINVAL;
		}
		/* an address specified */
		ret = kstrtoul(&argv[1][0], 0, (unsigned long *)&addr);
		if (ret) {
			pr_info("Failed to parse address.\n");
			return ret;
		}
	} else {
=======

	/* try to parse an address. if that fails, try to read the
	 * input as a symbol. */
	if (kstrtoul(argv[1], 0, (unsigned long *)&addr)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* a symbol specified */
		symbol = argv[1];
		/* TODO: support .init module functions */
		ret = traceprobe_split_symbol_offset(symbol, &offset);
<<<<<<< HEAD
		if (ret) {
			pr_info("Failed to parse symbol.\n");
=======
		if (ret || offset < 0 || offset > UINT_MAX) {
			pr_info("Failed to parse either an address or a symbol.\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return ret;
		}
		if (offset && is_return) {
			pr_info("Return probe must be used without offset.\n");
			return -EINVAL;
		}
<<<<<<< HEAD
=======
	} else if (is_return) {
		pr_info("Return probe point must be a symbol.\n");
		return -EINVAL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	argc -= 2; argv += 2;

	/* setup a probe */
	if (!event) {
		/* Make a new event name */
		if (symbol)
			snprintf(buf, MAX_EVENT_NAME_LEN, "%c_%s_%ld",
				 is_return ? 'r' : 'p', symbol, offset);
		else
			snprintf(buf, MAX_EVENT_NAME_LEN, "%c_0x%p",
				 is_return ? 'r' : 'p', addr);
		event = buf;
	}
<<<<<<< HEAD
	tp = alloc_trace_probe(group, event, addr, symbol, offset, argc,
			       is_return);
	if (IS_ERR(tp)) {
		pr_info("Failed to allocate trace_probe.(%d)\n",
			(int)PTR_ERR(tp));
		return PTR_ERR(tp);
=======
	tk = alloc_trace_kprobe(group, event, addr, symbol, offset, argc,
			       is_return);
	if (IS_ERR(tk)) {
		pr_info("Failed to allocate trace_probe.(%d)\n",
			(int)PTR_ERR(tk));
		return PTR_ERR(tk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* parse arguments */
	ret = 0;
	for (i = 0; i < argc && i < MAX_TRACE_ARGS; i++) {
<<<<<<< HEAD
		/* Increment count for freeing args in error case */
		tp->nr_args++;
=======
		struct probe_arg *parg = &tk->tp.args[i];

		/* Increment count for freeing args in error case */
		tk->tp.nr_args++;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* Parse argument name */
		arg = strchr(argv[i], '=');
		if (arg) {
			*arg++ = '\0';
<<<<<<< HEAD
			tp->args[i].name = kstrdup(argv[i], GFP_KERNEL);
=======
			parg->name = kstrdup(argv[i], GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		} else {
			arg = argv[i];
			/* If argument name is omitted, set "argN" */
			snprintf(buf, MAX_EVENT_NAME_LEN, "arg%d", i + 1);
<<<<<<< HEAD
			tp->args[i].name = kstrdup(buf, GFP_KERNEL);
		}

		if (!tp->args[i].name) {
=======
			parg->name = kstrdup(buf, GFP_KERNEL);
		}

		if (!parg->name) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			pr_info("Failed to allocate argument[%d] name.\n", i);
			ret = -ENOMEM;
			goto error;
		}

<<<<<<< HEAD
		if (!is_good_name(tp->args[i].name)) {
			pr_info("Invalid argument[%d] name: %s\n",
				i, tp->args[i].name);
=======
		if (!is_good_name(parg->name)) {
			pr_info("Invalid argument[%d] name: %s\n",
				i, parg->name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EINVAL;
			goto error;
		}

<<<<<<< HEAD
		if (traceprobe_conflict_field_name(tp->args[i].name,
							tp->args, i)) {
=======
		if (traceprobe_conflict_field_name(parg->name,
							tk->tp.args, i)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			pr_info("Argument[%d] name '%s' conflicts with "
				"another field.\n", i, argv[i]);
			ret = -EINVAL;
			goto error;
		}

		/* Parse fetch argument */
<<<<<<< HEAD
		ret = traceprobe_parse_probe_arg(arg, &tp->size, &tp->args[i],
						is_return, true);
=======
		ret = traceprobe_parse_probe_arg(arg, &tk->tp.size, parg,
						is_return, true,
						kprobes_fetch_type_table);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (ret) {
			pr_info("Parse error at argument[%d]. (%d)\n", i, ret);
			goto error;
		}
	}

<<<<<<< HEAD
	ret = register_trace_probe(tp);
=======
	ret = register_trace_kprobe(tk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		goto error;
	return 0;

error:
<<<<<<< HEAD
	free_trace_probe(tp);
	return ret;
}

static int release_all_trace_probes(void)
{
	struct trace_probe *tp;
=======
	free_trace_kprobe(tk);
	return ret;
}

static int release_all_trace_kprobes(void)
{
	struct trace_kprobe *tk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret = 0;

	mutex_lock(&probe_lock);
	/* Ensure no probe is in use. */
<<<<<<< HEAD
	list_for_each_entry(tp, &probe_list, list)
		if (trace_probe_is_enabled(tp)) {
=======
	list_for_each_entry(tk, &probe_list, list)
		if (trace_probe_is_enabled(&tk->tp)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ret = -EBUSY;
			goto end;
		}
	/* TODO: Use batch unregistration */
	while (!list_empty(&probe_list)) {
<<<<<<< HEAD
		tp = list_entry(probe_list.next, struct trace_probe, list);
		ret = unregister_trace_probe(tp);
		if (ret)
			goto end;
		free_trace_probe(tp);
=======
		tk = list_entry(probe_list.next, struct trace_kprobe, list);
		ret = unregister_trace_kprobe(tk);
		if (ret)
			goto end;
		free_trace_kprobe(tk);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

end:
	mutex_unlock(&probe_lock);

	return ret;
}

/* Probes listing interfaces */
static void *probes_seq_start(struct seq_file *m, loff_t *pos)
{
	mutex_lock(&probe_lock);
	return seq_list_start(&probe_list, *pos);
}

static void *probes_seq_next(struct seq_file *m, void *v, loff_t *pos)
{
	return seq_list_next(v, &probe_list, pos);
}

static void probes_seq_stop(struct seq_file *m, void *v)
{
	mutex_unlock(&probe_lock);
}

static int probes_seq_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
	struct trace_probe *tp = v;
	int i;

	seq_printf(m, "%c", trace_probe_is_return(tp) ? 'r' : 'p');
	seq_printf(m, ":%s/%s", tp->call.class->system, tp->call.name);

	if (!tp->symbol)
		seq_printf(m, " 0x%p", tp->rp.kp.addr);
	else if (tp->rp.kp.offset)
		seq_printf(m, " %s+%u", trace_probe_symbol(tp),
			   tp->rp.kp.offset);
	else
		seq_printf(m, " %s", trace_probe_symbol(tp));

	for (i = 0; i < tp->nr_args; i++)
		seq_printf(m, " %s=%s", tp->args[i].name, tp->args[i].comm);
	seq_printf(m, "\n");
=======
	struct trace_kprobe *tk = v;
	int i;

	seq_putc(m, trace_kprobe_is_return(tk) ? 'r' : 'p');
	seq_printf(m, ":%s/%s", tk->tp.call.class->system,
			trace_event_name(&tk->tp.call));

	if (!tk->symbol)
		seq_printf(m, " 0x%p", tk->rp.kp.addr);
	else if (tk->rp.kp.offset)
		seq_printf(m, " %s+%u", trace_kprobe_symbol(tk),
			   tk->rp.kp.offset);
	else
		seq_printf(m, " %s", trace_kprobe_symbol(tk));

	for (i = 0; i < tk->tp.nr_args; i++)
		seq_printf(m, " %s=%s", tk->tp.args[i].name, tk->tp.args[i].comm);
	seq_putc(m, '\n');
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static const struct seq_operations probes_seq_op = {
	.start  = probes_seq_start,
	.next   = probes_seq_next,
	.stop   = probes_seq_stop,
	.show   = probes_seq_show
};

static int probes_open(struct inode *inode, struct file *file)
{
	int ret;

	if ((file->f_mode & FMODE_WRITE) && (file->f_flags & O_TRUNC)) {
<<<<<<< HEAD
		ret = release_all_trace_probes();
=======
		ret = release_all_trace_kprobes();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (ret < 0)
			return ret;
	}

	return seq_open(file, &probes_seq_op);
}

static ssize_t probes_write(struct file *file, const char __user *buffer,
			    size_t count, loff_t *ppos)
{
	return traceprobe_probes_write(file, buffer, count, ppos,
<<<<<<< HEAD
			create_trace_probe);
=======
			create_trace_kprobe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct file_operations kprobe_events_ops = {
	.owner          = THIS_MODULE,
	.open           = probes_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
	.write		= probes_write,
};

/* Probes profiling interfaces */
static int probes_profile_seq_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
	struct trace_probe *tp = v;

	seq_printf(m, "  %-44s %15lu %15lu\n", tp->call.name, tp->nhit,
		   tp->rp.kp.nmissed);
=======
	struct trace_kprobe *tk = v;
	unsigned long nhit = 0;
	int cpu;

	for_each_possible_cpu(cpu)
		nhit += *per_cpu_ptr(tk->nhit, cpu);

	seq_printf(m, "  %-44s %15lu %15lu\n",
		   trace_event_name(&tk->tp.call), nhit,
		   tk->rp.kp.nmissed);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

static const struct seq_operations profile_seq_op = {
	.start  = probes_seq_start,
	.next   = probes_seq_next,
	.stop   = probes_seq_stop,
	.show   = probes_profile_seq_show
};

static int profile_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &profile_seq_op);
}

static const struct file_operations kprobe_profile_ops = {
	.owner          = THIS_MODULE,
	.open           = profile_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
};

<<<<<<< HEAD
/* Sum up total data length for dynamic arraies (strings) */
static __kprobes int __get_data_size(struct trace_probe *tp,
				     struct pt_regs *regs)
{
	int i, ret = 0;
	u32 len;

	for (i = 0; i < tp->nr_args; i++)
		if (unlikely(tp->args[i].fetch_size.fn)) {
			call_fetch(&tp->args[i].fetch_size, regs, &len);
			ret += len;
		}

	return ret;
}

/* Store the value of each argument */
static __kprobes void store_trace_args(int ent_size, struct trace_probe *tp,
				       struct pt_regs *regs,
				       u8 *data, int maxlen)
{
	int i;
	u32 end = tp->size;
	u32 *dl;	/* Data (relative) location */

	for (i = 0; i < tp->nr_args; i++) {
		if (unlikely(tp->args[i].fetch_size.fn)) {
			/*
			 * First, we set the relative location and
			 * maximum data length to *dl
			 */
			dl = (u32 *)(data + tp->args[i].offset);
			*dl = make_data_rloc(maxlen, end - tp->args[i].offset);
			/* Then try to fetch string or dynamic array data */
			call_fetch(&tp->args[i].fetch, regs, dl);
			/* Reduce maximum length */
			end += get_rloc_len(*dl);
			maxlen -= get_rloc_len(*dl);
			/* Trick here, convert data_rloc to data_loc */
			*dl = convert_rloc_to_loc(*dl,
				 ent_size + tp->args[i].offset);
		} else
			/* Just fetching data normally */
			call_fetch(&tp->args[i].fetch, regs,
				   data + tp->args[i].offset);
	}
}

/* Kprobe handler */
static __kprobes void
__kprobe_trace_func(struct trace_probe *tp, struct pt_regs *regs,
		    struct ftrace_event_file *ftrace_file)
=======
/* Kprobe handler */
static nokprobe_inline void
__kprobe_trace_func(struct trace_kprobe *tk, struct pt_regs *regs,
		    struct trace_event_file *trace_file)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct kprobe_trace_entry_head *entry;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size, dsize, pc;
	unsigned long irq_flags;
<<<<<<< HEAD
	struct ftrace_event_call *call = &tp->call;

	WARN_ON(call != ftrace_file->event_call);

	if (test_bit(FTRACE_EVENT_FL_SOFT_DISABLED_BIT, &ftrace_file->flags))
=======
	struct trace_event_call *call = &tk->tp.call;

	WARN_ON(call != trace_file->event_call);

	if (trace_trigger_soft_disabled(trace_file))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	local_save_flags(irq_flags);
	pc = preempt_count();

<<<<<<< HEAD
	dsize = __get_data_size(tp, regs);
	size = sizeof(*entry) + tp->size + dsize;

	event = trace_event_buffer_lock_reserve(&buffer, ftrace_file,
=======
	dsize = __get_data_size(&tk->tp, regs);
	size = sizeof(*entry) + tk->tp.size + dsize;

	event = trace_event_buffer_lock_reserve(&buffer, trace_file,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						call->event.type,
						size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
<<<<<<< HEAD
	entry->ip = (unsigned long)tp->rp.kp.addr;
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	if (!filter_current_check_discard(buffer, call, entry, event))
		trace_buffer_unlock_commit_regs(buffer, event,
						irq_flags, pc, regs);
}

static __kprobes void
kprobe_trace_func(struct trace_probe *tp, struct pt_regs *regs)
{
	/*
	 * Note: preempt is already disabled around the kprobe handler.
	 * However, we still need an smp_read_barrier_depends() corresponding
	 * to smp_wmb() in rcu_assign_pointer() to access the pointer.
	 */
	struct ftrace_event_file **file = rcu_dereference_raw(tp->files);

	if (unlikely(!file))
		return;

	while (*file) {
		__kprobe_trace_func(tp, regs, *file);
		file++;
	}
}

/* Kretprobe handler */
static __kprobes void
__kretprobe_trace_func(struct trace_probe *tp, struct kretprobe_instance *ri,
		       struct pt_regs *regs,
		       struct ftrace_event_file *ftrace_file)
=======
	entry->ip = (unsigned long)tk->rp.kp.addr;
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);

	event_trigger_unlock_commit_regs(trace_file, buffer, event,
					 entry, irq_flags, pc, regs);
}

static void
kprobe_trace_func(struct trace_kprobe *tk, struct pt_regs *regs)
{
	struct event_file_link *link;

	list_for_each_entry_rcu(link, &tk->tp.files, list)
		__kprobe_trace_func(tk, regs, link->file);
}
NOKPROBE_SYMBOL(kprobe_trace_func);

/* Kretprobe handler */
static nokprobe_inline void
__kretprobe_trace_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		       struct pt_regs *regs,
		       struct trace_event_file *trace_file)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct kretprobe_trace_entry_head *entry;
	struct ring_buffer_event *event;
	struct ring_buffer *buffer;
	int size, pc, dsize;
	unsigned long irq_flags;
<<<<<<< HEAD
	struct ftrace_event_call *call = &tp->call;

	WARN_ON(call != ftrace_file->event_call);

	if (test_bit(FTRACE_EVENT_FL_SOFT_DISABLED_BIT, &ftrace_file->flags))
=======
	struct trace_event_call *call = &tk->tp.call;

	WARN_ON(call != trace_file->event_call);

	if (trace_trigger_soft_disabled(trace_file))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	local_save_flags(irq_flags);
	pc = preempt_count();

<<<<<<< HEAD
	dsize = __get_data_size(tp, regs);
	size = sizeof(*entry) + tp->size + dsize;

	event = trace_event_buffer_lock_reserve(&buffer, ftrace_file,
=======
	dsize = __get_data_size(&tk->tp, regs);
	size = sizeof(*entry) + tk->tp.size + dsize;

	event = trace_event_buffer_lock_reserve(&buffer, trace_file,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						call->event.type,
						size, irq_flags, pc);
	if (!event)
		return;

	entry = ring_buffer_event_data(event);
<<<<<<< HEAD
	entry->func = (unsigned long)tp->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	if (!filter_current_check_discard(buffer, call, entry, event))
		trace_buffer_unlock_commit_regs(buffer, event,
						irq_flags, pc, regs);
}

static __kprobes void
kretprobe_trace_func(struct trace_probe *tp, struct kretprobe_instance *ri,
		     struct pt_regs *regs)
{
	/*
	 * Note: preempt is already disabled around the kprobe handler.
	 * However, we still need an smp_read_barrier_depends() corresponding
	 * to smp_wmb() in rcu_assign_pointer() to access the pointer.
	 */
	struct ftrace_event_file **file = rcu_dereference_raw(tp->files);

	if (unlikely(!file))
		return;

	while (*file) {
		__kretprobe_trace_func(tp, ri, regs, *file);
		file++;
	}
}
=======
	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);

	event_trigger_unlock_commit_regs(trace_file, buffer, event,
					 entry, irq_flags, pc, regs);
}

static void
kretprobe_trace_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		     struct pt_regs *regs)
{
	struct event_file_link *link;

	list_for_each_entry_rcu(link, &tk->tp.files, list)
		__kretprobe_trace_func(tk, ri, regs, link->file);
}
NOKPROBE_SYMBOL(kretprobe_trace_func);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Event entry printers */
static enum print_line_t
print_kprobe_event(struct trace_iterator *iter, int flags,
		   struct trace_event *event)
{
	struct kprobe_trace_entry_head *field;
	struct trace_seq *s = &iter->seq;
	struct trace_probe *tp;
	u8 *data;
	int i;

	field = (struct kprobe_trace_entry_head *)iter->ent;
	tp = container_of(event, struct trace_probe, call.event);

<<<<<<< HEAD
	if (!trace_seq_printf(s, "%s: (", tp->call.name))
		goto partial;

	if (!seq_print_ip_sym(s, field->ip, flags | TRACE_ITER_SYM_OFFSET))
		goto partial;

	if (!trace_seq_puts(s, ")"))
		goto partial;
=======
	trace_seq_printf(s, "%s: (", trace_event_name(&tp->call));

	if (!seq_print_ip_sym(s, field->ip, flags | TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_putc(s, ')');
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	data = (u8 *)&field[1];
	for (i = 0; i < tp->nr_args; i++)
		if (!tp->args[i].type->print(s, tp->args[i].name,
					     data + tp->args[i].offset, field))
<<<<<<< HEAD
			goto partial;

	if (!trace_seq_puts(s, "\n"))
		goto partial;

	return TRACE_TYPE_HANDLED;
partial:
	return TRACE_TYPE_PARTIAL_LINE;
=======
			goto out;

	trace_seq_putc(s, '\n');
 out:
	return trace_handle_return(s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static enum print_line_t
print_kretprobe_event(struct trace_iterator *iter, int flags,
		      struct trace_event *event)
{
	struct kretprobe_trace_entry_head *field;
	struct trace_seq *s = &iter->seq;
	struct trace_probe *tp;
	u8 *data;
	int i;

	field = (struct kretprobe_trace_entry_head *)iter->ent;
	tp = container_of(event, struct trace_probe, call.event);

<<<<<<< HEAD
	if (!trace_seq_printf(s, "%s: (", tp->call.name))
		goto partial;

	if (!seq_print_ip_sym(s, field->ret_ip, flags | TRACE_ITER_SYM_OFFSET))
		goto partial;

	if (!trace_seq_puts(s, " <- "))
		goto partial;

	if (!seq_print_ip_sym(s, field->func, flags & ~TRACE_ITER_SYM_OFFSET))
		goto partial;

	if (!trace_seq_puts(s, ")"))
		goto partial;
=======
	trace_seq_printf(s, "%s: (", trace_event_name(&tp->call));

	if (!seq_print_ip_sym(s, field->ret_ip, flags | TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_puts(s, " <- ");

	if (!seq_print_ip_sym(s, field->func, flags & ~TRACE_ITER_SYM_OFFSET))
		goto out;

	trace_seq_putc(s, ')');
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	data = (u8 *)&field[1];
	for (i = 0; i < tp->nr_args; i++)
		if (!tp->args[i].type->print(s, tp->args[i].name,
					     data + tp->args[i].offset, field))
<<<<<<< HEAD
			goto partial;

	if (!trace_seq_puts(s, "\n"))
		goto partial;

	return TRACE_TYPE_HANDLED;
partial:
	return TRACE_TYPE_PARTIAL_LINE;
}


static int kprobe_event_define_fields(struct ftrace_event_call *event_call)
{
	int ret, i;
	struct kprobe_trace_entry_head field;
	struct trace_probe *tp = (struct trace_probe *)event_call->data;

	DEFINE_FIELD(unsigned long, ip, FIELD_STRING_IP, 0);
	/* Set argument names as fields */
	for (i = 0; i < tp->nr_args; i++) {
		ret = trace_define_field(event_call, tp->args[i].type->fmttype,
					 tp->args[i].name,
					 sizeof(field) + tp->args[i].offset,
					 tp->args[i].type->size,
					 tp->args[i].type->is_signed,
=======
			goto out;

	trace_seq_putc(s, '\n');

 out:
	return trace_handle_return(s);
}


static int kprobe_event_define_fields(struct trace_event_call *event_call)
{
	int ret, i;
	struct kprobe_trace_entry_head field;
	struct trace_kprobe *tk = (struct trace_kprobe *)event_call->data;

	DEFINE_FIELD(unsigned long, ip, FIELD_STRING_IP, 0);
	/* Set argument names as fields */
	for (i = 0; i < tk->tp.nr_args; i++) {
		struct probe_arg *parg = &tk->tp.args[i];

		ret = trace_define_field(event_call, parg->type->fmttype,
					 parg->name,
					 sizeof(field) + parg->offset,
					 parg->type->size,
					 parg->type->is_signed,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					 FILTER_OTHER);
		if (ret)
			return ret;
	}
	return 0;
}

<<<<<<< HEAD
static int kretprobe_event_define_fields(struct ftrace_event_call *event_call)
{
	int ret, i;
	struct kretprobe_trace_entry_head field;
	struct trace_probe *tp = (struct trace_probe *)event_call->data;
=======
static int kretprobe_event_define_fields(struct trace_event_call *event_call)
{
	int ret, i;
	struct kretprobe_trace_entry_head field;
	struct trace_kprobe *tk = (struct trace_kprobe *)event_call->data;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	DEFINE_FIELD(unsigned long, func, FIELD_STRING_FUNC, 0);
	DEFINE_FIELD(unsigned long, ret_ip, FIELD_STRING_RETIP, 0);
	/* Set argument names as fields */
<<<<<<< HEAD
	for (i = 0; i < tp->nr_args; i++) {
		ret = trace_define_field(event_call, tp->args[i].type->fmttype,
					 tp->args[i].name,
					 sizeof(field) + tp->args[i].offset,
					 tp->args[i].type->size,
					 tp->args[i].type->is_signed,
=======
	for (i = 0; i < tk->tp.nr_args; i++) {
		struct probe_arg *parg = &tk->tp.args[i];

		ret = trace_define_field(event_call, parg->type->fmttype,
					 parg->name,
					 sizeof(field) + parg->offset,
					 parg->type->size,
					 parg->type->is_signed,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					 FILTER_OTHER);
		if (ret)
			return ret;
	}
	return 0;
}

<<<<<<< HEAD
static int __set_print_fmt(struct trace_probe *tp, char *buf, int len)
{
	int i;
	int pos = 0;

	const char *fmt, *arg;

	if (!trace_probe_is_return(tp)) {
		fmt = "(%lx)";
		arg = "REC->" FIELD_STRING_IP;
	} else {
		fmt = "(%lx <- %lx)";
		arg = "REC->" FIELD_STRING_FUNC ", REC->" FIELD_STRING_RETIP;
	}

	/* When len=0, we just calculate the needed length */
#define LEN_OR_ZERO (len ? len - pos : 0)

	pos += snprintf(buf + pos, LEN_OR_ZERO, "\"%s", fmt);

	for (i = 0; i < tp->nr_args; i++) {
		pos += snprintf(buf + pos, LEN_OR_ZERO, " %s=%s",
				tp->args[i].name, tp->args[i].type->fmt);
	}

	pos += snprintf(buf + pos, LEN_OR_ZERO, "\", %s", arg);

	for (i = 0; i < tp->nr_args; i++) {
		if (strcmp(tp->args[i].type->name, "string") == 0)
			pos += snprintf(buf + pos, LEN_OR_ZERO,
					", __get_str(%s)",
					tp->args[i].name);
		else
			pos += snprintf(buf + pos, LEN_OR_ZERO, ", REC->%s",
					tp->args[i].name);
	}

#undef LEN_OR_ZERO

	/* return the length of print_fmt */
	return pos;
}

static int set_print_fmt(struct trace_probe *tp)
{
	int len;
	char *print_fmt;

	/* First: called with 0 length to calculate the needed length */
	len = __set_print_fmt(tp, NULL, 0);
	print_fmt = kmalloc(len + 1, GFP_KERNEL);
	if (!print_fmt)
		return -ENOMEM;

	/* Second: actually write the @print_fmt */
	__set_print_fmt(tp, print_fmt, len + 1);
	tp->call.print_fmt = print_fmt;

	return 0;
}

#ifdef CONFIG_PERF_EVENTS

/* Kprobe profile handler */
static __kprobes void
kprobe_perf_func(struct trace_probe *tp, struct pt_regs *regs)
{
	struct ftrace_event_call *call = &tp->call;
=======
#ifdef CONFIG_PERF_EVENTS

/* Kprobe profile handler */
static void
kprobe_perf_func(struct trace_kprobe *tk, struct pt_regs *regs)
{
	struct trace_event_call *call = &tk->tp.call;
	struct bpf_prog *prog = call->prog;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct kprobe_trace_entry_head *entry;
	struct hlist_head *head;
	int size, __size, dsize;
	int rctx;

<<<<<<< HEAD
	dsize = __get_data_size(tp, regs);
	__size = sizeof(*entry) + tp->size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);
	if (WARN_ONCE(size > PERF_MAX_TRACE_SIZE,
		     "profile buffer not large enough"))
		return;

	entry = perf_trace_buf_prepare(size, call->event.type, regs, &rctx);
	if (!entry)
		return;

	entry->ip = (unsigned long)tp->rp.kp.addr;
	memset(&entry[1], 0, dsize);
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	head = this_cpu_ptr(call->perf_events);
	perf_trace_buf_submit(entry, size, rctx,
					entry->ip, 1, regs, head, NULL);
}

/* Kretprobe profile handler */
static __kprobes void
kretprobe_perf_func(struct trace_probe *tp, struct kretprobe_instance *ri,
		    struct pt_regs *regs)
{
	struct ftrace_event_call *call = &tp->call;
=======
	if (prog && !trace_call_bpf(prog, regs))
		return;

	head = this_cpu_ptr(call->perf_events);
	if (hlist_empty(head))
		return;

	dsize = __get_data_size(&tk->tp, regs);
	__size = sizeof(*entry) + tk->tp.size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

	entry = perf_trace_buf_alloc(size, NULL, &rctx);
	if (!entry)
		return;

	entry->ip = (unsigned long)tk->rp.kp.addr;
	memset(&entry[1], 0, dsize);
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);
	perf_trace_buf_submit(entry, size, rctx, call->event.type, 1, regs,
			      head, NULL);
}
NOKPROBE_SYMBOL(kprobe_perf_func);

/* Kretprobe profile handler */
static void
kretprobe_perf_func(struct trace_kprobe *tk, struct kretprobe_instance *ri,
		    struct pt_regs *regs)
{
	struct trace_event_call *call = &tk->tp.call;
	struct bpf_prog *prog = call->prog;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct kretprobe_trace_entry_head *entry;
	struct hlist_head *head;
	int size, __size, dsize;
	int rctx;

<<<<<<< HEAD
	dsize = __get_data_size(tp, regs);
	__size = sizeof(*entry) + tp->size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);
	if (WARN_ONCE(size > PERF_MAX_TRACE_SIZE,
		     "profile buffer not large enough"))
		return;

	entry = perf_trace_buf_prepare(size, call->event.type, regs, &rctx);
	if (!entry)
		return;

	entry->func = (unsigned long)tp->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), tp, regs, (u8 *)&entry[1], dsize);

	head = this_cpu_ptr(call->perf_events);
	perf_trace_buf_submit(entry, size, rctx,
					entry->ret_ip, 1, regs, head, NULL);
}
#endif	/* CONFIG_PERF_EVENTS */

static __kprobes
int kprobe_register(struct ftrace_event_call *event,
		    enum trace_reg type, void *data)
{
	struct trace_probe *tp = (struct trace_probe *)event->data;
	struct ftrace_event_file *file = data;

	switch (type) {
	case TRACE_REG_REGISTER:
		return enable_trace_probe(tp, file);
	case TRACE_REG_UNREGISTER:
		return disable_trace_probe(tp, file);

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return enable_trace_probe(tp, NULL);
	case TRACE_REG_PERF_UNREGISTER:
		return disable_trace_probe(tp, NULL);
=======
	if (prog && !trace_call_bpf(prog, regs))
		return;

	head = this_cpu_ptr(call->perf_events);
	if (hlist_empty(head))
		return;

	dsize = __get_data_size(&tk->tp, regs);
	__size = sizeof(*entry) + tk->tp.size + dsize;
	size = ALIGN(__size + sizeof(u32), sizeof(u64));
	size -= sizeof(u32);

	entry = perf_trace_buf_alloc(size, NULL, &rctx);
	if (!entry)
		return;

	entry->func = (unsigned long)tk->rp.kp.addr;
	entry->ret_ip = (unsigned long)ri->ret_addr;
	store_trace_args(sizeof(*entry), &tk->tp, regs, (u8 *)&entry[1], dsize);
	perf_trace_buf_submit(entry, size, rctx, call->event.type, 1, regs,
			      head, NULL);
}
NOKPROBE_SYMBOL(kretprobe_perf_func);
#endif	/* CONFIG_PERF_EVENTS */

/*
 * called by perf_trace_init() or __ftrace_set_clr_event() under event_mutex.
 *
 * kprobe_trace_self_tests_init() does enable_trace_probe/disable_trace_probe
 * lockless, but we can't race with this __init function.
 */
static int kprobe_register(struct trace_event_call *event,
			   enum trace_reg type, void *data)
{
	struct trace_kprobe *tk = (struct trace_kprobe *)event->data;
	struct trace_event_file *file = data;

	switch (type) {
	case TRACE_REG_REGISTER:
		return enable_trace_kprobe(tk, file);
	case TRACE_REG_UNREGISTER:
		return disable_trace_kprobe(tk, file);

#ifdef CONFIG_PERF_EVENTS
	case TRACE_REG_PERF_REGISTER:
		return enable_trace_kprobe(tk, NULL);
	case TRACE_REG_PERF_UNREGISTER:
		return disable_trace_kprobe(tk, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case TRACE_REG_PERF_OPEN:
	case TRACE_REG_PERF_CLOSE:
	case TRACE_REG_PERF_ADD:
	case TRACE_REG_PERF_DEL:
		return 0;
#endif
	}
	return 0;
}

<<<<<<< HEAD
static __kprobes
int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(kp, struct trace_probe, rp.kp);

	tp->nhit++;

	if (tp->flags & TP_FLAG_TRACE)
		kprobe_trace_func(tp, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tp->flags & TP_FLAG_PROFILE)
		kprobe_perf_func(tp, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
}

static __kprobes
int kretprobe_dispatcher(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct trace_probe *tp = container_of(ri->rp, struct trace_probe, rp);

	tp->nhit++;

	if (tp->flags & TP_FLAG_TRACE)
		kretprobe_trace_func(tp, ri, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tp->flags & TP_FLAG_PROFILE)
		kretprobe_perf_func(tp, ri, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
}
=======
static int kprobe_dispatcher(struct kprobe *kp, struct pt_regs *regs)
{
	struct trace_kprobe *tk = container_of(kp, struct trace_kprobe, rp.kp);

	raw_cpu_inc(*tk->nhit);

	if (tk->tp.flags & TP_FLAG_TRACE)
		kprobe_trace_func(tk, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tk->tp.flags & TP_FLAG_PROFILE)
		kprobe_perf_func(tk, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
}
NOKPROBE_SYMBOL(kprobe_dispatcher);

static int
kretprobe_dispatcher(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct trace_kprobe *tk = container_of(ri->rp, struct trace_kprobe, rp);

	raw_cpu_inc(*tk->nhit);

	if (tk->tp.flags & TP_FLAG_TRACE)
		kretprobe_trace_func(tk, ri, regs);
#ifdef CONFIG_PERF_EVENTS
	if (tk->tp.flags & TP_FLAG_PROFILE)
		kretprobe_perf_func(tk, ri, regs);
#endif
	return 0;	/* We don't tweek kernel, so just return 0 */
}
NOKPROBE_SYMBOL(kretprobe_dispatcher);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct trace_event_functions kretprobe_funcs = {
	.trace		= print_kretprobe_event
};

static struct trace_event_functions kprobe_funcs = {
	.trace		= print_kprobe_event
};

<<<<<<< HEAD
static int register_probe_event(struct trace_probe *tp)
{
	struct ftrace_event_call *call = &tp->call;
	int ret;

	/* Initialize ftrace_event_call */
	INIT_LIST_HEAD(&call->class->fields);
	if (trace_probe_is_return(tp)) {
=======
static int register_kprobe_event(struct trace_kprobe *tk)
{
	struct trace_event_call *call = &tk->tp.call;
	int ret;

	/* Initialize trace_event_call */
	INIT_LIST_HEAD(&call->class->fields);
	if (trace_kprobe_is_return(tk)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		call->event.funcs = &kretprobe_funcs;
		call->class->define_fields = kretprobe_event_define_fields;
	} else {
		call->event.funcs = &kprobe_funcs;
		call->class->define_fields = kprobe_event_define_fields;
	}
<<<<<<< HEAD
	if (set_print_fmt(tp) < 0)
		return -ENOMEM;
	ret = register_ftrace_event(&call->event);
=======
	if (set_print_fmt(&tk->tp, trace_kprobe_is_return(tk)) < 0)
		return -ENOMEM;
	ret = register_trace_event(&call->event);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!ret) {
		kfree(call->print_fmt);
		return -ENODEV;
	}
<<<<<<< HEAD
	call->flags = 0;
	call->class->reg = kprobe_register;
	call->data = tp;
	ret = trace_add_event_call(call);
	if (ret) {
		pr_info("Failed to register kprobe event: %s\n", call->name);
		kfree(call->print_fmt);
		unregister_ftrace_event(&call->event);
=======
	call->flags = TRACE_EVENT_FL_KPROBE;
	call->class->reg = kprobe_register;
	call->data = tk;
	ret = trace_add_event_call(call);
	if (ret) {
		pr_info("Failed to register kprobe event: %s\n",
			trace_event_name(call));
		kfree(call->print_fmt);
		unregister_trace_event(&call->event);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return ret;
}

<<<<<<< HEAD
static int unregister_probe_event(struct trace_probe *tp)
=======
static int unregister_kprobe_event(struct trace_kprobe *tk)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int ret;

	/* tp->event is unregistered in trace_remove_event_call() */
<<<<<<< HEAD
	ret = trace_remove_event_call(&tp->call);
	if (!ret)
		kfree(tp->call.print_fmt);
	return ret;
}

/* Make a debugfs interface for controlling probe points */
=======
	ret = trace_remove_event_call(&tk->tp.call);
	if (!ret)
		kfree(tk->tp.call.print_fmt);
	return ret;
}

/* Make a tracefs interface for controlling probe points */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static __init int init_kprobe_trace(void)
{
	struct dentry *d_tracer;
	struct dentry *entry;

<<<<<<< HEAD
	if (register_module_notifier(&trace_probe_module_nb))
		return -EINVAL;

	d_tracer = tracing_init_dentry();
	if (!d_tracer)
		return 0;

	entry = debugfs_create_file("kprobe_events", 0644, d_tracer,
=======
	if (register_module_notifier(&trace_kprobe_module_nb))
		return -EINVAL;

	d_tracer = tracing_init_dentry();
	if (IS_ERR(d_tracer))
		return 0;

	entry = tracefs_create_file("kprobe_events", 0644, d_tracer,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				    NULL, &kprobe_events_ops);

	/* Event list interface */
	if (!entry)
<<<<<<< HEAD
		pr_warning("Could not create debugfs "
			   "'kprobe_events' entry\n");

	/* Profile interface */
	entry = debugfs_create_file("kprobe_profile", 0444, d_tracer,
				    NULL, &kprobe_profile_ops);

	if (!entry)
		pr_warning("Could not create debugfs "
			   "'kprobe_profile' entry\n");
=======
		pr_warn("Could not create tracefs 'kprobe_events' entry\n");

	/* Profile interface */
	entry = tracefs_create_file("kprobe_profile", 0444, d_tracer,
				    NULL, &kprobe_profile_ops);

	if (!entry)
		pr_warn("Could not create tracefs 'kprobe_profile' entry\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
fs_initcall(init_kprobe_trace);


#ifdef CONFIG_FTRACE_STARTUP_TEST

/*
 * The "__used" keeps gcc from removing the function symbol
 * from the kallsyms table.
 */
static __used int kprobe_trace_selftest_target(int a1, int a2, int a3,
					       int a4, int a5, int a6)
{
	return a1 + a2 + a3 + a4 + a5 + a6;
}

<<<<<<< HEAD
static struct ftrace_event_file *
find_trace_probe_file(struct trace_probe *tp, struct trace_array *tr)
{
	struct ftrace_event_file *file;

	list_for_each_entry(file, &tr->events, list)
		if (file->event_call == &tp->call)
=======
static struct trace_event_file *
find_trace_probe_file(struct trace_kprobe *tk, struct trace_array *tr)
{
	struct trace_event_file *file;

	list_for_each_entry(file, &tr->events, list)
		if (file->event_call == &tk->tp.call)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return file;

	return NULL;
}

<<<<<<< HEAD
=======
/*
 * Nobody but us can call enable_trace_kprobe/disable_trace_kprobe at this
 * stage, we can do this lockless.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static __init int kprobe_trace_self_tests_init(void)
{
	int ret, warn = 0;
	int (*target)(int, int, int, int, int, int);
<<<<<<< HEAD
	struct trace_probe *tp;
	struct ftrace_event_file *file;
=======
	struct trace_kprobe *tk;
	struct trace_event_file *file;

	if (tracing_is_disabled())
		return -ENODEV;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	target = kprobe_trace_selftest_target;

	pr_info("Testing kprobe tracing: ");

	ret = traceprobe_command("p:testprobe kprobe_trace_selftest_target "
				  "$stack $stack0 +0($stack)",
<<<<<<< HEAD
				  create_trace_probe);
=======
				  create_trace_kprobe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on probing function entry.\n");
		warn++;
	} else {
		/* Enable trace point */
<<<<<<< HEAD
		tp = find_trace_probe("testprobe", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tp == NULL)) {
			pr_warn("error on getting new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tp, top_trace_array());
=======
		tk = find_trace_kprobe("testprobe", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tk == NULL)) {
			pr_warn("error on getting new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tk, top_trace_array());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (WARN_ON_ONCE(file == NULL)) {
				pr_warn("error on getting probe file.\n");
				warn++;
			} else
<<<<<<< HEAD
				enable_trace_probe(tp, file);
=======
				enable_trace_kprobe(tk, file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	ret = traceprobe_command("r:testprobe2 kprobe_trace_selftest_target "
<<<<<<< HEAD
				  "$retval", create_trace_probe);
=======
				  "$retval", create_trace_kprobe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on probing function return.\n");
		warn++;
	} else {
		/* Enable trace point */
<<<<<<< HEAD
		tp = find_trace_probe("testprobe2", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tp == NULL)) {
			pr_warn("error on getting 2nd new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tp, top_trace_array());
=======
		tk = find_trace_kprobe("testprobe2", KPROBE_EVENT_SYSTEM);
		if (WARN_ON_ONCE(tk == NULL)) {
			pr_warn("error on getting 2nd new probe.\n");
			warn++;
		} else {
			file = find_trace_probe_file(tk, top_trace_array());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (WARN_ON_ONCE(file == NULL)) {
				pr_warn("error on getting probe file.\n");
				warn++;
			} else
<<<<<<< HEAD
				enable_trace_probe(tp, file);
=======
				enable_trace_kprobe(tk, file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	if (warn)
		goto end;

	ret = target(1, 2, 3, 4, 5, 6);

	/* Disable trace points before removing it */
<<<<<<< HEAD
	tp = find_trace_probe("testprobe", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tp == NULL)) {
		pr_warn("error on getting test probe.\n");
		warn++;
	} else {
		file = find_trace_probe_file(tp, top_trace_array());
=======
	tk = find_trace_kprobe("testprobe", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tk == NULL)) {
		pr_warn("error on getting test probe.\n");
		warn++;
	} else {
		file = find_trace_probe_file(tk, top_trace_array());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (WARN_ON_ONCE(file == NULL)) {
			pr_warn("error on getting probe file.\n");
			warn++;
		} else
<<<<<<< HEAD
			disable_trace_probe(tp, file);
	}

	tp = find_trace_probe("testprobe2", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tp == NULL)) {
		pr_warn("error on getting 2nd test probe.\n");
		warn++;
	} else {
		file = find_trace_probe_file(tp, top_trace_array());
=======
			disable_trace_kprobe(tk, file);
	}

	tk = find_trace_kprobe("testprobe2", KPROBE_EVENT_SYSTEM);
	if (WARN_ON_ONCE(tk == NULL)) {
		pr_warn("error on getting 2nd test probe.\n");
		warn++;
	} else {
		file = find_trace_probe_file(tk, top_trace_array());
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (WARN_ON_ONCE(file == NULL)) {
			pr_warn("error on getting probe file.\n");
			warn++;
		} else
<<<<<<< HEAD
			disable_trace_probe(tp, file);
	}

	ret = traceprobe_command("-:testprobe", create_trace_probe);
=======
			disable_trace_kprobe(tk, file);
	}

	ret = traceprobe_command("-:testprobe", create_trace_kprobe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on deleting a probe.\n");
		warn++;
	}

<<<<<<< HEAD
	ret = traceprobe_command("-:testprobe2", create_trace_probe);
=======
	ret = traceprobe_command("-:testprobe2", create_trace_kprobe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (WARN_ON_ONCE(ret)) {
		pr_warn("error on deleting a probe.\n");
		warn++;
	}

end:
<<<<<<< HEAD
	release_all_trace_probes();
=======
	release_all_trace_kprobes();
	/*
	 * Wait for the optimizer work to finish. Otherwise it might fiddle
	 * with probes in already freed __init text.
	 */
	wait_for_kprobe_optimizer();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (warn)
		pr_cont("NG: Some tests are failed. Please check them.\n");
	else
		pr_cont("OK\n");
	return 0;
}

late_initcall(kprobe_trace_self_tests_init);

#endif
