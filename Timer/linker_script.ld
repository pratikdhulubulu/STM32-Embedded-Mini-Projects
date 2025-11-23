/*-----------------------------------------------------------------------------
 * Entry Point: Reset_Handler
 * This symbol must exist in the startup file.
 *----------------------------------------------------------------------------*/
ENTRY(Reset_Handler)

/*-----------------------------------------------------------------------------
 * Memory Regions (From STM32F446RE Datasheet)
 *----------------------------------------------------------------------------*/
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K   /* Program memory */
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K   /* SRAM1 + SRAM2 */
}

/*-----------------------------------------------------------------------------
 * Section Placement
 * Defines how each section is mapped to memory.
 *----------------------------------------------------------------------------*/
SECTIONS
{
  /*---------------------------------------------------------------------------
   * 1. Vector Table and Code (.isr_vector + .text)
   *---------------------------------------------------------------------------*/
  .text :
  {
    KEEP(*(.isr_vector))      /* Interrupt Vector Table (Reset, ISRs) */
    *(.text*)                 /* All code (.text, .text.Reset_Handler, etc.) */
    *(.rodata*)               /* Read-only data (const, lookup tables) */
    *(.glue_7) *(.glue_7t)    /* ARM-Thumb interworking support */
    *(.eh_frame)              /* Exception handling data (C++) */

    . = ALIGN(4);             /* Align next address on 4-byte boundary */
    _etext = .;               /* Marks end of code section */
  } > FLASH

  /*---------------------------------------------------------------------------
   * 2. Initialized Data (.data)
   *   - Data copied from Flash (_sidata) to RAM (_sdata → _edata)
   *---------------------------------------------------------------------------*/
  .data : AT(_etext)
  {
    _sidata = LOADADDR(.data); /* Address in Flash where .data is stored */

    . = ALIGN(4);
    _sdata = .;                /* Start of .data in RAM */
    *(.data*)
    . = ALIGN(4);
    _edata = .;                /* End of .data in RAM */
  } > RAM

  /*---------------------------------------------------------------------------
   * 3. Uninitialized Data (.bss)
   *   - Zero-initialized at runtime (_sbss → _ebss)
   *---------------------------------------------------------------------------*/
  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;                 /* Start of .bss in RAM */
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;                 /* End of .bss in RAM */
  } > RAM

  /*---------------------------------------------------------------------------
   * 4. Heap and Stack region
   *   - Reserve space for dynamic memory and stack.
   *---------------------------------------------------------------------------*/
  ._user_heap_stack (NOLOAD):
  {
    . = ALIGN(8);
    _sheap = .;                /* Heap start */
    . = . + 0x00001000;        /* Reserve 4 KB for heap */
    _eheap = .;

    . = ALIGN(8);
    _sstack = .;               /* Stack start (grows down) */
    . = . + 0x00002000;        /* Reserve 8 KB for stack */
    _estack = ORIGIN(RAM) + LENGTH(RAM); /* Top of stack */
  } > RAM

  /*---------------------------------------------------------------------------
   * 5. Discard Debug Sections (optional)
   *---------------------------------------------------------------------------*/
  /DISCARD/ :
  {
    *(.comment)
    *(.note*)
  }
}

/*-----------------------------------------------------------------------------
 * End of Linker Script
 *----------------------------------------------------------------------------*/
