MEMORY
{
   FLASH(rx): ORIGIN =0x00000000, LENGTH =64k
   SRAM(rwx): ORIGIN =0x00010000, LENGTH =32k
   UART(rwx): ORIGIN =0x00020000, LENGTH =16
}

_stack_top = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
   .text :
   {
      KEEP(*(.vectors))
      *(.text)
      *(.rodata)
      *(.rodata.*)
      . = ALIGN(4);
      _etext = .;
   } >FLASH

   .data : 
   { 
      _sdata = .;
      *(.data)
      . = ALIGN(4);
      _edata = .;
   } >SRAM AT >FLASH

   .bss :
   {
      _sbss = .;
      *(.bss)
      . = ALIGN(4);
      _ebss = .;
      _end = .;
   } > SRAM

   .uart :
   {
      *(.uart)
   } >UART
}