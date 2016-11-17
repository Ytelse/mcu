#include "mstate.h"

void MSTATE_init(MSTATE_init_struct_t* initstruct)  {
	/* Set initial state */
	MSTATE_set(initstruct->mcuState | initstruct->bufSelect);
}

void __MSTATE_init(MSTATE_init_struct_t* initstruct) {
	__MSTATE_set(initstruct->mcuState | initstruct->bufSelect);
}