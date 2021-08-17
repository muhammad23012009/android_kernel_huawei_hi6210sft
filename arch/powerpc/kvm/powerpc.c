/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Copyright IBM Corp. 2007
 *
 * Authors: Hollis Blanchard <hollisb@us.ibm.com>
 *          Christian Ehrhardt <ehrhardt@linux.vnet.ibm.com>
 */

#include <linux/errno.h>
#include <linux/err.h>
#include <linux/kvm_host.h>
#include <linux/vmalloc.h>
#include <linux/hrtimer.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/file.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
#include <linux/irqbypass.h>
#include <linux/kvm_irqfd.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/cputable.h>
#include <asm/uaccess.h>
#include <asm/kvm_ppc.h>
#include <asm/tlbflush.h>
#include <asm/cputhreads.h>
#include <asm/irqflags.h>
<<<<<<< HEAD
=======
#include <asm/iommu.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "timing.h"
#include "irq.h"
#include "../mm/mmu_decl.h"

#define CREATE_TRACE_POINTS
#include "trace.h"

<<<<<<< HEAD
=======
struct kvmppc_ops *kvmppc_hv_ops;
EXPORT_SYMBOL_GPL(kvmppc_hv_ops);
struct kvmppc_ops *kvmppc_pr_ops;
EXPORT_SYMBOL_GPL(kvmppc_pr_ops);


>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int kvm_arch_vcpu_runnable(struct kvm_vcpu *v)
{
	return !!(v->arch.pending_exceptions) ||
	       v->requests;
}

int kvm_arch_vcpu_should_kick(struct kvm_vcpu *vcpu)
{
	return 1;
}

<<<<<<< HEAD
#ifndef CONFIG_KVM_BOOK3S_64_HV
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Common checks before entering the guest world.  Call with interrupts
 * disabled.
 *
 * returns:
 *
 * == 1 if we're ready to go into guest state
 * <= 0 if we need to go back to the host with return value
 */
int kvmppc_prepare_to_enter(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	int r = 1;

	WARN_ON_ONCE(!irqs_disabled());
=======
	int r;

	WARN_ON(irqs_disabled());
	hard_irq_disable();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	while (true) {
		if (need_resched()) {
			local_irq_enable();
			cond_resched();
<<<<<<< HEAD
			local_irq_disable();
=======
			hard_irq_disable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		}

		if (signal_pending(current)) {
			kvmppc_account_exit(vcpu, SIGNAL_EXITS);
			vcpu->run->exit_reason = KVM_EXIT_INTR;
			r = -EINTR;
			break;
		}

		vcpu->mode = IN_GUEST_MODE;

		/*
		 * Reading vcpu->requests must happen after setting vcpu->mode,
		 * so we don't miss a request because the requester sees
		 * OUTSIDE_GUEST_MODE and assumes we'll be checking requests
		 * before next entering the guest (and thus doesn't IPI).
<<<<<<< HEAD
=======
		 * This also orders the write to mode from any reads
		 * to the page tables done while the VCPU is running.
		 * Please see the comment in kvm_flush_remote_tlbs.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		 */
		smp_mb();

		if (vcpu->requests) {
			/* Make sure we process requests preemptable */
			local_irq_enable();
			trace_kvm_check_requests(vcpu);
			r = kvmppc_core_check_requests(vcpu);
<<<<<<< HEAD
			local_irq_disable();
=======
			hard_irq_disable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (r > 0)
				continue;
			break;
		}

		if (kvmppc_core_prepare_to_enter(vcpu)) {
			/* interrupts got enabled in between, so we
			   are back at square 1 */
			continue;
		}

<<<<<<< HEAD
#ifdef CONFIG_PPC64
		/* lazy EE magic */
		hard_irq_disable();
		if (lazy_irq_pending()) {
			/* Got an interrupt in between, try again */
			local_irq_enable();
			local_irq_disable();
			kvm_guest_exit();
			continue;
		}

		trace_hardirqs_on();
#endif

		kvm_guest_enter();
		break;
	}

	return r;
}
#endif /* CONFIG_KVM_BOOK3S_64_HV */
=======
		guest_enter_irqoff();
		return 1;
	}

	/* return to host */
	local_irq_enable();
	return r;
}
EXPORT_SYMBOL_GPL(kvmppc_prepare_to_enter);

#if defined(CONFIG_PPC_BOOK3S_64) && defined(CONFIG_KVM_BOOK3S_PR_POSSIBLE)
static void kvmppc_swab_shared(struct kvm_vcpu *vcpu)
{
	struct kvm_vcpu_arch_shared *shared = vcpu->arch.shared;
	int i;

	shared->sprg0 = swab64(shared->sprg0);
	shared->sprg1 = swab64(shared->sprg1);
	shared->sprg2 = swab64(shared->sprg2);
	shared->sprg3 = swab64(shared->sprg3);
	shared->srr0 = swab64(shared->srr0);
	shared->srr1 = swab64(shared->srr1);
	shared->dar = swab64(shared->dar);
	shared->msr = swab64(shared->msr);
	shared->dsisr = swab32(shared->dsisr);
	shared->int_pending = swab32(shared->int_pending);
	for (i = 0; i < ARRAY_SIZE(shared->sr); i++)
		shared->sr[i] = swab32(shared->sr[i]);
}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvmppc_kvm_pv(struct kvm_vcpu *vcpu)
{
	int nr = kvmppc_get_gpr(vcpu, 11);
	int r;
	unsigned long __maybe_unused param1 = kvmppc_get_gpr(vcpu, 3);
	unsigned long __maybe_unused param2 = kvmppc_get_gpr(vcpu, 4);
	unsigned long __maybe_unused param3 = kvmppc_get_gpr(vcpu, 5);
	unsigned long __maybe_unused param4 = kvmppc_get_gpr(vcpu, 6);
	unsigned long r2 = 0;

<<<<<<< HEAD
	if (!(vcpu->arch.shared->msr & MSR_SF)) {
=======
	if (!(kvmppc_get_msr(vcpu) & MSR_SF)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* 32 bit mode */
		param1 &= 0xffffffff;
		param2 &= 0xffffffff;
		param3 &= 0xffffffff;
		param4 &= 0xffffffff;
	}

	switch (nr) {
	case KVM_HCALL_TOKEN(KVM_HC_PPC_MAP_MAGIC_PAGE):
	{
<<<<<<< HEAD
		vcpu->arch.magic_page_pa = param1;
		vcpu->arch.magic_page_ea = param2;
=======
#if defined(CONFIG_PPC_BOOK3S_64) && defined(CONFIG_KVM_BOOK3S_PR_POSSIBLE)
		/* Book3S can be little endian, find it out here */
		int shared_big_endian = true;
		if (vcpu->arch.intr_msr & MSR_LE)
			shared_big_endian = false;
		if (shared_big_endian != vcpu->arch.shared_big_endian)
			kvmppc_swab_shared(vcpu);
		vcpu->arch.shared_big_endian = shared_big_endian;
#endif

		if (!(param2 & MAGIC_PAGE_FLAG_NOT_MAPPED_NX)) {
			/*
			 * Older versions of the Linux magic page code had
			 * a bug where they would map their trampoline code
			 * NX. If that's the case, remove !PR NX capability.
			 */
			vcpu->arch.disable_kernel_nx = true;
			kvm_make_request(KVM_REQ_TLB_FLUSH, vcpu);
		}

		vcpu->arch.magic_page_pa = param1 & ~0xfffULL;
		vcpu->arch.magic_page_ea = param2 & ~0xfffULL;

#ifdef CONFIG_PPC_64K_PAGES
		/*
		 * Make sure our 4k magic page is in the same window of a 64k
		 * page within the guest and within the host's page.
		 */
		if ((vcpu->arch.magic_page_pa & 0xf000) !=
		    ((ulong)vcpu->arch.shared & 0xf000)) {
			void *old_shared = vcpu->arch.shared;
			ulong shared = (ulong)vcpu->arch.shared;
			void *new_shared;

			shared &= PAGE_MASK;
			shared |= vcpu->arch.magic_page_pa & 0xf000;
			new_shared = (void*)shared;
			memcpy(new_shared, old_shared, 0x1000);
			vcpu->arch.shared = new_shared;
		}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		r2 = KVM_MAGIC_FEAT_SR | KVM_MAGIC_FEAT_MAS0_TO_SPRG7;

		r = EV_SUCCESS;
		break;
	}
	case KVM_HCALL_TOKEN(KVM_HC_FEATURES):
		r = EV_SUCCESS;
#if defined(CONFIG_PPC_BOOK3S) || defined(CONFIG_KVM_E500V2)
<<<<<<< HEAD
		/* XXX Missing magic page on 44x */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		r2 |= (1 << KVM_FEATURE_MAGIC_PAGE);
#endif

		/* Second return value is in r4 */
		break;
	case EV_HCALL_TOKEN(EV_IDLE):
		r = EV_SUCCESS;
		kvm_vcpu_block(vcpu);
		clear_bit(KVM_REQ_UNHALT, &vcpu->requests);
		break;
	default:
		r = EV_UNIMPLEMENTED;
		break;
	}

	kvmppc_set_gpr(vcpu, 4, r2);

	return r;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_kvm_pv);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvmppc_sanity_check(struct kvm_vcpu *vcpu)
{
	int r = false;

	/* We have to know what CPU to virtualize */
	if (!vcpu->arch.pvr)
		goto out;

	/* PAPR only works with book3s_64 */
	if ((vcpu->arch.cpu_type != KVM_CPU_3S_64) && vcpu->arch.papr_enabled)
		goto out;

<<<<<<< HEAD
#ifdef CONFIG_KVM_BOOK3S_64_HV
	/* HV KVM can only do PAPR mode for now */
	if (!vcpu->arch.papr_enabled)
		goto out;
#endif
=======
	/* HV KVM can only do PAPR mode for now */
	if (!vcpu->arch.papr_enabled && is_kvmppc_hv_enabled(vcpu->kvm))
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_KVM_BOOKE_HV
	if (!cpu_has_feature(CPU_FTR_EMB_HV))
		goto out;
#endif

	r = true;

out:
	vcpu->arch.sane = r;
	return r ? 0 : -EINVAL;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_sanity_check);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvmppc_emulate_mmio(struct kvm_run *run, struct kvm_vcpu *vcpu)
{
	enum emulation_result er;
	int r;

<<<<<<< HEAD
	er = kvmppc_emulate_instruction(run, vcpu);
=======
	er = kvmppc_emulate_loadstore(vcpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	switch (er) {
	case EMULATE_DONE:
		/* Future optimization: only reload non-volatiles if they were
		 * actually modified. */
		r = RESUME_GUEST_NV;
		break;
<<<<<<< HEAD
=======
	case EMULATE_AGAIN:
		r = RESUME_GUEST;
		break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case EMULATE_DO_MMIO:
		run->exit_reason = KVM_EXIT_MMIO;
		/* We must reload nonvolatiles because "update" load/store
		 * instructions modify register state. */
		/* Future optimization: only reload non-volatiles if they were
		 * actually modified. */
		r = RESUME_HOST_NV;
		break;
	case EMULATE_FAIL:
<<<<<<< HEAD
		/* XXX Deliver Program interrupt to guest. */
		printk(KERN_EMERG "%s: emulation failed (%08x)\n", __func__,
		       kvmppc_get_last_inst(vcpu));
		r = RESUME_HOST;
		break;
=======
	{
		u32 last_inst;

		kvmppc_get_last_inst(vcpu, INST_GENERIC, &last_inst);
		/* XXX Deliver Program interrupt to guest. */
		pr_emerg("%s: emulation failed (%08x)\n", __func__, last_inst);
		r = RESUME_HOST;
		break;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		WARN_ON(1);
		r = RESUME_GUEST;
	}

	return r;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_emulate_mmio);

int kvmppc_st(struct kvm_vcpu *vcpu, ulong *eaddr, int size, void *ptr,
	      bool data)
{
	ulong mp_pa = vcpu->arch.magic_page_pa & KVM_PAM & PAGE_MASK;
	struct kvmppc_pte pte;
	int r;

	vcpu->stat.st++;

	r = kvmppc_xlate(vcpu, *eaddr, data ? XLATE_DATA : XLATE_INST,
			 XLATE_WRITE, &pte);
	if (r < 0)
		return r;

	*eaddr = pte.raddr;

	if (!pte.may_write)
		return -EPERM;

	/* Magic page override */
	if (kvmppc_supports_magic_page(vcpu) && mp_pa &&
	    ((pte.raddr & KVM_PAM & PAGE_MASK) == mp_pa) &&
	    !(kvmppc_get_msr(vcpu) & MSR_PR)) {
		void *magic = vcpu->arch.shared;
		magic += pte.eaddr & 0xfff;
		memcpy(magic, ptr, size);
		return EMULATE_DONE;
	}

	if (kvm_write_guest(vcpu->kvm, pte.raddr, ptr, size))
		return EMULATE_DO_MMIO;

	return EMULATE_DONE;
}
EXPORT_SYMBOL_GPL(kvmppc_st);

int kvmppc_ld(struct kvm_vcpu *vcpu, ulong *eaddr, int size, void *ptr,
		      bool data)
{
	ulong mp_pa = vcpu->arch.magic_page_pa & KVM_PAM & PAGE_MASK;
	struct kvmppc_pte pte;
	int rc;

	vcpu->stat.ld++;

	rc = kvmppc_xlate(vcpu, *eaddr, data ? XLATE_DATA : XLATE_INST,
			  XLATE_READ, &pte);
	if (rc)
		return rc;

	*eaddr = pte.raddr;

	if (!pte.may_read)
		return -EPERM;

	if (!data && !pte.may_execute)
		return -ENOEXEC;

	/* Magic page override */
	if (kvmppc_supports_magic_page(vcpu) && mp_pa &&
	    ((pte.raddr & KVM_PAM & PAGE_MASK) == mp_pa) &&
	    !(kvmppc_get_msr(vcpu) & MSR_PR)) {
		void *magic = vcpu->arch.shared;
		magic += pte.eaddr & 0xfff;
		memcpy(ptr, magic, size);
		return EMULATE_DONE;
	}

	if (kvm_read_guest(vcpu->kvm, pte.raddr, ptr, size))
		return EMULATE_DO_MMIO;

	return EMULATE_DONE;
}
EXPORT_SYMBOL_GPL(kvmppc_ld);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvm_arch_hardware_enable(void)
{
	return 0;
}

int kvm_arch_hardware_setup(void)
{
	return 0;
}

void kvm_arch_check_processor_compat(void *rtn)
{
	*(int *)rtn = kvmppc_core_check_processor_compat();
}

int kvm_arch_init_vm(struct kvm *kvm, unsigned long type)
{
<<<<<<< HEAD
	if (type)
		return -EINVAL;

	return kvmppc_core_init_vm(kvm);
=======
	struct kvmppc_ops *kvm_ops = NULL;
	/*
	 * if we have both HV and PR enabled, default is HV
	 */
	if (type == 0) {
		if (kvmppc_hv_ops)
			kvm_ops = kvmppc_hv_ops;
		else
			kvm_ops = kvmppc_pr_ops;
		if (!kvm_ops)
			goto err_out;
	} else	if (type == KVM_VM_PPC_HV) {
		if (!kvmppc_hv_ops)
			goto err_out;
		kvm_ops = kvmppc_hv_ops;
	} else if (type == KVM_VM_PPC_PR) {
		if (!kvmppc_pr_ops)
			goto err_out;
		kvm_ops = kvmppc_pr_ops;
	} else
		goto err_out;

	if (kvm_ops->owner && !try_module_get(kvm_ops->owner))
		return -ENOENT;

	kvm->arch.kvm_ops = kvm_ops;
	return kvmppc_core_init_vm(kvm);
err_out:
	return -EINVAL;
}

bool kvm_arch_has_vcpu_debugfs(void)
{
	return false;
}

int kvm_arch_create_vcpu_debugfs(struct kvm_vcpu *vcpu)
{
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void kvm_arch_destroy_vm(struct kvm *kvm)
{
	unsigned int i;
	struct kvm_vcpu *vcpu;

<<<<<<< HEAD
=======
#ifdef CONFIG_KVM_XICS
	/*
	 * We call kick_all_cpus_sync() to ensure that all
	 * CPUs have executed any pending IPIs before we
	 * continue and free VCPUs structures below.
	 */
	if (is_kvmppc_hv_enabled(kvm))
		kick_all_cpus_sync();
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kvm_for_each_vcpu(i, vcpu, kvm)
		kvm_arch_vcpu_free(vcpu);

	mutex_lock(&kvm->lock);
	for (i = 0; i < atomic_read(&kvm->online_vcpus); i++)
		kvm->vcpus[i] = NULL;

	atomic_set(&kvm->online_vcpus, 0);

	kvmppc_core_destroy_vm(kvm);

	mutex_unlock(&kvm->lock);
<<<<<<< HEAD
=======

	/* drop the module reference */
	module_put(kvm->arch.kvm_ops->owner);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int kvm_vm_ioctl_check_extension(struct kvm *kvm, long ext)
{
	int r;
<<<<<<< HEAD
=======
	/* Assume we're using HV mode when the HV module is loaded */
	int hv_enabled = kvmppc_hv_ops ? 1 : 0;

	if (kvm) {
		/*
		 * Hooray - we know which VM type we're running on. Depend on
		 * that rather than the guess above.
		 */
		hv_enabled = is_kvmppc_hv_enabled(kvm);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	switch (ext) {
#ifdef CONFIG_BOOKE
	case KVM_CAP_PPC_BOOKE_SREGS:
	case KVM_CAP_PPC_BOOKE_WATCHDOG:
	case KVM_CAP_PPC_EPR:
#else
	case KVM_CAP_PPC_SEGSTATE:
	case KVM_CAP_PPC_HIOR:
	case KVM_CAP_PPC_PAPR:
#endif
	case KVM_CAP_PPC_UNSET_IRQ:
	case KVM_CAP_PPC_IRQ_LEVEL:
	case KVM_CAP_ENABLE_CAP:
<<<<<<< HEAD
=======
	case KVM_CAP_ENABLE_CAP_VM:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case KVM_CAP_ONE_REG:
	case KVM_CAP_IOEVENTFD:
	case KVM_CAP_DEVICE_CTRL:
		r = 1;
		break;
<<<<<<< HEAD
#ifndef CONFIG_KVM_BOOK3S_64_HV
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case KVM_CAP_PPC_PAIRED_SINGLES:
	case KVM_CAP_PPC_OSI:
	case KVM_CAP_PPC_GET_PVINFO:
#if defined(CONFIG_KVM_E500V2) || defined(CONFIG_KVM_E500MC)
	case KVM_CAP_SW_TLB:
#endif
<<<<<<< HEAD
#ifdef CONFIG_KVM_MPIC
	case KVM_CAP_IRQ_MPIC:
#endif
		r = 1;
		break;
=======
		/* We support this only for PR */
		r = !hv_enabled;
		break;
#ifdef CONFIG_KVM_MMIO
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case KVM_CAP_COALESCED_MMIO:
		r = KVM_COALESCED_MMIO_PAGE_OFFSET;
		break;
#endif
<<<<<<< HEAD
#ifdef CONFIG_PPC_BOOK3S_64
	case KVM_CAP_SPAPR_TCE:
	case KVM_CAP_PPC_ALLOC_HTAB:
	case KVM_CAP_PPC_RTAS:
=======
#ifdef CONFIG_KVM_MPIC
	case KVM_CAP_IRQ_MPIC:
		r = 1;
		break;
#endif

#ifdef CONFIG_PPC_BOOK3S_64
	case KVM_CAP_SPAPR_TCE:
	case KVM_CAP_SPAPR_TCE_64:
	case KVM_CAP_PPC_ALLOC_HTAB:
	case KVM_CAP_PPC_RTAS:
	case KVM_CAP_PPC_FIXUP_HCALL:
	case KVM_CAP_PPC_ENABLE_HCALL:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_KVM_XICS
	case KVM_CAP_IRQ_XICS:
#endif
		r = 1;
		break;
#endif /* CONFIG_PPC_BOOK3S_64 */
<<<<<<< HEAD
#ifdef CONFIG_KVM_BOOK3S_64_HV
	case KVM_CAP_PPC_SMT:
		r = threads_per_core;
		break;
	case KVM_CAP_PPC_RMA:
		r = 1;
		/* PPC970 requires an RMA */
		if (cpu_has_feature(CPU_FTR_ARCH_201))
			r = 2;
		break;
#endif
	case KVM_CAP_SYNC_MMU:
#ifdef CONFIG_KVM_BOOK3S_64_HV
		r = cpu_has_feature(CPU_FTR_ARCH_206) ? 1 : 0;
=======
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
	case KVM_CAP_PPC_SMT:
		if (hv_enabled)
			r = threads_per_subcore;
		else
			r = 0;
		break;
	case KVM_CAP_PPC_RMA:
		r = 0;
		break;
	case KVM_CAP_PPC_HWRNG:
		r = kvmppc_hwrng_present();
		break;
#endif
	case KVM_CAP_SYNC_MMU:
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
		r = hv_enabled;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#elif defined(KVM_ARCH_WANT_MMU_NOTIFIER)
		r = 1;
#else
		r = 0;
<<<<<<< HEAD
		break;
#endif
#ifdef CONFIG_KVM_BOOK3S_64_HV
	case KVM_CAP_PPC_HTAB_FD:
		r = 1;
		break;
#endif
		break;
=======
#endif
		break;
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
	case KVM_CAP_PPC_HTAB_FD:
		r = hv_enabled;
		break;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case KVM_CAP_NR_VCPUS:
		/*
		 * Recommending a number of CPUs is somewhat arbitrary; we
		 * return the number of present CPUs for -HV (since a host
		 * will have secondary threads "offline"), and for other KVM
		 * implementations just count online CPUs.
		 */
<<<<<<< HEAD
#ifdef CONFIG_KVM_BOOK3S_64_HV
		r = num_present_cpus();
#else
		r = num_online_cpus();
#endif
=======
		if (hv_enabled)
			r = num_present_cpus();
		else
			r = num_online_cpus();
		break;
	case KVM_CAP_NR_MEMSLOTS:
		r = KVM_USER_MEM_SLOTS;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case KVM_CAP_MAX_VCPUS:
		r = KVM_MAX_VCPUS;
		break;
#ifdef CONFIG_PPC_BOOK3S_64
	case KVM_CAP_PPC_GET_SMMU_INFO:
		r = 1;
		break;
<<<<<<< HEAD
#endif
=======
	case KVM_CAP_SPAPR_MULTITCE:
		r = 1;
		break;
#endif
	case KVM_CAP_PPC_HTM:
		r = cpu_has_feature(CPU_FTR_TM_COMP) && hv_enabled;
		break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		r = 0;
		break;
	}
	return r;

}

long kvm_arch_dev_ioctl(struct file *filp,
                        unsigned int ioctl, unsigned long arg)
{
	return -EINVAL;
}

void kvm_arch_free_memslot(struct kvm *kvm, struct kvm_memory_slot *free,
			   struct kvm_memory_slot *dont)
{
	kvmppc_core_free_memslot(kvm, free, dont);
}

int kvm_arch_create_memslot(struct kvm *kvm, struct kvm_memory_slot *slot,
			    unsigned long npages)
{
	return kvmppc_core_create_memslot(kvm, slot, npages);
}

int kvm_arch_prepare_memory_region(struct kvm *kvm,
				   struct kvm_memory_slot *memslot,
<<<<<<< HEAD
				   struct kvm_userspace_memory_region *mem,
=======
				   const struct kvm_userspace_memory_region *mem,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				   enum kvm_mr_change change)
{
	return kvmppc_core_prepare_memory_region(kvm, memslot, mem);
}

void kvm_arch_commit_memory_region(struct kvm *kvm,
<<<<<<< HEAD
				   struct kvm_userspace_memory_region *mem,
				   const struct kvm_memory_slot *old,
				   enum kvm_mr_change change)
{
	kvmppc_core_commit_memory_region(kvm, mem, old);
=======
				   const struct kvm_userspace_memory_region *mem,
				   const struct kvm_memory_slot *old,
				   const struct kvm_memory_slot *new,
				   enum kvm_mr_change change)
{
	kvmppc_core_commit_memory_region(kvm, mem, old, new);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void kvm_arch_flush_shadow_memslot(struct kvm *kvm,
				   struct kvm_memory_slot *slot)
{
	kvmppc_core_flush_memslot(kvm, slot);
}

struct kvm_vcpu *kvm_arch_vcpu_create(struct kvm *kvm, unsigned int id)
{
	struct kvm_vcpu *vcpu;
	vcpu = kvmppc_core_vcpu_create(kvm, id);
	if (!IS_ERR(vcpu)) {
		vcpu->arch.wqp = &vcpu->wq;
		kvmppc_create_vcpu_debugfs(vcpu, id);
	}
	return vcpu;
}

<<<<<<< HEAD
int kvm_arch_vcpu_postcreate(struct kvm_vcpu *vcpu)
{
	return 0;
=======
void kvm_arch_vcpu_postcreate(struct kvm_vcpu *vcpu)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void kvm_arch_vcpu_free(struct kvm_vcpu *vcpu)
{
	/* Make sure we're not using the vcpu anymore */
	hrtimer_cancel(&vcpu->arch.dec_timer);
<<<<<<< HEAD
	tasklet_kill(&vcpu->arch.tasklet);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	kvmppc_remove_vcpu_debugfs(vcpu);

	switch (vcpu->arch.irq_type) {
	case KVMPPC_IRQ_MPIC:
		kvmppc_mpic_disconnect_vcpu(vcpu->arch.mpic, vcpu);
		break;
	case KVMPPC_IRQ_XICS:
		kvmppc_xics_free_icp(vcpu);
		break;
	}

	kvmppc_core_vcpu_free(vcpu);
}

void kvm_arch_vcpu_destroy(struct kvm_vcpu *vcpu)
{
	kvm_arch_vcpu_free(vcpu);
}

int kvm_cpu_has_pending_timer(struct kvm_vcpu *vcpu)
{
	return kvmppc_core_pending_dec(vcpu);
}

<<<<<<< HEAD
/*
 * low level hrtimer wake routine. Because this runs in hardirq context
 * we schedule a tasklet to do the real work.
 */
enum hrtimer_restart kvmppc_decrementer_wakeup(struct hrtimer *timer)
=======
static enum hrtimer_restart kvmppc_decrementer_wakeup(struct hrtimer *timer)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct kvm_vcpu *vcpu;

	vcpu = container_of(timer, struct kvm_vcpu, arch.dec_timer);
<<<<<<< HEAD
	tasklet_schedule(&vcpu->arch.tasklet);
=======
	kvmppc_decrementer_func(vcpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return HRTIMER_NORESTART;
}

int kvm_arch_vcpu_init(struct kvm_vcpu *vcpu)
{
	int ret;

	hrtimer_init(&vcpu->arch.dec_timer, CLOCK_REALTIME, HRTIMER_MODE_ABS);
<<<<<<< HEAD
	tasklet_init(&vcpu->arch.tasklet, kvmppc_decrementer_func, (ulong)vcpu);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vcpu->arch.dec_timer.function = kvmppc_decrementer_wakeup;
	vcpu->arch.dec_expires = ~(u64)0;

#ifdef CONFIG_KVM_EXIT_TIMING
	mutex_init(&vcpu->arch.exit_timing_lock);
#endif
	ret = kvmppc_subarch_vcpu_init(vcpu);
	return ret;
}

void kvm_arch_vcpu_uninit(struct kvm_vcpu *vcpu)
{
	kvmppc_mmu_destroy(vcpu);
	kvmppc_subarch_vcpu_uninit(vcpu);
}

void kvm_arch_vcpu_load(struct kvm_vcpu *vcpu, int cpu)
{
#ifdef CONFIG_BOOKE
	/*
	 * vrsave (formerly usprg0) isn't used by Linux, but may
	 * be used by the guest.
	 *
	 * On non-booke this is associated with Altivec and
	 * is handled by code in book3s.c.
	 */
	mtspr(SPRN_VRSAVE, vcpu->arch.vrsave);
#endif
	kvmppc_core_vcpu_load(vcpu, cpu);
}

void kvm_arch_vcpu_put(struct kvm_vcpu *vcpu)
{
	kvmppc_core_vcpu_put(vcpu);
#ifdef CONFIG_BOOKE
	vcpu->arch.vrsave = mfspr(SPRN_VRSAVE);
#endif
}

<<<<<<< HEAD
static void kvmppc_complete_dcr_load(struct kvm_vcpu *vcpu,
                                     struct kvm_run *run)
{
	kvmppc_set_gpr(vcpu, vcpu->arch.io_gpr, run->dcr.data);
=======
/*
 * irq_bypass_add_producer and irq_bypass_del_producer are only
 * useful if the architecture supports PCI passthrough.
 * irq_bypass_stop and irq_bypass_start are not needed and so
 * kvm_ops are not defined for them.
 */
bool kvm_arch_has_irq_bypass(void)
{
	return ((kvmppc_hv_ops && kvmppc_hv_ops->irq_bypass_add_producer) ||
		(kvmppc_pr_ops && kvmppc_pr_ops->irq_bypass_add_producer));
}

int kvm_arch_irq_bypass_add_producer(struct irq_bypass_consumer *cons,
				     struct irq_bypass_producer *prod)
{
	struct kvm_kernel_irqfd *irqfd =
		container_of(cons, struct kvm_kernel_irqfd, consumer);
	struct kvm *kvm = irqfd->kvm;

	if (kvm->arch.kvm_ops->irq_bypass_add_producer)
		return kvm->arch.kvm_ops->irq_bypass_add_producer(cons, prod);

	return 0;
}

void kvm_arch_irq_bypass_del_producer(struct irq_bypass_consumer *cons,
				      struct irq_bypass_producer *prod)
{
	struct kvm_kernel_irqfd *irqfd =
		container_of(cons, struct kvm_kernel_irqfd, consumer);
	struct kvm *kvm = irqfd->kvm;

	if (kvm->arch.kvm_ops->irq_bypass_del_producer)
		kvm->arch.kvm_ops->irq_bypass_del_producer(cons, prod);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void kvmppc_complete_mmio_load(struct kvm_vcpu *vcpu,
                                      struct kvm_run *run)
{
	u64 uninitialized_var(gpr);

	if (run->mmio.len > sizeof(gpr)) {
		printk(KERN_ERR "bad MMIO length: %d\n", run->mmio.len);
		return;
	}

<<<<<<< HEAD
	if (vcpu->arch.mmio_is_bigendian) {
=======
	if (!vcpu->arch.mmio_host_swabbed) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		switch (run->mmio.len) {
		case 8: gpr = *(u64 *)run->mmio.data; break;
		case 4: gpr = *(u32 *)run->mmio.data; break;
		case 2: gpr = *(u16 *)run->mmio.data; break;
		case 1: gpr = *(u8 *)run->mmio.data; break;
		}
	} else {
<<<<<<< HEAD
		/* Convert BE data from userland back to LE. */
		switch (run->mmio.len) {
		case 4: gpr = ld_le32((u32 *)run->mmio.data); break;
		case 2: gpr = ld_le16((u16 *)run->mmio.data); break;
=======
		switch (run->mmio.len) {
		case 8: gpr = swab64(*(u64 *)run->mmio.data); break;
		case 4: gpr = swab32(*(u32 *)run->mmio.data); break;
		case 2: gpr = swab16(*(u16 *)run->mmio.data); break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case 1: gpr = *(u8 *)run->mmio.data; break;
		}
	}

	if (vcpu->arch.mmio_sign_extend) {
		switch (run->mmio.len) {
#ifdef CONFIG_PPC64
		case 4:
			gpr = (s64)(s32)gpr;
			break;
#endif
		case 2:
			gpr = (s64)(s16)gpr;
			break;
		case 1:
			gpr = (s64)(s8)gpr;
			break;
		}
	}

	kvmppc_set_gpr(vcpu, vcpu->arch.io_gpr, gpr);

	switch (vcpu->arch.io_gpr & KVM_MMIO_REG_EXT_MASK) {
	case KVM_MMIO_REG_GPR:
		kvmppc_set_gpr(vcpu, vcpu->arch.io_gpr, gpr);
		break;
	case KVM_MMIO_REG_FPR:
<<<<<<< HEAD
		vcpu->arch.fpr[vcpu->arch.io_gpr & KVM_MMIO_REG_MASK] = gpr;
=======
		VCPU_FPR(vcpu, vcpu->arch.io_gpr & KVM_MMIO_REG_MASK) = gpr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
#ifdef CONFIG_PPC_BOOK3S
	case KVM_MMIO_REG_QPR:
		vcpu->arch.qpr[vcpu->arch.io_gpr & KVM_MMIO_REG_MASK] = gpr;
		break;
	case KVM_MMIO_REG_FQPR:
<<<<<<< HEAD
		vcpu->arch.fpr[vcpu->arch.io_gpr & KVM_MMIO_REG_MASK] = gpr;
=======
		VCPU_FPR(vcpu, vcpu->arch.io_gpr & KVM_MMIO_REG_MASK) = gpr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		vcpu->arch.qpr[vcpu->arch.io_gpr & KVM_MMIO_REG_MASK] = gpr;
		break;
#endif
	default:
		BUG();
	}
}

<<<<<<< HEAD
int kvmppc_handle_load(struct kvm_run *run, struct kvm_vcpu *vcpu,
                       unsigned int rt, unsigned int bytes, int is_bigendian)
{
	int idx, ret;
=======
static int __kvmppc_handle_load(struct kvm_run *run, struct kvm_vcpu *vcpu,
				unsigned int rt, unsigned int bytes,
				int is_default_endian, int sign_extend)
{
	int idx, ret;
	bool host_swabbed;

	/* Pity C doesn't have a logical XOR operator */
	if (kvmppc_need_byteswap(vcpu)) {
		host_swabbed = is_default_endian;
	} else {
		host_swabbed = !is_default_endian;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (bytes > sizeof(run->mmio.data)) {
		printk(KERN_ERR "%s: bad MMIO length: %d\n", __func__,
		       run->mmio.len);
	}

	run->mmio.phys_addr = vcpu->arch.paddr_accessed;
	run->mmio.len = bytes;
	run->mmio.is_write = 0;

	vcpu->arch.io_gpr = rt;
<<<<<<< HEAD
	vcpu->arch.mmio_is_bigendian = is_bigendian;
	vcpu->mmio_needed = 1;
	vcpu->mmio_is_write = 0;
	vcpu->arch.mmio_sign_extend = 0;

	idx = srcu_read_lock(&vcpu->kvm->srcu);

	ret = kvm_io_bus_read(vcpu->kvm, KVM_MMIO_BUS, run->mmio.phys_addr,
=======
	vcpu->arch.mmio_host_swabbed = host_swabbed;
	vcpu->mmio_needed = 1;
	vcpu->mmio_is_write = 0;
	vcpu->arch.mmio_sign_extend = sign_extend;

	idx = srcu_read_lock(&vcpu->kvm->srcu);

	ret = kvm_io_bus_read(vcpu, KVM_MMIO_BUS, run->mmio.phys_addr,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			      bytes, &run->mmio.data);

	srcu_read_unlock(&vcpu->kvm->srcu, idx);

	if (!ret) {
		kvmppc_complete_mmio_load(vcpu, run);
		vcpu->mmio_needed = 0;
		return EMULATE_DONE;
	}

	return EMULATE_DO_MMIO;
}

<<<<<<< HEAD
/* Same as above, but sign extends */
int kvmppc_handle_loads(struct kvm_run *run, struct kvm_vcpu *vcpu,
                        unsigned int rt, unsigned int bytes, int is_bigendian)
{
	int r;

	vcpu->arch.mmio_sign_extend = 1;
	r = kvmppc_handle_load(run, vcpu, rt, bytes, is_bigendian);

	return r;
}

int kvmppc_handle_store(struct kvm_run *run, struct kvm_vcpu *vcpu,
                        u64 val, unsigned int bytes, int is_bigendian)
{
	void *data = run->mmio.data;
	int idx, ret;
=======
int kvmppc_handle_load(struct kvm_run *run, struct kvm_vcpu *vcpu,
		       unsigned int rt, unsigned int bytes,
		       int is_default_endian)
{
	return __kvmppc_handle_load(run, vcpu, rt, bytes, is_default_endian, 0);
}
EXPORT_SYMBOL_GPL(kvmppc_handle_load);

/* Same as above, but sign extends */
int kvmppc_handle_loads(struct kvm_run *run, struct kvm_vcpu *vcpu,
			unsigned int rt, unsigned int bytes,
			int is_default_endian)
{
	return __kvmppc_handle_load(run, vcpu, rt, bytes, is_default_endian, 1);
}

int kvmppc_handle_store(struct kvm_run *run, struct kvm_vcpu *vcpu,
			u64 val, unsigned int bytes, int is_default_endian)
{
	void *data = run->mmio.data;
	int idx, ret;
	bool host_swabbed;

	/* Pity C doesn't have a logical XOR operator */
	if (kvmppc_need_byteswap(vcpu)) {
		host_swabbed = is_default_endian;
	} else {
		host_swabbed = !is_default_endian;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (bytes > sizeof(run->mmio.data)) {
		printk(KERN_ERR "%s: bad MMIO length: %d\n", __func__,
		       run->mmio.len);
	}

	run->mmio.phys_addr = vcpu->arch.paddr_accessed;
	run->mmio.len = bytes;
	run->mmio.is_write = 1;
	vcpu->mmio_needed = 1;
	vcpu->mmio_is_write = 1;

	/* Store the value at the lowest bytes in 'data'. */
<<<<<<< HEAD
	if (is_bigendian) {
=======
	if (!host_swabbed) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		switch (bytes) {
		case 8: *(u64 *)data = val; break;
		case 4: *(u32 *)data = val; break;
		case 2: *(u16 *)data = val; break;
		case 1: *(u8  *)data = val; break;
		}
	} else {
<<<<<<< HEAD
		/* Store LE value into 'data'. */
		switch (bytes) {
		case 4: st_le32(data, val); break;
		case 2: st_le16(data, val); break;
		case 1: *(u8 *)data = val; break;
=======
		switch (bytes) {
		case 8: *(u64 *)data = swab64(val); break;
		case 4: *(u32 *)data = swab32(val); break;
		case 2: *(u16 *)data = swab16(val); break;
		case 1: *(u8  *)data = val; break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	idx = srcu_read_lock(&vcpu->kvm->srcu);

<<<<<<< HEAD
	ret = kvm_io_bus_write(vcpu->kvm, KVM_MMIO_BUS, run->mmio.phys_addr,
=======
	ret = kvm_io_bus_write(vcpu, KVM_MMIO_BUS, run->mmio.phys_addr,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			       bytes, &run->mmio.data);

	srcu_read_unlock(&vcpu->kvm->srcu, idx);

	if (!ret) {
		vcpu->mmio_needed = 0;
		return EMULATE_DONE;
	}

	return EMULATE_DO_MMIO;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_handle_store);

int kvm_vcpu_ioctl_get_one_reg(struct kvm_vcpu *vcpu, struct kvm_one_reg *reg)
{
	int r = 0;
	union kvmppc_one_reg val;
	int size;

	size = one_reg_size(reg->id);
	if (size > sizeof(val))
		return -EINVAL;

	r = kvmppc_get_one_reg(vcpu, reg->id, &val);
	if (r == -EINVAL) {
		r = 0;
		switch (reg->id) {
#ifdef CONFIG_ALTIVEC
		case KVM_REG_PPC_VR0 ... KVM_REG_PPC_VR31:
			if (!cpu_has_feature(CPU_FTR_ALTIVEC)) {
				r = -ENXIO;
				break;
			}
			val.vval = vcpu->arch.vr.vr[reg->id - KVM_REG_PPC_VR0];
			break;
		case KVM_REG_PPC_VSCR:
			if (!cpu_has_feature(CPU_FTR_ALTIVEC)) {
				r = -ENXIO;
				break;
			}
			val = get_reg_val(reg->id, vcpu->arch.vr.vscr.u[3]);
			break;
		case KVM_REG_PPC_VRSAVE:
			val = get_reg_val(reg->id, vcpu->arch.vrsave);
			break;
#endif /* CONFIG_ALTIVEC */
		default:
			r = -EINVAL;
			break;
		}
	}

	if (r)
		return r;

	if (copy_to_user((char __user *)(unsigned long)reg->addr, &val, size))
		r = -EFAULT;

	return r;
}

int kvm_vcpu_ioctl_set_one_reg(struct kvm_vcpu *vcpu, struct kvm_one_reg *reg)
{
	int r;
	union kvmppc_one_reg val;
	int size;

	size = one_reg_size(reg->id);
	if (size > sizeof(val))
		return -EINVAL;

	if (copy_from_user(&val, (char __user *)(unsigned long)reg->addr, size))
		return -EFAULT;

	r = kvmppc_set_one_reg(vcpu, reg->id, &val);
	if (r == -EINVAL) {
		r = 0;
		switch (reg->id) {
#ifdef CONFIG_ALTIVEC
		case KVM_REG_PPC_VR0 ... KVM_REG_PPC_VR31:
			if (!cpu_has_feature(CPU_FTR_ALTIVEC)) {
				r = -ENXIO;
				break;
			}
			vcpu->arch.vr.vr[reg->id - KVM_REG_PPC_VR0] = val.vval;
			break;
		case KVM_REG_PPC_VSCR:
			if (!cpu_has_feature(CPU_FTR_ALTIVEC)) {
				r = -ENXIO;
				break;
			}
			vcpu->arch.vr.vscr.u[3] = set_reg_val(reg->id, val);
			break;
		case KVM_REG_PPC_VRSAVE:
			if (!cpu_has_feature(CPU_FTR_ALTIVEC)) {
				r = -ENXIO;
				break;
			}
			vcpu->arch.vrsave = set_reg_val(reg->id, val);
			break;
#endif /* CONFIG_ALTIVEC */
		default:
			r = -EINVAL;
			break;
		}
	}

	return r;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvm_arch_vcpu_ioctl_run(struct kvm_vcpu *vcpu, struct kvm_run *run)
{
	int r;
	sigset_t sigsaved;

	if (vcpu->sigset_active)
		sigprocmask(SIG_SETMASK, &vcpu->sigset, &sigsaved);

	if (vcpu->mmio_needed) {
		if (!vcpu->mmio_is_write)
			kvmppc_complete_mmio_load(vcpu, run);
		vcpu->mmio_needed = 0;
<<<<<<< HEAD
	} else if (vcpu->arch.dcr_needed) {
		if (!vcpu->arch.dcr_is_write)
			kvmppc_complete_dcr_load(vcpu, run);
		vcpu->arch.dcr_needed = 0;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else if (vcpu->arch.osi_needed) {
		u64 *gprs = run->osi.gprs;
		int i;

		for (i = 0; i < 32; i++)
			kvmppc_set_gpr(vcpu, i, gprs[i]);
		vcpu->arch.osi_needed = 0;
	} else if (vcpu->arch.hcall_needed) {
		int i;

		kvmppc_set_gpr(vcpu, 3, run->papr_hcall.ret);
		for (i = 0; i < 9; ++i)
			kvmppc_set_gpr(vcpu, 4 + i, run->papr_hcall.args[i]);
		vcpu->arch.hcall_needed = 0;
#ifdef CONFIG_BOOKE
	} else if (vcpu->arch.epr_needed) {
		kvmppc_set_epr(vcpu, run->epr.epr);
		vcpu->arch.epr_needed = 0;
#endif
	}

	r = kvmppc_vcpu_run(run, vcpu);

	if (vcpu->sigset_active)
		sigprocmask(SIG_SETMASK, &sigsaved, NULL);

	return r;
}

int kvm_vcpu_ioctl_interrupt(struct kvm_vcpu *vcpu, struct kvm_interrupt *irq)
{
	if (irq->irq == KVM_INTERRUPT_UNSET) {
		kvmppc_core_dequeue_external(vcpu);
		return 0;
	}

	kvmppc_core_queue_external(vcpu, irq);

	kvm_vcpu_kick(vcpu);

	return 0;
}

static int kvm_vcpu_ioctl_enable_cap(struct kvm_vcpu *vcpu,
				     struct kvm_enable_cap *cap)
{
	int r;

	if (cap->flags)
		return -EINVAL;

	switch (cap->cap) {
	case KVM_CAP_PPC_OSI:
		r = 0;
		vcpu->arch.osi_enabled = true;
		break;
	case KVM_CAP_PPC_PAPR:
		r = 0;
		vcpu->arch.papr_enabled = true;
		break;
	case KVM_CAP_PPC_EPR:
		r = 0;
		if (cap->args[0])
			vcpu->arch.epr_flags |= KVMPPC_EPR_USER;
		else
			vcpu->arch.epr_flags &= ~KVMPPC_EPR_USER;
		break;
#ifdef CONFIG_BOOKE
	case KVM_CAP_PPC_BOOKE_WATCHDOG:
		r = 0;
		vcpu->arch.watchdog_enabled = true;
		break;
#endif
#if defined(CONFIG_KVM_E500V2) || defined(CONFIG_KVM_E500MC)
	case KVM_CAP_SW_TLB: {
		struct kvm_config_tlb cfg;
		void __user *user_ptr = (void __user *)(uintptr_t)cap->args[0];

		r = -EFAULT;
		if (copy_from_user(&cfg, user_ptr, sizeof(cfg)))
			break;

		r = kvm_vcpu_ioctl_config_tlb(vcpu, &cfg);
		break;
	}
#endif
#ifdef CONFIG_KVM_MPIC
	case KVM_CAP_IRQ_MPIC: {
<<<<<<< HEAD
		struct file *filp;
		struct kvm_device *dev;

		r = -EBADF;
		filp = fget(cap->args[0]);
		if (!filp)
			break;

		r = -EPERM;
		dev = kvm_device_from_filp(filp);
		if (dev)
			r = kvmppc_mpic_connect_vcpu(dev, vcpu, cap->args[1]);

		fput(filp);
=======
		struct fd f;
		struct kvm_device *dev;

		r = -EBADF;
		f = fdget(cap->args[0]);
		if (!f.file)
			break;

		r = -EPERM;
		dev = kvm_device_from_filp(f.file);
		if (dev)
			r = kvmppc_mpic_connect_vcpu(dev, vcpu, cap->args[1]);

		fdput(f);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	}
#endif
#ifdef CONFIG_KVM_XICS
	case KVM_CAP_IRQ_XICS: {
<<<<<<< HEAD
		struct file *filp;
		struct kvm_device *dev;

		r = -EBADF;
		filp = fget(cap->args[0]);
		if (!filp)
			break;

		r = -EPERM;
		dev = kvm_device_from_filp(filp);
		if (dev)
			r = kvmppc_xics_connect_vcpu(dev, vcpu, cap->args[1]);

		fput(filp);
=======
		struct fd f;
		struct kvm_device *dev;

		r = -EBADF;
		f = fdget(cap->args[0]);
		if (!f.file)
			break;

		r = -EPERM;
		dev = kvm_device_from_filp(f.file);
		if (dev)
			r = kvmppc_xics_connect_vcpu(dev, vcpu, cap->args[1]);

		fdput(f);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	}
#endif /* CONFIG_KVM_XICS */
	default:
		r = -EINVAL;
		break;
	}

	if (!r)
		r = kvmppc_sanity_check(vcpu);

	return r;
}

<<<<<<< HEAD
=======
bool kvm_arch_intc_initialized(struct kvm *kvm)
{
#ifdef CONFIG_KVM_MPIC
	if (kvm->arch.mpic)
		return true;
#endif
#ifdef CONFIG_KVM_XICS
	if (kvm->arch.xics)
		return true;
#endif
	return false;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int kvm_arch_vcpu_ioctl_get_mpstate(struct kvm_vcpu *vcpu,
                                    struct kvm_mp_state *mp_state)
{
	return -EINVAL;
}

int kvm_arch_vcpu_ioctl_set_mpstate(struct kvm_vcpu *vcpu,
                                    struct kvm_mp_state *mp_state)
{
	return -EINVAL;
}

long kvm_arch_vcpu_ioctl(struct file *filp,
                         unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = (void __user *)arg;
	long r;

	switch (ioctl) {
	case KVM_INTERRUPT: {
		struct kvm_interrupt irq;
		r = -EFAULT;
		if (copy_from_user(&irq, argp, sizeof(irq)))
			goto out;
		r = kvm_vcpu_ioctl_interrupt(vcpu, &irq);
		goto out;
	}

	case KVM_ENABLE_CAP:
	{
		struct kvm_enable_cap cap;
		r = -EFAULT;
		if (copy_from_user(&cap, argp, sizeof(cap)))
			goto out;
		r = kvm_vcpu_ioctl_enable_cap(vcpu, &cap);
		break;
	}

	case KVM_SET_ONE_REG:
	case KVM_GET_ONE_REG:
	{
		struct kvm_one_reg reg;
		r = -EFAULT;
		if (copy_from_user(&reg, argp, sizeof(reg)))
			goto out;
		if (ioctl == KVM_SET_ONE_REG)
			r = kvm_vcpu_ioctl_set_one_reg(vcpu, &reg);
		else
			r = kvm_vcpu_ioctl_get_one_reg(vcpu, &reg);
		break;
	}

#if defined(CONFIG_KVM_E500V2) || defined(CONFIG_KVM_E500MC)
	case KVM_DIRTY_TLB: {
		struct kvm_dirty_tlb dirty;
		r = -EFAULT;
		if (copy_from_user(&dirty, argp, sizeof(dirty)))
			goto out;
		r = kvm_vcpu_ioctl_dirty_tlb(vcpu, &dirty);
		break;
	}
#endif
	default:
		r = -EINVAL;
	}

out:
	return r;
}

int kvm_arch_vcpu_fault(struct kvm_vcpu *vcpu, struct vm_fault *vmf)
{
	return VM_FAULT_SIGBUS;
}

static int kvm_vm_ioctl_get_pvinfo(struct kvm_ppc_pvinfo *pvinfo)
{
	u32 inst_nop = 0x60000000;
#ifdef CONFIG_KVM_BOOKE_HV
	u32 inst_sc1 = 0x44000022;
<<<<<<< HEAD
	pvinfo->hcall[0] = inst_sc1;
	pvinfo->hcall[1] = inst_nop;
	pvinfo->hcall[2] = inst_nop;
	pvinfo->hcall[3] = inst_nop;
=======
	pvinfo->hcall[0] = cpu_to_be32(inst_sc1);
	pvinfo->hcall[1] = cpu_to_be32(inst_nop);
	pvinfo->hcall[2] = cpu_to_be32(inst_nop);
	pvinfo->hcall[3] = cpu_to_be32(inst_nop);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
	u32 inst_lis = 0x3c000000;
	u32 inst_ori = 0x60000000;
	u32 inst_sc = 0x44000002;
	u32 inst_imm_mask = 0xffff;

	/*
	 * The hypercall to get into KVM from within guest context is as
	 * follows:
	 *
	 *    lis r0, r0, KVM_SC_MAGIC_R0@h
	 *    ori r0, KVM_SC_MAGIC_R0@l
	 *    sc
	 *    nop
	 */
<<<<<<< HEAD
	pvinfo->hcall[0] = inst_lis | ((KVM_SC_MAGIC_R0 >> 16) & inst_imm_mask);
	pvinfo->hcall[1] = inst_ori | (KVM_SC_MAGIC_R0 & inst_imm_mask);
	pvinfo->hcall[2] = inst_sc;
	pvinfo->hcall[3] = inst_nop;
=======
	pvinfo->hcall[0] = cpu_to_be32(inst_lis | ((KVM_SC_MAGIC_R0 >> 16) & inst_imm_mask));
	pvinfo->hcall[1] = cpu_to_be32(inst_ori | (KVM_SC_MAGIC_R0 & inst_imm_mask));
	pvinfo->hcall[2] = cpu_to_be32(inst_sc);
	pvinfo->hcall[3] = cpu_to_be32(inst_nop);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

	pvinfo->flags = KVM_PPC_PVINFO_FLAGS_EV_IDLE;

	return 0;
}

int kvm_vm_ioctl_irq_line(struct kvm *kvm, struct kvm_irq_level *irq_event,
			  bool line_status)
{
	if (!irqchip_in_kernel(kvm))
		return -ENXIO;

	irq_event->status = kvm_set_irq(kvm, KVM_USERSPACE_IRQ_SOURCE_ID,
					irq_event->irq, irq_event->level,
					line_status);
	return 0;
}

<<<<<<< HEAD
=======

static int kvm_vm_ioctl_enable_cap(struct kvm *kvm,
				   struct kvm_enable_cap *cap)
{
	int r;

	if (cap->flags)
		return -EINVAL;

	switch (cap->cap) {
#ifdef CONFIG_KVM_BOOK3S_64_HANDLER
	case KVM_CAP_PPC_ENABLE_HCALL: {
		unsigned long hcall = cap->args[0];

		r = -EINVAL;
		if (hcall > MAX_HCALL_OPCODE || (hcall & 3) ||
		    cap->args[1] > 1)
			break;
		if (!kvmppc_book3s_hcall_implemented(kvm, hcall))
			break;
		if (cap->args[1])
			set_bit(hcall / 4, kvm->arch.enabled_hcalls);
		else
			clear_bit(hcall / 4, kvm->arch.enabled_hcalls);
		r = 0;
		break;
	}
#endif
	default:
		r = -EINVAL;
		break;
	}

	return r;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
long kvm_arch_vm_ioctl(struct file *filp,
                       unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm __maybe_unused = filp->private_data;
	void __user *argp = (void __user *)arg;
	long r;

	switch (ioctl) {
	case KVM_PPC_GET_PVINFO: {
		struct kvm_ppc_pvinfo pvinfo;
		memset(&pvinfo, 0, sizeof(pvinfo));
		r = kvm_vm_ioctl_get_pvinfo(&pvinfo);
		if (copy_to_user(argp, &pvinfo, sizeof(pvinfo))) {
			r = -EFAULT;
			goto out;
		}

		break;
	}
<<<<<<< HEAD
#ifdef CONFIG_PPC_BOOK3S_64
	case KVM_CREATE_SPAPR_TCE: {
		struct kvm_create_spapr_tce create_tce;

		r = -EFAULT;
		if (copy_from_user(&create_tce, argp, sizeof(create_tce)))
			goto out;
		r = kvm_vm_ioctl_create_spapr_tce(kvm, &create_tce);
		goto out;
	}
#endif /* CONFIG_PPC_BOOK3S_64 */

#ifdef CONFIG_KVM_BOOK3S_64_HV
	case KVM_ALLOCATE_RMA: {
		struct kvm_allocate_rma rma;
		struct kvm *kvm = filp->private_data;

		r = kvm_vm_ioctl_allocate_rma(kvm, &rma);
		if (r >= 0 && copy_to_user(argp, &rma, sizeof(rma)))
			r = -EFAULT;
		break;
	}

	case KVM_PPC_ALLOCATE_HTAB: {
		u32 htab_order;

		r = -EFAULT;
		if (get_user(htab_order, (u32 __user *)argp))
			break;
		r = kvmppc_alloc_reset_hpt(kvm, &htab_order);
		if (r)
			break;
		r = -EFAULT;
		if (put_user(htab_order, (u32 __user *)argp))
			break;
		r = 0;
		break;
	}

	case KVM_PPC_GET_HTAB_FD: {
		struct kvm_get_htab_fd ghf;

		r = -EFAULT;
		if (copy_from_user(&ghf, argp, sizeof(ghf)))
			break;
		r = kvm_vm_ioctl_get_htab_fd(kvm, &ghf);
		break;
	}
#endif /* CONFIG_KVM_BOOK3S_64_HV */

#ifdef CONFIG_PPC_BOOK3S_64
	case KVM_PPC_GET_SMMU_INFO: {
		struct kvm_ppc_smmu_info info;

		memset(&info, 0, sizeof(info));
		r = kvm_vm_ioctl_get_smmu_info(kvm, &info);
=======
	case KVM_ENABLE_CAP:
	{
		struct kvm_enable_cap cap;
		r = -EFAULT;
		if (copy_from_user(&cap, argp, sizeof(cap)))
			goto out;
		r = kvm_vm_ioctl_enable_cap(kvm, &cap);
		break;
	}
#ifdef CONFIG_PPC_BOOK3S_64
	case KVM_CREATE_SPAPR_TCE_64: {
		struct kvm_create_spapr_tce_64 create_tce_64;

		r = -EFAULT;
		if (copy_from_user(&create_tce_64, argp, sizeof(create_tce_64)))
			goto out;
		if (create_tce_64.flags) {
			r = -EINVAL;
			goto out;
		}
		r = kvm_vm_ioctl_create_spapr_tce(kvm, &create_tce_64);
		goto out;
	}
	case KVM_CREATE_SPAPR_TCE: {
		struct kvm_create_spapr_tce create_tce;
		struct kvm_create_spapr_tce_64 create_tce_64;

		r = -EFAULT;
		if (copy_from_user(&create_tce, argp, sizeof(create_tce)))
			goto out;

		create_tce_64.liobn = create_tce.liobn;
		create_tce_64.page_shift = IOMMU_PAGE_SHIFT_4K;
		create_tce_64.offset = 0;
		create_tce_64.size = create_tce.window_size >>
				IOMMU_PAGE_SHIFT_4K;
		create_tce_64.flags = 0;
		r = kvm_vm_ioctl_create_spapr_tce(kvm, &create_tce_64);
		goto out;
	}
	case KVM_PPC_GET_SMMU_INFO: {
		struct kvm_ppc_smmu_info info;
		struct kvm *kvm = filp->private_data;

		memset(&info, 0, sizeof(info));
		r = kvm->arch.kvm_ops->get_smmu_info(kvm, &info);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (r >= 0 && copy_to_user(argp, &info, sizeof(info)))
			r = -EFAULT;
		break;
	}
	case KVM_PPC_RTAS_DEFINE_TOKEN: {
		struct kvm *kvm = filp->private_data;

		r = kvm_vm_ioctl_rtas_define_token(kvm, argp);
		break;
	}
<<<<<<< HEAD
#endif /* CONFIG_PPC_BOOK3S_64 */
	default:
		r = -ENOTTY;
	}

=======
	default: {
		struct kvm *kvm = filp->private_data;
		r = kvm->arch.kvm_ops->arch_vm_ioctl(filp, ioctl, arg);
	}
#else /* CONFIG_PPC_BOOK3S_64 */
	default:
		r = -ENOTTY;
#endif
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return r;
}

static unsigned long lpid_inuse[BITS_TO_LONGS(KVMPPC_NR_LPIDS)];
static unsigned long nr_lpids;

long kvmppc_alloc_lpid(void)
{
	long lpid;

	do {
		lpid = find_first_zero_bit(lpid_inuse, KVMPPC_NR_LPIDS);
		if (lpid >= nr_lpids) {
			pr_err("%s: No LPIDs free\n", __func__);
			return -ENOMEM;
		}
	} while (test_and_set_bit(lpid, lpid_inuse));

	return lpid;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_alloc_lpid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void kvmppc_claim_lpid(long lpid)
{
	set_bit(lpid, lpid_inuse);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_claim_lpid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void kvmppc_free_lpid(long lpid)
{
	clear_bit(lpid, lpid_inuse);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_free_lpid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void kvmppc_init_lpid(unsigned long nr_lpids_param)
{
	nr_lpids = min_t(unsigned long, KVMPPC_NR_LPIDS, nr_lpids_param);
	memset(lpid_inuse, 0, sizeof(lpid_inuse));
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvmppc_init_lpid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int kvm_arch_init(void *opaque)
{
	return 0;
}
<<<<<<< HEAD
=======

EXPORT_TRACEPOINT_SYMBOL_GPL(kvm_ppc_instr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
