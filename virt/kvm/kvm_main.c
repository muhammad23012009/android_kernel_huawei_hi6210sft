/*
 * Kernel-based Virtual Machine driver for Linux
 *
 * This module enables machines with Intel VT-x extensions to run virtual
 * machines without emulation or binary translation.
 *
 * Copyright (C) 2006 Qumranet, Inc.
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 *
 * Authors:
 *   Avi Kivity   <avi@qumranet.com>
 *   Yaniv Kamay  <yaniv@qumranet.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

<<<<<<< HEAD
#include "iodev.h"
=======
#include <kvm/iodev.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/kvm_host.h>
#include <linux/kvm.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/reboot.h>
#include <linux/debugfs.h>
#include <linux/highmem.h>
#include <linux/file.h>
#include <linux/syscore_ops.h>
#include <linux/cpu.h>
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/anon_inodes.h>
#include <linux/profile.h>
#include <linux/kvm_para.h>
#include <linux/pagemap.h>
#include <linux/mman.h>
#include <linux/swap.h>
#include <linux/bitops.h>
#include <linux/spinlock.h>
#include <linux/compat.h>
#include <linux/srcu.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
#include <linux/sort.h>
#include <linux/bsearch.h>
<<<<<<< HEAD
=======
#include <linux/kthread.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/processor.h>
#include <asm/io.h>
#include <asm/ioctl.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>

#include "coalesced_mmio.h"
#include "async_pf.h"
<<<<<<< HEAD
=======
#include "vfio.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define CREATE_TRACE_POINTS
#include <trace/events/kvm.h>

<<<<<<< HEAD
MODULE_AUTHOR("Qumranet");
MODULE_LICENSE("GPL");

/*
 * Ordering of locks:
 *
 * 		kvm->lock --> kvm->slots_lock --> kvm->irq_lock
 */

DEFINE_SPINLOCK(kvm_lock);
=======
/* Worst case buffer size needed for holding an integer. */
#define ITOA_MAX_LEN 12

MODULE_AUTHOR("Qumranet");
MODULE_LICENSE("GPL");

/* Architectures should define their poll value according to the halt latency */
static unsigned int halt_poll_ns = KVM_HALT_POLL_NS_DEFAULT;
module_param(halt_poll_ns, uint, S_IRUGO | S_IWUSR);

/* Default doubles per-vcpu halt_poll_ns. */
static unsigned int halt_poll_ns_grow = 2;
module_param(halt_poll_ns_grow, uint, S_IRUGO | S_IWUSR);

/* Default resets per-vcpu halt_poll_ns . */
static unsigned int halt_poll_ns_shrink;
module_param(halt_poll_ns_shrink, uint, S_IRUGO | S_IWUSR);

/*
 * Ordering of locks:
 *
 *	kvm->lock --> kvm->slots_lock --> kvm->irq_lock
 */

DEFINE_MUTEX(kvm_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static DEFINE_RAW_SPINLOCK(kvm_count_lock);
LIST_HEAD(vm_list);

static cpumask_var_t cpus_hardware_enabled;
<<<<<<< HEAD
static int kvm_usage_count = 0;
=======
static int kvm_usage_count;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static atomic_t hardware_enable_failed;

struct kmem_cache *kvm_vcpu_cache;
EXPORT_SYMBOL_GPL(kvm_vcpu_cache);

static __read_mostly struct preempt_ops kvm_preempt_ops;

struct dentry *kvm_debugfs_dir;
<<<<<<< HEAD

static long kvm_vcpu_ioctl(struct file *file, unsigned int ioctl,
			   unsigned long arg);
#ifdef CONFIG_COMPAT
=======
EXPORT_SYMBOL_GPL(kvm_debugfs_dir);

static int kvm_debugfs_num_entries;
static const struct file_operations *stat_fops_per_vm[];

static long kvm_vcpu_ioctl(struct file *file, unsigned int ioctl,
			   unsigned long arg);
#ifdef CONFIG_KVM_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static long kvm_vcpu_compat_ioctl(struct file *file, unsigned int ioctl,
				  unsigned long arg);
#endif
static int hardware_enable_all(void);
static void hardware_disable_all(void);

static void kvm_io_bus_destroy(struct kvm_io_bus *bus);
<<<<<<< HEAD
static void update_memslots(struct kvm_memslots *slots,
			    struct kvm_memory_slot *new, u64 last_generation);

static void kvm_release_pfn_dirty(pfn_t pfn);
static void mark_page_dirty_in_slot(struct kvm *kvm,
				    struct kvm_memory_slot *memslot, gfn_t gfn);
=======

static void kvm_release_pfn_dirty(kvm_pfn_t pfn);
static void mark_page_dirty_in_slot(struct kvm_memory_slot *memslot, gfn_t gfn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

__visible bool kvm_rebooting;
EXPORT_SYMBOL_GPL(kvm_rebooting);

static bool largepages_enabled = true;

<<<<<<< HEAD
bool kvm_is_mmio_pfn(pfn_t pfn)
{
	if (pfn_valid(pfn))
		return !is_zero_pfn(pfn) && PageReserved(pfn_to_page(pfn));
=======
__weak void kvm_arch_mmu_notifier_invalidate_range(struct kvm *kvm,
		unsigned long start, unsigned long end)
{
}

bool kvm_is_zone_device_pfn(kvm_pfn_t pfn)
{
	/*
	 * The metadata used by is_zone_device_page() to determine whether or
	 * not a page is ZONE_DEVICE is guaranteed to be valid if and only if
	 * the device has been pinned, e.g. by get_user_pages().  WARN if the
	 * page_count() is zero to help detect bad usage of this helper.
	 */
	if (!pfn_valid(pfn) || WARN_ON_ONCE(!page_count(pfn_to_page(pfn))))
		return false;

	return is_zone_device_page(pfn_to_page(pfn));
}

bool kvm_is_reserved_pfn(kvm_pfn_t pfn)
{
	/*
	 * ZONE_DEVICE pages currently set PG_reserved, but from a refcounting
	 * perspective they are "normal" pages, albeit with slightly different
	 * usage rules.
	 */
	if (pfn_valid(pfn))
		return PageReserved(pfn_to_page(pfn)) &&
		       !is_zero_pfn(pfn) &&
		       !kvm_is_zone_device_pfn(pfn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return true;
}

/*
 * Switches to specified vcpu, until a matching vcpu_put()
 */
int vcpu_load(struct kvm_vcpu *vcpu)
{
	int cpu;

	if (mutex_lock_killable(&vcpu->mutex))
		return -EINTR;
<<<<<<< HEAD
	if (unlikely(vcpu->pid != current->pids[PIDTYPE_PID].pid)) {
		/* The thread running this VCPU changed. */
		struct pid *oldpid = vcpu->pid;
		struct pid *newpid = get_task_pid(current, PIDTYPE_PID);
		rcu_assign_pointer(vcpu->pid, newpid);
		if (oldpid)
			synchronize_rcu();
		put_pid(oldpid);
	}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cpu = get_cpu();
	preempt_notifier_register(&vcpu->preempt_notifier);
	kvm_arch_vcpu_load(vcpu, cpu);
	put_cpu();
	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(vcpu_load);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void vcpu_put(struct kvm_vcpu *vcpu)
{
	preempt_disable();
	kvm_arch_vcpu_put(vcpu);
	preempt_notifier_unregister(&vcpu->preempt_notifier);
	preempt_enable();
	mutex_unlock(&vcpu->mutex);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(vcpu_put);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void ack_flush(void *_completed)
{
}

<<<<<<< HEAD
static bool make_all_cpus_request(struct kvm *kvm, unsigned int req)
=======
bool kvm_make_all_cpus_request(struct kvm *kvm, unsigned int req)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int i, cpu, me;
	cpumask_var_t cpus;
	bool called = true;
	struct kvm_vcpu *vcpu;

	zalloc_cpumask_var(&cpus, GFP_ATOMIC);

	me = get_cpu();
	kvm_for_each_vcpu(i, vcpu, kvm) {
		kvm_make_request(req, vcpu);
		cpu = vcpu->cpu;

<<<<<<< HEAD
		/* Set ->requests bit before we read ->mode */
		smp_mb();
=======
		/* Set ->requests bit before we read ->mode. */
		smp_mb__after_atomic();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (cpus != NULL && cpu != -1 && cpu != me &&
		      kvm_vcpu_exiting_guest_mode(vcpu) != OUTSIDE_GUEST_MODE)
			cpumask_set_cpu(cpu, cpus);
	}
	if (unlikely(cpus == NULL))
		smp_call_function_many(cpu_online_mask, ack_flush, NULL, 1);
	else if (!cpumask_empty(cpus))
		smp_call_function_many(cpus, ack_flush, NULL, 1);
	else
		called = false;
	put_cpu();
	free_cpumask_var(cpus);
	return called;
}

<<<<<<< HEAD
void kvm_flush_remote_tlbs(struct kvm *kvm)
{
	long dirty_count = kvm->tlbs_dirty;

	smp_mb();
	if (make_all_cpus_request(kvm, KVM_REQ_TLB_FLUSH))
		++kvm->stat.remote_tlb_flush;
	cmpxchg(&kvm->tlbs_dirty, dirty_count, 0);
}

void kvm_reload_remote_mmus(struct kvm *kvm)
{
	make_all_cpus_request(kvm, KVM_REQ_MMU_RELOAD);
}

void kvm_make_mclock_inprogress_request(struct kvm *kvm)
{
	make_all_cpus_request(kvm, KVM_REQ_MCLOCK_INPROGRESS);
}

void kvm_make_scan_ioapic_request(struct kvm *kvm)
{
	make_all_cpus_request(kvm, KVM_REQ_SCAN_IOAPIC);
=======
#ifndef CONFIG_HAVE_KVM_ARCH_TLB_FLUSH_ALL
void kvm_flush_remote_tlbs(struct kvm *kvm)
{
	/*
	 * Read tlbs_dirty before setting KVM_REQ_TLB_FLUSH in
	 * kvm_make_all_cpus_request.
	 */
	long dirty_count = smp_load_acquire(&kvm->tlbs_dirty);

	/*
	 * We want to publish modifications to the page tables before reading
	 * mode. Pairs with a memory barrier in arch-specific code.
	 * - x86: smp_mb__after_srcu_read_unlock in vcpu_enter_guest
	 * and smp_mb in walk_shadow_page_lockless_begin/end.
	 * - powerpc: smp_mb in kvmppc_prepare_to_enter.
	 *
	 * There is already an smp_mb__after_atomic() before
	 * kvm_make_all_cpus_request() reads vcpu->mode. We reuse that
	 * barrier here.
	 */
	if (kvm_make_all_cpus_request(kvm, KVM_REQ_TLB_FLUSH))
		++kvm->stat.remote_tlb_flush;
	cmpxchg(&kvm->tlbs_dirty, dirty_count, 0);
}
EXPORT_SYMBOL_GPL(kvm_flush_remote_tlbs);
#endif

void kvm_reload_remote_mmus(struct kvm *kvm)
{
	kvm_make_all_cpus_request(kvm, KVM_REQ_MMU_RELOAD);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int kvm_vcpu_init(struct kvm_vcpu *vcpu, struct kvm *kvm, unsigned id)
{
	struct page *page;
	int r;

	mutex_init(&vcpu->mutex);
	vcpu->cpu = -1;
	vcpu->kvm = kvm;
	vcpu->vcpu_id = id;
	vcpu->pid = NULL;
<<<<<<< HEAD
	init_waitqueue_head(&vcpu->wq);
	kvm_async_pf_vcpu_init(vcpu);

=======
	init_swait_queue_head(&vcpu->wq);
	kvm_async_pf_vcpu_init(vcpu);

	vcpu->pre_pcpu = -1;
	INIT_LIST_HEAD(&vcpu->blocked_vcpu_list);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page) {
		r = -ENOMEM;
		goto fail;
	}
	vcpu->run = page_address(page);

	kvm_vcpu_set_in_spin_loop(vcpu, false);
	kvm_vcpu_set_dy_eligible(vcpu, false);
	vcpu->preempted = false;

	r = kvm_arch_vcpu_init(vcpu);
	if (r < 0)
		goto fail_free_run;
	return 0;

fail_free_run:
	free_page((unsigned long)vcpu->run);
fail:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_init);

void kvm_vcpu_uninit(struct kvm_vcpu *vcpu)
{
	put_pid(vcpu->pid);
	kvm_arch_vcpu_uninit(vcpu);
	free_page((unsigned long)vcpu->run);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_uninit);

#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
static inline struct kvm *mmu_notifier_to_kvm(struct mmu_notifier *mn)
{
	return container_of(mn, struct kvm, mmu_notifier);
}

static void kvm_mmu_notifier_invalidate_page(struct mmu_notifier *mn,
					     struct mm_struct *mm,
					     unsigned long address)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush, idx;

	/*
	 * When ->invalidate_page runs, the linux pte has been zapped
	 * already but the page is still allocated until
	 * ->invalidate_page returns. So if we increase the sequence
	 * here the kvm page fault will notice if the spte can't be
	 * established because the page is going to be freed. If
	 * instead the kvm page fault establishes the spte before
	 * ->invalidate_page runs, kvm_unmap_hva will release it
	 * before returning.
	 *
	 * The sequence increase only need to be seen at spin_unlock
	 * time, and not at spin_lock time.
	 *
	 * Increasing the sequence after the spin_unlock would be
	 * unsafe because the kvm page fault could then establish the
	 * pte after kvm_unmap_hva returned, without noticing the page
	 * is going to be freed.
	 */
	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);

	kvm->mmu_notifier_seq++;
	need_tlb_flush = kvm_unmap_hva(kvm, address) | kvm->tlbs_dirty;
	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
<<<<<<< HEAD
=======

	kvm_arch_mmu_notifier_invalidate_page(kvm, address);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	srcu_read_unlock(&kvm->srcu, idx);
}

static void kvm_mmu_notifier_change_pte(struct mmu_notifier *mn,
					struct mm_struct *mm,
					unsigned long address,
					pte_t pte)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	kvm->mmu_notifier_seq++;
	kvm_set_spte_hva(kvm, address, pte);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
}

static void kvm_mmu_notifier_invalidate_range_start(struct mmu_notifier *mn,
						    struct mm_struct *mm,
						    unsigned long start,
						    unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush = 0, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	/*
	 * The count increase must become visible at unlock time as no
	 * spte can be established without taking the mmu_lock and
	 * count is also read inside the mmu_lock critical section.
	 */
	kvm->mmu_notifier_count++;
	need_tlb_flush = kvm_unmap_hva_range(kvm, start, end);
<<<<<<< HEAD
	need_tlb_flush |= kvm->tlbs_dirty;
	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
=======
	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush || kvm->tlbs_dirty)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);

	kvm_arch_mmu_notifier_invalidate_range(kvm, start, end);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	srcu_read_unlock(&kvm->srcu, idx);
}

static void kvm_mmu_notifier_invalidate_range_end(struct mmu_notifier *mn,
						  struct mm_struct *mm,
						  unsigned long start,
						  unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);

	spin_lock(&kvm->mmu_lock);
	/*
	 * This sequence increase will notify the kvm page fault that
	 * the page that is going to be mapped in the spte could have
	 * been freed.
	 */
	kvm->mmu_notifier_seq++;
	smp_wmb();
	/*
	 * The above sequence increase must be visible before the
	 * below count decrease, which is ensured by the smp_wmb above
	 * in conjunction with the smp_rmb in mmu_notifier_retry().
	 */
	kvm->mmu_notifier_count--;
	spin_unlock(&kvm->mmu_lock);

	BUG_ON(kvm->mmu_notifier_count < 0);
}

static int kvm_mmu_notifier_clear_flush_young(struct mmu_notifier *mn,
					      struct mm_struct *mm,
<<<<<<< HEAD
					      unsigned long address)
=======
					      unsigned long start,
					      unsigned long end)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);

<<<<<<< HEAD
	young = kvm_age_hva(kvm, address);
=======
	young = kvm_age_hva(kvm, start, end);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (young)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
}

<<<<<<< HEAD
=======
static int kvm_mmu_notifier_clear_young(struct mmu_notifier *mn,
					struct mm_struct *mm,
					unsigned long start,
					unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	/*
	 * Even though we do not flush TLB, this will still adversely
	 * affect performance on pre-Haswell Intel EPT, where there is
	 * no EPT Access Bit to clear so that we have to tear down EPT
	 * tables instead. If we find this unacceptable, we can always
	 * add a parameter to kvm_age_hva so that it effectively doesn't
	 * do anything on clear_young.
	 *
	 * Also note that currently we never issue secondary TLB flushes
	 * from clear_young, leaving this job up to the regular system
	 * cadence. If we find this inaccurate, we might come up with a
	 * more sophisticated heuristic later.
	 */
	young = kvm_age_hva(kvm, start, end);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int kvm_mmu_notifier_test_young(struct mmu_notifier *mn,
				       struct mm_struct *mm,
				       unsigned long address)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	young = kvm_test_age_hva(kvm, address);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
}

static void kvm_mmu_notifier_release(struct mmu_notifier *mn,
				     struct mm_struct *mm)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
	kvm_arch_flush_shadow_all(kvm);
	srcu_read_unlock(&kvm->srcu, idx);
}

static const struct mmu_notifier_ops kvm_mmu_notifier_ops = {
	.invalidate_page	= kvm_mmu_notifier_invalidate_page,
	.invalidate_range_start	= kvm_mmu_notifier_invalidate_range_start,
	.invalidate_range_end	= kvm_mmu_notifier_invalidate_range_end,
	.clear_flush_young	= kvm_mmu_notifier_clear_flush_young,
<<<<<<< HEAD
=======
	.clear_young		= kvm_mmu_notifier_clear_young,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.test_young		= kvm_mmu_notifier_test_young,
	.change_pte		= kvm_mmu_notifier_change_pte,
	.release		= kvm_mmu_notifier_release,
};

static int kvm_init_mmu_notifier(struct kvm *kvm)
{
	kvm->mmu_notifier.ops = &kvm_mmu_notifier_ops;
	return mmu_notifier_register(&kvm->mmu_notifier, current->mm);
}

#else  /* !(CONFIG_MMU_NOTIFIER && KVM_ARCH_WANT_MMU_NOTIFIER) */

static int kvm_init_mmu_notifier(struct kvm *kvm)
{
	return 0;
}

#endif /* CONFIG_MMU_NOTIFIER && KVM_ARCH_WANT_MMU_NOTIFIER */

<<<<<<< HEAD
static void kvm_init_memslots_id(struct kvm *kvm)
{
	int i;
	struct kvm_memslots *slots = kvm->memslots;

	for (i = 0; i < KVM_MEM_SLOTS_NUM; i++)
		slots->id_to_index[i] = slots->memslots[i].id = i;
=======
static struct kvm_memslots *kvm_alloc_memslots(void)
{
	int i;
	struct kvm_memslots *slots;

	slots = kvm_kvzalloc(sizeof(struct kvm_memslots));
	if (!slots)
		return NULL;

	/*
	 * Init kvm generation close to the maximum to easily test the
	 * code of handling generation number wrap-around.
	 */
	slots->generation = -150;
	for (i = 0; i < KVM_MEM_SLOTS_NUM; i++)
		slots->id_to_index[i] = slots->memslots[i].id = i;

	return slots;
}

static void kvm_destroy_dirty_bitmap(struct kvm_memory_slot *memslot)
{
	if (!memslot->dirty_bitmap)
		return;

	kvfree(memslot->dirty_bitmap);
	memslot->dirty_bitmap = NULL;
}

/*
 * Free any memory in @free but not in @dont.
 */
static void kvm_free_memslot(struct kvm *kvm, struct kvm_memory_slot *free,
			      struct kvm_memory_slot *dont)
{
	if (!dont || free->dirty_bitmap != dont->dirty_bitmap)
		kvm_destroy_dirty_bitmap(free);

	kvm_arch_free_memslot(kvm, free, dont);

	free->npages = 0;
}

static void kvm_free_memslots(struct kvm *kvm, struct kvm_memslots *slots)
{
	struct kvm_memory_slot *memslot;

	if (!slots)
		return;

	kvm_for_each_memslot(memslot, slots)
		kvm_free_memslot(kvm, memslot, NULL);

	kvfree(slots);
}

static void kvm_destroy_vm_debugfs(struct kvm *kvm)
{
	int i;

	if (!kvm->debugfs_dentry)
		return;

	debugfs_remove_recursive(kvm->debugfs_dentry);

	if (kvm->debugfs_stat_data) {
		for (i = 0; i < kvm_debugfs_num_entries; i++)
			kfree(kvm->debugfs_stat_data[i]);
		kfree(kvm->debugfs_stat_data);
	}
}

static int kvm_create_vm_debugfs(struct kvm *kvm, int fd)
{
	char dir_name[ITOA_MAX_LEN * 2];
	struct kvm_stat_data *stat_data;
	struct kvm_stats_debugfs_item *p;

	if (!debugfs_initialized())
		return 0;

	snprintf(dir_name, sizeof(dir_name), "%d-%d", task_pid_nr(current), fd);
	kvm->debugfs_dentry = debugfs_create_dir(dir_name,
						 kvm_debugfs_dir);
	if (!kvm->debugfs_dentry)
		return -ENOMEM;

	kvm->debugfs_stat_data = kcalloc(kvm_debugfs_num_entries,
					 sizeof(*kvm->debugfs_stat_data),
					 GFP_KERNEL);
	if (!kvm->debugfs_stat_data)
		return -ENOMEM;

	for (p = debugfs_entries; p->name; p++) {
		stat_data = kzalloc(sizeof(*stat_data), GFP_KERNEL);
		if (!stat_data)
			return -ENOMEM;

		stat_data->kvm = kvm;
		stat_data->offset = p->offset;
		kvm->debugfs_stat_data[p - debugfs_entries] = stat_data;
		if (!debugfs_create_file(p->name, 0444,
					 kvm->debugfs_dentry,
					 stat_data,
					 stat_fops_per_vm[p->kind]))
			return -ENOMEM;
	}
	return 0;
}

/*
 * Called after the VM is otherwise initialized, but just before adding it to
 * the vm_list.
 */
int __weak kvm_arch_post_init_vm(struct kvm *kvm)
{
	return 0;
}

/*
 * Called just after removing the VM from the vm_list, but before doing any
 * other destruction.
 */
void __weak kvm_arch_pre_destroy_vm(struct kvm *kvm)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct kvm *kvm_create_vm(unsigned long type)
{
	int r, i;
	struct kvm *kvm = kvm_arch_alloc_vm();

	if (!kvm)
		return ERR_PTR(-ENOMEM);

	spin_lock_init(&kvm->mmu_lock);
	atomic_inc(&current->mm->mm_count);
	kvm->mm = current->mm;
	kvm_eventfd_init(kvm);
	mutex_init(&kvm->lock);
	mutex_init(&kvm->irq_lock);
	mutex_init(&kvm->slots_lock);
	atomic_set(&kvm->users_count, 1);
	INIT_LIST_HEAD(&kvm->devices);

	r = kvm_arch_init_vm(kvm, type);
	if (r)
		goto out_err_no_disable;

	r = hardware_enable_all();
	if (r)
		goto out_err_no_disable;

<<<<<<< HEAD
#ifdef CONFIG_HAVE_KVM_IRQCHIP
	INIT_HLIST_HEAD(&kvm->mask_notifier_list);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_HAVE_KVM_IRQFD
	INIT_HLIST_HEAD(&kvm->irq_ack_notifier_list);
#endif

	BUILD_BUG_ON(KVM_MEM_SLOTS_NUM > SHRT_MAX);

	r = -ENOMEM;
<<<<<<< HEAD
	kvm->memslots = kzalloc(sizeof(struct kvm_memslots), GFP_KERNEL);
	if (!kvm->memslots)
		goto out_err_no_srcu;
	kvm_init_memslots_id(kvm);
=======
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++) {
		kvm->memslots[i] = kvm_alloc_memslots();
		if (!kvm->memslots[i])
			goto out_err_no_srcu;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (init_srcu_struct(&kvm->srcu))
		goto out_err_no_srcu;
	if (init_srcu_struct(&kvm->irq_srcu))
		goto out_err_no_irq_srcu;
	for (i = 0; i < KVM_NR_BUSES; i++) {
		kvm->buses[i] = kzalloc(sizeof(struct kvm_io_bus),
					GFP_KERNEL);
		if (!kvm->buses[i])
<<<<<<< HEAD
			goto out_err;
=======
			goto out_err_no_mmu_notifier;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	r = kvm_init_mmu_notifier(kvm);
	if (r)
<<<<<<< HEAD
		goto out_err;

	spin_lock(&kvm_lock);
	list_add(&kvm->vm_list, &vm_list);
	spin_unlock(&kvm_lock);
=======
		goto out_err_no_mmu_notifier;

	r = kvm_arch_post_init_vm(kvm);
	if (r)
		goto out_err;

	mutex_lock(&kvm_lock);
	list_add(&kvm->vm_list, &vm_list);
	mutex_unlock(&kvm_lock);

	preempt_notifier_inc();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return kvm;

out_err:
<<<<<<< HEAD
=======
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
	if (kvm->mmu_notifier.ops)
		mmu_notifier_unregister(&kvm->mmu_notifier, current->mm);
#endif
out_err_no_mmu_notifier:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cleanup_srcu_struct(&kvm->irq_srcu);
out_err_no_irq_srcu:
	cleanup_srcu_struct(&kvm->srcu);
out_err_no_srcu:
	hardware_disable_all();
out_err_no_disable:
	for (i = 0; i < KVM_NR_BUSES; i++)
		kfree(kvm->buses[i]);
<<<<<<< HEAD
	kfree(kvm->memslots);
=======
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++)
		kvm_free_memslots(kvm, kvm->memslots[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kvm_arch_free_vm(kvm);
	mmdrop(current->mm);
	return ERR_PTR(r);
}

/*
 * Avoid using vmalloc for a small buffer.
 * Should not be used when the size is statically known.
 */
void *kvm_kvzalloc(unsigned long size)
{
	if (size > PAGE_SIZE)
		return vzalloc(size);
	else
		return kzalloc(size, GFP_KERNEL);
}

<<<<<<< HEAD
void kvm_kvfree(const void *addr)
{
	if (is_vmalloc_addr(addr))
		vfree(addr);
	else
		kfree(addr);
}

static void kvm_destroy_dirty_bitmap(struct kvm_memory_slot *memslot)
{
	if (!memslot->dirty_bitmap)
		return;

	kvm_kvfree(memslot->dirty_bitmap);
	memslot->dirty_bitmap = NULL;
}

/*
 * Free any memory in @free but not in @dont.
 */
static void kvm_free_physmem_slot(struct kvm *kvm, struct kvm_memory_slot *free,
				  struct kvm_memory_slot *dont)
{
	if (!dont || free->dirty_bitmap != dont->dirty_bitmap)
		kvm_destroy_dirty_bitmap(free);

	kvm_arch_free_memslot(kvm, free, dont);

	free->npages = 0;
}

static void kvm_free_physmem(struct kvm *kvm)
{
	struct kvm_memslots *slots = kvm->memslots;
	struct kvm_memory_slot *memslot;

	kvm_for_each_memslot(memslot, slots)
		kvm_free_physmem_slot(kvm, memslot, NULL);

	kfree(kvm->memslots);
}

static void kvm_destroy_devices(struct kvm *kvm)
{
	struct list_head *node, *tmp;

	list_for_each_safe(node, tmp, &kvm->devices) {
		struct kvm_device *dev =
			list_entry(node, struct kvm_device, vm_node);

		list_del(node);
=======
static void kvm_destroy_devices(struct kvm *kvm)
{
	struct kvm_device *dev, *tmp;

	/*
	 * We do not need to take the kvm->lock here, because nobody else
	 * has a reference to the struct kvm at this point and therefore
	 * cannot access the devices list anyhow.
	 */
	list_for_each_entry_safe(dev, tmp, &kvm->devices, vm_node) {
		list_del(&dev->vm_node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev->ops->destroy(dev);
	}
}

static void kvm_destroy_vm(struct kvm *kvm)
{
	int i;
	struct mm_struct *mm = kvm->mm;

<<<<<<< HEAD
	kvm_arch_sync_events(kvm);
	spin_lock(&kvm_lock);
	list_del(&kvm->vm_list);
	spin_unlock(&kvm_lock);
=======
	kvm_destroy_vm_debugfs(kvm);
	kvm_arch_sync_events(kvm);
	mutex_lock(&kvm_lock);
	list_del(&kvm->vm_list);
	mutex_unlock(&kvm_lock);
	kvm_arch_pre_destroy_vm(kvm);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kvm_free_irq_routing(kvm);
	for (i = 0; i < KVM_NR_BUSES; i++) {
		if (kvm->buses[i])
			kvm_io_bus_destroy(kvm->buses[i]);
		kvm->buses[i] = NULL;
	}
	kvm_coalesced_mmio_free(kvm);
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
	mmu_notifier_unregister(&kvm->mmu_notifier, kvm->mm);
#else
	kvm_arch_flush_shadow_all(kvm);
#endif
	kvm_arch_destroy_vm(kvm);
	kvm_destroy_devices(kvm);
<<<<<<< HEAD
	kvm_free_physmem(kvm);
	cleanup_srcu_struct(&kvm->irq_srcu);
	cleanup_srcu_struct(&kvm->srcu);
	kvm_arch_free_vm(kvm);
=======
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++)
		kvm_free_memslots(kvm, kvm->memslots[i]);
	cleanup_srcu_struct(&kvm->irq_srcu);
	cleanup_srcu_struct(&kvm->srcu);
	kvm_arch_free_vm(kvm);
	preempt_notifier_dec();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	hardware_disable_all();
	mmdrop(mm);
}

void kvm_get_kvm(struct kvm *kvm)
{
	atomic_inc(&kvm->users_count);
}
EXPORT_SYMBOL_GPL(kvm_get_kvm);

void kvm_put_kvm(struct kvm *kvm)
{
	if (atomic_dec_and_test(&kvm->users_count))
		kvm_destroy_vm(kvm);
}
EXPORT_SYMBOL_GPL(kvm_put_kvm);


static int kvm_vm_release(struct inode *inode, struct file *filp)
{
	struct kvm *kvm = filp->private_data;

	kvm_irqfd_release(kvm);

	kvm_put_kvm(kvm);
	return 0;
}

/*
 * Allocation size is twice as large as the actual dirty bitmap size.
 * See x86's kvm_vm_ioctl_get_dirty_log() why this is needed.
 */
static int kvm_create_dirty_bitmap(struct kvm_memory_slot *memslot)
{
<<<<<<< HEAD
#ifndef CONFIG_S390
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long dirty_bytes = 2 * kvm_dirty_bitmap_bytes(memslot);

	memslot->dirty_bitmap = kvm_kvzalloc(dirty_bytes);
	if (!memslot->dirty_bitmap)
		return -ENOMEM;

<<<<<<< HEAD
#endif /* !CONFIG_S390 */
	return 0;
}

static int cmp_memslot(const void *slot1, const void *slot2)
{
	struct kvm_memory_slot *s1, *s2;

	s1 = (struct kvm_memory_slot *)slot1;
	s2 = (struct kvm_memory_slot *)slot2;

	if (s1->npages < s2->npages)
		return 1;
	if (s1->npages > s2->npages)
		return -1;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/*
<<<<<<< HEAD
 * Sort the memslots base on its size, so the larger slots
 * will get better fit.
 */
static void sort_memslots(struct kvm_memslots *slots)
{
	int i;

	sort(slots->memslots, KVM_MEM_SLOTS_NUM,
	      sizeof(struct kvm_memory_slot), cmp_memslot, NULL);

	for (i = 0; i < KVM_MEM_SLOTS_NUM; i++)
		slots->id_to_index[slots->memslots[i].id] = i;
}

static void update_memslots(struct kvm_memslots *slots,
			    struct kvm_memory_slot *new,
			    u64 last_generation)
{
	if (new) {
		int id = new->id;
		struct kvm_memory_slot *old = id_to_memslot(slots, id);
		unsigned long npages = old->npages;

		*old = *new;
		if (new->npages != npages)
			sort_memslots(slots);
	}

	slots->generation = last_generation + 1;
}

static int check_memory_region_flags(struct kvm_userspace_memory_region *mem)
=======
 * Insert memslot and re-sort memslots based on their GFN,
 * so binary search could be used to lookup GFN.
 * Sorting algorithm takes advantage of having initially
 * sorted array and known changed memslot position.
 */
static void update_memslots(struct kvm_memslots *slots,
			    struct kvm_memory_slot *new)
{
	int id = new->id;
	int i = slots->id_to_index[id];
	struct kvm_memory_slot *mslots = slots->memslots;

	WARN_ON(mslots[i].id != id);
	if (!new->npages) {
		WARN_ON(!mslots[i].npages);
		if (mslots[i].npages)
			slots->used_slots--;
	} else {
		if (!mslots[i].npages)
			slots->used_slots++;
	}

	while (i < KVM_MEM_SLOTS_NUM - 1 &&
	       new->base_gfn <= mslots[i + 1].base_gfn) {
		if (!mslots[i + 1].npages)
			break;
		mslots[i] = mslots[i + 1];
		slots->id_to_index[mslots[i].id] = i;
		i++;
	}

	/*
	 * The ">=" is needed when creating a slot with base_gfn == 0,
	 * so that it moves before all those with base_gfn == npages == 0.
	 *
	 * On the other hand, if new->npages is zero, the above loop has
	 * already left i pointing to the beginning of the empty part of
	 * mslots, and the ">=" would move the hole backwards in this
	 * case---which is wrong.  So skip the loop when deleting a slot.
	 */
	if (new->npages) {
		while (i > 0 &&
		       new->base_gfn >= mslots[i - 1].base_gfn) {
			mslots[i] = mslots[i - 1];
			slots->id_to_index[mslots[i].id] = i;
			i--;
		}
	} else
		WARN_ON_ONCE(i != slots->used_slots);

	mslots[i] = *new;
	slots->id_to_index[mslots[i].id] = i;
}

static int check_memory_region_flags(const struct kvm_userspace_memory_region *mem)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u32 valid_flags = KVM_MEM_LOG_DIRTY_PAGES;

#ifdef __KVM_HAVE_READONLY_MEM
	valid_flags |= KVM_MEM_READONLY;
#endif

	if (mem->flags & ~valid_flags)
		return -EINVAL;

	return 0;
}

static struct kvm_memslots *install_new_memslots(struct kvm *kvm,
<<<<<<< HEAD
		struct kvm_memslots *slots, struct kvm_memory_slot *new)
{
	struct kvm_memslots *old_memslots = kvm->memslots;

	update_memslots(slots, new, kvm->memslots->generation);
	rcu_assign_pointer(kvm->memslots, slots);
	synchronize_srcu_expedited(&kvm->srcu);

	kvm_arch_memslots_updated(kvm);
=======
		int as_id, struct kvm_memslots *slots)
{
	struct kvm_memslots *old_memslots = __kvm_memslots(kvm, as_id);

	/*
	 * Set the low bit in the generation, which disables SPTE caching
	 * until the end of synchronize_srcu_expedited.
	 */
	WARN_ON(old_memslots->generation & 1);
	slots->generation = old_memslots->generation + 1;

	rcu_assign_pointer(kvm->memslots[as_id], slots);
	synchronize_srcu_expedited(&kvm->srcu);

	/*
	 * Increment the new memslot generation a second time. This prevents
	 * vm exits that race with memslot updates from caching a memslot
	 * generation that will (potentially) be valid forever.
	 */
	slots->generation++;

	kvm_arch_memslots_updated(kvm, slots);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return old_memslots;
}

/*
 * Allocate some memory and give it an address in the guest physical address
 * space.
 *
 * Discontiguous memory is allowed, mostly for framebuffers.
 *
<<<<<<< HEAD
 * Must be called holding mmap_sem for write.
 */
int __kvm_set_memory_region(struct kvm *kvm,
			    struct kvm_userspace_memory_region *mem)
=======
 * Must be called holding kvm->slots_lock for write.
 */
int __kvm_set_memory_region(struct kvm *kvm,
			    const struct kvm_userspace_memory_region *mem)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int r;
	gfn_t base_gfn;
	unsigned long npages;
	struct kvm_memory_slot *slot;
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots = NULL, *old_memslots;
<<<<<<< HEAD
=======
	int as_id, id;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	enum kvm_mr_change change;

	r = check_memory_region_flags(mem);
	if (r)
		goto out;

	r = -EINVAL;
<<<<<<< HEAD
=======
	as_id = mem->slot >> 16;
	id = (u16)mem->slot;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* General sanity checks */
	if (mem->memory_size & (PAGE_SIZE - 1))
		goto out;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		goto out;
	/* We can read the guest memory with __xxx_user() later on. */
<<<<<<< HEAD
	if ((mem->slot < KVM_USER_MEM_SLOTS) &&
=======
	if ((id < KVM_USER_MEM_SLOTS) &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	    ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	     !access_ok(VERIFY_WRITE,
			(void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size)))
		goto out;
<<<<<<< HEAD
	if (mem->slot >= KVM_MEM_SLOTS_NUM)
=======
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_MEM_SLOTS_NUM)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		goto out;

<<<<<<< HEAD
	slot = id_to_memslot(kvm->memslots, mem->slot);
=======
	slot = id_to_memslot(__kvm_memslots(kvm, as_id), id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	npages = mem->memory_size >> PAGE_SHIFT;

	if (npages > KVM_MEM_MAX_NR_PAGES)
		goto out;

<<<<<<< HEAD
	if (!npages)
		mem->flags &= ~KVM_MEM_LOG_DIRTY_PAGES;

	new = old = *slot;

	new.id = mem->slot;
=======
	new = old = *slot;

	new.id = id;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	new.base_gfn = base_gfn;
	new.npages = npages;
	new.flags = mem->flags;

	if (npages) {
		if (!old.npages)
			change = KVM_MR_CREATE;
		else { /* Modify an existing slot. */
			if ((mem->userspace_addr != old.userspace_addr) ||
			    (npages != old.npages) ||
			    ((new.flags ^ old.flags) & KVM_MEM_READONLY))
				goto out;

			if (base_gfn != old.base_gfn)
				change = KVM_MR_MOVE;
			else if (new.flags != old.flags)
				change = KVM_MR_FLAGS_ONLY;
			else { /* Nothing to change. */
				r = 0;
				goto out;
			}
		}
<<<<<<< HEAD
	} else if (old.npages) {
		change = KVM_MR_DELETE;
	} else /* Modify a non-existent slot: disallowed. */
		goto out;
=======
	} else {
		if (!old.npages)
			goto out;

		change = KVM_MR_DELETE;
		new.base_gfn = 0;
		new.flags = 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		/* Check for overlaps */
		r = -EEXIST;
<<<<<<< HEAD
		kvm_for_each_memslot(slot, kvm->memslots) {
			if ((slot->id >= KVM_USER_MEM_SLOTS) ||
			    (slot->id == mem->slot))
=======
		kvm_for_each_memslot(slot, __kvm_memslots(kvm, as_id)) {
			if (slot->id == id)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				continue;
			if (!((base_gfn + npages <= slot->base_gfn) ||
			      (base_gfn >= slot->base_gfn + slot->npages)))
				goto out;
		}
	}

	/* Free page dirty bitmap if unneeded */
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;

	r = -ENOMEM;
	if (change == KVM_MR_CREATE) {
		new.userspace_addr = mem->userspace_addr;

		if (kvm_arch_create_memslot(kvm, &new, npages))
			goto out_free;
	}

	/* Allocate page dirty bitmap if needed */
	if ((new.flags & KVM_MEM_LOG_DIRTY_PAGES) && !new.dirty_bitmap) {
		if (kvm_create_dirty_bitmap(&new) < 0)
			goto out_free;
	}

<<<<<<< HEAD
	if ((change == KVM_MR_DELETE) || (change == KVM_MR_MOVE)) {
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
		slot = id_to_memslot(slots, mem->slot);
		slot->flags |= KVM_MEMSLOT_INVALID;

		old_memslots = install_new_memslots(kvm, slots, NULL);
=======
	slots = kvm_kvzalloc(sizeof(struct kvm_memslots));
	if (!slots)
		goto out_free;
	memcpy(slots, __kvm_memslots(kvm, as_id), sizeof(struct kvm_memslots));

	if ((change == KVM_MR_DELETE) || (change == KVM_MR_MOVE)) {
		slot = id_to_memslot(slots, id);
		slot->flags |= KVM_MEMSLOT_INVALID;

		old_memslots = install_new_memslots(kvm, as_id, slots);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* slot was deleted or moved, clear iommu mapping */
		kvm_iommu_unmap_pages(kvm, &old);
		/* From this point no new shadow pages pointing to a deleted,
		 * or moved, memslot will be created.
		 *
		 * validation of sp->gfn happens in:
<<<<<<< HEAD
		 * 	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 * 	- kvm_is_visible_gfn (mmu_check_roots)
		 */
		kvm_arch_flush_shadow_memslot(kvm, slot);
=======
		 *	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 *	- kvm_is_visible_gfn (mmu_check_roots)
		 */
		kvm_arch_flush_shadow_memslot(kvm, slot);

		/*
		 * We can re-use the old_memslots from above, the only difference
		 * from the currently installed memslots is the invalid flag.  This
		 * will get overwritten by update_memslots anyway.
		 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		slots = old_memslots;
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, mem, change);
	if (r)
		goto out_slots;

<<<<<<< HEAD
	r = -ENOMEM;
	/*
	 * We can re-use the old_memslots from above, the only difference
	 * from the currently installed memslots is the invalid flag.  This
	 * will get overwritten by update_memslots anyway.
	 */
	if (!slots) {
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
	}

	/* actual memory is freed via old in kvm_free_physmem_slot below */
=======
	/* actual memory is freed via old in kvm_free_memslot below */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (change == KVM_MR_DELETE) {
		new.dirty_bitmap = NULL;
		memset(&new.arch, 0, sizeof(new.arch));
	}

<<<<<<< HEAD
	old_memslots = install_new_memslots(kvm, slots, &new);

	kvm_arch_commit_memory_region(kvm, mem, &old, change);

	kvm_free_physmem_slot(kvm, &old, &new);
	kfree(old_memslots);
=======
	update_memslots(slots, &new);
	old_memslots = install_new_memslots(kvm, as_id, slots);

	kvm_arch_commit_memory_region(kvm, mem, &old, &new, change);

	kvm_free_memslot(kvm, &old, &new);
	kvfree(old_memslots);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * IOMMU mapping:  New slots need to be mapped.  Old slots need to be
	 * un-mapped and re-mapped if their base changes.  Since base change
	 * unmapping is handled above with slot deletion, mapping alone is
	 * needed here.  Anything else the iommu might care about for existing
	 * slots (size changes, userspace addr changes and read-only flag
	 * changes) is disallowed above, so any other attribute changes getting
	 * here can be skipped.
	 */
<<<<<<< HEAD
	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
=======
	if (as_id == 0 && (change == KVM_MR_CREATE || change == KVM_MR_MOVE)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		r = kvm_iommu_map_pages(kvm, &new);
		return r;
	}

	return 0;

out_slots:
<<<<<<< HEAD
	kfree(slots);
out_free:
	kvm_free_physmem_slot(kvm, &new, &old);
=======
	kvfree(slots);
out_free:
	kvm_free_memslot(kvm, &new, &old);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return r;
}
EXPORT_SYMBOL_GPL(__kvm_set_memory_region);

int kvm_set_memory_region(struct kvm *kvm,
<<<<<<< HEAD
			  struct kvm_userspace_memory_region *mem)
=======
			  const struct kvm_userspace_memory_region *mem)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int r;

	mutex_lock(&kvm->slots_lock);
	r = __kvm_set_memory_region(kvm, mem);
	mutex_unlock(&kvm->slots_lock);
	return r;
}
EXPORT_SYMBOL_GPL(kvm_set_memory_region);

static int kvm_vm_ioctl_set_memory_region(struct kvm *kvm,
					  struct kvm_userspace_memory_region *mem)
{
<<<<<<< HEAD
	if (mem->slot >= KVM_USER_MEM_SLOTS)
		return -EINVAL;
=======
	if ((u16)mem->slot >= KVM_USER_MEM_SLOTS)
		return -EINVAL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return kvm_set_memory_region(kvm, mem);
}

int kvm_get_dirty_log(struct kvm *kvm,
			struct kvm_dirty_log *log, int *is_dirty)
{
<<<<<<< HEAD
	struct kvm_memory_slot *memslot;
	int r, i;
=======
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
	int r, i, as_id, id;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long n;
	unsigned long any = 0;

	r = -EINVAL;
<<<<<<< HEAD
	if (log->slot >= KVM_USER_MEM_SLOTS)
		goto out;

	memslot = id_to_memslot(kvm->memslots, log->slot);
=======
	as_id = log->slot >> 16;
	id = (u16)log->slot;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_USER_MEM_SLOTS)
		goto out;

	slots = __kvm_memslots(kvm, as_id);
	memslot = id_to_memslot(slots, id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	r = -ENOENT;
	if (!memslot->dirty_bitmap)
		goto out;

	n = kvm_dirty_bitmap_bytes(memslot);

	for (i = 0; !any && i < n/sizeof(long); ++i)
		any = memslot->dirty_bitmap[i];

	r = -EFAULT;
	if (copy_to_user(log->dirty_bitmap, memslot->dirty_bitmap, n))
		goto out;

	if (any)
		*is_dirty = 1;

	r = 0;
out:
	return r;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvm_get_dirty_log);

#ifdef CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT
/**
 * kvm_get_dirty_log_protect - get a snapshot of dirty pages, and if any pages
 *	are dirty write protect them for next write.
 * @kvm:	pointer to kvm instance
 * @log:	slot id and address to which we copy the log
 * @is_dirty:	flag set if any page is dirty
 *
 * We need to keep it in mind that VCPU threads can write to the bitmap
 * concurrently. So, to avoid losing track of dirty pages we keep the
 * following order:
 *
 *    1. Take a snapshot of the bit and clear it if needed.
 *    2. Write protect the corresponding page.
 *    3. Copy the snapshot to the userspace.
 *    4. Upon return caller flushes TLB's if needed.
 *
 * Between 2 and 4, the guest may write to the page using the remaining TLB
 * entry.  This is not a problem because the page is reported dirty using
 * the snapshot taken before and step 4 ensures that writes done after
 * exiting to userspace will be logged for the next call.
 *
 */
int kvm_get_dirty_log_protect(struct kvm *kvm,
			struct kvm_dirty_log *log, bool *is_dirty)
{
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
	int r, i, as_id, id;
	unsigned long n;
	unsigned long *dirty_bitmap;
	unsigned long *dirty_bitmap_buffer;

	r = -EINVAL;
	as_id = log->slot >> 16;
	id = (u16)log->slot;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_USER_MEM_SLOTS)
		goto out;

	slots = __kvm_memslots(kvm, as_id);
	memslot = id_to_memslot(slots, id);

	dirty_bitmap = memslot->dirty_bitmap;
	r = -ENOENT;
	if (!dirty_bitmap)
		goto out;

	n = kvm_dirty_bitmap_bytes(memslot);

	dirty_bitmap_buffer = dirty_bitmap + n / sizeof(long);
	memset(dirty_bitmap_buffer, 0, n);

	spin_lock(&kvm->mmu_lock);
	*is_dirty = false;
	for (i = 0; i < n / sizeof(long); i++) {
		unsigned long mask;
		gfn_t offset;

		if (!dirty_bitmap[i])
			continue;

		*is_dirty = true;

		mask = xchg(&dirty_bitmap[i], 0);
		dirty_bitmap_buffer[i] = mask;

		if (mask) {
			offset = i * BITS_PER_LONG;
			kvm_arch_mmu_enable_log_dirty_pt_masked(kvm, memslot,
								offset, mask);
		}
	}

	spin_unlock(&kvm->mmu_lock);

	r = -EFAULT;
	if (copy_to_user(log->dirty_bitmap, dirty_bitmap_buffer, n))
		goto out;

	r = 0;
out:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_get_dirty_log_protect);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

bool kvm_largepages_enabled(void)
{
	return largepages_enabled;
}

void kvm_disable_largepages(void)
{
	largepages_enabled = false;
}
EXPORT_SYMBOL_GPL(kvm_disable_largepages);

struct kvm_memory_slot *gfn_to_memslot(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_memslot(kvm_memslots(kvm), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_memslot);

<<<<<<< HEAD
int kvm_is_visible_gfn(struct kvm *kvm, gfn_t gfn)
=======
struct kvm_memory_slot *kvm_vcpu_gfn_to_memslot(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return __gfn_to_memslot(kvm_vcpu_memslots(vcpu), gfn);
}

bool kvm_is_visible_gfn(struct kvm *kvm, gfn_t gfn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct kvm_memory_slot *memslot = gfn_to_memslot(kvm, gfn);

	if (!memslot || memslot->id >= KVM_USER_MEM_SLOTS ||
	      memslot->flags & KVM_MEMSLOT_INVALID)
<<<<<<< HEAD
		return 0;

	return 1;
=======
		return false;

	return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL_GPL(kvm_is_visible_gfn);

unsigned long kvm_host_page_size(struct kvm *kvm, gfn_t gfn)
{
	struct vm_area_struct *vma;
	unsigned long addr, size;

	size = PAGE_SIZE;

	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr))
		return PAGE_SIZE;

	down_read(&current->mm->mmap_sem);
	vma = find_vma(current->mm, addr);
	if (!vma)
		goto out;

	size = vma_kernel_pagesize(vma);

out:
	up_read(&current->mm->mmap_sem);

	return size;
}

static bool memslot_is_readonly(struct kvm_memory_slot *slot)
{
	return slot->flags & KVM_MEM_READONLY;
}

static unsigned long __gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				       gfn_t *nr_pages, bool write)
{
	if (!slot || slot->flags & KVM_MEMSLOT_INVALID)
		return KVM_HVA_ERR_BAD;

	if (memslot_is_readonly(slot) && write)
		return KVM_HVA_ERR_RO_BAD;

	if (nr_pages)
		*nr_pages = slot->npages - (gfn - slot->base_gfn);

	return __gfn_to_hva_memslot(slot, gfn);
}

static unsigned long gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				     gfn_t *nr_pages)
{
	return __gfn_to_hva_many(slot, gfn, nr_pages, true);
}

unsigned long gfn_to_hva_memslot(struct kvm_memory_slot *slot,
					gfn_t gfn)
{
	return gfn_to_hva_many(slot, gfn, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_hva_memslot);

unsigned long gfn_to_hva(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_hva_many(gfn_to_memslot(kvm, gfn), gfn, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_hva);

<<<<<<< HEAD
=======
unsigned long kvm_vcpu_gfn_to_hva(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return gfn_to_hva_many(kvm_vcpu_gfn_to_memslot(vcpu, gfn), gfn, NULL);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_hva);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * If writable is set to false, the hva returned by this function is only
 * allowed to be read.
 */
unsigned long gfn_to_hva_memslot_prot(struct kvm_memory_slot *slot,
				      gfn_t gfn, bool *writable)
{
	unsigned long hva = __gfn_to_hva_many(slot, gfn, NULL, false);

	if (!kvm_is_error_hva(hva) && writable)
		*writable = !memslot_is_readonly(slot);

	return hva;
}

unsigned long gfn_to_hva_prot(struct kvm *kvm, gfn_t gfn, bool *writable)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);

	return gfn_to_hva_memslot_prot(slot, gfn, writable);
}

<<<<<<< HEAD
static int kvm_read_hva(void *data, void __user *hva, int len)
{
	return __copy_from_user(data, hva, len);
}

static int kvm_read_hva_atomic(void *data, void __user *hva, int len)
{
	return __copy_from_user_inatomic(data, hva, len);
}

static int get_user_page_nowait(struct task_struct *tsk, struct mm_struct *mm,
	unsigned long start, int write, struct page **page)
{
	int flags = FOLL_TOUCH | FOLL_NOWAIT | FOLL_HWPOISON | FOLL_GET;
=======
unsigned long kvm_vcpu_gfn_to_hva_prot(struct kvm_vcpu *vcpu, gfn_t gfn, bool *writable)
{
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

	return gfn_to_hva_memslot_prot(slot, gfn, writable);
}

static int get_user_page_nowait(unsigned long start, int write,
		struct page **page)
{
	int flags = FOLL_NOWAIT | FOLL_HWPOISON;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (write)
		flags |= FOLL_WRITE;

<<<<<<< HEAD
	return __get_user_pages(tsk, mm, start, 1, flags, page, NULL, NULL);
=======
	return get_user_pages(start, 1, flags, page, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline int check_user_page_hwpoison(unsigned long addr)
{
<<<<<<< HEAD
	int rc, flags = FOLL_TOUCH | FOLL_HWPOISON | FOLL_WRITE;

	rc = __get_user_pages(current, current->mm, addr, 1,
			      flags, NULL, NULL, NULL);
=======
	int rc, flags = FOLL_HWPOISON | FOLL_WRITE;

	rc = get_user_pages(addr, 1, flags, NULL, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return rc == -EHWPOISON;
}

/*
 * The atomic path to get the writable pfn which will be stored in @pfn,
 * true indicates success, otherwise false is returned.
 */
static bool hva_to_pfn_fast(unsigned long addr, bool atomic, bool *async,
<<<<<<< HEAD
			    bool write_fault, bool *writable, pfn_t *pfn)
=======
			    bool write_fault, bool *writable, kvm_pfn_t *pfn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct page *page[1];
	int npages;

	if (!(async || atomic))
		return false;

	/*
	 * Fast pin a writable pfn only if it is a write fault request
	 * or the caller allows to map a writable pfn for a read fault
	 * request.
	 */
	if (!(write_fault || writable))
		return false;

	npages = __get_user_pages_fast(addr, 1, 1, page);
	if (npages == 1) {
		*pfn = page_to_pfn(page[0]);

		if (writable)
			*writable = true;
		return true;
	}

	return false;
}

/*
 * The slow path to get the pfn of the specified host virtual address,
 * 1 indicates success, -errno is returned if error is detected.
 */
static int hva_to_pfn_slow(unsigned long addr, bool *async, bool write_fault,
<<<<<<< HEAD
			   bool *writable, pfn_t *pfn)
=======
			   bool *writable, kvm_pfn_t *pfn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct page *page[1];
	int npages = 0;

	might_sleep();

	if (writable)
		*writable = write_fault;

	if (async) {
		down_read(&current->mm->mmap_sem);
<<<<<<< HEAD
		npages = get_user_page_nowait(current, current->mm,
					      addr, write_fault, page);
		up_read(&current->mm->mmap_sem);
	} else
		npages = get_user_pages_fast(addr, 1, write_fault,
					     page);
=======
		npages = get_user_page_nowait(addr, write_fault, page);
		up_read(&current->mm->mmap_sem);
	} else {
		unsigned int flags = FOLL_TOUCH | FOLL_HWPOISON;

		if (write_fault)
			flags |= FOLL_WRITE;

		npages = __get_user_pages_unlocked(current, current->mm, addr, 1,
						   page, flags);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (npages != 1)
		return npages;

	/* map read fault as writable if possible */
	if (unlikely(!write_fault) && writable) {
		struct page *wpage[1];

		npages = __get_user_pages_fast(addr, 1, 1, wpage);
		if (npages == 1) {
			*writable = true;
			put_page(page[0]);
			page[0] = wpage[0];
		}

		npages = 1;
	}
	*pfn = page_to_pfn(page[0]);
	return npages;
}

static bool vma_is_valid(struct vm_area_struct *vma, bool write_fault)
{
	if (unlikely(!(vma->vm_flags & VM_READ)))
		return false;

	if (write_fault && (unlikely(!(vma->vm_flags & VM_WRITE))))
		return false;

	return true;
}

<<<<<<< HEAD
=======
static int hva_to_pfn_remapped(struct vm_area_struct *vma,
			       unsigned long addr, bool *async,
			       bool write_fault, bool *writable,
			       kvm_pfn_t *p_pfn)
{
	unsigned long pfn;
	int r;

	r = follow_pfn(vma, addr, &pfn);
	if (r) {
		/*
		 * get_user_pages fails for VM_IO and VM_PFNMAP vmas and does
		 * not call the fault handler, so do it here.
		 */
		bool unlocked = false;
		r = fixup_user_fault(current, current->mm, addr,
				     (write_fault ? FAULT_FLAG_WRITE : 0),
				     &unlocked);
		if (unlocked)
			return -EAGAIN;
		if (r)
			return r;

		r = follow_pfn(vma, addr, &pfn);
		if (r)
			return r;

	}

	if (writable)
		*writable = true;

	/*
	 * Get a reference here because callers of *hva_to_pfn* and
	 * *gfn_to_pfn* ultimately call kvm_release_pfn_clean on the
	 * returned pfn.  This is only needed if the VMA has VM_MIXEDMAP
	 * set, but the kvm_get_pfn/kvm_release_pfn_clean pair will
	 * simply do nothing for reserved pfns.
	 *
	 * Whoever called remap_pfn_range is also going to call e.g.
	 * unmap_mapping_range before the underlying pages are freed,
	 * causing a call to our MMU notifier.
	 */ 
	kvm_get_pfn(pfn);

	*p_pfn = pfn;
	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Pin guest page in memory and return its pfn.
 * @addr: host virtual address which maps memory to the guest
 * @atomic: whether this function can sleep
 * @async: whether this function need to wait IO complete if the
 *         host page is not in the memory
 * @write_fault: whether we should get a writable host page
 * @writable: whether it allows to map a writable host page for !@write_fault
 *
 * The function will map a writable host page for these two cases:
 * 1): @write_fault = true
 * 2): @write_fault = false && @writable, @writable will tell the caller
 *     whether the mapping is writable.
 */
<<<<<<< HEAD
static pfn_t hva_to_pfn(unsigned long addr, bool atomic, bool *async,
			bool write_fault, bool *writable)
{
	struct vm_area_struct *vma;
	pfn_t pfn = 0;
	int npages;
=======
static kvm_pfn_t hva_to_pfn(unsigned long addr, bool atomic, bool *async,
			bool write_fault, bool *writable)
{
	struct vm_area_struct *vma;
	kvm_pfn_t pfn = 0;
	int npages, r;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* we can do it either atomically or asynchronously, not both */
	BUG_ON(atomic && async);

	if (hva_to_pfn_fast(addr, atomic, async, write_fault, writable, &pfn))
		return pfn;

	if (atomic)
		return KVM_PFN_ERR_FAULT;

	npages = hva_to_pfn_slow(addr, async, write_fault, writable, &pfn);
	if (npages == 1)
		return pfn;

	down_read(&current->mm->mmap_sem);
	if (npages == -EHWPOISON ||
	      (!async && check_user_page_hwpoison(addr))) {
		pfn = KVM_PFN_ERR_HWPOISON;
		goto exit;
	}

<<<<<<< HEAD
=======
retry:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vma = find_vma_intersection(current->mm, addr, addr + 1);

	if (vma == NULL)
		pfn = KVM_PFN_ERR_FAULT;
<<<<<<< HEAD
	else if ((vma->vm_flags & VM_PFNMAP)) {
		pfn = ((addr - vma->vm_start) >> PAGE_SHIFT) +
			vma->vm_pgoff;
		BUG_ON(!kvm_is_mmio_pfn(pfn));
=======
	else if (vma->vm_flags & (VM_IO | VM_PFNMAP)) {
		r = hva_to_pfn_remapped(vma, addr, async, write_fault, writable, &pfn);
		if (r == -EAGAIN)
			goto retry;
		if (r < 0)
			pfn = KVM_PFN_ERR_FAULT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		if (async && vma_is_valid(vma, write_fault))
			*async = true;
		pfn = KVM_PFN_ERR_FAULT;
	}
exit:
	up_read(&current->mm->mmap_sem);
	return pfn;
}

<<<<<<< HEAD
static pfn_t
__gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn, bool atomic,
		     bool *async, bool write_fault, bool *writable)
{
	unsigned long addr = __gfn_to_hva_many(slot, gfn, NULL, write_fault);

	if (addr == KVM_HVA_ERR_RO_BAD)
		return KVM_PFN_ERR_RO_FAULT;

	if (kvm_is_error_hva(addr))
		return KVM_PFN_NOSLOT;
=======
kvm_pfn_t __gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn,
			       bool atomic, bool *async, bool write_fault,
			       bool *writable)
{
	unsigned long addr = __gfn_to_hva_many(slot, gfn, NULL, write_fault);

	if (addr == KVM_HVA_ERR_RO_BAD) {
		if (writable)
			*writable = false;
		return KVM_PFN_ERR_RO_FAULT;
	}

	if (kvm_is_error_hva(addr)) {
		if (writable)
			*writable = false;
		return KVM_PFN_NOSLOT;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Do not map writable pfn in the readonly memslot. */
	if (writable && memslot_is_readonly(slot)) {
		*writable = false;
		writable = NULL;
	}

	return hva_to_pfn(addr, atomic, async, write_fault,
			  writable);
}
<<<<<<< HEAD

static pfn_t __gfn_to_pfn(struct kvm *kvm, gfn_t gfn, bool atomic, bool *async,
			  bool write_fault, bool *writable)
{
	struct kvm_memory_slot *slot;

	if (async)
		*async = false;

	slot = gfn_to_memslot(kvm, gfn);

	return __gfn_to_pfn_memslot(slot, gfn, atomic, async, write_fault,
				    writable);
}

pfn_t gfn_to_pfn_atomic(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_pfn(kvm, gfn, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_atomic);

pfn_t gfn_to_pfn_async(struct kvm *kvm, gfn_t gfn, bool *async,
		       bool write_fault, bool *writable)
{
	return __gfn_to_pfn(kvm, gfn, false, async, write_fault, writable);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_async);

pfn_t gfn_to_pfn(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_pfn(kvm, gfn, false, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn);

pfn_t gfn_to_pfn_prot(struct kvm *kvm, gfn_t gfn, bool write_fault,
		      bool *writable)
{
	return __gfn_to_pfn(kvm, gfn, false, NULL, write_fault, writable);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_prot);

pfn_t gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return __gfn_to_pfn_memslot(slot, gfn, false, NULL, true, NULL);
}

pfn_t gfn_to_pfn_memslot_atomic(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return __gfn_to_pfn_memslot(slot, gfn, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot_atomic);

int gfn_to_page_many_atomic(struct kvm *kvm, gfn_t gfn, struct page **pages,
								  int nr_pages)
=======
EXPORT_SYMBOL_GPL(__gfn_to_pfn_memslot);

kvm_pfn_t gfn_to_pfn_prot(struct kvm *kvm, gfn_t gfn, bool write_fault,
		      bool *writable)
{
	return __gfn_to_pfn_memslot(gfn_to_memslot(kvm, gfn), gfn, false, NULL,
				    write_fault, writable);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_prot);

kvm_pfn_t gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return __gfn_to_pfn_memslot(slot, gfn, false, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot);

kvm_pfn_t gfn_to_pfn_memslot_atomic(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return __gfn_to_pfn_memslot(slot, gfn, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot_atomic);

kvm_pfn_t gfn_to_pfn_atomic(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_pfn_memslot_atomic(gfn_to_memslot(kvm, gfn), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_atomic);

kvm_pfn_t kvm_vcpu_gfn_to_pfn_atomic(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return gfn_to_pfn_memslot_atomic(kvm_vcpu_gfn_to_memslot(vcpu, gfn), gfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_pfn_atomic);

kvm_pfn_t gfn_to_pfn(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_pfn_memslot(gfn_to_memslot(kvm, gfn), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn);

kvm_pfn_t kvm_vcpu_gfn_to_pfn(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	return gfn_to_pfn_memslot(kvm_vcpu_gfn_to_memslot(vcpu, gfn), gfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_pfn);

int gfn_to_page_many_atomic(struct kvm_memory_slot *slot, gfn_t gfn,
			    struct page **pages, int nr_pages)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long addr;
	gfn_t entry;

<<<<<<< HEAD
	addr = gfn_to_hva_many(gfn_to_memslot(kvm, gfn), gfn, &entry);
=======
	addr = gfn_to_hva_many(slot, gfn, &entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (kvm_is_error_hva(addr))
		return -1;

	if (entry < nr_pages)
		return 0;

	return __get_user_pages_fast(addr, nr_pages, 1, pages);
}
EXPORT_SYMBOL_GPL(gfn_to_page_many_atomic);

<<<<<<< HEAD
static struct page *kvm_pfn_to_page(pfn_t pfn)
=======
static struct page *kvm_pfn_to_page(kvm_pfn_t pfn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (is_error_noslot_pfn(pfn))
		return KVM_ERR_PTR_BAD_PAGE;

<<<<<<< HEAD
	if (kvm_is_mmio_pfn(pfn)) {
=======
	if (kvm_is_reserved_pfn(pfn)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		WARN_ON(1);
		return KVM_ERR_PTR_BAD_PAGE;
	}

	return pfn_to_page(pfn);
}

struct page *gfn_to_page(struct kvm *kvm, gfn_t gfn)
{
<<<<<<< HEAD
	pfn_t pfn;
=======
	kvm_pfn_t pfn;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pfn = gfn_to_pfn(kvm, gfn);

	return kvm_pfn_to_page(pfn);
}
<<<<<<< HEAD

EXPORT_SYMBOL_GPL(gfn_to_page);

=======
EXPORT_SYMBOL_GPL(gfn_to_page);

struct page *kvm_vcpu_gfn_to_page(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	kvm_pfn_t pfn;

	pfn = kvm_vcpu_gfn_to_pfn(vcpu, gfn);

	return kvm_pfn_to_page(pfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_gfn_to_page);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void kvm_release_page_clean(struct page *page)
{
	WARN_ON(is_error_page(page));

	kvm_release_pfn_clean(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_release_page_clean);

<<<<<<< HEAD
void kvm_release_pfn_clean(pfn_t pfn)
{
	if (!is_error_noslot_pfn(pfn) && !kvm_is_mmio_pfn(pfn))
=======
void kvm_release_pfn_clean(kvm_pfn_t pfn)
{
	if (!is_error_noslot_pfn(pfn) && !kvm_is_reserved_pfn(pfn))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		put_page(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_release_pfn_clean);

void kvm_release_page_dirty(struct page *page)
{
	WARN_ON(is_error_page(page));

	kvm_release_pfn_dirty(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_release_page_dirty);

<<<<<<< HEAD
static void kvm_release_pfn_dirty(pfn_t pfn)
=======
static void kvm_release_pfn_dirty(kvm_pfn_t pfn)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	kvm_set_pfn_dirty(pfn);
	kvm_release_pfn_clean(pfn);
}

<<<<<<< HEAD
void kvm_set_pfn_dirty(pfn_t pfn)
{
	if (!kvm_is_mmio_pfn(pfn)) {
		struct page *page = pfn_to_page(pfn);
=======
void kvm_set_pfn_dirty(kvm_pfn_t pfn)
{
	if (!kvm_is_reserved_pfn(pfn) && !kvm_is_zone_device_pfn(pfn)) {
		struct page *page = pfn_to_page(pfn);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (!PageReserved(page))
			SetPageDirty(page);
	}
}
EXPORT_SYMBOL_GPL(kvm_set_pfn_dirty);

<<<<<<< HEAD
void kvm_set_pfn_accessed(pfn_t pfn)
{
	if (!kvm_is_mmio_pfn(pfn))
=======
void kvm_set_pfn_accessed(kvm_pfn_t pfn)
{
	if (!kvm_is_reserved_pfn(pfn) && !kvm_is_zone_device_pfn(pfn))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		mark_page_accessed(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_set_pfn_accessed);

<<<<<<< HEAD
void kvm_get_pfn(pfn_t pfn)
{
	if (!kvm_is_mmio_pfn(pfn))
=======
void kvm_get_pfn(kvm_pfn_t pfn)
{
	if (!kvm_is_reserved_pfn(pfn))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		get_page(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_get_pfn);

static int next_segment(unsigned long len, int offset)
{
	if (len > PAGE_SIZE - offset)
		return PAGE_SIZE - offset;
	else
		return len;
}

<<<<<<< HEAD
int kvm_read_guest_page(struct kvm *kvm, gfn_t gfn, void *data, int offset,
			int len)
=======
static int __kvm_read_guest_page(struct kvm_memory_slot *slot, gfn_t gfn,
				 void *data, int offset, int len)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int r;
	unsigned long addr;

<<<<<<< HEAD
	addr = gfn_to_hva_prot(kvm, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = kvm_read_hva(data, (void __user *)addr + offset, len);
=======
	addr = gfn_to_hva_memslot_prot(slot, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = __copy_from_user(data, (void __user *)addr + offset, len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (r)
		return -EFAULT;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(kvm_read_guest_page);

=======

int kvm_read_guest_page(struct kvm *kvm, gfn_t gfn, void *data, int offset,
			int len)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);

	return __kvm_read_guest_page(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_read_guest_page);

int kvm_vcpu_read_guest_page(struct kvm_vcpu *vcpu, gfn_t gfn, void *data,
			     int offset, int len)
{
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

	return __kvm_read_guest_page(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_read_guest_page);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int kvm_read_guest(struct kvm *kvm, gpa_t gpa, void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_read_guest_page(kvm, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_read_guest);

<<<<<<< HEAD
int kvm_read_guest_atomic(struct kvm *kvm, gpa_t gpa, void *data,
			  unsigned long len)
{
	int r;
	unsigned long addr;
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int offset = offset_in_page(gpa);

	addr = gfn_to_hva_prot(kvm, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	pagefault_disable();
	r = kvm_read_hva_atomic(data, (void __user *)addr + offset, len);
=======
int kvm_vcpu_read_guest(struct kvm_vcpu *vcpu, gpa_t gpa, void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_vcpu_read_guest_page(vcpu, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_read_guest);

static int __kvm_read_guest_atomic(struct kvm_memory_slot *slot, gfn_t gfn,
			           void *data, int offset, unsigned long len)
{
	int r;
	unsigned long addr;

	addr = gfn_to_hva_memslot_prot(slot, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	pagefault_disable();
	r = __copy_from_user_inatomic(data, (void __user *)addr + offset, len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	pagefault_enable();
	if (r)
		return -EFAULT;
	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(kvm_read_guest_atomic);

int kvm_write_guest_page(struct kvm *kvm, gfn_t gfn, const void *data,
			 int offset, int len)
=======

int kvm_read_guest_atomic(struct kvm *kvm, gpa_t gpa, void *data,
			  unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);
	int offset = offset_in_page(gpa);

	return __kvm_read_guest_atomic(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_read_guest_atomic);

int kvm_vcpu_read_guest_atomic(struct kvm_vcpu *vcpu, gpa_t gpa,
			       void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);
	int offset = offset_in_page(gpa);

	return __kvm_read_guest_atomic(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_read_guest_atomic);

static int __kvm_write_guest_page(struct kvm_memory_slot *memslot, gfn_t gfn,
			          const void *data, int offset, int len)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int r;
	unsigned long addr;

<<<<<<< HEAD
	addr = gfn_to_hva(kvm, gfn);
=======
	addr = gfn_to_hva_memslot(memslot, gfn);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = __copy_to_user((void __user *)addr + offset, data, len);
	if (r)
		return -EFAULT;
<<<<<<< HEAD
	mark_page_dirty(kvm, gfn);
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_write_guest_page);

=======
	mark_page_dirty_in_slot(memslot, gfn);
	return 0;
}

int kvm_write_guest_page(struct kvm *kvm, gfn_t gfn,
			 const void *data, int offset, int len)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);

	return __kvm_write_guest_page(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_write_guest_page);

int kvm_vcpu_write_guest_page(struct kvm_vcpu *vcpu, gfn_t gfn,
			      const void *data, int offset, int len)
{
	struct kvm_memory_slot *slot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);

	return __kvm_write_guest_page(slot, gfn, data, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_write_guest_page);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int kvm_write_guest(struct kvm *kvm, gpa_t gpa, const void *data,
		    unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_write_guest_page(kvm, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvm_write_guest);

int kvm_vcpu_write_guest(struct kvm_vcpu *vcpu, gpa_t gpa, const void *data,
		         unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_vcpu_write_guest_page(vcpu, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_write_guest);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvm_gfn_to_hva_cache_init(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			      gpa_t gpa, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int offset = offset_in_page(gpa);
	gfn_t start_gfn = gpa >> PAGE_SHIFT;
	gfn_t end_gfn = (gpa + len - 1) >> PAGE_SHIFT;
	gfn_t nr_pages_needed = end_gfn - start_gfn + 1;
	gfn_t nr_pages_avail;

	ghc->gpa = gpa;
	ghc->generation = slots->generation;
	ghc->len = len;
	ghc->memslot = gfn_to_memslot(kvm, start_gfn);
<<<<<<< HEAD
	ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn, &nr_pages_avail);
	if (!kvm_is_error_hva(ghc->hva) && nr_pages_avail >= nr_pages_needed) {
=======
	ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn, NULL);
	if (!kvm_is_error_hva(ghc->hva) && nr_pages_needed <= 1) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ghc->hva += offset;
	} else {
		/*
		 * If the requested region crosses two memslots, we still
		 * verify that the entire region is valid here.
		 */
		while (start_gfn <= end_gfn) {
			ghc->memslot = gfn_to_memslot(kvm, start_gfn);
			ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn,
						   &nr_pages_avail);
			if (kvm_is_error_hva(ghc->hva))
				return -EFAULT;
			start_gfn += nr_pages_avail;
		}
		/* Use the slow path for cross page reads and writes. */
		ghc->memslot = NULL;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_gfn_to_hva_cache_init);

int kvm_write_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;

	BUG_ON(len > ghc->len);

	if (slots->generation != ghc->generation)
		kvm_gfn_to_hva_cache_init(kvm, ghc, ghc->gpa, ghc->len);

<<<<<<< HEAD
	if (unlikely(!ghc->memslot))
		return kvm_write_guest(kvm, ghc->gpa, data, len);

	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

	r = __copy_to_user((void __user *)ghc->hva, data, len);
	if (r)
		return -EFAULT;
	mark_page_dirty_in_slot(kvm, ghc->memslot, ghc->gpa >> PAGE_SHIFT);
=======
	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

	if (unlikely(!ghc->memslot))
		return kvm_write_guest(kvm, ghc->gpa, data, len);

	r = __copy_to_user((void __user *)ghc->hva, data, len);
	if (r)
		return -EFAULT;
	mark_page_dirty_in_slot(ghc->memslot, ghc->gpa >> PAGE_SHIFT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}
EXPORT_SYMBOL_GPL(kvm_write_guest_cached);

int kvm_read_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;

	BUG_ON(len > ghc->len);

	if (slots->generation != ghc->generation)
		kvm_gfn_to_hva_cache_init(kvm, ghc, ghc->gpa, ghc->len);

<<<<<<< HEAD
	if (unlikely(!ghc->memslot))
		return kvm_read_guest(kvm, ghc->gpa, data, len);

	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

=======
	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

	if (unlikely(!ghc->memslot))
		return kvm_read_guest(kvm, ghc->gpa, data, len);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	r = __copy_from_user(data, (void __user *)ghc->hva, len);
	if (r)
		return -EFAULT;

	return 0;
}
EXPORT_SYMBOL_GPL(kvm_read_guest_cached);

int kvm_clear_guest_page(struct kvm *kvm, gfn_t gfn, int offset, int len)
{
	const void *zero_page = (const void *) __va(page_to_phys(ZERO_PAGE(0)));

	return kvm_write_guest_page(kvm, gfn, zero_page, offset, len);
}
EXPORT_SYMBOL_GPL(kvm_clear_guest_page);

int kvm_clear_guest(struct kvm *kvm, gpa_t gpa, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

<<<<<<< HEAD
        while ((seg = next_segment(len, offset)) != 0) {
=======
	while ((seg = next_segment(len, offset)) != 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ret = kvm_clear_guest_page(kvm, gfn, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_clear_guest);

<<<<<<< HEAD
static void mark_page_dirty_in_slot(struct kvm *kvm,
				    struct kvm_memory_slot *memslot,
=======
static void mark_page_dirty_in_slot(struct kvm_memory_slot *memslot,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				    gfn_t gfn)
{
	if (memslot && memslot->dirty_bitmap) {
		unsigned long rel_gfn = gfn - memslot->base_gfn;

		set_bit_le(rel_gfn, memslot->dirty_bitmap);
	}
}

void mark_page_dirty(struct kvm *kvm, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	memslot = gfn_to_memslot(kvm, gfn);
<<<<<<< HEAD
	mark_page_dirty_in_slot(kvm, memslot, gfn);
=======
	mark_page_dirty_in_slot(memslot, gfn);
}
EXPORT_SYMBOL_GPL(mark_page_dirty);

void kvm_vcpu_mark_page_dirty(struct kvm_vcpu *vcpu, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	memslot = kvm_vcpu_gfn_to_memslot(vcpu, gfn);
	mark_page_dirty_in_slot(memslot, gfn);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_mark_page_dirty);

static void grow_halt_poll_ns(struct kvm_vcpu *vcpu)
{
	unsigned int old, val, grow;

	old = val = vcpu->halt_poll_ns;
	grow = READ_ONCE(halt_poll_ns_grow);
	/* 10us base */
	if (val == 0 && grow)
		val = 10000;
	else
		val *= grow;

	if (val > halt_poll_ns)
		val = halt_poll_ns;

	vcpu->halt_poll_ns = val;
	trace_kvm_halt_poll_ns_grow(vcpu->vcpu_id, val, old);
}

static void shrink_halt_poll_ns(struct kvm_vcpu *vcpu)
{
	unsigned int old, val, shrink;

	old = val = vcpu->halt_poll_ns;
	shrink = READ_ONCE(halt_poll_ns_shrink);
	if (shrink == 0)
		val = 0;
	else
		val /= shrink;

	vcpu->halt_poll_ns = val;
	trace_kvm_halt_poll_ns_shrink(vcpu->vcpu_id, val, old);
}

static int kvm_vcpu_check_block(struct kvm_vcpu *vcpu)
{
	if (kvm_arch_vcpu_runnable(vcpu)) {
		kvm_make_request(KVM_REQ_UNHALT, vcpu);
		return -EINTR;
	}
	if (kvm_cpu_has_pending_timer(vcpu))
		return -EINTR;
	if (signal_pending(current))
		return -EINTR;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * The vCPU has executed a HLT instruction with in-kernel mode enabled.
 */
void kvm_vcpu_block(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	DEFINE_WAIT(wait);

	for (;;) {
		prepare_to_wait(&vcpu->wq, &wait, TASK_INTERRUPTIBLE);

		if (kvm_arch_vcpu_runnable(vcpu)) {
			kvm_make_request(KVM_REQ_UNHALT, vcpu);
			break;
		}
		if (kvm_cpu_has_pending_timer(vcpu))
			break;
		if (signal_pending(current))
			break;

		schedule();
	}

	finish_wait(&vcpu->wq, &wait);
}

#ifndef CONFIG_S390
=======
	ktime_t start, cur;
	DECLARE_SWAITQUEUE(wait);
	bool waited = false;
	u64 block_ns;

	start = cur = ktime_get();
	if (vcpu->halt_poll_ns) {
		ktime_t stop = ktime_add_ns(ktime_get(), vcpu->halt_poll_ns);

		++vcpu->stat.halt_attempted_poll;
		do {
			/*
			 * This sets KVM_REQ_UNHALT if an interrupt
			 * arrives.
			 */
			if (kvm_vcpu_check_block(vcpu) < 0) {
				++vcpu->stat.halt_successful_poll;
				if (!vcpu_valid_wakeup(vcpu))
					++vcpu->stat.halt_poll_invalid;
				goto out;
			}
			cur = ktime_get();
		} while (single_task_running() && ktime_before(cur, stop));
	}

	kvm_arch_vcpu_blocking(vcpu);

	for (;;) {
		prepare_to_swait(&vcpu->wq, &wait, TASK_INTERRUPTIBLE);

		if (kvm_vcpu_check_block(vcpu) < 0)
			break;

		waited = true;
		schedule();
	}

	finish_swait(&vcpu->wq, &wait);
	cur = ktime_get();

	kvm_arch_vcpu_unblocking(vcpu);
out:
	block_ns = ktime_to_ns(cur) - ktime_to_ns(start);

	if (!vcpu_valid_wakeup(vcpu))
		shrink_halt_poll_ns(vcpu);
	else if (halt_poll_ns) {
		if (block_ns <= vcpu->halt_poll_ns)
			;
		/* we had a long block, shrink polling */
		else if (vcpu->halt_poll_ns && block_ns > halt_poll_ns)
			shrink_halt_poll_ns(vcpu);
		/* we had a short halt and our poll time is too small */
		else if (vcpu->halt_poll_ns < halt_poll_ns &&
			block_ns < halt_poll_ns)
			grow_halt_poll_ns(vcpu);
	} else
		vcpu->halt_poll_ns = 0;

	trace_kvm_vcpu_wakeup(block_ns, waited, vcpu_valid_wakeup(vcpu));
	kvm_arch_vcpu_block_finish(vcpu);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_block);

#ifndef CONFIG_S390
void kvm_vcpu_wake_up(struct kvm_vcpu *vcpu)
{
	struct swait_queue_head *wqp;

	wqp = kvm_arch_vcpu_wq(vcpu);
	if (swait_active(wqp)) {
		swake_up(wqp);
		++vcpu->stat.halt_wakeup;
	}

}
EXPORT_SYMBOL_GPL(kvm_vcpu_wake_up);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Kick a sleeping VCPU, or a guest VCPU in guest mode, into host kernel mode.
 */
void kvm_vcpu_kick(struct kvm_vcpu *vcpu)
{
	int me;
	int cpu = vcpu->cpu;
<<<<<<< HEAD
	wait_queue_head_t *wqp;

	wqp = kvm_arch_vcpu_wq(vcpu);
	if (waitqueue_active(wqp)) {
		wake_up_interruptible(wqp);
		++vcpu->stat.halt_wakeup;
	}

=======

	kvm_vcpu_wake_up(vcpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	me = get_cpu();
	if (cpu != me && (unsigned)cpu < nr_cpu_ids && cpu_online(cpu))
		if (kvm_arch_vcpu_should_kick(vcpu))
			smp_send_reschedule(cpu);
	put_cpu();
}
EXPORT_SYMBOL_GPL(kvm_vcpu_kick);
#endif /* !CONFIG_S390 */

<<<<<<< HEAD
bool kvm_vcpu_yield_to(struct kvm_vcpu *target)
{
	struct pid *pid;
	struct task_struct *task = NULL;
	bool ret = false;
=======
int kvm_vcpu_yield_to(struct kvm_vcpu *target)
{
	struct pid *pid;
	struct task_struct *task = NULL;
	int ret = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	rcu_read_lock();
	pid = rcu_dereference(target->pid);
	if (pid)
		task = get_pid_task(pid, PIDTYPE_PID);
	rcu_read_unlock();
	if (!task)
		return ret;
<<<<<<< HEAD
	if (task->flags & PF_VCPU) {
		put_task_struct(task);
		return ret;
	}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = yield_to(task, 1);
	put_task_struct(task);

	return ret;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_yield_to);

/*
 * Helper that checks whether a VCPU is eligible for directed yield.
 * Most eligible candidate to yield is decided by following heuristics:
 *
 *  (a) VCPU which has not done pl-exit or cpu relax intercepted recently
 *  (preempted lock holder), indicated by @in_spin_loop.
 *  Set at the beiginning and cleared at the end of interception/PLE handler.
 *
 *  (b) VCPU which has done pl-exit/ cpu relax intercepted but did not get
 *  chance last time (mostly it has become eligible now since we have probably
 *  yielded to lockholder in last iteration. This is done by toggling
 *  @dy_eligible each time a VCPU checked for eligibility.)
 *
 *  Yielding to a recently pl-exited/cpu relax intercepted VCPU before yielding
 *  to preempted lock-holder could result in wrong VCPU selection and CPU
 *  burning. Giving priority for a potential lock-holder increases lock
 *  progress.
 *
 *  Since algorithm is based on heuristics, accessing another VCPU data without
 *  locking does not harm. It may result in trying to yield to  same VCPU, fail
 *  and continue with next VCPU and so on.
 */
static bool kvm_vcpu_eligible_for_directed_yield(struct kvm_vcpu *vcpu)
{
#ifdef CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT
	bool eligible;

	eligible = !vcpu->spin_loop.in_spin_loop ||
		    vcpu->spin_loop.dy_eligible;

	if (vcpu->spin_loop.in_spin_loop)
		kvm_vcpu_set_dy_eligible(vcpu, !vcpu->spin_loop.dy_eligible);

	return eligible;
#else
	return true;
#endif
}

void kvm_vcpu_on_spin(struct kvm_vcpu *me)
{
	struct kvm *kvm = me->kvm;
	struct kvm_vcpu *vcpu;
	int last_boosted_vcpu = me->kvm->last_boosted_vcpu;
	int yielded = 0;
	int try = 3;
	int pass;
	int i;

	kvm_vcpu_set_in_spin_loop(me, true);
	/*
	 * We boost the priority of a VCPU that is runnable but not
	 * currently running, because it got preempted by something
	 * else and called schedule in __vcpu_run.  Hopefully that
	 * VCPU is holding the lock that we need and will release it.
	 * We approximate round-robin by starting at the last boosted VCPU.
	 */
	for (pass = 0; pass < 2 && !yielded && try; pass++) {
		kvm_for_each_vcpu(i, vcpu, kvm) {
			if (!pass && i <= last_boosted_vcpu) {
				i = last_boosted_vcpu;
				continue;
			} else if (pass && i > last_boosted_vcpu)
				break;
			if (!ACCESS_ONCE(vcpu->preempted))
				continue;
			if (vcpu == me)
				continue;
<<<<<<< HEAD
			if (waitqueue_active(&vcpu->wq) && !kvm_arch_vcpu_runnable(vcpu))
=======
			if (swait_active(&vcpu->wq) && !kvm_arch_vcpu_runnable(vcpu))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				continue;
			if (!kvm_vcpu_eligible_for_directed_yield(vcpu))
				continue;

			yielded = kvm_vcpu_yield_to(vcpu);
			if (yielded > 0) {
				kvm->last_boosted_vcpu = i;
				break;
			} else if (yielded < 0) {
				try--;
				if (!try)
					break;
			}
		}
	}
	kvm_vcpu_set_in_spin_loop(me, false);

	/* Ensure vcpu is not eligible during next spinloop */
	kvm_vcpu_set_dy_eligible(me, false);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_on_spin);

static int kvm_vcpu_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct kvm_vcpu *vcpu = vma->vm_file->private_data;
	struct page *page;

	if (vmf->pgoff == 0)
		page = virt_to_page(vcpu->run);
#ifdef CONFIG_X86
	else if (vmf->pgoff == KVM_PIO_PAGE_OFFSET)
		page = virt_to_page(vcpu->arch.pio_data);
#endif
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	else if (vmf->pgoff == KVM_COALESCED_MMIO_PAGE_OFFSET)
		page = virt_to_page(vcpu->kvm->coalesced_mmio_ring);
#endif
	else
		return kvm_arch_vcpu_fault(vcpu, vmf);
	get_page(page);
	vmf->page = page;
	return 0;
}

static const struct vm_operations_struct kvm_vcpu_vm_ops = {
	.fault = kvm_vcpu_fault,
};

static int kvm_vcpu_mmap(struct file *file, struct vm_area_struct *vma)
{
	vma->vm_ops = &kvm_vcpu_vm_ops;
	return 0;
}

static int kvm_vcpu_release(struct inode *inode, struct file *filp)
{
	struct kvm_vcpu *vcpu = filp->private_data;

<<<<<<< HEAD
=======
	debugfs_remove_recursive(vcpu->debugfs_dentry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kvm_put_kvm(vcpu->kvm);
	return 0;
}

static struct file_operations kvm_vcpu_fops = {
	.release        = kvm_vcpu_release,
	.unlocked_ioctl = kvm_vcpu_ioctl,
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_KVM_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.compat_ioctl   = kvm_vcpu_compat_ioctl,
#endif
	.mmap           = kvm_vcpu_mmap,
	.llseek		= noop_llseek,
};

/*
 * Allocates an inode for the vcpu.
 */
static int create_vcpu_fd(struct kvm_vcpu *vcpu)
{
	return anon_inode_getfd("kvm-vcpu", &kvm_vcpu_fops, vcpu, O_RDWR | O_CLOEXEC);
}

<<<<<<< HEAD
=======
static int kvm_create_vcpu_debugfs(struct kvm_vcpu *vcpu)
{
	char dir_name[ITOA_MAX_LEN * 2];
	int ret;

	if (!kvm_arch_has_vcpu_debugfs())
		return 0;

	if (!debugfs_initialized())
		return 0;

	snprintf(dir_name, sizeof(dir_name), "vcpu%d", vcpu->vcpu_id);
	vcpu->debugfs_dentry = debugfs_create_dir(dir_name,
								vcpu->kvm->debugfs_dentry);
	if (!vcpu->debugfs_dentry)
		return -ENOMEM;

	ret = kvm_arch_create_vcpu_debugfs(vcpu);
	if (ret < 0) {
		debugfs_remove_recursive(vcpu->debugfs_dentry);
		return ret;
	}

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Creates some virtual cpus.  Good luck creating more than one.
 */
static int kvm_vm_ioctl_create_vcpu(struct kvm *kvm, u32 id)
{
	int r;
<<<<<<< HEAD
	struct kvm_vcpu *vcpu, *v;

	if (id >= KVM_MAX_VCPUS)
		return -EINVAL;

	vcpu = kvm_arch_vcpu_create(kvm, id);
	if (IS_ERR(vcpu))
		return PTR_ERR(vcpu);
=======
	struct kvm_vcpu *vcpu;

	if (id >= KVM_MAX_VCPU_ID)
		return -EINVAL;

	mutex_lock(&kvm->lock);
	if (kvm->created_vcpus == KVM_MAX_VCPUS) {
		mutex_unlock(&kvm->lock);
		return -EINVAL;
	}

	kvm->created_vcpus++;
	mutex_unlock(&kvm->lock);

	vcpu = kvm_arch_vcpu_create(kvm, id);
	if (IS_ERR(vcpu)) {
		r = PTR_ERR(vcpu);
		goto vcpu_decrement;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	preempt_notifier_init(&vcpu->preempt_notifier, &kvm_preempt_ops);

	r = kvm_arch_vcpu_setup(vcpu);
	if (r)
		goto vcpu_destroy;

<<<<<<< HEAD
	mutex_lock(&kvm->lock);
	if (!kvm_vcpu_compatible(vcpu)) {
		r = -EINVAL;
		goto unlock_vcpu_destroy;
	}
	if (atomic_read(&kvm->online_vcpus) == KVM_MAX_VCPUS) {
		r = -EINVAL;
		goto unlock_vcpu_destroy;
	}

	kvm_for_each_vcpu(r, v, kvm)
		if (v->vcpu_id == id) {
			r = -EEXIST;
			goto unlock_vcpu_destroy;
		}

=======
	r = kvm_create_vcpu_debugfs(vcpu);
	if (r)
		goto vcpu_destroy;

	mutex_lock(&kvm->lock);
	if (kvm_get_vcpu_by_id(kvm, id)) {
		r = -EEXIST;
		goto unlock_vcpu_destroy;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	BUG_ON(kvm->vcpus[atomic_read(&kvm->online_vcpus)]);

	/* Now it's all set up, let userspace reach it */
	kvm_get_kvm(kvm);
	r = create_vcpu_fd(vcpu);
	if (r < 0) {
		kvm_put_kvm(kvm);
		goto unlock_vcpu_destroy;
	}

	kvm->vcpus[atomic_read(&kvm->online_vcpus)] = vcpu;
<<<<<<< HEAD
=======

	/*
	 * Pairs with smp_rmb() in kvm_get_vcpu.  Write kvm->vcpus
	 * before kvm->online_vcpu's incremented value.
	 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	smp_wmb();
	atomic_inc(&kvm->online_vcpus);

	mutex_unlock(&kvm->lock);
	kvm_arch_vcpu_postcreate(vcpu);
	return r;

unlock_vcpu_destroy:
	mutex_unlock(&kvm->lock);
<<<<<<< HEAD
vcpu_destroy:
	kvm_arch_vcpu_destroy(vcpu);
=======
	debugfs_remove_recursive(vcpu->debugfs_dentry);
vcpu_destroy:
	kvm_arch_vcpu_destroy(vcpu);
vcpu_decrement:
	mutex_lock(&kvm->lock);
	kvm->created_vcpus--;
	mutex_unlock(&kvm->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return r;
}

static int kvm_vcpu_ioctl_set_sigmask(struct kvm_vcpu *vcpu, sigset_t *sigset)
{
	if (sigset) {
		sigdelsetmask(sigset, sigmask(SIGKILL)|sigmask(SIGSTOP));
		vcpu->sigset_active = 1;
		vcpu->sigset = *sigset;
	} else
		vcpu->sigset_active = 0;
	return 0;
}

static long kvm_vcpu_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r;
	struct kvm_fpu *fpu = NULL;
	struct kvm_sregs *kvm_sregs = NULL;

	if (vcpu->kvm->mm != current->mm)
		return -EIO;

	if (unlikely(_IOC_TYPE(ioctl) != KVMIO))
		return -EINVAL;

#if defined(CONFIG_S390) || defined(CONFIG_PPC) || defined(CONFIG_MIPS)
	/*
	 * Special cases: vcpu ioctls that are asynchronous to vcpu execution,
	 * so vcpu_load() would break it.
	 */
<<<<<<< HEAD
	if (ioctl == KVM_S390_INTERRUPT || ioctl == KVM_INTERRUPT)
=======
	if (ioctl == KVM_S390_INTERRUPT || ioctl == KVM_S390_IRQ || ioctl == KVM_INTERRUPT)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return kvm_arch_vcpu_ioctl(filp, ioctl, arg);
#endif


	r = vcpu_load(vcpu);
	if (r)
		return r;
	switch (ioctl) {
	case KVM_RUN:
		r = -EINVAL;
		if (arg)
			goto out;
<<<<<<< HEAD
=======
		if (unlikely(vcpu->pid != current->pids[PIDTYPE_PID].pid)) {
			/* The thread running this VCPU changed. */
			struct pid *oldpid = vcpu->pid;
			struct pid *newpid = get_task_pid(current, PIDTYPE_PID);

			rcu_assign_pointer(vcpu->pid, newpid);
			if (oldpid)
				synchronize_rcu();
			put_pid(oldpid);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		r = kvm_arch_vcpu_ioctl_run(vcpu, vcpu->run);
		trace_kvm_userspace_exit(vcpu->run->exit_reason, r);
		break;
	case KVM_GET_REGS: {
		struct kvm_regs *kvm_regs;

		r = -ENOMEM;
		kvm_regs = kzalloc(sizeof(struct kvm_regs), GFP_KERNEL);
		if (!kvm_regs)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_regs(vcpu, kvm_regs);
		if (r)
			goto out_free1;
		r = -EFAULT;
		if (copy_to_user(argp, kvm_regs, sizeof(struct kvm_regs)))
			goto out_free1;
		r = 0;
out_free1:
		kfree(kvm_regs);
		break;
	}
	case KVM_SET_REGS: {
		struct kvm_regs *kvm_regs;

		r = -ENOMEM;
		kvm_regs = memdup_user(argp, sizeof(*kvm_regs));
		if (IS_ERR(kvm_regs)) {
			r = PTR_ERR(kvm_regs);
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_regs(vcpu, kvm_regs);
		kfree(kvm_regs);
		break;
	}
	case KVM_GET_SREGS: {
		kvm_sregs = kzalloc(sizeof(struct kvm_sregs), GFP_KERNEL);
		r = -ENOMEM;
		if (!kvm_sregs)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_sregs(vcpu, kvm_sregs);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, kvm_sregs, sizeof(struct kvm_sregs)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_SREGS: {
		kvm_sregs = memdup_user(argp, sizeof(*kvm_sregs));
		if (IS_ERR(kvm_sregs)) {
			r = PTR_ERR(kvm_sregs);
			kvm_sregs = NULL;
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_sregs(vcpu, kvm_sregs);
		break;
	}
	case KVM_GET_MP_STATE: {
		struct kvm_mp_state mp_state;

		r = kvm_arch_vcpu_ioctl_get_mpstate(vcpu, &mp_state);
		if (r)
			goto out;
		r = -EFAULT;
<<<<<<< HEAD
		if (copy_to_user(argp, &mp_state, sizeof mp_state))
=======
		if (copy_to_user(argp, &mp_state, sizeof(mp_state)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_MP_STATE: {
		struct kvm_mp_state mp_state;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&mp_state, argp, sizeof mp_state))
=======
		if (copy_from_user(&mp_state, argp, sizeof(mp_state)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_arch_vcpu_ioctl_set_mpstate(vcpu, &mp_state);
		break;
	}
	case KVM_TRANSLATE: {
		struct kvm_translation tr;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&tr, argp, sizeof tr))
=======
		if (copy_from_user(&tr, argp, sizeof(tr)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_arch_vcpu_ioctl_translate(vcpu, &tr);
		if (r)
			goto out;
		r = -EFAULT;
<<<<<<< HEAD
		if (copy_to_user(argp, &tr, sizeof tr))
=======
		if (copy_to_user(argp, &tr, sizeof(tr)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_GUEST_DEBUG: {
		struct kvm_guest_debug dbg;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&dbg, argp, sizeof dbg))
=======
		if (copy_from_user(&dbg, argp, sizeof(dbg)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_arch_vcpu_ioctl_set_guest_debug(vcpu, &dbg);
		break;
	}
	case KVM_SET_SIGNAL_MASK: {
		struct kvm_signal_mask __user *sigmask_arg = argp;
		struct kvm_signal_mask kvm_sigmask;
		sigset_t sigset, *p;

		p = NULL;
		if (argp) {
			r = -EFAULT;
			if (copy_from_user(&kvm_sigmask, argp,
<<<<<<< HEAD
					   sizeof kvm_sigmask))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof sigset)
				goto out;
			r = -EFAULT;
			if (copy_from_user(&sigset, sigmask_arg->sigset,
					   sizeof sigset))
=======
					   sizeof(kvm_sigmask)))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof(sigset))
				goto out;
			r = -EFAULT;
			if (copy_from_user(&sigset, sigmask_arg->sigset,
					   sizeof(sigset)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto out;
			p = &sigset;
		}
		r = kvm_vcpu_ioctl_set_sigmask(vcpu, p);
		break;
	}
	case KVM_GET_FPU: {
		fpu = kzalloc(sizeof(struct kvm_fpu), GFP_KERNEL);
		r = -ENOMEM;
		if (!fpu)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_fpu(vcpu, fpu);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, fpu, sizeof(struct kvm_fpu)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_FPU: {
		fpu = memdup_user(argp, sizeof(*fpu));
		if (IS_ERR(fpu)) {
			r = PTR_ERR(fpu);
			fpu = NULL;
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_fpu(vcpu, fpu);
		break;
	}
	default:
		r = kvm_arch_vcpu_ioctl(filp, ioctl, arg);
	}
out:
	vcpu_put(vcpu);
	kfree(fpu);
	kfree(kvm_sregs);
	return r;
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_KVM_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static long kvm_vcpu_compat_ioctl(struct file *filp,
				  unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = compat_ptr(arg);
	int r;

	if (vcpu->kvm->mm != current->mm)
		return -EIO;

	switch (ioctl) {
	case KVM_SET_SIGNAL_MASK: {
		struct kvm_signal_mask __user *sigmask_arg = argp;
		struct kvm_signal_mask kvm_sigmask;
		compat_sigset_t csigset;
		sigset_t sigset;

		if (argp) {
			r = -EFAULT;
			if (copy_from_user(&kvm_sigmask, argp,
<<<<<<< HEAD
					   sizeof kvm_sigmask))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof csigset)
				goto out;
			r = -EFAULT;
			if (copy_from_user(&csigset, sigmask_arg->sigset,
					   sizeof csigset))
=======
					   sizeof(kvm_sigmask)))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof(csigset))
				goto out;
			r = -EFAULT;
			if (copy_from_user(&csigset, sigmask_arg->sigset,
					   sizeof(csigset)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto out;
			sigset_from_compat(&sigset, &csigset);
			r = kvm_vcpu_ioctl_set_sigmask(vcpu, &sigset);
		} else
			r = kvm_vcpu_ioctl_set_sigmask(vcpu, NULL);
		break;
	}
	default:
		r = kvm_vcpu_ioctl(filp, ioctl, arg);
	}

out:
	return r;
}
#endif

static int kvm_device_ioctl_attr(struct kvm_device *dev,
				 int (*accessor)(struct kvm_device *dev,
						 struct kvm_device_attr *attr),
				 unsigned long arg)
{
	struct kvm_device_attr attr;

	if (!accessor)
		return -EPERM;

	if (copy_from_user(&attr, (void __user *)arg, sizeof(attr)))
		return -EFAULT;

	return accessor(dev, &attr);
}

static long kvm_device_ioctl(struct file *filp, unsigned int ioctl,
			     unsigned long arg)
{
	struct kvm_device *dev = filp->private_data;

<<<<<<< HEAD
=======
	if (dev->kvm->mm != current->mm)
		return -EIO;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	switch (ioctl) {
	case KVM_SET_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->set_attr, arg);
	case KVM_GET_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->get_attr, arg);
	case KVM_HAS_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->has_attr, arg);
	default:
		if (dev->ops->ioctl)
			return dev->ops->ioctl(dev, ioctl, arg);

		return -ENOTTY;
	}
}

static int kvm_device_release(struct inode *inode, struct file *filp)
{
	struct kvm_device *dev = filp->private_data;
	struct kvm *kvm = dev->kvm;

	kvm_put_kvm(kvm);
	return 0;
}

static const struct file_operations kvm_device_fops = {
	.unlocked_ioctl = kvm_device_ioctl,
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_KVM_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.compat_ioctl = kvm_device_ioctl,
#endif
	.release = kvm_device_release,
};

struct kvm_device *kvm_device_from_filp(struct file *filp)
{
	if (filp->f_op != &kvm_device_fops)
		return NULL;

	return filp->private_data;
}

static struct kvm_device_ops *kvm_device_ops_table[KVM_DEV_TYPE_MAX] = {
#ifdef CONFIG_KVM_MPIC
	[KVM_DEV_TYPE_FSL_MPIC_20]	= &kvm_mpic_ops,
	[KVM_DEV_TYPE_FSL_MPIC_42]	= &kvm_mpic_ops,
#endif

#ifdef CONFIG_KVM_XICS
	[KVM_DEV_TYPE_XICS]		= &kvm_xics_ops,
#endif
};

int kvm_register_device_ops(struct kvm_device_ops *ops, u32 type)
{
	if (type >= ARRAY_SIZE(kvm_device_ops_table))
		return -ENOSPC;

	if (kvm_device_ops_table[type] != NULL)
		return -EEXIST;

	kvm_device_ops_table[type] = ops;
	return 0;
}

<<<<<<< HEAD
=======
void kvm_unregister_device_ops(u32 type)
{
	if (kvm_device_ops_table[type] != NULL)
		kvm_device_ops_table[type] = NULL;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int kvm_ioctl_create_device(struct kvm *kvm,
				   struct kvm_create_device *cd)
{
	struct kvm_device_ops *ops = NULL;
	struct kvm_device *dev;
	bool test = cd->flags & KVM_CREATE_DEVICE_TEST;
	int ret;

	if (cd->type >= ARRAY_SIZE(kvm_device_ops_table))
		return -ENODEV;

	ops = kvm_device_ops_table[cd->type];
	if (ops == NULL)
		return -ENODEV;

	if (test)
		return 0;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->ops = ops;
	dev->kvm = kvm;

<<<<<<< HEAD
	ret = ops->create(dev, cd->type);
	if (ret < 0) {
		kfree(dev);
		return ret;
	}

	ret = anon_inode_getfd(ops->name, &kvm_device_fops, dev, O_RDWR | O_CLOEXEC);
	if (ret < 0) {
=======
	mutex_lock(&kvm->lock);
	ret = ops->create(dev, cd->type);
	if (ret < 0) {
		mutex_unlock(&kvm->lock);
		kfree(dev);
		return ret;
	}
	list_add(&dev->vm_node, &kvm->devices);
	mutex_unlock(&kvm->lock);

	if (ops->init)
		ops->init(dev);

	kvm_get_kvm(kvm);
	ret = anon_inode_getfd(ops->name, &kvm_device_fops, dev, O_RDWR | O_CLOEXEC);
	if (ret < 0) {
		kvm_put_kvm(kvm);
		mutex_lock(&kvm->lock);
		list_del(&dev->vm_node);
		mutex_unlock(&kvm->lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ops->destroy(dev);
		return ret;
	}

<<<<<<< HEAD
	list_add(&dev->vm_node, &kvm->devices);
	kvm_get_kvm(kvm);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cd->fd = ret;
	return 0;
}

static long kvm_vm_ioctl_check_extension_generic(struct kvm *kvm, long arg)
{
	switch (arg) {
	case KVM_CAP_USER_MEMORY:
	case KVM_CAP_DESTROY_MEMORY_REGION_WORKS:
	case KVM_CAP_JOIN_MEMORY_REGIONS_WORKS:
<<<<<<< HEAD
#ifdef CONFIG_KVM_APIC_ARCHITECTURE
	case KVM_CAP_SET_BOOT_CPU_ID:
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case KVM_CAP_INTERNAL_ERROR_DATA:
#ifdef CONFIG_HAVE_KVM_MSI
	case KVM_CAP_SIGNAL_MSI:
#endif
#ifdef CONFIG_HAVE_KVM_IRQFD
<<<<<<< HEAD
	case KVM_CAP_IRQFD_RESAMPLE:
#endif
=======
	case KVM_CAP_IRQFD:
	case KVM_CAP_IRQFD_RESAMPLE:
#endif
	case KVM_CAP_IOEVENTFD_ANY_LENGTH:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case KVM_CAP_CHECK_EXTENSION_VM:
		return 1;
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
	case KVM_CAP_IRQ_ROUTING:
		return KVM_MAX_IRQ_ROUTES;
#endif
<<<<<<< HEAD
=======
#if KVM_ADDRESS_SPACE_NUM > 1
	case KVM_CAP_MULTI_ADDRESS_SPACE:
		return KVM_ADDRESS_SPACE_NUM;
#endif
	case KVM_CAP_MAX_VCPU_ID:
		return KVM_MAX_VCPU_ID;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		break;
	}
	return kvm_vm_ioctl_check_extension(kvm, arg);
}

static long kvm_vm_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r;

	if (kvm->mm != current->mm)
		return -EIO;
	switch (ioctl) {
	case KVM_CREATE_VCPU:
		r = kvm_vm_ioctl_create_vcpu(kvm, arg);
		break;
	case KVM_SET_USER_MEMORY_REGION: {
		struct kvm_userspace_memory_region kvm_userspace_mem;

		r = -EFAULT;
		if (copy_from_user(&kvm_userspace_mem, argp,
<<<<<<< HEAD
						sizeof kvm_userspace_mem))
=======
						sizeof(kvm_userspace_mem)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;

		r = kvm_vm_ioctl_set_memory_region(kvm, &kvm_userspace_mem);
		break;
	}
	case KVM_GET_DIRTY_LOG: {
		struct kvm_dirty_log log;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&log, argp, sizeof log))
=======
		if (copy_from_user(&log, argp, sizeof(log)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_vm_ioctl_get_dirty_log(kvm, &log);
		break;
	}
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	case KVM_REGISTER_COALESCED_MMIO: {
		struct kvm_coalesced_mmio_zone zone;
<<<<<<< HEAD
		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof zone))
=======

		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof(zone)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_vm_ioctl_register_coalesced_mmio(kvm, &zone);
		break;
	}
	case KVM_UNREGISTER_COALESCED_MMIO: {
		struct kvm_coalesced_mmio_zone zone;
<<<<<<< HEAD
		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof zone))
=======

		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof(zone)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_vm_ioctl_unregister_coalesced_mmio(kvm, &zone);
		break;
	}
#endif
	case KVM_IRQFD: {
		struct kvm_irqfd data;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&data, argp, sizeof data))
=======
		if (copy_from_user(&data, argp, sizeof(data)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_irqfd(kvm, &data);
		break;
	}
	case KVM_IOEVENTFD: {
		struct kvm_ioeventfd data;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&data, argp, sizeof data))
=======
		if (copy_from_user(&data, argp, sizeof(data)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_ioeventfd(kvm, &data);
		break;
	}
<<<<<<< HEAD
#ifdef CONFIG_KVM_APIC_ARCHITECTURE
	case KVM_SET_BOOT_CPU_ID:
		r = 0;
		mutex_lock(&kvm->lock);
		if (atomic_read(&kvm->online_vcpus) != 0)
			r = -EBUSY;
		else
			kvm->bsp_vcpu_id = arg;
		mutex_unlock(&kvm->lock);
		break;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_HAVE_KVM_MSI
	case KVM_SIGNAL_MSI: {
		struct kvm_msi msi;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&msi, argp, sizeof msi))
=======
		if (copy_from_user(&msi, argp, sizeof(msi)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;
		r = kvm_send_userspace_msi(kvm, &msi);
		break;
	}
#endif
#ifdef __KVM_HAVE_IRQ_LINE
	case KVM_IRQ_LINE_STATUS:
	case KVM_IRQ_LINE: {
		struct kvm_irq_level irq_event;

		r = -EFAULT;
<<<<<<< HEAD
		if (copy_from_user(&irq_event, argp, sizeof irq_event))
=======
		if (copy_from_user(&irq_event, argp, sizeof(irq_event)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out;

		r = kvm_vm_ioctl_irq_line(kvm, &irq_event,
					ioctl == KVM_IRQ_LINE_STATUS);
		if (r)
			goto out;

		r = -EFAULT;
		if (ioctl == KVM_IRQ_LINE_STATUS) {
<<<<<<< HEAD
			if (copy_to_user(argp, &irq_event, sizeof irq_event))
=======
			if (copy_to_user(argp, &irq_event, sizeof(irq_event)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto out;
		}

		r = 0;
		break;
	}
#endif
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
	case KVM_SET_GSI_ROUTING: {
		struct kvm_irq_routing routing;
		struct kvm_irq_routing __user *urouting;
<<<<<<< HEAD
		struct kvm_irq_routing_entry *entries;
=======
		struct kvm_irq_routing_entry *entries = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		r = -EFAULT;
		if (copy_from_user(&routing, argp, sizeof(routing)))
			goto out;
		r = -EINVAL;
		if (routing.nr > KVM_MAX_IRQ_ROUTES)
			goto out;
		if (routing.flags)
			goto out;
<<<<<<< HEAD
		r = -ENOMEM;
		entries = vmalloc(routing.nr * sizeof(*entries));
		if (!entries)
			goto out;
		r = -EFAULT;
		urouting = argp;
		if (copy_from_user(entries, urouting->entries,
				   routing.nr * sizeof(*entries)))
			goto out_free_irq_routing;
		r = kvm_set_irq_routing(kvm, entries, routing.nr,
					routing.flags);
	out_free_irq_routing:
=======
		if (routing.nr) {
			r = -ENOMEM;
			entries = vmalloc(routing.nr * sizeof(*entries));
			if (!entries)
				goto out;
			r = -EFAULT;
			urouting = argp;
			if (copy_from_user(entries, urouting->entries,
					   routing.nr * sizeof(*entries)))
				goto out_free_irq_routing;
		}
		r = kvm_set_irq_routing(kvm, entries, routing.nr,
					routing.flags);
out_free_irq_routing:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		vfree(entries);
		break;
	}
#endif /* CONFIG_HAVE_KVM_IRQ_ROUTING */
	case KVM_CREATE_DEVICE: {
		struct kvm_create_device cd;

		r = -EFAULT;
		if (copy_from_user(&cd, argp, sizeof(cd)))
			goto out;

		r = kvm_ioctl_create_device(kvm, &cd);
		if (r)
			goto out;

		r = -EFAULT;
		if (copy_to_user(argp, &cd, sizeof(cd)))
			goto out;

		r = 0;
		break;
	}
	case KVM_CHECK_EXTENSION:
		r = kvm_vm_ioctl_check_extension_generic(kvm, arg);
		break;
	default:
		r = kvm_arch_vm_ioctl(filp, ioctl, arg);
<<<<<<< HEAD
		if (r == -ENOTTY)
			r = kvm_vm_ioctl_assigned_device(kvm, ioctl, arg);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
out:
	return r;
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_KVM_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct compat_kvm_dirty_log {
	__u32 slot;
	__u32 padding1;
	union {
		compat_uptr_t dirty_bitmap; /* one bit per page */
		__u64 padding2;
	};
};

static long kvm_vm_compat_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	int r;

	if (kvm->mm != current->mm)
		return -EIO;
	switch (ioctl) {
	case KVM_GET_DIRTY_LOG: {
		struct compat_kvm_dirty_log compat_log;
		struct kvm_dirty_log log;

		r = -EFAULT;
		if (copy_from_user(&compat_log, (void __user *)arg,
				   sizeof(compat_log)))
			goto out;
		log.slot	 = compat_log.slot;
		log.padding1	 = compat_log.padding1;
		log.padding2	 = compat_log.padding2;
		log.dirty_bitmap = compat_ptr(compat_log.dirty_bitmap);

		r = kvm_vm_ioctl_get_dirty_log(kvm, &log);
		break;
	}
	default:
		r = kvm_vm_ioctl(filp, ioctl, arg);
	}

out:
	return r;
}
#endif

static struct file_operations kvm_vm_fops = {
	.release        = kvm_vm_release,
	.unlocked_ioctl = kvm_vm_ioctl,
<<<<<<< HEAD
#ifdef CONFIG_COMPAT
=======
#ifdef CONFIG_KVM_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.compat_ioctl   = kvm_vm_compat_ioctl,
#endif
	.llseek		= noop_llseek,
};

static int kvm_dev_ioctl_create_vm(unsigned long type)
{
	int r;
	struct kvm *kvm;
<<<<<<< HEAD
=======
	struct file *file;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	kvm = kvm_create_vm(type);
	if (IS_ERR(kvm))
		return PTR_ERR(kvm);
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	r = kvm_coalesced_mmio_init(kvm);
	if (r < 0) {
		kvm_put_kvm(kvm);
		return r;
	}
#endif
<<<<<<< HEAD
	r = anon_inode_getfd("kvm-vm", &kvm_vm_fops, kvm, O_RDWR | O_CLOEXEC);
	if (r < 0)
		kvm_put_kvm(kvm);

=======
	r = get_unused_fd_flags(O_CLOEXEC);
	if (r < 0) {
		kvm_put_kvm(kvm);
		return r;
	}
	file = anon_inode_getfile("kvm-vm", &kvm_vm_fops, kvm, O_RDWR);
	if (IS_ERR(file)) {
		put_unused_fd(r);
		kvm_put_kvm(kvm);
		return PTR_ERR(file);
	}

	if (kvm_create_vm_debugfs(kvm, r) < 0) {
		put_unused_fd(r);
		fput(file);
		return -ENOMEM;
	}

	fd_install(r, file);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return r;
}

static long kvm_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case KVM_GET_API_VERSION:
		if (arg)
			goto out;
		r = KVM_API_VERSION;
		break;
	case KVM_CREATE_VM:
		r = kvm_dev_ioctl_create_vm(arg);
		break;
	case KVM_CHECK_EXTENSION:
		r = kvm_vm_ioctl_check_extension_generic(NULL, arg);
		break;
	case KVM_GET_VCPU_MMAP_SIZE:
		if (arg)
			goto out;
		r = PAGE_SIZE;     /* struct kvm_run */
#ifdef CONFIG_X86
		r += PAGE_SIZE;    /* pio data page */
#endif
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
		r += PAGE_SIZE;    /* coalesced mmio ring page */
#endif
		break;
	case KVM_TRACE_ENABLE:
	case KVM_TRACE_PAUSE:
	case KVM_TRACE_DISABLE:
		r = -EOPNOTSUPP;
		break;
	default:
		return kvm_arch_dev_ioctl(filp, ioctl, arg);
	}
out:
	return r;
}

static struct file_operations kvm_chardev_ops = {
	.unlocked_ioctl = kvm_dev_ioctl,
	.compat_ioctl   = kvm_dev_ioctl,
	.llseek		= noop_llseek,
};

static struct miscdevice kvm_dev = {
	KVM_MINOR,
	"kvm",
	&kvm_chardev_ops,
};

static void hardware_enable_nolock(void *junk)
{
	int cpu = raw_smp_processor_id();
	int r;

	if (cpumask_test_cpu(cpu, cpus_hardware_enabled))
		return;

	cpumask_set_cpu(cpu, cpus_hardware_enabled);

	r = kvm_arch_hardware_enable();

	if (r) {
		cpumask_clear_cpu(cpu, cpus_hardware_enabled);
		atomic_inc(&hardware_enable_failed);
<<<<<<< HEAD
		printk(KERN_INFO "kvm: enabling virtualization on "
				 "CPU%d failed\n", cpu);
	}
}

static void hardware_enable(void)
=======
		pr_info("kvm: enabling virtualization on CPU%d failed\n", cpu);
	}
}

static int kvm_starting_cpu(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	raw_spin_lock(&kvm_count_lock);
	if (kvm_usage_count)
		hardware_enable_nolock(NULL);
	raw_spin_unlock(&kvm_count_lock);
<<<<<<< HEAD
=======
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void hardware_disable_nolock(void *junk)
{
	int cpu = raw_smp_processor_id();

	if (!cpumask_test_cpu(cpu, cpus_hardware_enabled))
		return;
	cpumask_clear_cpu(cpu, cpus_hardware_enabled);
	kvm_arch_hardware_disable();
}

<<<<<<< HEAD
static void hardware_disable(void)
=======
static int kvm_dying_cpu(unsigned int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	raw_spin_lock(&kvm_count_lock);
	if (kvm_usage_count)
		hardware_disable_nolock(NULL);
	raw_spin_unlock(&kvm_count_lock);
<<<<<<< HEAD
=======
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void hardware_disable_all_nolock(void)
{
	BUG_ON(!kvm_usage_count);

	kvm_usage_count--;
	if (!kvm_usage_count)
		on_each_cpu(hardware_disable_nolock, NULL, 1);
}

static void hardware_disable_all(void)
{
	raw_spin_lock(&kvm_count_lock);
	hardware_disable_all_nolock();
	raw_spin_unlock(&kvm_count_lock);
}

static int hardware_enable_all(void)
{
	int r = 0;

	raw_spin_lock(&kvm_count_lock);

	kvm_usage_count++;
	if (kvm_usage_count == 1) {
		atomic_set(&hardware_enable_failed, 0);
		on_each_cpu(hardware_enable_nolock, NULL, 1);

		if (atomic_read(&hardware_enable_failed)) {
			hardware_disable_all_nolock();
			r = -EBUSY;
		}
	}

	raw_spin_unlock(&kvm_count_lock);

	return r;
}

<<<<<<< HEAD
static int kvm_cpu_hotplug(struct notifier_block *notifier, unsigned long val,
			   void *v)
{
	int cpu = (long)v;

	val &= ~CPU_TASKS_FROZEN;
	switch (val) {
	case CPU_DYING:
		printk(KERN_INFO "kvm: disabling virtualization on CPU%d\n",
		       cpu);
		hardware_disable();
		break;
	case CPU_STARTING:
		printk(KERN_INFO "kvm: enabling virtualization on CPU%d\n",
		       cpu);
		hardware_enable();
		break;
	}
	return NOTIFY_OK;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int kvm_reboot(struct notifier_block *notifier, unsigned long val,
		      void *v)
{
	/*
	 * Some (well, at least mine) BIOSes hang on reboot if
	 * in vmx root mode.
	 *
	 * And Intel TXT required VMX off for all cpu when system shutdown.
	 */
<<<<<<< HEAD
	printk(KERN_INFO "kvm: exiting hardware virtualization\n");
=======
	pr_info("kvm: exiting hardware virtualization\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kvm_rebooting = true;
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	return NOTIFY_OK;
}

static struct notifier_block kvm_reboot_notifier = {
	.notifier_call = kvm_reboot,
	.priority = 0,
};

static void kvm_io_bus_destroy(struct kvm_io_bus *bus)
{
	int i;

	for (i = 0; i < bus->dev_count; i++) {
		struct kvm_io_device *pos = bus->range[i].dev;

		kvm_iodevice_destructor(pos);
	}
	kfree(bus);
}

<<<<<<< HEAD
static int kvm_io_bus_sort_cmp(const void *p1, const void *p2)
{
	const struct kvm_io_range *r1 = p1;
	const struct kvm_io_range *r2 = p2;

	if (r1->addr < r2->addr)
		return -1;
	if (r1->addr + r1->len > r2->addr + r2->len)
		return 1;
	return 0;
}

=======
static inline int kvm_io_bus_cmp(const struct kvm_io_range *r1,
				 const struct kvm_io_range *r2)
{
	gpa_t addr1 = r1->addr;
	gpa_t addr2 = r2->addr;

	if (addr1 < addr2)
		return -1;

	/* If r2->len == 0, match the exact address.  If r2->len != 0,
	 * accept any overlapping write.  Any order is acceptable for
	 * overlapping ranges, because kvm_io_bus_get_first_dev ensures
	 * we process all of them.
	 */
	if (r2->len) {
		addr1 += r1->len;
		addr2 += r2->len;
	}

	if (addr1 > addr2)
		return 1;

	return 0;
}

static int kvm_io_bus_sort_cmp(const void *p1, const void *p2)
{
	return kvm_io_bus_cmp(p1, p2);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int kvm_io_bus_insert_dev(struct kvm_io_bus *bus, struct kvm_io_device *dev,
			  gpa_t addr, int len)
{
	bus->range[bus->dev_count++] = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
		.dev = dev,
	};

	sort(bus->range, bus->dev_count, sizeof(struct kvm_io_range),
		kvm_io_bus_sort_cmp, NULL);

	return 0;
}

static int kvm_io_bus_get_first_dev(struct kvm_io_bus *bus,
			     gpa_t addr, int len)
{
	struct kvm_io_range *range, key;
	int off;

	key = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	range = bsearch(&key, bus->range, bus->dev_count,
			sizeof(struct kvm_io_range), kvm_io_bus_sort_cmp);
	if (range == NULL)
		return -ENOENT;

	off = range - bus->range;

<<<<<<< HEAD
	while (off > 0 && kvm_io_bus_sort_cmp(&key, &bus->range[off-1]) == 0)
=======
	while (off > 0 && kvm_io_bus_cmp(&key, &bus->range[off-1]) == 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		off--;

	return off;
}

<<<<<<< HEAD
/* kvm_io_bus_write - called under kvm->slots_lock */
int kvm_io_bus_write(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
		     int len, const void *val)
{
	int idx;
	struct kvm_io_bus *bus;
	struct kvm_io_range range;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	idx = kvm_io_bus_get_first_dev(bus, addr, len);
=======
static int __kvm_io_bus_write(struct kvm_vcpu *vcpu, struct kvm_io_bus *bus,
			      struct kvm_io_range *range, const void *val)
{
	int idx;

	idx = kvm_io_bus_get_first_dev(bus, range->addr, range->len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
<<<<<<< HEAD
		kvm_io_bus_sort_cmp(&range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_write(bus->range[idx].dev, addr, len, val))
			return 0;
=======
		kvm_io_bus_cmp(range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_write(vcpu, bus->range[idx].dev, range->addr,
					range->len, val))
			return idx;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		idx++;
	}

	return -EOPNOTSUPP;
}

<<<<<<< HEAD
/* kvm_io_bus_read - called under kvm->slots_lock */
int kvm_io_bus_read(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
		    int len, void *val)
{
	int idx;
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
=======
/* kvm_io_bus_write - called under kvm->slots_lock */
int kvm_io_bus_write(struct kvm_vcpu *vcpu, enum kvm_bus bus_idx, gpa_t addr,
		     int len, const void *val)
{
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
	int r;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

<<<<<<< HEAD
	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	if (!bus)
		return -ENOMEM;
	if (!bus)
		return -ENOMEM;
	idx = kvm_io_bus_get_first_dev(bus, addr, len);
=======
	bus = srcu_dereference(vcpu->kvm->buses[bus_idx], &vcpu->kvm->srcu);
	if (!bus)
		return -ENOMEM;
	r = __kvm_io_bus_write(vcpu, bus, &range, val);
	return r < 0 ? r : 0;
}

/* kvm_io_bus_write_cookie - called under kvm->slots_lock */
int kvm_io_bus_write_cookie(struct kvm_vcpu *vcpu, enum kvm_bus bus_idx,
			    gpa_t addr, int len, const void *val, long cookie)
{
	struct kvm_io_bus *bus;
	struct kvm_io_range range;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(vcpu->kvm->buses[bus_idx], &vcpu->kvm->srcu);
	if (!bus)
		return -ENOMEM;

	/* First try the device referenced by cookie. */
	if ((cookie >= 0) && (cookie < bus->dev_count) &&
	    (kvm_io_bus_cmp(&range, &bus->range[cookie]) == 0))
		if (!kvm_iodevice_write(vcpu, bus->range[cookie].dev, addr, len,
					val))
			return cookie;

	/*
	 * cookie contained garbage; fall back to search and return the
	 * correct cookie value.
	 */
	return __kvm_io_bus_write(vcpu, bus, &range, val);
}

static int __kvm_io_bus_read(struct kvm_vcpu *vcpu, struct kvm_io_bus *bus,
			     struct kvm_io_range *range, void *val)
{
	int idx;

	idx = kvm_io_bus_get_first_dev(bus, range->addr, range->len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
<<<<<<< HEAD
		kvm_io_bus_sort_cmp(&range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_read(bus->range[idx].dev, addr, len, val))
			return 0;
=======
		kvm_io_bus_cmp(range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_read(vcpu, bus->range[idx].dev, range->addr,
				       range->len, val))
			return idx;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		idx++;
	}

	return -EOPNOTSUPP;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvm_io_bus_write);

/* kvm_io_bus_read - called under kvm->slots_lock */
int kvm_io_bus_read(struct kvm_vcpu *vcpu, enum kvm_bus bus_idx, gpa_t addr,
		    int len, void *val)
{
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
	int r;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(vcpu->kvm->buses[bus_idx], &vcpu->kvm->srcu);
	if (!bus)
		return -ENOMEM;
	r = __kvm_io_bus_read(vcpu, bus, &range, val);
	return r < 0 ? r : 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Caller must hold slots_lock. */
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev)
{
	struct kvm_io_bus *new_bus, *bus;

	bus = kvm->buses[bus_idx];
	if (!bus)
		return -ENOMEM;

	/* exclude ioeventfd which is limited by maximum fd */
	if (bus->dev_count - bus->ioeventfd_count > NR_IOBUS_DEVS - 1)
		return -ENOSPC;

<<<<<<< HEAD
	new_bus = kzalloc(sizeof(*bus) + ((bus->dev_count + 1) *
=======
	new_bus = kmalloc(sizeof(*bus) + ((bus->dev_count + 1) *
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;
	memcpy(new_bus, bus, sizeof(*bus) + (bus->dev_count *
	       sizeof(struct kvm_io_range)));
	kvm_io_bus_insert_dev(new_bus, dev, addr, len);
	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);

	return 0;
}

/* Caller must hold slots_lock. */
void kvm_io_bus_unregister_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			       struct kvm_io_device *dev)
{
<<<<<<< HEAD
	int i;
=======
	int i, j;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct kvm_io_bus *new_bus, *bus;

	bus = kvm->buses[bus_idx];
	if (!bus)
		return;

	for (i = 0; i < bus->dev_count; i++)
		if (bus->range[i].dev == dev) {
			break;
		}

	if (i == bus->dev_count)
		return;

<<<<<<< HEAD
	new_bus = kzalloc(sizeof(*bus) + ((bus->dev_count - 1) *
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
	if (!new_bus)  {
		pr_err("kvm: failed to shrink bus, removing it completely\n");
		goto broken;
	}

	memcpy(new_bus, bus, sizeof(*bus) + i * sizeof(struct kvm_io_range));
	new_bus->dev_count--;
	memcpy(new_bus->range + i, bus->range + i + 1,
	       (new_bus->dev_count - i) * sizeof(struct kvm_io_range));

broken:
=======
	new_bus = kmalloc(sizeof(*bus) + ((bus->dev_count - 1) *
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
	if (new_bus) {
		memcpy(new_bus, bus, sizeof(*bus) + i * sizeof(struct kvm_io_range));
		new_bus->dev_count--;
		memcpy(new_bus->range + i, bus->range + i + 1,
		       (new_bus->dev_count - i) * sizeof(struct kvm_io_range));
	} else {
		pr_err("kvm: failed to shrink bus, removing it completely\n");
		for (j = 0; j < bus->dev_count; j++) {
			if (j == i)
				continue;
			kvm_iodevice_destructor(bus->range[j].dev);
		}
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);
	return;
}

<<<<<<< HEAD
static struct notifier_block kvm_cpu_notifier = {
	.notifier_call = kvm_cpu_hotplug,
=======
struct kvm_io_device *kvm_io_bus_get_dev(struct kvm *kvm, enum kvm_bus bus_idx,
					 gpa_t addr)
{
	struct kvm_io_bus *bus;
	int dev_idx, srcu_idx;
	struct kvm_io_device *iodev = NULL;

	srcu_idx = srcu_read_lock(&kvm->srcu);

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	if (!bus)
		goto out_unlock;

	dev_idx = kvm_io_bus_get_first_dev(bus, addr, 1);
	if (dev_idx < 0)
		goto out_unlock;

	iodev = bus->range[dev_idx].dev;

out_unlock:
	srcu_read_unlock(&kvm->srcu, srcu_idx);

	return iodev;
}
EXPORT_SYMBOL_GPL(kvm_io_bus_get_dev);

static int kvm_debugfs_open(struct inode *inode, struct file *file,
			   int (*get)(void *, u64 *), int (*set)(void *, u64),
			   const char *fmt)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)
					  inode->i_private;

	/* The debugfs files are a reference to the kvm struct which
	 * is still valid when kvm_destroy_vm is called.
	 * To avoid the race between open and the removal of the debugfs
	 * directory we test against the users count.
	 */
	if (!atomic_add_unless(&stat_data->kvm->users_count, 1, 0))
		return -ENOENT;

	if (simple_attr_open(inode, file, get, set, fmt)) {
		kvm_put_kvm(stat_data->kvm);
		return -ENOMEM;
	}

	return 0;
}

static int kvm_debugfs_release(struct inode *inode, struct file *file)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)
					  inode->i_private;

	simple_attr_release(inode, file);
	kvm_put_kvm(stat_data->kvm);

	return 0;
}

static int vm_stat_get_per_vm(void *data, u64 *val)
{
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;

	*val = *(ulong *)((void *)stat_data->kvm + stat_data->offset);

	return 0;
}

static int vm_stat_get_per_vm_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, vm_stat_get_per_vm,
				NULL, "%llu\n");
}

static const struct file_operations vm_stat_get_per_vm_fops = {
	.owner   = THIS_MODULE,
	.open    = vm_stat_get_per_vm_open,
	.release = kvm_debugfs_release,
	.read    = simple_attr_read,
	.write   = simple_attr_write,
	.llseek  = generic_file_llseek,
};

static int vcpu_stat_get_per_vm(void *data, u64 *val)
{
	int i;
	struct kvm_stat_data *stat_data = (struct kvm_stat_data *)data;
	struct kvm_vcpu *vcpu;

	*val = 0;

	kvm_for_each_vcpu(i, vcpu, stat_data->kvm)
		*val += *(u64 *)((void *)vcpu + stat_data->offset);

	return 0;
}

static int vcpu_stat_get_per_vm_open(struct inode *inode, struct file *file)
{
	__simple_attr_check_format("%llu\n", 0ull);
	return kvm_debugfs_open(inode, file, vcpu_stat_get_per_vm,
				 NULL, "%llu\n");
}

static const struct file_operations vcpu_stat_get_per_vm_fops = {
	.owner   = THIS_MODULE,
	.open    = vcpu_stat_get_per_vm_open,
	.release = kvm_debugfs_release,
	.read    = simple_attr_read,
	.write   = simple_attr_write,
	.llseek  = generic_file_llseek,
};

static const struct file_operations *stat_fops_per_vm[] = {
	[KVM_STAT_VCPU] = &vcpu_stat_get_per_vm_fops,
	[KVM_STAT_VM]   = &vm_stat_get_per_vm_fops,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int vm_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
<<<<<<< HEAD

	*val = 0;
	spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list)
		*val += *(u32 *)((void *)kvm + offset);
	spin_unlock(&kvm_lock);
=======
	struct kvm_stat_data stat_tmp = {.offset = offset};
	u64 tmp_val;

	*val = 0;
	mutex_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
		stat_tmp.kvm = kvm;
		vm_stat_get_per_vm((void *)&stat_tmp, &tmp_val);
		*val += tmp_val;
	}
	mutex_unlock(&kvm_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vm_stat_fops, vm_stat_get, NULL, "%llu\n");

static int vcpu_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
<<<<<<< HEAD
	struct kvm_vcpu *vcpu;
	int i;

	*val = 0;
	spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list)
		kvm_for_each_vcpu(i, vcpu, kvm)
			*val += *(u32 *)((void *)vcpu + offset);

	spin_unlock(&kvm_lock);
=======
	struct kvm_stat_data stat_tmp = {.offset = offset};
	u64 tmp_val;

	*val = 0;
	mutex_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list) {
		stat_tmp.kvm = kvm;
		vcpu_stat_get_per_vm((void *)&stat_tmp, &tmp_val);
		*val += tmp_val;
	}
	mutex_unlock(&kvm_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vcpu_stat_fops, vcpu_stat_get, NULL, "%llu\n");

static const struct file_operations *stat_fops[] = {
	[KVM_STAT_VCPU] = &vcpu_stat_fops,
	[KVM_STAT_VM]   = &vm_stat_fops,
};

static int kvm_init_debug(void)
{
	int r = -EEXIST;
	struct kvm_stats_debugfs_item *p;

	kvm_debugfs_dir = debugfs_create_dir("kvm", NULL);
	if (kvm_debugfs_dir == NULL)
		goto out;

<<<<<<< HEAD
	for (p = debugfs_entries; p->name; ++p) {
		p->dentry = debugfs_create_file(p->name, 0444, kvm_debugfs_dir,
						(void *)(long)p->offset,
						stat_fops[p->kind]);
		if (p->dentry == NULL)
=======
	kvm_debugfs_num_entries = 0;
	for (p = debugfs_entries; p->name; ++p, kvm_debugfs_num_entries++) {
		if (!debugfs_create_file(p->name, 0444, kvm_debugfs_dir,
					 (void *)(long)p->offset,
					 stat_fops[p->kind]))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out_dir;
	}

	return 0;

out_dir:
	debugfs_remove_recursive(kvm_debugfs_dir);
out:
	return r;
}

<<<<<<< HEAD
static void kvm_exit_debug(void)
{
	struct kvm_stats_debugfs_item *p;

	for (p = debugfs_entries; p->name; ++p)
		debugfs_remove(p->dentry);
	debugfs_remove(kvm_debugfs_dir);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int kvm_suspend(void)
{
	if (kvm_usage_count)
		hardware_disable_nolock(NULL);
	return 0;
}

static void kvm_resume(void)
{
	if (kvm_usage_count) {
		WARN_ON(raw_spin_is_locked(&kvm_count_lock));
		hardware_enable_nolock(NULL);
	}
}

static struct syscore_ops kvm_syscore_ops = {
	.suspend = kvm_suspend,
	.resume = kvm_resume,
};

static inline
struct kvm_vcpu *preempt_notifier_to_vcpu(struct preempt_notifier *pn)
{
	return container_of(pn, struct kvm_vcpu, preempt_notifier);
}

static void kvm_sched_in(struct preempt_notifier *pn, int cpu)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (vcpu->preempted)
		vcpu->preempted = false;

	kvm_arch_sched_in(vcpu, cpu);

	kvm_arch_vcpu_load(vcpu, cpu);
}

static void kvm_sched_out(struct preempt_notifier *pn,
			  struct task_struct *next)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);

	if (current->state == TASK_RUNNING)
		vcpu->preempted = true;
	kvm_arch_vcpu_put(vcpu);
}

int kvm_init(void *opaque, unsigned vcpu_size, unsigned vcpu_align,
		  struct module *module)
{
	int r;
	int cpu;

	r = kvm_arch_init(opaque);
	if (r)
		goto out_fail;

	/*
	 * kvm_arch_init makes sure there's at most one caller
	 * for architectures that support multiple implementations,
	 * like intel and amd on x86.
	 * kvm_arch_init must be called before kvm_irqfd_init to avoid creating
	 * conflicts in case kvm is already setup for another implementation.
	 */
	r = kvm_irqfd_init();
	if (r)
		goto out_irqfd;

	if (!zalloc_cpumask_var(&cpus_hardware_enabled, GFP_KERNEL)) {
		r = -ENOMEM;
		goto out_free_0;
	}

	r = kvm_arch_hardware_setup();
	if (r < 0)
		goto out_free_0a;

	for_each_online_cpu(cpu) {
		smp_call_function_single(cpu,
				kvm_arch_check_processor_compat,
				&r, 1);
		if (r < 0)
			goto out_free_1;
	}

<<<<<<< HEAD
	r = register_cpu_notifier(&kvm_cpu_notifier);
=======
	r = cpuhp_setup_state_nocalls(CPUHP_AP_KVM_STARTING, "AP_KVM_STARTING",
				      kvm_starting_cpu, kvm_dying_cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (r)
		goto out_free_2;
	register_reboot_notifier(&kvm_reboot_notifier);

	/* A kmem cache lets us meet the alignment requirements of fx_save. */
	if (!vcpu_align)
		vcpu_align = __alignof__(struct kvm_vcpu);
	kvm_vcpu_cache = kmem_cache_create("kvm_vcpu", vcpu_size, vcpu_align,
<<<<<<< HEAD
					   0, NULL);
=======
					   SLAB_ACCOUNT, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!kvm_vcpu_cache) {
		r = -ENOMEM;
		goto out_free_3;
	}

	r = kvm_async_pf_init();
	if (r)
		goto out_free;

	kvm_chardev_ops.owner = module;
	kvm_vm_fops.owner = module;
	kvm_vcpu_fops.owner = module;

	r = misc_register(&kvm_dev);
	if (r) {
<<<<<<< HEAD
		printk(KERN_ERR "kvm: misc device register failed\n");
=======
		pr_err("kvm: misc device register failed\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out_unreg;
	}

	register_syscore_ops(&kvm_syscore_ops);

	kvm_preempt_ops.sched_in = kvm_sched_in;
	kvm_preempt_ops.sched_out = kvm_sched_out;

	r = kvm_init_debug();
	if (r) {
<<<<<<< HEAD
		printk(KERN_ERR "kvm: create debugfs files failed\n");
		goto out_undebugfs;
	}

=======
		pr_err("kvm: create debugfs files failed\n");
		goto out_undebugfs;
	}

	r = kvm_vfio_ops_init();
	WARN_ON(r);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;

out_undebugfs:
	unregister_syscore_ops(&kvm_syscore_ops);
	misc_deregister(&kvm_dev);
out_unreg:
	kvm_async_pf_deinit();
out_free:
	kmem_cache_destroy(kvm_vcpu_cache);
out_free_3:
	unregister_reboot_notifier(&kvm_reboot_notifier);
<<<<<<< HEAD
	unregister_cpu_notifier(&kvm_cpu_notifier);
=======
	cpuhp_remove_state_nocalls(CPUHP_AP_KVM_STARTING);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out_free_2:
out_free_1:
	kvm_arch_hardware_unsetup();
out_free_0a:
	free_cpumask_var(cpus_hardware_enabled);
out_free_0:
	kvm_irqfd_exit();
out_irqfd:
	kvm_arch_exit();
out_fail:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_init);

void kvm_exit(void)
{
<<<<<<< HEAD
	kvm_exit_debug();
=======
	debugfs_remove_recursive(kvm_debugfs_dir);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	misc_deregister(&kvm_dev);
	kmem_cache_destroy(kvm_vcpu_cache);
	kvm_async_pf_deinit();
	unregister_syscore_ops(&kvm_syscore_ops);
	unregister_reboot_notifier(&kvm_reboot_notifier);
<<<<<<< HEAD
	unregister_cpu_notifier(&kvm_cpu_notifier);
=======
	cpuhp_remove_state_nocalls(CPUHP_AP_KVM_STARTING);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	kvm_arch_hardware_unsetup();
	kvm_arch_exit();
	kvm_irqfd_exit();
	free_cpumask_var(cpus_hardware_enabled);
<<<<<<< HEAD
}
EXPORT_SYMBOL_GPL(kvm_exit);
=======
	kvm_vfio_ops_exit();
}
EXPORT_SYMBOL_GPL(kvm_exit);

struct kvm_vm_worker_thread_context {
	struct kvm *kvm;
	struct task_struct *parent;
	struct completion init_done;
	kvm_vm_thread_fn_t thread_fn;
	uintptr_t data;
	int err;
};

static int kvm_vm_worker_thread(void *context)
{
	/*
	 * The init_context is allocated on the stack of the parent thread, so
	 * we have to locally copy anything that is needed beyond initialization
	 */
	struct kvm_vm_worker_thread_context *init_context = context;
	struct kvm *kvm = init_context->kvm;
	kvm_vm_thread_fn_t thread_fn = init_context->thread_fn;
	uintptr_t data = init_context->data;
	int err;

	err = kthread_park(current);
	/* kthread_park(current) is never supposed to return an error */
	WARN_ON(err != 0);
	if (err)
		goto init_complete;

	err = cgroup_attach_task_all(init_context->parent, current);
	if (err) {
		kvm_err("%s: cgroup_attach_task_all failed with err %d\n",
			__func__, err);
		goto init_complete;
	}

	set_user_nice(current, task_nice(init_context->parent));

init_complete:
	init_context->err = err;
	complete(&init_context->init_done);
	init_context = NULL;

	if (err)
		return err;

	/* Wait to be woken up by the spawner before proceeding. */
	kthread_parkme();

	if (!kthread_should_stop())
		err = thread_fn(kvm, data);

	return err;
}

int kvm_vm_create_worker_thread(struct kvm *kvm, kvm_vm_thread_fn_t thread_fn,
				uintptr_t data, const char *name,
				struct task_struct **thread_ptr)
{
	struct kvm_vm_worker_thread_context init_context = {};
	struct task_struct *thread;

	*thread_ptr = NULL;
	init_context.kvm = kvm;
	init_context.parent = current;
	init_context.thread_fn = thread_fn;
	init_context.data = data;
	init_completion(&init_context.init_done);

	thread = kthread_run(kvm_vm_worker_thread, &init_context,
			     "%s-%d", name, task_pid_nr(current));
	if (IS_ERR(thread))
		return PTR_ERR(thread);

	/* kthread_run is never supposed to return NULL */
	WARN_ON(thread == NULL);

	wait_for_completion(&init_context.init_done);

	if (!init_context.err)
		*thread_ptr = thread;

	return init_context.err;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
