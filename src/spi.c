#include <stdio.h>
#include "spi.h"

extern void spi_init_68681(void);
extern void spi_enable_68681(void);
extern void spi_disable_68681(void);
extern uint8_t spi_sendrecv_68681(uint8_t data);

//-----------------------------------------------------------------
// spi_init: Initialise SPI master
//-----------------------------------------------------------------
void spi_init(void)           
{         
    spi_init_68681();
}
//-----------------------------------------------------------------
// spi_cs: Set chip select
//-----------------------------------------------------------------
void spi_cs(uint32_t value)
{
    value ? spi_disable_68681() : spi_enable_68681();
}

//-----------------------------------------------------------------
// spi_sendrecv: Send or receive a character
//-----------------------------------------------------------------
uint8_t spi_sendrecv(uint8_t data)
{
    // 1. Write data to SPI Tx FIFO
    // 2. Wait for Tx complete
    // 3. Read SPI Rx FIFO and return
    return spi_sendrecv_68681(data);
}

//-----------------------------------------------------------------
// spi_readblock: Read a block of data from a device
//-----------------------------------------------------------------
void spi_readblock(uint8_t *ptr, int length)
{
    int i;

    for (i=0;i<length;i++)
        *ptr++ = spi_sendrecv(0xFF);
}

//-----------------------------------------------------------------
// spi_writeblock: Write a block of data to a device
//-----------------------------------------------------------------
void spi_writeblock(uint8_t *ptr, int length)
{
    int i;

    for (i=0;i<length;i++)
        spi_sendrecv(*ptr++);
}
