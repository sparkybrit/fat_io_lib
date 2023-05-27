
uart_output_port_ss_bitmask=0b00001000
uart_output_port_mosi_bitmask=0b00100000
uart_input_port_miso_bit_number=3
uart_output_port_clk_bitmask=0b10000000

#---------------------------------------------------------------------------------
# spi_init_68681
#
# Initialise the UART for SPI
# CLK normally LOW
# MOSI normally HIGH
# Each data bit is latched on RISING edge of CLK
# Data is BIG endian (most significant bit first)
#---------------------------------------------------------------------------------

    .text
    .global spi_init_68681
    .align  2
spi_init_68681:
    # set UART Ouptput Port as GPIO
    clr.b   uart_output_port_configuration_register              
    # Set SPI MOSI high, SS high SPI CLK low    
    move.b  #uart_output_port_clk_bitmask,uart_output_port_set_register
    rts


#---------------------------------------------------------------------------------
# spi_enable_68681
# Assert SPI SS 
#---------------------------------------------------------------------------------

    .global spi_enable_68681
    .align  2
spi_enable_68681:
 	move.l %d0,-(%sp)
 	move.l %d1,-(%sp)
    move.b  #0xff,%d0
    bsr     spi_sendrecv
    # set the SPI SS low 
    move.b  #uart_output_port_ss_bitmask,uart_output_port_set_register      
    move.b  #0xff,%d0
    bsr     spi_sendrecv
 	move.l (%sp)+,%d1
 	move.l (%sp)+,%d0
    rts


#---------------------------------------------------------------------------------
# spi_disable_68681
# Unassert SPI SS 
#---------------------------------------------------------------------------------

    .global spi_disable_68681
    .align  2
spi_disable_68681:
 	move.l %d0,-(%sp)
 	move.l %d1,-(%sp)
    move.b  #0xff,%d0
    bsr     spi_sendrecv
    # set the SPI SS high 
    move.b  #uart_output_port_ss_bitmask,uart_output_port_clear_register    
    move.b  #0xff,%d0
    bsr     spi_sendrecv
    move.b  #0xff,%d0
    bsr     spi_sendrecv
 	move.l (%sp)+,%d1
 	move.l (%sp)+,%d0
    rts


#---------------------------------------------------------------------------------
# spi_sendrecv_68681
# Send %d0.b to SPI
# %d0.b: in  
# Returns
# %d0.b: out
# clobbers %d1
#---------------------------------------------------------------------------------

    .global spi_sendrecv_68681
    .align  2
spi_sendrecv_68681:
    move.l  %sp@(4),%d0
    # set the SPI CLK low (should be low already)
    move.b  #uart_output_port_clk_bitmask,uart_output_port_set_register 

    move.w  #7,%d1
    .spi_sendrecv_next_bit:    
        # put bit 7 of %d0 in the C bit and 0 in bit 0
        lsl.b   #1,%d0

        # write MOSI (OUTPUT)  
        # use the C bit to set MOSI                                                 
        bcs     .spi_sendrecv_set_mosi
            # set MOSI low 
            move.b  #uart_output_port_mosi_bitmask,uart_output_port_set_register 
            bra     .spi_sendrecv_set_clock
        .spi_sendrecv_set_mosi:
            # set MOSI high 
            move.b  #uart_output_port_mosi_bitmask,uart_output_port_clear_register 
        .spi_sendrecv_set_clock:

        # set the SPI CLK high 
        move.b  #uart_output_port_clk_bitmask,uart_output_port_clear_register   

        # read MISO (INPUT)
        btst.b  #uart_input_port_miso_bit_number,uart_input_port_register   
        beq     .spi_sendrecv_miso_not_set
            ori.b    #1,%d0
        .spi_sendrecv_miso_not_set:

        # set the SPI CLK low 
        move.b  #uart_output_port_clk_bitmask,uart_output_port_set_register     
        dbra    %d1,.spi_sendrecv_next_bit

    # set the SPI MOSI high   
    move.b  #uart_output_port_mosi_bitmask,uart_output_port_clear_register  
    rts
