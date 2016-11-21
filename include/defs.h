#ifndef __DEFS_H_
#define __DEFS_H_

/* To avoid overflows, make sure BUFFERSIZE_SEND is some multiple of 2 */
#define BUFFERSIZE_RECV 64
#define BUFFERSIZE_SEND 4096*4

/* Macro for removing "unused" warnings */
#define UNUSED(x) (void)(x)


#endif
