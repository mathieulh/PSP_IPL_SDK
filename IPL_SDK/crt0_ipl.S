# PSP Software Development Kit - http://www.pspdev.org
# -----------------------------------------------------------------------
# Licensed under the BSD license, see LICENSE in PSPSDK root for details.
#
# crt0.S - Startup functions and definitions. Inspired by nem's helloworld.
#
# Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
# Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
# Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
#
# $Id: crt0.S 363 2005-06-27 20:35:14Z tyranid $

		.set noreorder

		.text

		.extern main

#include "as_reg_compat.h"

##############################################################################

		# Support for _init() and _fini().
		.global _init
		.global _fini
		.type	_init, @function
		.type	_fini, @function

		# The .weak keyword ensures there's no error if
		# _init/_fini aren't defined.
		.weak	_init
		.weak	_fini

#--------------------------------------------------------------------------------
# 00h-10h HEADER block for exploit installer
#--------------------------------------------------------------------------------
		.section .init
		.word	_body          # +00 load address
		.word   __tsize        # +04 program size
		.word	_start         # +08 entry address
		.word	0x00000000     # +0c 00000000 (sum32 of last block)
_body:
# comment "PSP IPL BINARY"
		.word   0x20505350
		.word   0x204C5049
		.word   0x414E4942
		.word   0x00005952

		.section .text
#--------------------------------------------------------------------------------
# Entry point
#--------------------------------------------------------------------------------
		.global _start
		.ent _start

_start:
#
# Clear BSS
		la		$t0,__sbss_start
		la		$t1,__sbss_end
	loop:
		sw		$0,0($t0)
		sltu	$v0, $t0, $t1
		bne		$v0, $0, loop
		addiu	$t0, $t0, 4
#
#call _init
		la		$gp, _gp
		la		$t0, _init
		beqz	$t0, 1f
		nop
		jalr	$t0
		nop
1:
# go main
		addiu	$a1,0
		addiu	$a2,0
		jal		main
		nop
halt:
		j		halt
		nop
;
		.end _start
#
#
#
		.global _exit
		.ent _exit

_exit:
# Exit
		la		$t0, _fini
		beqz	$t0, 2f
		nop

		jalr	$t0
		nop
2:

		lw		$ra, 0($sp)
		jr		$ra
		addiu	$sp, 0x10
			
		.end _exit

		.bss
