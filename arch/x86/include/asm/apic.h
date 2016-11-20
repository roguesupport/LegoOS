/*
 * Copyright (c) 2016 Wuklab, Purdue University. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef _ASM_X86_APIC_H_
#define _ASM_X86_APIC_H_

#include <asm/msr.h>
#include <asm/processor.h>

#include <lego/types.h>
#include <lego/compiler.h>

#define IO_APIC_DEFAULT_PHYS_BASE	0xfec00000
#define	APIC_DEFAULT_PHYS_BASE		0xfee00000

/*
 * This is the IO-APIC register space as specified
 * by Intel docs:
 */
#define IO_APIC_SLOT_SIZE		1024

#define	APIC_ID		0x20

#define	APIC_LVR	0x30
#define		APIC_LVR_MASK		0xFF00FF
#define		APIC_LVR_DIRECTED_EOI	(1 << 24)
#define		GET_APIC_VERSION(x)	((x) & 0xFFu)
#define		GET_APIC_MAXLVT(x)	(((x) >> 16) & 0xFFu)
#ifdef CONFIG_X86_32
#  define	APIC_INTEGRATED(x)	((x) & 0xF0u)
#else
#  define	APIC_INTEGRATED(x)	(1)
#endif
#define		APIC_XAPIC(x)		((x) >= 0x14)
#define		APIC_EXT_SPACE(x)	((x) & 0x80000000)
#define	APIC_TASKPRI	0x80
#define		APIC_TPRI_MASK		0xFFu
#define	APIC_ARBPRI	0x90
#define		APIC_ARBPRI_MASK	0xFFu
#define	APIC_PROCPRI	0xA0
#define	APIC_EOI	0xB0
#define		APIC_EOI_ACK		0x0 /* Docs say 0 for future compat. */
#define	APIC_RRR	0xC0
#define	APIC_LDR	0xD0
#define		APIC_LDR_MASK		(0xFFu << 24)
#define		GET_APIC_LOGICAL_ID(x)	(((x) >> 24) & 0xFFu)
#define		SET_APIC_LOGICAL_ID(x)	(((x) << 24))
#define		APIC_ALL_CPUS		0xFFu
#define	APIC_DFR	0xE0
#define		APIC_DFR_CLUSTER		0x0FFFFFFFul
#define		APIC_DFR_FLAT			0xFFFFFFFFul
#define	APIC_SPIV	0xF0
#define		APIC_SPIV_DIRECTED_EOI		(1 << 12)
#define		APIC_SPIV_FOCUS_DISABLED	(1 << 9)
#define		APIC_SPIV_APIC_ENABLED		(1 << 8)
#define	APIC_ISR	0x100
#define	APIC_ISR_NR     0x8     /* Number of 32 bit ISR registers. */
#define	APIC_TMR	0x180
#define	APIC_IRR	0x200
#define	APIC_ESR	0x280
#define		APIC_ESR_SEND_CS	0x00001
#define		APIC_ESR_RECV_CS	0x00002
#define		APIC_ESR_SEND_ACC	0x00004
#define		APIC_ESR_RECV_ACC	0x00008
#define		APIC_ESR_SENDILL	0x00020
#define		APIC_ESR_RECVILL	0x00040
#define		APIC_ESR_ILLREGA	0x00080
#define 	APIC_LVTCMCI	0x2f0
#define	APIC_ICR	0x300
#define		APIC_DEST_SELF		0x40000
#define		APIC_DEST_ALLINC	0x80000
#define		APIC_DEST_ALLBUT	0xC0000
#define		APIC_ICR_RR_MASK	0x30000
#define		APIC_ICR_RR_INVALID	0x00000
#define		APIC_ICR_RR_INPROG	0x10000
#define		APIC_ICR_RR_VALID	0x20000
#define		APIC_INT_LEVELTRIG	0x08000
#define		APIC_INT_ASSERT		0x04000
#define		APIC_ICR_BUSY		0x01000
#define		APIC_DEST_LOGICAL	0x00800
#define		APIC_DEST_PHYSICAL	0x00000
#define		APIC_DM_FIXED		0x00000
#define		APIC_DM_FIXED_MASK	0x00700
#define		APIC_DM_LOWEST		0x00100
#define		APIC_DM_SMI		0x00200
#define		APIC_DM_REMRD		0x00300
#define		APIC_DM_NMI		0x00400
#define		APIC_DM_INIT		0x00500
#define		APIC_DM_STARTUP		0x00600
#define		APIC_DM_EXTINT		0x00700
#define		APIC_VECTOR_MASK	0x000FF
#define	APIC_ICR2	0x310
#define		GET_APIC_DEST_FIELD(x)	(((x) >> 24) & 0xFF)
#define		SET_APIC_DEST_FIELD(x)	((x) << 24)
#define	APIC_LVTT	0x320
#define	APIC_LVTTHMR	0x330
#define	APIC_LVTPC	0x340
#define	APIC_LVT0	0x350
#define		APIC_LVT_TIMER_BASE_MASK	(0x3 << 18)
#define		GET_APIC_TIMER_BASE(x)		(((x) >> 18) & 0x3)
#define		SET_APIC_TIMER_BASE(x)		(((x) << 18))
#define		APIC_TIMER_BASE_CLKIN		0x0
#define		APIC_TIMER_BASE_TMBASE		0x1
#define		APIC_TIMER_BASE_DIV		0x2
#define		APIC_LVT_TIMER_ONESHOT		(0 << 17)
#define		APIC_LVT_TIMER_PERIODIC		(1 << 17)
#define		APIC_LVT_TIMER_TSCDEADLINE	(2 << 17)
#define		APIC_LVT_MASKED			(1 << 16)
#define		APIC_LVT_LEVEL_TRIGGER		(1 << 15)
#define		APIC_LVT_REMOTE_IRR		(1 << 14)
#define		APIC_INPUT_POLARITY		(1 << 13)
#define		APIC_SEND_PENDING		(1 << 12)
#define		APIC_MODE_MASK			0x700
#define		GET_APIC_DELIVERY_MODE(x)	(((x) >> 8) & 0x7)
#define		SET_APIC_DELIVERY_MODE(x, y)	(((x) & ~0x700) | ((y) << 8))
#define			APIC_MODE_FIXED		0x0
#define			APIC_MODE_NMI		0x4
#define			APIC_MODE_EXTINT	0x7
#define	APIC_LVT1	0x360
#define	APIC_LVTERR	0x370
#define	APIC_TMICT	0x380
#define	APIC_TMCCT	0x390
#define	APIC_TDCR	0x3E0
#define APIC_SELF_IPI	0x3F0
#define		APIC_TDR_DIV_TMBASE	(1 << 2)
#define		APIC_TDR_DIV_1		0xB
#define		APIC_TDR_DIV_2		0x0
#define		APIC_TDR_DIV_4		0x1
#define		APIC_TDR_DIV_8		0x2
#define		APIC_TDR_DIV_16		0x3
#define		APIC_TDR_DIV_32		0x8
#define		APIC_TDR_DIV_64		0x9
#define		APIC_TDR_DIV_128	0xA
#define	APIC_EFEAT	0x400
#define	APIC_ECTRL	0x410
#define APIC_EILVTn(n)	(0x500 + 0x10 * n)
#define		APIC_EILVT_NR_AMD_K8	1	/* # of extended interrupts */
#define		APIC_EILVT_NR_AMD_10H	4
#define		APIC_EILVT_NR_MAX	APIC_EILVT_NR_AMD_10H
#define		APIC_EILVT_LVTOFF(x)	(((x) >> 4) & 0xF)
#define		APIC_EILVT_MSG_FIX	0x0
#define		APIC_EILVT_MSG_SMI	0x2
#define		APIC_EILVT_MSG_NMI	0x4
#define		APIC_EILVT_MSG_EXT	0x7
#define		APIC_EILVT_MASKED	(1 << 16)

#define APIC_BASE	(fix_to_virt(FIX_APIC_BASE))
#define APIC_BASE_MSR	0x800
#define XAPIC_ENABLE	(1UL << 11)
#define X2APIC_ENABLE	(1UL << 10)

#ifdef CONFIG_X86_32
# define MAX_IO_APICS 64
# define MAX_LOCAL_APIC 256
#else
# define MAX_IO_APICS 128
# define MAX_LOCAL_APIC 32768
#endif

/*
 * All x86-64 systems are xAPIC compatible.
 * In the following, "apicid" is a physical APIC ID.
 */
#define XAPIC_DEST_CPUS_SHIFT	4
#define XAPIC_DEST_CPUS_MASK	((1u << XAPIC_DEST_CPUS_SHIFT) - 1)
#define XAPIC_DEST_CLUSTER_MASK	(XAPIC_DEST_CPUS_MASK << XAPIC_DEST_CPUS_SHIFT)
#define APIC_CLUSTER(apicid)	((apicid) & XAPIC_DEST_CLUSTER_MASK)
#define APIC_CLUSTERID(apicid)	(APIC_CLUSTER(apicid) >> XAPIC_DEST_CPUS_SHIFT)
#define APIC_CPUID(apicid)	((apicid) & XAPIC_DEST_CPUS_MASK)
#define NUM_APIC_CLUSTERS	((BAD_APICID + 1) >> XAPIC_DEST_CPUS_SHIFT)

struct local_apic {

/*000*/	struct { u32 __reserved[4]; } __reserved_01;

/*010*/	struct { u32 __reserved[4]; } __reserved_02;

/*020*/	struct { /* APIC ID Register */
		u32   __reserved_1	: 24,
			phys_apic_id	:  4,
			__reserved_2	:  4;
		u32 __reserved[3];
	} id;

/*030*/	const
	struct { /* APIC Version Register */
		u32   version		:  8,
			__reserved_1	:  8,
			max_lvt		:  8,
			__reserved_2	:  8;
		u32 __reserved[3];
	} version;

/*040*/	struct { u32 __reserved[4]; } __reserved_03;

/*050*/	struct { u32 __reserved[4]; } __reserved_04;

/*060*/	struct { u32 __reserved[4]; } __reserved_05;

/*070*/	struct { u32 __reserved[4]; } __reserved_06;

/*080*/	struct { /* Task Priority Register */
		u32   priority	:  8,
			__reserved_1	: 24;
		u32 __reserved_2[3];
	} tpr;

/*090*/	const
	struct { /* Arbitration Priority Register */
		u32   priority	:  8,
			__reserved_1	: 24;
		u32 __reserved_2[3];
	} apr;

/*0A0*/	const
	struct { /* Processor Priority Register */
		u32   priority	:  8,
			__reserved_1	: 24;
		u32 __reserved_2[3];
	} ppr;

/*0B0*/	struct { /* End Of Interrupt Register */
		u32   eoi;
		u32 __reserved[3];
	} eoi;

/*0C0*/	struct { u32 __reserved[4]; } __reserved_07;

/*0D0*/	struct { /* Logical Destination Register */
		u32   __reserved_1	: 24,
			logical_dest	:  8;
		u32 __reserved_2[3];
	} ldr;

/*0E0*/	struct { /* Destination Format Register */
		u32   __reserved_1	: 28,
			model		:  4;
		u32 __reserved_2[3];
	} dfr;

/*0F0*/	struct { /* Spurious Interrupt Vector Register */
		u32	spurious_vector	:  8,
			apic_enabled	:  1,
			focus_cpu	:  1,
			__reserved_2	: 22;
		u32 __reserved_3[3];
	} svr;

/*100*/	struct { /* In Service Register */
/*170*/		u32 bitfield;
		u32 __reserved[3];
	} isr [8];

/*180*/	struct { /* Trigger Mode Register */
/*1F0*/		u32 bitfield;
		u32 __reserved[3];
	} tmr [8];

/*200*/	struct { /* Interrupt Request Register */
/*270*/		u32 bitfield;
		u32 __reserved[3];
	} irr [8];

/*280*/	union { /* Error Status Register */
		struct {
			u32   send_cs_error			:  1,
				receive_cs_error		:  1,
				send_accept_error		:  1,
				receive_accept_error		:  1,
				__reserved_1			:  1,
				send_illegal_vector		:  1,
				receive_illegal_vector		:  1,
				illegal_register_address	:  1,
				__reserved_2			: 24;
			u32 __reserved_3[3];
		} error_bits;
		struct {
			u32 errors;
			u32 __reserved_3[3];
		} all_errors;
	} esr;

/*290*/	struct { u32 __reserved[4]; } __reserved_08;

/*2A0*/	struct { u32 __reserved[4]; } __reserved_09;

/*2B0*/	struct { u32 __reserved[4]; } __reserved_10;

/*2C0*/	struct { u32 __reserved[4]; } __reserved_11;

/*2D0*/	struct { u32 __reserved[4]; } __reserved_12;

/*2E0*/	struct { u32 __reserved[4]; } __reserved_13;

/*2F0*/	struct { u32 __reserved[4]; } __reserved_14;

/*300*/	struct { /* Interrupt Command Register 1 */
		u32   vector			:  8,
			delivery_mode		:  3,
			destination_mode	:  1,
			delivery_status		:  1,
			__reserved_1		:  1,
			level			:  1,
			trigger			:  1,
			__reserved_2		:  2,
			shorthand		:  2,
			__reserved_3		:  12;
		u32 __reserved_4[3];
	} icr1;

/*310*/	struct { /* Interrupt Command Register 2 */
		union {
			u32   __reserved_1	: 24,
				phys_dest	:  4,
				__reserved_2	:  4;
			u32   __reserved_3	: 24,
				logical_dest	:  8;
		} dest;
		u32 __reserved_4[3];
	} icr2;

/*320*/	struct { /* LVT - Timer */
		u32   vector		:  8,
			__reserved_1	:  4,
			delivery_status	:  1,
			__reserved_2	:  3,
			mask		:  1,
			timer_mode	:  1,
			__reserved_3	: 14;
		u32 __reserved_4[3];
	} lvt_timer;

/*330*/	struct { /* LVT - Thermal Sensor */
		u32  vector		:  8,
			delivery_mode	:  3,
			__reserved_1	:  1,
			delivery_status	:  1,
			__reserved_2	:  3,
			mask		:  1,
			__reserved_3	: 15;
		u32 __reserved_4[3];
	} lvt_thermal;

/*340*/	struct { /* LVT - Performance Counter */
		u32   vector		:  8,
			delivery_mode	:  3,
			__reserved_1	:  1,
			delivery_status	:  1,
			__reserved_2	:  3,
			mask		:  1,
			__reserved_3	: 15;
		u32 __reserved_4[3];
	} lvt_pc;

/*350*/	struct { /* LVT - LINT0 */
		u32   vector		:  8,
			delivery_mode	:  3,
			__reserved_1	:  1,
			delivery_status	:  1,
			polarity	:  1,
			remote_irr	:  1,
			trigger		:  1,
			mask		:  1,
			__reserved_2	: 15;
		u32 __reserved_3[3];
	} lvt_lint0;

/*360*/	struct { /* LVT - LINT1 */
		u32   vector		:  8,
			delivery_mode	:  3,
			__reserved_1	:  1,
			delivery_status	:  1,
			polarity	:  1,
			remote_irr	:  1,
			trigger		:  1,
			mask		:  1,
			__reserved_2	: 15;
		u32 __reserved_3[3];
	} lvt_lint1;

/*370*/	struct { /* LVT - Error */
		u32   vector		:  8,
			__reserved_1	:  4,
			delivery_status	:  1,
			__reserved_2	:  3,
			mask		:  1,
			__reserved_3	: 15;
		u32 __reserved_4[3];
	} lvt_error;

/*380*/	struct { /* Timer Initial Count Register */
		u32   initial_count;
		u32 __reserved_2[3];
	} timer_icr;

/*390*/	const
	struct { /* Timer Current Count Register */
		u32   curr_count;
		u32 __reserved_2[3];
	} timer_ccr;

/*3A0*/	struct { u32 __reserved[4]; } __reserved_16;

/*3B0*/	struct { u32 __reserved[4]; } __reserved_17;

/*3C0*/	struct { u32 __reserved[4]; } __reserved_18;

/*3D0*/	struct { u32 __reserved[4]; } __reserved_19;

/*3E0*/	struct { /* Timer Divide Configuration Register */
		u32   divisor		:  4,
			__reserved_1	: 28;
		u32 __reserved_2[3];
	} timer_dcr;

/*3F0*/	struct { u32 __reserved[4]; } __reserved_20;

} __attribute__ ((packed));

#ifdef CONFIG_X86_32
 #define BAD_APICID 0xFFu
#else
 #define BAD_APICID 0xFFFFu
#endif

enum ioapic_irq_destination_types {
	dest_Fixed		= 0,
	dest_LowestPrio		= 1,
	dest_SMI		= 2,
	dest__reserved_1	= 3,
	dest_NMI		= 4,
	dest_INIT		= 5,
	dest__reserved_2	= 6,
	dest_ExtINT		= 7
};

struct apic {
	const char *name;

	int (*probe)(void);
	int (*apic_id_valid)(int apicid);
	int (*apic_id_registered)(void);

	int irq_delivery_mode;
	int irq_dest_mode;

	unsigned int (*get_apic_id)(unsigned long x);
	unsigned long (*set_apic_id)(unsigned int id);

	/* ipi */
	void (*send_IPI)(int cpu, int vector);
	void (*send_IPI_allbutself)(int vector);
	void (*send_IPI_all)(int vector);
	void (*send_IPI_self)(int vector);

	/* wakeup_secondary_cpu */
	int (*wakeup_secondary_cpu)(int apicid, unsigned long start_eip);

	void (*inquire_remote_apic)(int apicid);

	/* apic ops */
	u32 (*read)(u32 reg);
	void (*write)(u32 reg, u32 v);
	/*
	 * ->eoi_write() has the same signature as ->write().
	 *
	 * Drivers can support both ->eoi_write() and ->write() by passing the same
	 * callback value. Kernel can override ->eoi_write() and fall back
	 * on write for EOI.
	 */
	void (*eoi_write)(u32 reg, u32 v);
	u64 (*icr_read)(void);
	void (*icr_write)(u32 low, u32 high);
	void (*wait_icr_idle)(void);
	u32 (*safe_wait_icr_idle)(void);

};

/*
 * Pointer to the local APIC driver in use on this system (there's
 * always just one such driver in use - the kernel decides via an
 * early probing process which one it picks - and then sticks to it):
 */
extern struct apic *apic;

/*
 * APIC drivers are probed based on how they are listed in the .apicdrivers
 * section. So the order is important and enforced by the ordering
 * of different apic driver files in the Makefile.
 *
 * For the files having two apic drivers, we use apic_drivers()
 * to enforce the order with in them.
 */
#define apic_driver(sym)						\
	static const struct apic *__apicdrivers_##sym __used		\
	__aligned(sizeof(struct apic *))				\
	__section(.apicdrivers) = { &sym }

#define apic_drivers(sym1, sym2)					\
	static struct apic *__apicdrivers_##sym1##sym2[2] __used	\
	__aligned(sizeof(struct apic *))				\
	__section(.apicdrivers) = { &sym1, &sym2 }

extern struct apic *__apicdrivers[], *__apicdrivers_end[];

static inline void native_apic_msr_write(u32 reg, u32 v)
{
	if (reg == APIC_DFR || reg == APIC_ID || reg == APIC_LDR ||
	    reg == APIC_LVR)
		return;

	wrmsr(APIC_BASE_MSR + (reg >> 4), v, 0);
}

static inline void native_apic_msr_eoi_write(u32 reg, u32 v)
{
	wrmsr(APIC_BASE_MSR + (APIC_EOI >> 4), APIC_EOI_ACK, 0);
}

static inline u32 native_apic_msr_read(u32 reg)
{
	u64 msr;

	if (reg == APIC_DFR)
		return -1;

	rdmsrl(APIC_BASE_MSR + (reg >> 4), msr);
	return (u32)msr;
}

#ifdef CONFIG_X86_X2APIC
extern int x2apic_mode;
extern int x2apic_phys;

void __init check_x2apic(void);

static inline int apic_is_x2apic_enabled(void)
{
	u64 msr;

	rdmsrl(MSR_IA32_APICBASE, msr);
	return msr & X2APIC_ENABLE;
}

static inline int x2apic_enabled(void)
{
	return cpu_has(X86_FEATURE_X2APIC) && apic_is_x2apic_enabled();
}

static inline int x2apic_supported(void)
{
	return cpu_has(X86_FEATURE_X2APIC);
}
#else
static inline void check_x2apic(void) { }
static inline void x2apic_setup(void) { }
static inline int x2apic_enabled(void) { return 0; }
static inline int x2apic_supported(void) { return 0; }
#endif /* CONFIG_X86_X2APIC */

void __init setup_apic_driver(void);

void native_apic_wait_icr_idle(void);
u32 native_safe_apic_wait_icr_idle(void);
void native_apic_icr_write(u32 low, u32 id);
u64 native_apic_icr_read(void);

#ifdef CONFIG_X86_LOCAL_APIC

static inline u32 apic_read(u32 reg)
{
	return apic->read(reg);
}

static inline void apic_write(u32 reg, u32 val)
{
	apic->write(reg, val);
}

static inline void apic_eoi(void)
{
	apic->eoi_write(APIC_EOI, APIC_EOI_ACK);
}

static inline u64 apic_icr_read(void)
{
	return apic->icr_read();
}

static inline void apic_icr_write(u32 low, u32 high)
{
	apic->icr_write(low, high);
}

static inline void apic_wait_icr_idle(void)
{
	apic->wait_icr_idle();
}

static inline u32 safe_apic_wait_icr_idle(void)
{
	return apic->safe_wait_icr_idle();
}

extern void __init apic_set_eoi_write(void (*eoi_write)(u32 reg, u32 v));

#else
static inline u32 apic_read(u32 reg) { return 0; }
static inline void apic_write(u32 reg, u32 val) { }
static inline void apic_eoi(void) { }
static inline u64 apic_icr_read(void) { return 0; }
static inline void apic_icr_write(u32 low, u32 high) { }
static inline void apic_wait_icr_idle(void) { }
static inline u32 safe_apic_wait_icr_idle(void) { return 0; }
static inline void apic_set_eoi_write(void (*eoi_write)(u32 reg, u32 v)) {}
#endif  /* CONFIG_X86_LOCAL_APIC */

#endif /* _ASM_X86_APIC_H_ */
