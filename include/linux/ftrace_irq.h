#ifndef _LINUX_FTRACE_IRQ_H
#define _LINUX_FTRACE_IRQ_H


#ifdef CONFIG_FTRACE_NMI_ENTER
<<<<<<< HEAD
extern void ftrace_nmi_enter(void);
extern void ftrace_nmi_exit(void);
#else
static inline void ftrace_nmi_enter(void) { }
static inline void ftrace_nmi_exit(void) { }
#endif

=======
extern void arch_ftrace_nmi_enter(void);
extern void arch_ftrace_nmi_exit(void);
#else
static inline void arch_ftrace_nmi_enter(void) { }
static inline void arch_ftrace_nmi_exit(void) { }
#endif

#ifdef CONFIG_HWLAT_TRACER
extern bool trace_hwlat_callback_enabled;
extern void trace_hwlat_callback(bool enter);
#endif

static inline void ftrace_nmi_enter(void)
{
#ifdef CONFIG_HWLAT_TRACER
	if (trace_hwlat_callback_enabled)
		trace_hwlat_callback(true);
#endif
	arch_ftrace_nmi_enter();
}

static inline void ftrace_nmi_exit(void)
{
	arch_ftrace_nmi_exit();
#ifdef CONFIG_HWLAT_TRACER
	if (trace_hwlat_callback_enabled)
		trace_hwlat_callback(false);
#endif
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _LINUX_FTRACE_IRQ_H */
