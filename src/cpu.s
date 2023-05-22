
    .text
    
    .global cpu_enable_interrupts
    .align  2
cpu_enable_interrupts:
    # Enable CPU Interrupts above level 2
	move.w  %sr,%d0
	andi.w  #0b1111100011111111,%d0
    ori.w   #0x0200,%d0
    move.w  %d0,%sr
    rts
