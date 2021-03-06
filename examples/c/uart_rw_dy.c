
/* standard headers */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/uart.h"

#ifndef FALSE
#define FALSE 0
#define TRUE (!FALSE)
#endif

/* UART port name */
const char* dev_path = "/dev/ttyS4";

volatile sig_atomic_t flag = 1;

void
sig_handler(int signum)
{
    if (signum == SIGINT) {
        fprintf(stdout, "Exiting...\n");
        flag = 0;
    }
}

int
main(int argc, char** argv)
{
    mraa_result_t status = MRAA_SUCCESS;
    mraa_uart_context uart;
//    char buffer[] = "Hello Mraa!";
   char buffer1[20];	

    int baudrate = 9600, stopbits = 1, databits = 8;
    mraa_uart_parity_t parity = MRAA_UART_PARITY_NONE;
    unsigned int ctsrts = FALSE, xonxoff = FALSE;
    const char* name = NULL;

    /* install signal handler */
    signal(SIGINT, sig_handler);

    /* initialize mraa for the platform (not needed most of the time) */
    mraa_init();

    //! [Interesting]
    /* initialize uart */
    uart = mraa_uart_init_raw(dev_path);
    if (uart == NULL) {
        fprintf(stderr, "Failed to initialize UART\n");
        return EXIT_FAILURE;
    }

    /* set serial port parameters */
    status = mraa_uart_settings(-1, &dev_path, &name, &baudrate, &databits, &stopbits, &parity, &ctsrts, &xonxoff);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    while (flag) {
        /* send data through uart */
//      printf("Enter Message:");
      buffer1[0]='\0';
      scanf("%s",buffer1);
        mraa_uart_write(uart, buffer1, sizeof(buffer1));
        sleep(2);
	mraa_uart_read(uart,buffer1,sizeof(buffer1));

            printf("Data Read: %s\n",buffer1);
	      
	
    }

    /* stop uart */
    mraa_uart_stop(uart);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the time) */
    mraa_deinit();

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* stop uart */
    mraa_uart_stop(uart);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;
}
