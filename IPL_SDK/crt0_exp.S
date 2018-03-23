#
# PSP IPL Exploit Boot Loader
#
# crt0.S - Startup functions and definitions. Inspired by nem's helloworld.
#
		.set noreorder

		.text

		.extern main

#include "as_reg_compat.h"


##############################################################################

		# Support for _init() and _fini().
;		.global _init
;		.global _fini
;		.type	_init, @function
;		.type	_fini, @function

		# The .weak keyword ensures there's no error if
		# _init/_fini aren't defined.
;		.weak	_init
;		.weak	_fini

		# Define the main thread attribute variable as weak
;		.weak   _main_thread_attr

_data_top:
#
#IPL Exploit Block
#Here is "PANDORA'S BATTERY" code.
#check readme.txt of "PANDORA'S BATTERY" for license
#
		.word	0x00000000,0x00000000,0xbfd00100,0x00000000
		.word	0x00000000,0x00000000,0x00000000,0x00000000
		.word	0xcd05a152,0x2859523a,0xf131d10a,0xcc2e87bd
		.word	0x2f02da14,0x66c78877,0xbd0732f3,0x4c9e081a
		.word	0x00000000,0x00000000,0x00000000,0x00000000
		.word	0x00000000,0x00000000,0x00000000,0x00000000
		.word	0x00000001,0x00000000,0x00000000,0x00000000
		.word	0x00000004,0x00000010,0x00000000,0x00000000
		.word	0x00000000,0x00000000,0x00000000,0x00000000
		.word	0x00000000,0x00000000,0x00000000,0x00000000
		.word	0x00000000,0x00000000,0x01000000,0x12745fc6
#
		.org _data_top + 0x100
#
#1st entry point
#
#move code to real address
		la		$t0,0xbfd00000
		la		$t1,_data_top
		addi		$t2,$0,0x1000
reload_loop:
		lw		$v0,0($t0)
		addi	$t0,4
		sw		$v0,0($t1)
		addi	$t2,-4
		bne	$t2, $0, reload_loop
		addi	$t1,4
#
#cache clear
		la	$t0,0x800102d8
		jalr	$t0
		nop
		la	$t0,0x800102a0
		jalr	$t0
		nop
#
#goto real entry address
		la		$t0,_start
		jr		$t0
		nop
#
		.global _start
		.ent _start
#
#real entry point
#
_start:
#;from loader
#;save r4,r5,r2,r20
#sp = $80013ff0
		lui 	$sp, 0x040d
		jal	entry
		lui	$gp,0x0000
#goto PRE-IPL again
		la 	$sp,0x80013ff0
		lui	$25,0x8001
		j	$25
		nop
		.end _start
		.global _exit
		.ent _exit

_exit:
# Exit
		.end _exit

		.bss
