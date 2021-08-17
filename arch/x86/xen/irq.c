#include <linux/hardirq.h>

#include <asm/x86_init.h>

#include <xen/interface/xen.h>
#include <xen/interface/sched.h>
#include <xen/interface/vcpu.h>
<<<<<<< HEAD
=======
#include <xen/features.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <xen/events.h>

#include <asm/xen/hypercall.h>
#include <asm/xen/hypervisor.h>

#include "xen-ops.h"

/*
 * Force a proper event-channel callback from Xen after clearing the
 * callback mask. We do this in a very simple manner, by making a call
 * down into Xen. The pending flag will be checked by Xen on return.
 */
void xen_force_evtchn_callback(void)
{
	(void)HYPERVISOR_xen_version(0, NULL);
}

<<<<<<< HEAD
static unsigned long xen_save_fl(void)
=======
asmlinkage __visible unsigned long xen_save_fl(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct vcpu_info *vcpu;
	unsigned long flags;

	vcpu = this_cpu_read(xen_vcpu);

	/* flag has opposite sense of mask */
	flags = !vcpu->evtchn_upcall_mask;

	/* convert to IF type flag
	   -0 -> 0x00000000
	   -1 -> 0xffffffff
	*/
	return (-flags) & X86_EFLAGS_IF;
}
PV_CALLEE_SAVE_REGS_THUNK(xen_save_fl);

<<<<<<< HEAD
static void xen_restore_fl(unsigned long flags)
=======
__visible void xen_restore_fl(unsigned long flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct vcpu_info *vcpu;

	/* convert from IF type flag */
	flags = !(flags & X86_EFLAGS_IF);

<<<<<<< HEAD
	/* There's a one instruction preempt window here.  We need to
	   make sure we're don't switch CPUs between getting the vcpu
	   pointer and updating the mask. */
	preempt_disable();
	vcpu = this_cpu_read(xen_vcpu);
	vcpu->evtchn_upcall_mask = flags;
	preempt_enable_no_resched();

	/* Doesn't matter if we get preempted here, because any
	   pending event will get dealt with anyway. */

	if (flags == 0) {
		preempt_check_resched();
		barrier(); /* unmask then check (avoid races) */
		if (unlikely(vcpu->evtchn_upcall_pending))
			xen_force_evtchn_callback();
	}
}
PV_CALLEE_SAVE_REGS_THUNK(xen_restore_fl);

static void xen_irq_disable(void)
=======
	/* See xen_irq_enable() for why preemption must be disabled. */
	preempt_disable();
	vcpu = this_cpu_read(xen_vcpu);
	vcpu->evtchn_upcall_mask = flags;

	if (flags == 0) {
		barrier(); /* unmask then check (avoid races) */
		if (unlikely(vcpu->evtchn_upcall_pending))
			xen_force_evtchn_callback();
		preempt_enable();
	} else
		preempt_enable_no_resched();
}
PV_CALLEE_SAVE_REGS_THUNK(xen_restore_fl);

asmlinkage __visible void xen_irq_disable(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/* There's a one instruction preempt window here.  We need to
	   make sure we're don't switch CPUs between getting the vcpu
	   pointer and updating the mask. */
	preempt_disable();
	this_cpu_read(xen_vcpu)->evtchn_upcall_mask = 1;
	preempt_enable_no_resched();
}
PV_CALLEE_SAVE_REGS_THUNK(xen_irq_disable);

<<<<<<< HEAD
static void xen_irq_enable(void)
{
	struct vcpu_info *vcpu;

	/* We don't need to worry about being preempted here, since
	   either a) interrupts are disabled, so no preemption, or b)
	   the caller is confused and is trying to re-enable interrupts
	   on an indeterminate processor. */
=======
asmlinkage __visible void xen_irq_enable(void)
{
	struct vcpu_info *vcpu;

	/*
	 * We may be preempted as soon as vcpu->evtchn_upcall_mask is
	 * cleared, so disable preemption to ensure we check for
	 * events on the VCPU we are still running on.
	 */
	preempt_disable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	vcpu = this_cpu_read(xen_vcpu);
	vcpu->evtchn_upcall_mask = 0;

	/* Doesn't matter if we get preempted here, because any
	   pending event will get dealt with anyway. */

	barrier(); /* unmask then check (avoid races) */
	if (unlikely(vcpu->evtchn_upcall_pending))
		xen_force_evtchn_callback();
<<<<<<< HEAD
=======

	preempt_enable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
PV_CALLEE_SAVE_REGS_THUNK(xen_irq_enable);

static void xen_safe_halt(void)
{
	/* Blocking includes an implicit local_irq_enable(). */
	if (HYPERVISOR_sched_op(SCHEDOP_block, NULL) != 0)
		BUG();
}

static void xen_halt(void)
{
	if (irqs_disabled())
<<<<<<< HEAD
		HYPERVISOR_vcpu_op(VCPUOP_down, smp_processor_id(), NULL);
=======
		HYPERVISOR_vcpu_op(VCPUOP_down,
				   xen_vcpu_nr(smp_processor_id()), NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else
		xen_safe_halt();
}

static const struct pv_irq_ops xen_irq_ops __initconst = {
	.save_fl = PV_CALLEE_SAVE(xen_save_fl),
	.restore_fl = PV_CALLEE_SAVE(xen_restore_fl),
	.irq_disable = PV_CALLEE_SAVE(xen_irq_disable),
	.irq_enable = PV_CALLEE_SAVE(xen_irq_enable),

	.safe_halt = xen_safe_halt,
	.halt = xen_halt,
#ifdef CONFIG_X86_64
	.adjust_exception_frame = xen_adjust_exception_frame,
#endif
};

void __init xen_init_irq_ops(void)
{
<<<<<<< HEAD
	pv_irq_ops = xen_irq_ops;
=======
	/* For PVH we use default pv_irq_ops settings. */
	if (!xen_feature(XENFEAT_hvm_callback_vector))
		pv_irq_ops = xen_irq_ops;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	x86_init.irqs.intr_init = xen_init_IRQ;
}
