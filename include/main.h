#ifndef _MAIN_H
#define _MAIN_H

typedef enum mcu_state {
//SETUP,    /* Allocate memory, initialize variables, open USB ports */
  IDLE,     /* Wait for host */
//CONNECT,  /* Set up USB connection to PC and FPGA */
  RUN,      /* Recieve data from FPGA and send it to PC when transfer is done */
  TESTRUN,  /* Generate random data and send it to PC on USB */
  FINALIZE, /* Deallocate memory, shut down USB... */
} mcu_state_t;

/* run states can be merged into mcu states */
typedef enum run_state {
  READY, /* Initial state */
  READ_FPGA, /* Read everything from the FPGA over EBI and store it in a BUFFER */
  WRITE_PC,  /* Write everything from the MCU to PC over USB */
} run_state_t;

typedef struct state {
  mcu_state_t mcu_state;
  run_state_t run_state;
} state_t;

void init_state();
void mcu_chill();
void mcu_run();
void mcu_test_run();
void test_fpga_connection();

extern state_t state;
extern int* buffer;

#endif

