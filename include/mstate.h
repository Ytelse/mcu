#ifndef __MSTATE_H_
#define __MSTATE_H_

#include "em_device.h"
#include "em_bus.h"

/* Address of status register */
/* WARNING: VERY UNSAFE, MAY LEAD TO BUGS THAT ARE REALLY HARD TO DEBUG */
/* IF SOME WEIRD, UNEXCPECTED STUFF HAPPENS, CHECK THIS FIRST! */

/**
 * TODO: If the inlined ASM works remove:
 *			-- MSTATE_STATUS_REG_BASE
 *			-- MSTATE_TypeDef
 *			-- MSTATE
 *			-- _MSTATE_RESERVED15
 *			-- _MSTATE_RESERVED16
 *			-- _MSTATE_RESERVED17
 *			-- _MSTATE_RESERVED18
 *			-- _MSTATE_RESERVED19
 */

#define MSTATE_STATUS_REG_BASE 	0x1001FFF0UL	/* SRAM base adress + 0x000FFFF0 */

typedef struct {
	__IOM uint32_t STATUS;		/* Status Register  */
	__IOM uint32_t RESERVED;	/* Reserved Register  */
} MSTATE_TypeDef;

#define MSTATE 					((MSTATE_TypeDef*) MSTATE_STATUS_REG_BASE)

/* STATE flags */

#define MSTATE_MCU_RUN			0x00000001	/* MCU in RUNNING state */
#define MSTATE_MCU_WAIT			0x00000002	/* MCU in WAITING state */
#define MSTATE_MCU_HALT			0x00000004	/* MCU halted */

#define _MSTATE_MCU_RESERVED 	0x00000008	/* Reserved for future use */

#define MSTATE_BUF_SEL_0		0x00000010 	/* Use image buffer 0 */
#define MSTATE_BUF_SEL_1		0x00000020 	/* Use image buffer 1 */
#define MSTATE_BUF_0_FULL		0x00000040 	/* Image buffer 0 full */
#define MSTATE_BUF_1_FULL		0x00000080 	/* Image buffer 1 full */
#define MSTATE_BUF_0_RDY		0x00000100 	/* Image buffer 0 empty */
#define MSTATE_BUF_1_RDY		0x00000200 	/* Image buffer 1 empty */

#define _MSTATE_BUF_0_RESERVED	0x00000400 	/* Reserved for future use */
#define _MSTATE_BUF_1_RESERVED	0x00000800 	/* Reserved for future use */

#define _MSTATE_RESERVED0		0x00001000  /* Reserved for future use */
#define _MSTATE_RESERVED1		0x00002000  /* Reserved for future use */
#define _MSTATE_RESERVED2		0x00004000  /* Reserved for future use */
#define _MSTATE_RESERVED3		0x00008000  /* Reserved for future use */

#define _MSTATE_RESERVED4		0x00010000  /* Reserved for future use */
#define _MSTATE_RESERVED5		0x00020000  /* Reserved for future use */
#define _MSTATE_RESERVED6		0x00040000  /* Reserved for future use */
#define _MSTATE_RESERVED7		0x00080000  /* Reserved for future use */

#define _MSTATE_RESERVED8		0x00100000  /* Reserved for future use */
#define _MSTATE_RESERVED9		0x00200000  /* Reserved for future use */
#define _MSTATE_RESERVED10		0x00400000  /* Reserved for future use */
#define _MSTATE_RESERVED11		0x00800000  /* Reserved for future use */

#define _MSTATE_RESERVED12		0x01000000  /* Reserved for future use */
#define _MSTATE_RESERVED13		0x02000000  /* Reserved for future use */
#define _MSTATE_RESERVED14		0x04000000  /* Reserved for future use */
#define _MSTATE_RESERVED15		0x08000000  /* Reserved for future use */

#define _MSTATE_RESERVED16		0x10000000  /* Reserved for future use */
#define _MSTATE_RESERVED17		0x20000000  /* Reserved for future use */
#define _MSTATE_RESERVED18		0x40000000  /* Reserved for future use */
#define _MSTATE_RESERVED19		0x80000000  /* Reserved for future use */

typedef struct {
	uint32_t mcuState;
	uint32_t bufSelect;
	/* Add others as necessary */
} MSTATE_init_struct_t;

void MSTATE_init(MSTATE_init_struct_t* initstruct);

/* ========== EXPERIMENTAL SECTION ============ */

/**
 * This section contains functions with inlined ASM for getting the contents
 * of the core APSR register. This is a special read/write register in the MCU core
 * that can be accessed even when not in a privileged section.
 * The APSR register only uses the 5 most significant bits APSR[31:27], the rest being
 * being reserved for future use. This means that there is a possibility that we can
 * exploit APSR[26:0] for our own use.
 */


#define __APSR_Q				0x08000000  /* Bit used by APSR */
#define __APSR_V				0x10000000  /* Bit used by APSR */
#define __APSR_C				0x20000000  /* Bit used by APSR */
#define __APSR_Z				0x40000000  /* Bit used by APSR */
#define __APSR_N				0x80000000  /* Bit used by APSR */

/**
 * Get APSR register.
 *
 * Inlined ASM for fetching the contents of the APSR register. 
 *
 * TODO: Figure out if we also will need a __MSTATE_get_atomic(void).
 * 
 * TODO: Test this function. Attempt several reads and see how the 
 *		 register changes over time. See if the reserved bits are
 *		 ever being written to.
 * 
 * WARNING: There is a possibility that this does not work at all.
 */

__attribute__((always_inline)) __STATIC_INLINE uint32_t __MSTATE_get_all(void) {
	uint32_t result;
	__ASM volatile ("MRS %0, apsr" : "=r" (result));
	return(result);
}

/**
 * Get specific MSTATE flag.
 *
 * Inlined function for getting the value of a specific MSTATE flag.
 *
 * TODO: Test this function. See __MSTATE_get_all() for how to test.
 *
 * WARNING: There is a possibility that this does not work at all.
 */

__attribute__((always_inline)) __STATIC_INLINE unsigned int __MSTATE_get(uint32_t flag) {
	return (flag & __MSTATE_get_all());
}

/**
 * Set specific flag(s) to 1.
 *
 * Inlined ASM for setting the contents of the APSR register in
 * a -- hopefully -- safe manner using read-modify-write.
 * This means we first read the contents of the register, then OR it with
 * the new flag(s) before writing it back.
 * 
 * TODO: Figure out how to make this operation atomic. As of now, this method can be
 * interrupted, and thus preventing the register from being updated.
 * 
 * TODO: Test this function. Attempt to write to the register and see if the
 *		 unused/reserved bits retain their value over many cycles, or if they are
 * 		 being overwritten by some other function/procedure.
 *
 * WARNING: There is a possibility that this does not work at all.
 */

__attribute__((always_inline)) __STATIC_INLINE void __MSTATE_set(uint32_t flags) {
	uint32_t value = __MSTATE_get_all();						/* Read   */
	value |= flags;												/* Modify */
	__ASM volatile ("MSR apsr, %0" : : "r" (value) : "memory"); /* Write  */
} 

/**
 * Clear specific flag(s) to 0.
 *
 * Inlined ASM for clearing the contents of the APSR register in
 * a -- hopefully -- safe manner using read-modify-write.
 * This means we first read the contents of the register, then AND it with
 * the bit-wise NOT of flags before writing it back.
 * 
 * TODO: Figure out how to make this operation atomic. As of now, this method can be
 * interrupted, and thus preventing the register from being updated.
 * 
 * TODO: Test this function. Attempt to write to the register and see if the
 *		 unused/reserved bits retain their value over many cycles, or if they are
 * 		 being overwritten by some other function/procedure.
 *
 * WARNING: There is a possibility that this does not work at all.
 */

__attribute__((always_inline)) __STATIC_INLINE void __MSTATE_clear(uint32_t flags) {
	uint32_t value = __MSTATE_get_all();
	value &= ~flags;
	__ASM volatile ("MSR apsr, %0" : : "r" (value) : "memory");
}

/**
 * Testing so far shows negative results when attempting to write to
 * APSR register.
 */

/* ========== END OF EXPERIMENTAL SECTION ============ */


/**
 * Inlined function for getting MSTATE status flags.
 */

__STATIC_INLINE uint32_t MSTATE_get_all(void) {
	return MSTATE->STATUS;
}

__STATIC_INLINE unsigned int MSTATE_get(uint32_t flag) {
	return BUS_RegBitRead(&MSTATE->STATUS, flag);
}

/**
 * Inlined function for setting MSTATE status flag(s).
 */

__STATIC_INLINE void MSTATE_set(uint32_t flags) {
	MSTATE->STATUS |= flags;
}

/**
 * Inlined function for clearing MSTATE status flag(s).
 */

__STATIC_INLINE void MSTATE_clear(uint32_t flags) {
	MSTATE->STATUS &= ~(flags);
}


#endif /* __MSTATE_H_ */