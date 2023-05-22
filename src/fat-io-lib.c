#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "spi.h"
#include "sd.h"
#include "timer.h"
#include "fat_filelib.h"

extern void uart_init(void);
extern void cpu_enable_interrupts(void);
extern volatile clock_t milliseconds;

//-----------------------------------------------------------------
// main
//-----------------------------------------------------------------
int main()
{
    uart_init();

    cpu_enable_interrupts();

    // Initialise SPI interface
    spi_init();

    printf("fat-io-lib.c:main:CLOCKS_PER_SEC = %d\n", CLOCKS_PER_SEC);

    for (int i=0; i < 20; i++)
    {
        printf("fat-io-lib.c:main:milliseconds = %d\n", milliseconds);
        printf("fat-io-lib.c:main:timer_now() = %d\n", timer_now());
    }
    
    printf("fat-io-lib.c:main:sleeping for 10s\n");

    timer_sleep(10000);
    printf("fat-io-lib.c:main:milliseconds = %d\n", milliseconds);

    // Initialise SD interface
    if (sd_init() < 0)
    {
        printf("ERROR: Cannot init SD card\n");
        return -1;
    }

    // Initialise File IO Library
    fl_init();

    // Attach media access functions to library
    if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
    {
        printf("ERROR: Failed to init file system\n");
        return -1;
    }
   
    // List the root directory
    fl_listdirectory("/");

    return 0;
}

