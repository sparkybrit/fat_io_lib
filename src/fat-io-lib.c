#include "fat_filelib.h"
#include "sd.h"
#include "spi.h"
#include "timer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void uart_init(void);
extern void cpu_enable_interrupts(void);
extern volatile clock_t milliseconds;

extern uint8_t _sprogram;

//-----------------------------------------------------------------
// main
//-----------------------------------------------------------------
int main()
{
    uart_init();

    printf("S-boot (c) Graeme Harker, 2023\r\n");

#ifdef DEBUG
    printf("fat-io-lib.c:main(): load address is %p\r\n", &_sprogram);
#endif

    cpu_enable_interrupts();

    // Initialise SPI interface
    spi_init();

    // Initialise SD interface
    if (sd_init() < 0) {
        printf("ERROR: Cannot init SD card\r\n");
        return -1;
    }

    // Initialise File IO Library
    fl_init();

    // Attach media access functions to library
    if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK) {
        printf("ERROR: Failed to init file system\r\n");
        return -1;
    }

    /*
        // List the root directory
        fl_listdirectory("/");

    #ifdef DEBUG
        printf("fat-io-lib.c:main(): returned from fl_listdirectory()\r\n");
    #endif
    */

    int bytes;
    FL_FILE* fl_spl;
    char* filename = "/s-boot.bin";

    if ((fl_spl = fl_fopen(filename, "r")) == NULL) {
        printf("ERROR: can't open %s\r\n", filename);
        return 1;
    }

#ifdef DEBUG
    printf("fat-io-lib.c:main(): fl_spl->filelength=%d\r\n", fl_spl->filelength);
#endif

    if ((bytes = fl_fread(&_sprogram, 1, fl_spl->filelength, fl_spl)) < 0) {
        printf("ERROR: can't read %s\r\n", filename);
        return 2;
    }

#ifdef DEBUG
    printf("fat-io-lib.c:main(): _sprogram=%p, bytes = %d\r\n", &_sprogram, bytes);

    char printable[] = "0123456789abcdef";

    for (int offset = 0; offset < bytes; offset++) 
    {
        uint8_t *cp = &_sprogram + offset;

        if (offset % 16 == 0) {
            if (offset > 0)
                printf(" |%s|\r\n", printable);

            printf("%p: ", cp);
        }

        if (*cp > ' ' && *cp < 127)
            printable[offset % 16] = *cp;
        else
            printable[offset % 16] = '?';

        printf("%02x ", *cp);
    }

    printf("\r\n");
#endif

    ((void (*)(void)) &_sprogram)();

    return 0;
}