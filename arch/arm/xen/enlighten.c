#include <xen/xen.h>
#include <xen/events.h>
#include <xen/grant_table.h>
#include <xen/hvm.h>
#include <xen/interface/vcpu.h>
#include <xen/interface/xen.h>
#include <xen/interface/memory.h>
#include <xen/interface/hvm/params.h>
#include <xen/features.h>
#include <xen/platform_pci.h>
#include <xen/xenbus.h>
#include <xen/page.h>
#include <xen/interface/sched.h>
#include <xen/xen-ops.h>
#include <asm/xen/hypervisor.h>
#include <asm/xen/hypercall.h>
<<<<<<< HEAD
#include <asm/system_misc.h>
=======
#include <asm/xen/xen-ops.h>
#include <asm/system_misc.h>
#include <asm/efi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/module.h>
#include <linux/of.h>
<<<<<<< HEAD
#include <linux/of_irq.h>
#include <linux/of_address.h>
=======
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/cpuidle.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/console.h>
#include <linux/pvclock_gtod.h>
#include <linux/time64.h>
#include <linux/timekeeping.h>
#include <linux/timekeeper_internal.h>
#include <linux/acpi.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/mm.h>

struct start_info _xen_start_info;
struct start_info *xen_start_info = &_xen_start_info;
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(xen_start_info);

enum xen_domain_type xen_domain_type = XEN_NATIVE;
EXPORT_SYMBOL_GPL(xen_domain_type);
=======
EXPORT_SYMBOL(xen_start_info);

enum xen_domain_type xen_domain_type = XEN_NATIVE;
EXPORT_SYMBOL(xen_domain_type);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct shared_info xen_dummy_shared_info;
struct shared_info *HYPERVISOR_shared_info = (void *)&xen_dummy_shared_info;

DEFINE_PER_CPU(struct vcpu_info *, xen_vcpu);
static struct vcpu_info __percpu *xen_vcpu_info;

<<<<<<< HEAD
=======
/* Linux <-> Xen vCPU id mapping */
DEFINE_PER_CPU(uint32_t, xen_vcpu_id);
EXPORT_PER_CPU_SYMBOL(xen_vcpu_id);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* These are unused until we support booting "pre-ballooned" */
unsigned long xen_released_pages;
struct xen_memory_region xen_extra_mem[XEN_EXTRA_MEM_MAX_REGIONS] __initdata;

<<<<<<< HEAD
/* TODO: to be removed */
__read_mostly int xen_have_vector_callback;
EXPORT_SYMBOL_GPL(xen_have_vector_callback);

int xen_platform_pci_unplug = XEN_UNPLUG_ALL;
EXPORT_SYMBOL_GPL(xen_platform_pci_unplug);

static __read_mostly int xen_events_irq = -1;

/* map fgmfn of domid to lpfn in the current domain */
static int map_foreign_page(unsigned long lpfn, unsigned long fgmfn,
			    unsigned int domid)
{
	int rc;
	struct xen_add_to_physmap_range xatp = {
		.domid = DOMID_SELF,
		.foreign_domid = domid,
		.size = 1,
		.space = XENMAPSPACE_gmfn_foreign,
	};
	xen_ulong_t idx = fgmfn;
	xen_pfn_t gpfn = lpfn;
	int err = 0;

	set_xen_guest_handle(xatp.idxs, &idx);
	set_xen_guest_handle(xatp.gpfns, &gpfn);
	set_xen_guest_handle(xatp.errs, &err);

	rc = HYPERVISOR_memory_op(XENMEM_add_to_physmap_range, &xatp);
	if (rc || err) {
		pr_warn("Failed to map pfn to mfn rc:%d:%d pfn:%lx mfn:%lx\n",
			rc, err, lpfn, fgmfn);
		return 1;
	}
	return 0;
}

struct remap_data {
	xen_pfn_t fgmfn; /* foreign domain's gmfn */
	pgprot_t prot;
	domid_t  domid;
	struct vm_area_struct *vma;
	int index;
	struct page **pages;
	struct xen_remap_mfn_info *info;
};

static int remap_pte_fn(pte_t *ptep, pgtable_t token, unsigned long addr,
			void *data)
{
	struct remap_data *info = data;
	struct page *page = info->pages[info->index++];
	unsigned long pfn = page_to_pfn(page);
	pte_t pte = pfn_pte(pfn, info->prot);

	if (map_foreign_page(pfn, info->fgmfn, info->domid))
		return -EFAULT;
	set_pte_at(info->vma->vm_mm, addr, ptep, pte);

	return 0;
}

int xen_remap_domain_mfn_range(struct vm_area_struct *vma,
			       unsigned long addr,
			       xen_pfn_t mfn, int nr,
			       pgprot_t prot, unsigned domid,
			       struct page **pages)
{
	int err;
	struct remap_data data;

	/* TBD: Batching, current sole caller only does page at a time */
	if (nr > 1)
		return -EINVAL;

	data.fgmfn = mfn;
	data.prot = prot;
	data.domid = domid;
	data.vma = vma;
	data.index = 0;
	data.pages = pages;
	err = apply_to_page_range(vma->vm_mm, addr, nr << PAGE_SHIFT,
				  remap_pte_fn, &data);
	return err;
}
EXPORT_SYMBOL_GPL(xen_remap_domain_mfn_range);

int xen_unmap_domain_mfn_range(struct vm_area_struct *vma,
			       int nr, struct page **pages)
{
	int i;

	for (i = 0; i < nr; i++) {
		struct xen_remove_from_physmap xrp;
		unsigned long rc, pfn;

		pfn = page_to_pfn(pages[i]);

		xrp.domid = DOMID_SELF;
		xrp.gpfn = pfn;
		rc = HYPERVISOR_memory_op(XENMEM_remove_from_physmap, &xrp);
		if (rc) {
			pr_warn("Failed to unmap pfn:%lx rc:%ld\n",
				pfn, rc);
			return rc;
		}
	}
	return 0;
}
EXPORT_SYMBOL_GPL(xen_unmap_domain_mfn_range);

static void __init xen_percpu_init(void *unused)
=======
static __read_mostly unsigned int xen_events_irq;

int xen_remap_domain_gfn_array(struct vm_area_struct *vma,
			       unsigned long addr,
			       xen_pfn_t *gfn, int nr,
			       int *err_ptr, pgprot_t prot,
			       unsigned domid,
			       struct page **pages)
{
	return xen_xlate_remap_gfn_array(vma, addr, gfn, nr, err_ptr,
					 prot, domid, pages);
}
EXPORT_SYMBOL_GPL(xen_remap_domain_gfn_array);

/* Not used by XENFEAT_auto_translated guests. */
int xen_remap_domain_gfn_range(struct vm_area_struct *vma,
                              unsigned long addr,
                              xen_pfn_t gfn, int nr,
                              pgprot_t prot, unsigned domid,
                              struct page **pages)
{
	return -ENOSYS;
}
EXPORT_SYMBOL_GPL(xen_remap_domain_gfn_range);

int xen_unmap_domain_gfn_range(struct vm_area_struct *vma,
			       int nr, struct page **pages)
{
	return xen_xlate_unmap_gfn_range(vma, nr, pages);
}
EXPORT_SYMBOL_GPL(xen_unmap_domain_gfn_range);

static void xen_read_wallclock(struct timespec64 *ts)
{
	u32 version;
	struct timespec64 now, ts_monotonic;
	struct shared_info *s = HYPERVISOR_shared_info;
	struct pvclock_wall_clock *wall_clock = &(s->wc);

	/* get wallclock at system boot */
	do {
		version = wall_clock->version;
		rmb();		/* fetch version before time */
		now.tv_sec  = ((uint64_t)wall_clock->sec_hi << 32) | wall_clock->sec;
		now.tv_nsec = wall_clock->nsec;
		rmb();		/* fetch time before checking version */
	} while ((wall_clock->version & 1) || (version != wall_clock->version));

	/* time since system boot */
	ktime_get_ts64(&ts_monotonic);
	*ts = timespec64_add(now, ts_monotonic);
}

static int xen_pvclock_gtod_notify(struct notifier_block *nb,
				   unsigned long was_set, void *priv)
{
	/* Protected by the calling core code serialization */
	static struct timespec64 next_sync;

	struct xen_platform_op op;
	struct timespec64 now, system_time;
	struct timekeeper *tk = priv;

	now.tv_sec = tk->xtime_sec;
	now.tv_nsec = (long)(tk->tkr_mono.xtime_nsec >> tk->tkr_mono.shift);
	system_time = timespec64_add(now, tk->wall_to_monotonic);

	/*
	 * We only take the expensive HV call when the clock was set
	 * or when the 11 minutes RTC synchronization time elapsed.
	 */
	if (!was_set && timespec64_compare(&now, &next_sync) < 0)
		return NOTIFY_OK;

	op.cmd = XENPF_settime64;
	op.u.settime64.mbz = 0;
	op.u.settime64.secs = now.tv_sec;
	op.u.settime64.nsecs = now.tv_nsec;
	op.u.settime64.system_time = timespec64_to_ns(&system_time);
	(void)HYPERVISOR_platform_op(&op);

	/*
	 * Move the next drift compensation time 11 minutes
	 * ahead. That's emulating the sync_cmos_clock() update for
	 * the hardware RTC.
	 */
	next_sync = now;
	next_sync.tv_sec += 11 * 60;

	return NOTIFY_OK;
}

static struct notifier_block xen_pvclock_gtod_notifier = {
	.notifier_call = xen_pvclock_gtod_notify,
};

static int xen_starting_cpu(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct vcpu_register_vcpu_info info;
	struct vcpu_info *vcpup;
	int err;
<<<<<<< HEAD
	int cpu = get_cpu();
=======

	/* 
	 * VCPUOP_register_vcpu_info cannot be called twice for the same
	 * vcpu, so if vcpu_info is already registered, just get out. This
	 * can happen with cpu-hotplug.
	 */
	if (per_cpu(xen_vcpu, cpu) != NULL)
		goto after_register_vcpu_info;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pr_info("Xen: initializing cpu%d\n", cpu);
	vcpup = per_cpu_ptr(xen_vcpu_info, cpu);

<<<<<<< HEAD
	info.mfn = __pa(vcpup) >> PAGE_SHIFT;
	info.offset = offset_in_page(vcpup);

	err = HYPERVISOR_vcpu_op(VCPUOP_register_vcpu_info, cpu, &info);
	BUG_ON(err);
	per_cpu(xen_vcpu, cpu) = vcpup;

	enable_percpu_irq(xen_events_irq, 0);
	put_cpu();
}

static void xen_restart(char str, const char *cmd)
=======
	info.mfn = virt_to_gfn(vcpup);
	info.offset = xen_offset_in_page(vcpup);

	err = HYPERVISOR_vcpu_op(VCPUOP_register_vcpu_info, xen_vcpu_nr(cpu),
				 &info);
	BUG_ON(err);
	per_cpu(xen_vcpu, cpu) = vcpup;

	xen_setup_runstate_info(cpu);

after_register_vcpu_info:
	enable_percpu_irq(xen_events_irq, 0);
	return 0;
}

static int xen_dying_cpu(unsigned int cpu)
{
	disable_percpu_irq(xen_events_irq);
	return 0;
}

static void xen_restart(enum reboot_mode reboot_mode, const char *cmd)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sched_shutdown r = { .reason = SHUTDOWN_reboot };
	int rc;
	rc = HYPERVISOR_sched_op(SCHEDOP_shutdown, &r);
<<<<<<< HEAD
	if (rc)
		BUG();
=======
	BUG_ON(rc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void xen_power_off(void)
{
	struct sched_shutdown r = { .reason = SHUTDOWN_poweroff };
	int rc;
	rc = HYPERVISOR_sched_op(SCHEDOP_shutdown, &r);
<<<<<<< HEAD
	if (rc)
		BUG();
=======
	BUG_ON(rc);
}

static irqreturn_t xen_arm_callback(int irq, void *arg)
{
	xen_hvm_evtchn_do_upcall();
	return IRQ_HANDLED;
}

static __initdata struct {
	const char *compat;
	const char *prefix;
	const char *version;
	bool found;
} hyper_node = {"xen,xen", "xen,xen-", NULL, false};

static int __init fdt_find_hyper_node(unsigned long node, const char *uname,
				      int depth, void *data)
{
	const void *s = NULL;
	int len;

	if (depth != 1 || strcmp(uname, "hypervisor") != 0)
		return 0;

	if (of_flat_dt_is_compatible(node, hyper_node.compat))
		hyper_node.found = true;

	s = of_get_flat_dt_prop(node, "compatible", &len);
	if (strlen(hyper_node.prefix) + 3  < len &&
	    !strncmp(hyper_node.prefix, s, strlen(hyper_node.prefix)))
		hyper_node.version = s + strlen(hyper_node.prefix);

	/*
	 * Check if Xen supports EFI by checking whether there is the
	 * "/hypervisor/uefi" node in DT. If so, runtime services are available
	 * through proxy functions (e.g. in case of Xen dom0 EFI implementation
	 * they call special hypercall which executes relevant EFI functions)
	 * and that is why they are always enabled.
	 */
	if (IS_ENABLED(CONFIG_XEN_EFI)) {
		if ((of_get_flat_dt_subnode_by_name(node, "uefi") > 0) &&
		    !efi_runtime_disabled())
			set_bit(EFI_RUNTIME_SERVICES, &efi.flags);
	}

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * see Documentation/devicetree/bindings/arm/xen.txt for the
 * documentation of the Xen Device Tree format.
 */
#define GRANT_TABLE_PHYSADDR 0
<<<<<<< HEAD
static int __init xen_guest_init(void)
{
	struct xen_add_to_physmap xatp;
	static struct shared_info *shared_info_page = 0;
	struct device_node *node;
	int len;
	const char *s = NULL;
	const char *version = NULL;
	const char *xen_prefix = "xen,xen-";
	struct resource res;

	node = of_find_compatible_node(NULL, NULL, "xen,xen");
	if (!node) {
		pr_debug("No Xen support\n");
		return 0;
	}
	s = of_get_property(node, "compatible", &len);
	if (strlen(xen_prefix) + 3  < len &&
			!strncmp(xen_prefix, s, strlen(xen_prefix)))
		version = s + strlen(xen_prefix);
	if (version == NULL) {
		pr_debug("Xen version not found\n");
		return 0;
	}
	if (of_address_to_resource(node, GRANT_TABLE_PHYSADDR, &res))
		return 0;
	xen_hvm_resume_frames = res.start >> PAGE_SHIFT;
	xen_events_irq = irq_of_parse_and_map(node, 0);
	pr_info("Xen %s support found, events_irq=%d gnttab_frame_pfn=%lx\n",
			version, xen_events_irq, xen_hvm_resume_frames);
	xen_domain_type = XEN_HVM_DOMAIN;

	xen_setup_features();
=======
void __init xen_early_init(void)
{
	of_scan_flat_dt(fdt_find_hyper_node, NULL);
	if (!hyper_node.found) {
		pr_debug("No Xen support\n");
		return;
	}

	if (hyper_node.version == NULL) {
		pr_debug("Xen version not found\n");
		return;
	}

	pr_info("Xen %s support found\n", hyper_node.version);

	xen_domain_type = XEN_HVM_DOMAIN;

	xen_setup_features();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (xen_feature(XENFEAT_dom0))
		xen_start_info->flags |= SIF_INITDOMAIN|SIF_PRIVILEGED;
	else
		xen_start_info->flags &= ~(SIF_INITDOMAIN|SIF_PRIVILEGED);

<<<<<<< HEAD
	if (!shared_info_page)
		shared_info_page = (struct shared_info *)
			get_zeroed_page(GFP_KERNEL);
=======
	if (!console_set_on_cmdline && !xen_initial_domain())
		add_preferred_console("hvc", 0, NULL);
}

static void __init xen_acpi_guest_init(void)
{
#ifdef CONFIG_ACPI
	struct xen_hvm_param a;
	int interrupt, trigger, polarity;

	a.domid = DOMID_SELF;
	a.index = HVM_PARAM_CALLBACK_IRQ;

	if (HYPERVISOR_hvm_op(HVMOP_get_param, &a)
	    || (a.value >> 56) != HVM_PARAM_CALLBACK_TYPE_PPI) {
		xen_events_irq = 0;
		return;
	}

	interrupt = a.value & 0xff;
	trigger = ((a.value >> 8) & 0x1) ? ACPI_EDGE_SENSITIVE
					 : ACPI_LEVEL_SENSITIVE;
	polarity = ((a.value >> 8) & 0x2) ? ACPI_ACTIVE_LOW
					  : ACPI_ACTIVE_HIGH;
	xen_events_irq = acpi_register_gsi(NULL, interrupt, trigger, polarity);
#endif
}

static void __init xen_dt_guest_init(void)
{
	struct device_node *xen_node;

	xen_node = of_find_compatible_node(NULL, NULL, "xen,xen");
	if (!xen_node) {
		pr_err("Xen support was detected before, but it has disappeared\n");
		return;
	}

	xen_events_irq = irq_of_parse_and_map(xen_node, 0);
}

static int __init xen_guest_init(void)
{
	struct xen_add_to_physmap xatp;
	struct shared_info *shared_info_page = NULL;
	int cpu;

	if (!xen_domain())
		return 0;

	if (!acpi_disabled)
		xen_acpi_guest_init();
	else
		xen_dt_guest_init();

	if (!xen_events_irq) {
		pr_err("Xen event channel interrupt not found\n");
		return -ENODEV;
	}

	/*
	 * The fdt parsing codes have set EFI_RUNTIME_SERVICES if Xen EFI
	 * parameters are found. Force enable runtime services.
	 */
	if (efi_enabled(EFI_RUNTIME_SERVICES))
		xen_efi_runtime_setup();

	shared_info_page = (struct shared_info *)get_zeroed_page(GFP_KERNEL);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!shared_info_page) {
		pr_err("not enough memory\n");
		return -ENOMEM;
	}
	xatp.domid = DOMID_SELF;
	xatp.idx = 0;
	xatp.space = XENMAPSPACE_shared_info;
<<<<<<< HEAD
	xatp.gpfn = __pa(shared_info_page) >> PAGE_SHIFT;
=======
	xatp.gpfn = virt_to_gfn(shared_info_page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (HYPERVISOR_memory_op(XENMEM_add_to_physmap, &xatp))
		BUG();

	HYPERVISOR_shared_info = (struct shared_info *)shared_info_page;

	/* xen_vcpu is a pointer to the vcpu_info struct in the shared_info
	 * page, we use it in the event channel upcall and in some pvclock
	 * related functions. 
	 * The shared info contains exactly 1 CPU (the boot CPU). The guest
	 * is required to use VCPUOP_register_vcpu_info to place vcpu info
	 * for secondary CPUs as they are brought up.
	 * For uniformity we use VCPUOP_register_vcpu_info even on cpu0.
	 */
	xen_vcpu_info = alloc_percpu(struct vcpu_info);
	if (xen_vcpu_info == NULL)
		return -ENOMEM;

<<<<<<< HEAD
=======
	/* Direct vCPU id mapping for ARM guests. */
	for_each_possible_cpu(cpu)
		per_cpu(xen_vcpu_id, cpu) = cpu;

	xen_auto_xlat_grant_frames.count = gnttab_max_grant_frames();
	if (xen_xlate_map_ballooned_pages(&xen_auto_xlat_grant_frames.pfn,
					  &xen_auto_xlat_grant_frames.vaddr,
					  xen_auto_xlat_grant_frames.count)) {
		free_percpu(xen_vcpu_info);
		return -ENOMEM;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	gnttab_init();
	if (!xen_initial_domain())
		xenbus_probe(NULL);

<<<<<<< HEAD
	return 0;
}
core_initcall(xen_guest_init);
=======
	/*
	 * Making sure board specific code will not set up ops for
	 * cpu idle and cpu freq.
	 */
	disable_cpuidle();
	disable_cpufreq();

	xen_init_IRQ();

	if (request_percpu_irq(xen_events_irq, xen_arm_callback,
			       "events", &xen_vcpu)) {
		pr_err("Error request IRQ %d\n", xen_events_irq);
		return -EINVAL;
	}

	xen_time_setup_guest();

	if (xen_initial_domain())
		pvclock_gtod_register_notifier(&xen_pvclock_gtod_notifier);

	return cpuhp_setup_state(CPUHP_AP_ARM_XEN_STARTING,
				 "AP_ARM_XEN_STARTING", xen_starting_cpu,
				 xen_dying_cpu);
}
early_initcall(xen_guest_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int __init xen_pm_init(void)
{
	if (!xen_domain())
		return -ENODEV;

	pm_power_off = xen_power_off;
	arm_pm_restart = xen_restart;
<<<<<<< HEAD
=======
	if (!xen_initial_domain()) {
		struct timespec64 ts;
		xen_read_wallclock(&ts);
		do_settimeofday64(&ts);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
late_initcall(xen_pm_init);

<<<<<<< HEAD
static irqreturn_t xen_arm_callback(int irq, void *arg)
{
	xen_hvm_evtchn_do_upcall();
	return IRQ_HANDLED;
}

static int __init xen_init_events(void)
{
	if (!xen_domain() || xen_events_irq < 0)
		return -ENODEV;

	xen_init_IRQ();

	if (request_percpu_irq(xen_events_irq, xen_arm_callback,
			"events", &xen_vcpu)) {
		pr_err("Error requesting IRQ %d\n", xen_events_irq);
		return -EINVAL;
	}

	on_each_cpu(xen_percpu_init, NULL, 0);

	return 0;
}
postcore_initcall(xen_init_events);

/* In the hypervisor.S file. */
=======

/* empty stubs */
void xen_arch_pre_suspend(void) { }
void xen_arch_post_suspend(int suspend_cancelled) { }
void xen_timer_resume(void) { }
void xen_arch_resume(void) { }
void xen_arch_suspend(void) { }


/* In the hypercall.S file. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL_GPL(HYPERVISOR_event_channel_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_grant_table_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_xen_version);
EXPORT_SYMBOL_GPL(HYPERVISOR_console_io);
EXPORT_SYMBOL_GPL(HYPERVISOR_sched_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_hvm_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_memory_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_physdev_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_vcpu_op);
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(HYPERVISOR_tmem_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_platform_op);
EXPORT_SYMBOL_GPL(HYPERVISOR_multicall);
EXPORT_SYMBOL_GPL(HYPERVISOR_vm_assist);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL_GPL(privcmd_call);
