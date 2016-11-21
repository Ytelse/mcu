#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "libusb.h"

#include "usb_device.h"
#include "callbacks.h"

#define UNUSED(x) (void) x

#define DBG_OUTPUT_FP "data_received.txt"
#define DBG_DC_OUTPUT_FP "decompressed_received.txt"

#define RECVTIMEOUT 1000
#define SENDTIMEOUT 1000

#define BUFFERSIZE_R 4096*4
#define BUFFERSIZE_S 64

#define TEST_DURATION 2.0f

enum {SEND, RECV, BOTH};

unsigned char usb_recv_buffer[BUFFERSIZE_R];
unsigned char start_msg[] = "run";
unsigned char stop_msg[] = "stop";

bool pending_recv, pending_write;

/* Function prototypes */
static void recv_async(libusb_device_handle* dev_handle, unsigned char* buffer, int buflen);
static void send_async(libusb_device_handle* dev_handle, unsigned char* buffer, int buflen);

int main(int argc, char** argv) {
	int rc;

	struct timeval start, end;
	long int ms;
	double s;

	fprintf(stdout, "\x1b[32m========================================================\x1b[0m\n");
	fprintf(stdout, "\x1b[36m                    USB <-> MCU <-> FPGA                \x1b[0m\n");
	fprintf(stdout, "\x1b[32m========================================================\x1b[0m\n");

	/* Debug output file */
	FILE* f;
	f = fopen(DBG_OUTPUT_FP, "w");
	assert(f);

	/* libusb variables */
	libusb_context* context = NULL;
	libusb_init(&context);
	libusb_set_debug(context, 3);

	libusb_device_handle* dev_handle = NULL;

	rc = connect(context, &dev_handle, 0);

	if (rc) {
		printf("Connection failed. Exiting...\n");
		return -1;
	}

	char* buffer = NULL;
	int read; size_t len;
	read = getline(&buffer, &len, stdin);

	int recvcount, loopcount;
	recvcount = loopcount = 0;

	memset(usb_recv_buffer, 0, BUFFERSIZE_R);

	gettimeofday(&start, NULL);

	/* Send start message to MCU */
	send_async(dev_handle, start_msg, 64);
	printf("Sent start message!\n");
	/* Set up first receive from MCU */
	recv_async(dev_handle, usb_recv_buffer, BUFFERSIZE_R);
	fprintf(stdout, "\rReceived transfers: %d", recvcount);
	fflush(stdout);

	while (1) {
		loopcount++;

		if (!pending_recv) {

			fwrite(usb_recv_buffer, sizeof(unsigned char), BUFFERSIZE_R, f);
			recvcount++;
			fprintf(stdout, "\rReceived transfers: %d, duration: %.3f, transfer speed: %.3f Mbps", recvcount, s, ((double)recvcount*BUFFERSIZE_R*8/1e6)/s);
			fflush(stdout);
			recv_async(dev_handle, usb_recv_buffer, BUFFERSIZE_R);
		}

		libusb_handle_events(context);

		gettimeofday(&end, NULL);
		ms = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
		s = ms/1e6;
		if (s >= TEST_DURATION) {
			send_async(dev_handle, stop_msg, 4);
			printf("\nSent stop message!\n");
			break;
		}
	}

	if (pending_recv || pending_write) {
		while (pending_recv || pending_write) {
			libusb_handle_events(context);
		}
		fprintf(f, "%s\n", usb_recv_buffer);
		recvcount++;
	}
	fprintf(stdout, "\x1b[32m========================================================\x1b[0m\n");
	fprintf(stdout, "Duration:                            %f\n", s);
	fprintf(stdout, "Loops:                               %d\n", loopcount);
	fprintf(stdout, "Messages received:                   %d\n", recvcount);
	fprintf(stdout, "Bytes/receive transfer:              %d\n", BUFFERSIZE_R);
	fprintf(stdout, "Data transferred:                    %f Mb\n", (double)recvcount*BUFFERSIZE_R*8/1e6);
	fprintf(stdout, "Avg. transfer speed:                 %f Mbps\n", ((double)recvcount*BUFFERSIZE_R*8/1e6)/s);
	fprintf(stdout, "\x1b[32m========================================================\x1b[0m\n");

	fclose(f);

	fprintf(stdout, "Decompressing data...\n");

	f = fopen(DBG_OUTPUT_FP, "rb");
	FILE* fout = fopen(DBG_DC_OUTPUT_FP, "w");

	while(!feof(f)) {
		unsigned char byte = getc(f);
		putc((unsigned char) ((byte >> 4)), fout); 		// High 4 bits
		putc((unsigned char) ((byte & 0x0f)), fout);	// Low 4 bits
	}

	fclose(fout);

	libusb_release_interface(dev_handle, 0);
	libusb_close(dev_handle);
	libusb_exit(context);
}

static void recv_async(libusb_device_handle* dev_handle, unsigned char* buffer, int buflen) {
	struct libusb_transfer* transfer = NULL;
	transfer = libusb_alloc_transfer(0);

	libusb_fill_bulk_transfer(
		transfer,
		dev_handle,
		EP_IN,
		buffer,
		buflen,
		&data_received_callback,
		NULL,
		RECVTIMEOUT
	);

	libusb_submit_transfer(transfer);

	pending_recv = true;
}

static void send_async(libusb_device_handle* dev_handle, unsigned char* buffer, int buflen) {
	struct libusb_transfer* transfer = NULL;
	transfer = libusb_alloc_transfer(0);

	libusb_fill_bulk_transfer(
		transfer,
		dev_handle,
		EP_OUT,
		buffer,
		buflen,
		&data_sent_callback,
		NULL,
		SENDTIMEOUT
	);

	libusb_submit_transfer(transfer);

	pending_write = true;
}

