#---------------------------------------------------------------------------------

uart_timer_interrupt_bit_mask = 0b00001000
uart_tx_ready_bit_number=2
uart_rx_ready_bit_number=0

#---------------------------------------------------------------------------------
# uart
#---------------------------------------------------------------------------------

    .section .uart
# 0
    .global uart_mode_register_a
uart_mode_register_a: 
    .ds.b   1
# 1
    .global uart_clock_select_register_a
uart_clock_select_register_a:
    .global uart_status_register_a
uart_status_register_a: 
    .ds.b   1
# 2
    .global uart_command_register_a
uart_command_register_a:
    .ds.b   1
# 3
    .global uart_transmit_buffer_a
uart_transmit_buffer_a:
    .global uart_receive_buffer_a
uart_receive_buffer_a:
    .ds.b   1
# 4
uart_aux_control_register:
    .ds.b   1
# 5
uart_interrupt_mask_register:
uart_interrupt_status_register:
    .ds.b   1
# 6 
uart_counter_timer_upper_preset_register:
    .ds.b   1
# 7
uart_counter_timer_lower_preset_register:
    .ds.b   1
# 8
uart_mode_register_b: 
    .ds.b   1
# 9
uart_clock_select_register_b:
uart_status_register_b: 
    .ds.b   1
# a
uart_command_register_b:
    .ds.b   1
# b
uart_transmit_buffer_b:
uart_receive_buffer_b:
    .ds.b   1
# c
uart_interrupt_vector_register:
    .ds.b   1
# d
    .global uart_output_port_configuration_register
uart_output_port_configuration_register:
    .global uart_input_port_register
uart_input_port_register:
    .ds.b   1
# e
uart_start_counter_command:
    .global uart_output_port_set_register
uart_output_port_set_register:
    .ds.b   1
# f
uart_stop_counter_command_register:
uart_stop_counter_command:
    .global uart_output_port_clear_register
uart_output_port_clear_register:
    .ds.b   1


#---------------------------------------------------------------------------------
# uart_init
# Initialise the UART
# clobbers %a0
#---------------------------------------------------------------------------------

    .text
    .global uart_init
    .align  2
uart_init:

# CRA: TxReset (XXX I suspect this command is unnecesary)
# NotImplemented[7]=[0]
# MiscCommand[6:4]=TxReset [011]
# DisableTx[3]=No[0]
# EnableTx[2]=No[0]
# DisableRx[1]=No[0]
# EnableRx[0]=No[0]
    move.b  #0b00110000,uart_command_register_a

# CRA: RxReset (XXX I suspect this command is unnecesary)
# NotImplemented[7]=[0]
# MiscCommand[6:4]=RxReset [010]
# DisableTx[3]=No[0]
# EnableTx[2]=No[0]
# DisableRx[1]=No[0]
# EnableRx[0]=No[0]
    move.b  #0b00100000,uart_command_register_a

# CRA: Reset MRA Pointer (XXX I suspect this command is unnecesary)
# NotImplemented[7]=[0]
# MiscCommand[6:4]=MraPointerReset [001]
# DisableTx[3]=No[0]
# EnableTx[2]=No[0]
# DisableRx[1]=No[0]
# EnableRx[0]=No[0]
    move.b  #0b00010000,uart_command_register_a

# ACR: Set Baud Rate Set 1, Set Counter/Timer Source as CLK, Set Input Port Interrupts Off
# BaudRateSet[7]=Set 1[0] / Set 2[1]
# CounterTimeMode[6:4]=ClkDivideBy16 [111]
# DeltaInteruptIp3[3]=Off [0]
# DeltaInteruptIp2[2]=Off [0]
# DeltaInteruptIp1[1]=Off [0]
# DeltaInteruptIp0[0]=Off [0]
    move.b  #0b11110000,uart_aux_control_register

# CSRA: Set 115.2k baud 
# Set Test Baud Rate Mode (by reading CRA)
# RxBaudRate[7:4]=9600 [1011]/ 38.4k [1100]/ 115.k [0110]
# TxBaudRate[3:0]=9600 [1011]/ 38.4k [1100]/ 115.k [0110]
    tst.b   uart_command_register_a
    move.b  #0b01100110,uart_clock_select_register_a
#   move.b  #0b10111011,uart_clock_select_register_a

# MRA1: 8 Bits, No Parity, Interrupt on FIFO Full, Char Mode (?)
# RxRtsMode[7]=OP0 set/cleared under program control [0] / OP0 is /RTSA [1]
# RxIrqMode[6]=Interrupt on receiver ready [0]/ Interrupt on FIFO full [1] 
# RxErrorMode[5]=Character mode (error status applies to top of FIFO) [0] 
# Parity[4:2]=No parity [100] 
# Bits[1:0]=8 bits/char [11]
    move.b  #0b10010011,uart_mode_register_a

# MRA2: Enable CTS, Enable RTS, Full Duplex, 1 Stop Bit
# ChannelMode[7:6]=Normal mode (full duplex)[00]
# RxRtsControl[5]=/RTSA set/cleared under program control [0] /RTSA cleared when all data tranmitted [1]
# RxCtsControl[4]=IP0 has no effect on transmitter [0] / IP0 is /CTSA [1]
# StopBitLength[3:0]=1 [0111]
    move.b  #0b00110111,uart_mode_register_a

# CRA: Enable Tx and Rx
# NotImplemented[7]=[0]
# MiscCommand[6:4]=None [000]
# DisableTx[3]=No[0]
# EnableTx[2]=Yes[1]
# DisableRx[1]=No[0]
# EnableRx[0]=Yes[1]
    move.b  #0b00000101,uart_command_register_a

# Set Timer/Counter Rate
# CLK is 3.6864 MHz
# CLK divded by 16 is 230 kHz
# CLK divided by 16 divided by 0x480 is 100 Hz (one interrupt every 10 ms)
	move.w #0x0480,uart_counter_timer_upper_preset_register

# Set Interupt Vector
	move.b #0x40,uart_interrupt_vector_register

# Enable Counter/Timer interrupts
	move.b #uart_timer_interrupt_bit_mask,uart_interrupt_mask_register

    rts

#---------------------------------------------------------------------------------
# uart_interrupt_handler
# Increments the milliseconds global counter every 10ms
#---------------------------------------------------------------------------------
    .bss
    .global milliseconds
milliseconds:
	.ds.l   1

    .text
    .global uart_interrupt_handler
    .align  2
uart_interrupt_handler:
    # Push the registers used in the handler
    # Clear Counter/Timer interrupt
	tst.b uart_stop_counter_command_register
    # Increment milliseconds since boot (1 tick equals 10ms)
	addi.l #10,milliseconds
    # Pull the pushed registers
	rte

#---------------------------------------------------------------------------------
# uart_write_s
# Write a string to the UART
# %a1: in &message    ; address of first char of zero-terminated string
#---------------------------------------------------------------------------------

    .align  2
    .global uart_puts
uart_puts:
    move.l  %sp@(4),%a1
    .global uart_write_s
uart_write_s:
    move.b  %d0,-(%sp)
    .uart_write_s_nextc:
        move.b  (%a1)+,%d0
        beq     .uart_write_s_return
        jsr     uart_write_byte_d0
        cmp.b   #'\n',%d0
        bne     .uart_write_s_nextc
        move.b  #'\r',%d0
        jsr     uart_write_byte_d0
        bra     .uart_write_s_nextc
    .uart_write_s_return:
    move.b  (%sp)+,%d0
    rts

#---------------------------------------------------------------------------------
# uart_write_byte_d0
# Write a char to the UART
# input
#   d0: byte
#---------------------------------------------------------------------------------
    
    .align  2
    .global uart_putc
uart_putc:
    move.l  %sp@(4),%d0
    .global uart_write_byte_d0
uart_write_byte_d0:
    .uart_test_tx_ready:
        btst.b  #uart_tx_ready_bit_number,uart_status_register_a
        beq     .uart_test_tx_ready
    move.b  %d0,uart_transmit_buffer_a
    rts


#---------------------------------------------------------------------------------
# uart_write_crlf
# Write CRLF to the UART
#---------------------------------------------------------------------------------

    .global uart_write_crlf
    .align  2
uart_write_crlf:
    move.b  %d0,-(%sp)
    move.b  #'\r',%d0
    jsr     uart_write_byte_d0
    move.b  #'\n',%d0
    jsr     uart_write_byte_d0
    move.b  (%sp)+,%d0
    rts


#---------------------------------------------------------------------------------
# uart_write_space
# Write space to the UART
#---------------------------------------------------------------------------------

    .global uart_write_space
    .align  2
uart_write_space:
    move.b  %d0,-(%sp)
    move.b  #' ',%d0
    jsr     uart_write_byte_d0
    move.b  (%sp)+,%d0
    rts

#---------------------------------------------------------------------------------
# uart_read_byte_d0
# Read a char from the UART
# d0: out char    ; char  
#---------------------------------------------------------------------------------

    .align  2
    .global uart_getc
uart_getc:
    .global uart_read_byte_d0
uart_read_byte_d0:
    uart_test_rx_ready:
        btst.b  #uart_rx_ready_bit_number,uart_status_register_a
        beq     uart_test_rx_ready
    move.b  uart_receive_buffer_a,%d0
    rts

