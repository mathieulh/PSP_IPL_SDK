#
# PSP original IPL (loaded by NAND exploit installer / boot loader)
#

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

#		.extern	__executable_start
#		.extern	edata

		.extern	__sbss_start
		.extern	__sbss_end

#--------------------------------------------------------------------------------
# +10h HEADER block for exploit installer
#--------------------------------------------------------------------------------
		.word	_start         # +00 load address
		.word	0x12345678     # +04 signature (replace size by installer)
		.word	_start         # +08 entry address
		.word	0x12345678     # +0c signature2

#--------------------------------------------------------------------------------
# Entry point
#--------------------------------------------------------------------------------
		.global _start
		.ent _start
_start:
# Stack Pointer
		lui		$sp, 0x040d

# Clear BSS
		la		$t0, __sbss_start
		la		$t1, __sbss_end
bss_clr:
		sw		$0,0($t0)
		addi	$t0,2
		sltu	$v0, $t0, $t1
		bne		$v0, $0, bss_clr
		nop
#call entry
		jal		entry
		la		$gp, _gp
#
#PRE-IPL recall
		la 	$sp,0x80013ff0
		lui	$25,0x8001
		j	$25
		nop
#
#		addiu 	$sp, -0x20
#		sw		$2, 0($sp)	
#		sw		$4, 4($sp)
#		sw		$5, 8($sp)
#		sw		$6,12($sp)
#		sw		$7,16($sp)
#		sw		$8,20($sp)
#		sw		$22,24($sp)
#		sw		$gp,28($sp)
#
# Clear BSS
#	loop:
#		sw		$0,0($t0)
#		sltu	$v0, $t0, $t1
#		bne		$v0, $0, loop
#		addiu	$t0, $t0, 4
#		la		$gp, _gp
#;		la		$t0, _init
#;		beqz	$t0, 1f
#;		nop
#;		jalr	$t0
#;		nop
#1:
#		jal		main
#		nop
#
#		lw		$2, 0($sp)	
#		lw		$4, 4($sp)
#		lw		$5, 8($sp)
#		lw		$6,12($sp)
#		lw		$7,16($sp)
#		lw		$8,20($sp)
#		lw		$22,24($sp)
#		lw		$gp,28($sp)
#		jr	$2
#		addu	$29,$22,$0
;
		.end _start

		.global _exit
		.ent _exit

_exit:
# Exit
		j _exit
		nop
		.end _exit

		.bss
_bss:
