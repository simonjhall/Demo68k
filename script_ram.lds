ENTRY(_start)

SECTIONS
{
/* origins */
/*  . = 0x120000;*/
/* big daddy pro */
/*	. = 0x420000;*/
/* dynamic daddy */
/*   . = 0x820000;*/
/* redux 040 */
   . = 0x12000;
  .text : { *(.text*) }
  .rodata : { *(.rodata*) }
  
  .data : { *(.data) } 
  .bss : { *(.bss) }
}
