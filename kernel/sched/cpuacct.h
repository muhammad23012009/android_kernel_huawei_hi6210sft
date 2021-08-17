#ifdef CONFIG_CGROUP_CPUACCT

extern void cpuacct_charge(struct task_struct *tsk, u64 cputime);
<<<<<<< HEAD
extern void cpuacct_account_field(struct task_struct *p, int index, u64 val);
=======
extern void cpuacct_account_field(struct task_struct *tsk, int index, u64 val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#else

static inline void cpuacct_charge(struct task_struct *tsk, u64 cputime)
{
}

static inline void
<<<<<<< HEAD
cpuacct_account_field(struct task_struct *p, int index, u64 val)
=======
cpuacct_account_field(struct task_struct *tsk, int index, u64 val)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
}

#endif
