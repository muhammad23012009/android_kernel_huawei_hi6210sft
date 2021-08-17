/*
 * s390 specific pci instructions
 *
 * Copyright IBM Corp. 2013
 */

#include <linux/export.h>
#include <linux/errno.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <asm/pci_insn.h>
=======
#include <asm/facility.h>
#include <asm/pci_insn.h>
#include <asm/pci_debug.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/processor.h>

#define ZPCI_INSN_BUSY_DELAY	1	/* 1 microsecond */

<<<<<<< HEAD
=======
static inline void zpci_err_insn(u8 cc, u8 status, u64 req, u64 offset)
{
	struct {
		u64 req;
		u64 offset;
		u8 cc;
		u8 status;
	} __packed data = {req, offset, cc, status};

	zpci_err_hex(&data, sizeof(data));
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Modify PCI Function Controls */
static inline u8 __mpcifc(u64 req, struct zpci_fib *fib, u8 *status)
{
	u8 cc;

	asm volatile (
		"	.insn	rxy,0xe300000000d0,%[req],%[fib]\n"
		"	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		: [cc] "=d" (cc), [req] "+d" (req), [fib] "+Q" (*fib)
		: : "cc");
	*status = req >> 24 & 0xff;
	return cc;
}

<<<<<<< HEAD
int s390pci_mod_fc(u64 req, struct zpci_fib *fib)
=======
int zpci_mod_fc(u64 req, struct zpci_fib *fib)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u8 cc, status;

	do {
		cc = __mpcifc(req, fib, &status);
		if (cc == 2)
			msleep(ZPCI_INSN_BUSY_DELAY);
	} while (cc == 2);

	if (cc)
<<<<<<< HEAD
		printk_once(KERN_ERR "%s: error cc: %d  status: %d\n",
			     __func__, cc, status);
=======
		zpci_err_insn(cc, status, req, 0);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return (cc) ? -EIO : 0;
}

/* Refresh PCI Translations */
static inline u8 __rpcit(u64 fn, u64 addr, u64 range, u8 *status)
{
	register u64 __addr asm("2") = addr;
	register u64 __range asm("3") = range;
	u8 cc;

	asm volatile (
		"	.insn	rre,0xb9d30000,%[fn],%[addr]\n"
		"	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		: [cc] "=d" (cc), [fn] "+d" (fn)
		: [addr] "d" (__addr), "d" (__range)
		: "cc");
	*status = fn >> 24 & 0xff;
	return cc;
}

<<<<<<< HEAD
int s390pci_refresh_trans(u64 fn, u64 addr, u64 range)
=======
int zpci_refresh_trans(u64 fn, u64 addr, u64 range)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u8 cc, status;

	do {
		cc = __rpcit(fn, addr, range, &status);
		if (cc == 2)
			udelay(ZPCI_INSN_BUSY_DELAY);
	} while (cc == 2);

	if (cc)
<<<<<<< HEAD
		printk_once(KERN_ERR "%s: error cc: %d  status: %d  dma_addr: %Lx  size: %Lx\n",
			    __func__, cc, status, addr, range);
=======
		zpci_err_insn(cc, status, addr, range);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return (cc) ? -EIO : 0;
}

/* Set Interruption Controls */
<<<<<<< HEAD
void set_irq_ctrl(u16 ctl, char *unused, u8 isc)
{
	asm volatile (
		"	.insn	rsy,0xeb00000000d1,%[ctl],%[isc],%[u]\n"
		: : [ctl] "d" (ctl), [isc] "d" (isc << 27), [u] "Q" (*unused));
}

/* PCI Load */
static inline int __pcilg(u64 *data, u64 req, u64 offset, u8 *status)
=======
int zpci_set_irq_ctrl(u16 ctl, char *unused, u8 isc)
{
	if (!test_facility(72))
		return -EIO;
	asm volatile (
		"	.insn	rsy,0xeb00000000d1,%[ctl],%[isc],%[u]\n"
		: : [ctl] "d" (ctl), [isc] "d" (isc << 27), [u] "Q" (*unused));
	return 0;
}

/* PCI Load */
static inline int ____pcilg(u64 *data, u64 req, u64 offset, u8 *status)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	register u64 __req asm("2") = req;
	register u64 __offset asm("3") = offset;
	int cc = -ENXIO;
	u64 __data;

	asm volatile (
		"	.insn	rre,0xb9d20000,%[data],%[req]\n"
		"0:	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		"1:\n"
		EX_TABLE(0b, 1b)
		: [cc] "+d" (cc), [data] "=d" (__data), [req] "+d" (__req)
		:  "d" (__offset)
		: "cc");
	*status = __req >> 24 & 0xff;
<<<<<<< HEAD
=======
	*data = __data;
	return cc;
}

static inline int __pcilg(u64 *data, u64 req, u64 offset, u8 *status)
{
	u64 __data;
	int cc;

	cc = ____pcilg(&__data, req, offset, status);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!cc)
		*data = __data;

	return cc;
}

<<<<<<< HEAD
int s390pci_load(u64 *data, u64 req, u64 offset)
=======
int zpci_load(u64 *data, u64 req, u64 offset)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u8 status;
	int cc;

	do {
		cc = __pcilg(data, req, offset, &status);
		if (cc == 2)
			udelay(ZPCI_INSN_BUSY_DELAY);
	} while (cc == 2);

	if (cc)
<<<<<<< HEAD
		printk_once(KERN_ERR "%s: error cc: %d  status: %d  req: %Lx  offset: %Lx\n",
			    __func__, cc, status, req, offset);
	return (cc > 0) ? -EIO : cc;
}
EXPORT_SYMBOL_GPL(s390pci_load);
=======
		zpci_err_insn(cc, status, req, offset);

	return (cc > 0) ? -EIO : cc;
}
EXPORT_SYMBOL_GPL(zpci_load);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* PCI Store */
static inline int __pcistg(u64 data, u64 req, u64 offset, u8 *status)
{
	register u64 __req asm("2") = req;
	register u64 __offset asm("3") = offset;
	int cc = -ENXIO;

	asm volatile (
		"	.insn	rre,0xb9d00000,%[data],%[req]\n"
		"0:	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		"1:\n"
		EX_TABLE(0b, 1b)
		: [cc] "+d" (cc), [req] "+d" (__req)
		: "d" (__offset), [data] "d" (data)
		: "cc");
	*status = __req >> 24 & 0xff;
	return cc;
}

<<<<<<< HEAD
int s390pci_store(u64 data, u64 req, u64 offset)
=======
int zpci_store(u64 data, u64 req, u64 offset)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u8 status;
	int cc;

	do {
		cc = __pcistg(data, req, offset, &status);
		if (cc == 2)
			udelay(ZPCI_INSN_BUSY_DELAY);
	} while (cc == 2);

	if (cc)
<<<<<<< HEAD
		printk_once(KERN_ERR "%s: error cc: %d  status: %d  req: %Lx  offset: %Lx\n",
			__func__, cc, status, req, offset);
	return (cc > 0) ? -EIO : cc;
}
EXPORT_SYMBOL_GPL(s390pci_store);
=======
		zpci_err_insn(cc, status, req, offset);

	return (cc > 0) ? -EIO : cc;
}
EXPORT_SYMBOL_GPL(zpci_store);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* PCI Store Block */
static inline int __pcistb(const u64 *data, u64 req, u64 offset, u8 *status)
{
	int cc = -ENXIO;

	asm volatile (
		"	.insn	rsy,0xeb00000000d0,%[req],%[offset],%[data]\n"
		"0:	ipm	%[cc]\n"
		"	srl	%[cc],28\n"
		"1:\n"
		EX_TABLE(0b, 1b)
		: [cc] "+d" (cc), [req] "+d" (req)
		: [offset] "d" (offset), [data] "Q" (*data)
		: "cc");
	*status = req >> 24 & 0xff;
	return cc;
}

<<<<<<< HEAD
int s390pci_store_block(const u64 *data, u64 req, u64 offset)
=======
int zpci_store_block(const u64 *data, u64 req, u64 offset)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	u8 status;
	int cc;

	do {
		cc = __pcistb(data, req, offset, &status);
		if (cc == 2)
			udelay(ZPCI_INSN_BUSY_DELAY);
	} while (cc == 2);

	if (cc)
<<<<<<< HEAD
		printk_once(KERN_ERR "%s: error cc: %d  status: %d  req: %Lx  offset: %Lx\n",
			    __func__, cc, status, req, offset);
	return (cc > 0) ? -EIO : cc;
}
EXPORT_SYMBOL_GPL(s390pci_store_block);
=======
		zpci_err_insn(cc, status, req, offset);

	return (cc > 0) ? -EIO : cc;
}
EXPORT_SYMBOL_GPL(zpci_store_block);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
