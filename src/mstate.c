#include "mstate.h"

void MSTATE_init(uint32_t* MSTATE, MSTATE_init_struct_t* initstruct)  {
	/* Set initial state */
	MSTATE_set(MSTATE, initstruct->mcuState);

	if (initstruct->bufSelect) {
		MSTATE_set(MSTATE, MSTATE_BUF_SEL);
		MSTATE_clear(MSTATE, MSTATE_BUF_1_FULL);
		MSTATE_set(MSTATE, MSTATE_BUF_1_RDY);
	} else {
		MSTATE_clear(MSTATE, MSTATE_BUF_SEL);
		MSTATE_clear(MSTATE, MSTATE_BUF_0_FULL);
		MSTATE_set(MSTATE, MSTATE_BUF_0_RDY);
	}
}