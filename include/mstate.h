#ifndef __MSTATE_H_
#define __MSTATE_H_

/* STATE flags */

#define MSTATE_MCU_RUN			0x00000001	/* MCU in RUNNING state */
#define MSTATE_MCU_WAIT			0x00000002	/* MCU in WAITING state */
#define MSTATE_MCU_HALT			0x00000004	/* MCU halted */

#define _MSTATE_MCU_RESERVED 	0x00000008	/* Reserved for future use */

#define MSTATE_BUF_SEL			0x00000010 	/* Buffer select, if 0, use image buffer 0, if 1 use image buffer 1 */
#define _MSTATE_BUF_RESERVED	0x00000020 	/* Reserved for future use */
#define MSTATE_BUF_0_FULL		0x00000040 	/* Image buffer 0 full */
#define MSTATE_BUF_1_FULL		0x00000080 	/* Image buffer 1 full */
#define MSTATE_BUF_0_RDY		0x00000100 	/* Image buffer 0 empty */
#define MSTATE_BUF_1_RDY		0x00000200 	/* Image buffer 1 empty */

#define _MSTATE_BUF_0_RESERVED	0x00000400 	/* Reserved for future use */
#define _MSTATE_BUF_1_RESERVED	0x00000800 	/* Reserved for future use */

#define MSTATE_CUR_BUF_RDY		0x00001000  /* Reserved for future use */
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

#endif /* __MSTATE_H_ */