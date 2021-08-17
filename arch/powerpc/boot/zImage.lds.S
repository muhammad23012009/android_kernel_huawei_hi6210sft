<<<<<<< HEAD
OUTPUT_ARCH(powerpc:common)
=======
#include <asm-generic/vmlinux.lds.h>

#ifdef CONFIG_PPC64_BOOT_WRAPPER
OUTPUT_ARCH(powerpc:common64)
#else
OUTPUT_ARCH(powerpc:common)
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
ENTRY(_zimage_start)
EXTERN(_zimage_start)
SECTIONS
{
  .text      :
  {
    _start = .;
    *(.text)
    *(.fixup)
    _etext = .;
  }
  . = ALIGN(4096);
  .data    :
  {
    *(.rodata*)
    *(.data*)
    *(.sdata*)
<<<<<<< HEAD
    *(.got2)
=======
#ifndef CONFIG_PPC64_BOOT_WRAPPER
    *(.got2)
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
  }
  .dynsym : { *(.dynsym) }
  .dynstr : { *(.dynstr) }
  .dynamic :
  {
    __dynamic_start = .;
    *(.dynamic)
  }
  .hash : { *(.hash) }
  .interp : { *(.interp) }
<<<<<<< HEAD
  .rela.dyn : { *(.rela*) }
=======
  .rela.dyn :
  {
#ifdef CONFIG_PPC64_BOOT_WRAPPER
    __rela_dyn_start = .;
#endif
    *(.rela*)
  }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

  . = ALIGN(8);
  .kernel:dtb :
  {
    _dtb_start = .;
    *(.kernel:dtb)
    _dtb_end = .;
  }

  . = ALIGN(4096);
  .kernel:vmlinux.strip :
  {
    _vmlinux_start =  .;
    *(.kernel:vmlinux.strip)
    _vmlinux_end =  .;
  }

  . = ALIGN(4096);
  .kernel:initrd :
  {
    _initrd_start =  .;
    *(.kernel:initrd)
    _initrd_end =  .;
  }

<<<<<<< HEAD
=======
#ifdef CONFIG_PPC64_BOOT_WRAPPER
  . = ALIGN(256);
  .got :
  {
    __toc_start = .;
    *(.got)
    *(.toc)
  }
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
  . = ALIGN(4096);
  .bss       :
  {
    _edata  =  .;
    __bss_start = .;
    *(.sbss)
    *(.bss)
    *(COMMON)
    _end = . ;
  }
}
