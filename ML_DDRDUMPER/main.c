/*
	PSP IPL patch & boot

	supported FW is **unknown**
*/

#include <psptypes.h>

#include "psp_uart.h"
#include "kprintf.h"
#include "patch.h"
#include "syscon.h"
#include "cache.h"

#include "memstk.h"
#include "tff.h"		/* Tiny-FatFs declarations */
#include "fatload.h"

#define PSP_IPL_FILENAME "ipl/ipl150.bin"

/****************************************************************************
	title message for header
****************************************************************************/
const char __title__[] __attribute__ (( section (".init"))) = "DDR-SDRAM DUMPER";

/****************************************************************************
	after DDR init
****************************************************************************/
void dump_mem(u32 a1,u32 a2,u32 a3,u32 a4)
{
#if 0
	Kprintf("DUMP HW Registers\n");
	dump_regs();
#endif

	Kprintf("DUMP VECTOR RAM\n");
	ms_save_file("vecmem.bin" ,(void *)0xbfc00000,0x1000);

	Kprintf("DUMP Kernel Memory\n");
	ms_save_file("kermem.bin" ,(void *)0x88000000,4*1024*1024);

#if 0
	Kprintf("DUMP Kernel Memory2\n");
	ms_save_file("kermem2.bin",(void *)0x84000000,4*1024*1024);

	Kprintf("DUMP User Memory\n");
	ms_save_file("usermem.bin" ,(void *)0x88800000,24*1024*1024);

	Kprintf("DONE\n");

	// WLAN ON , MS OFF
	REG32(0xbe240008)  = 0x80;
	REG32(0xbe24000c)  = 0x40;
#endif

	// reset reboot
	Kprintf("Reset PSP\n\n");
	pspSysconResetDevice(1);

	while(1);
}

/****************************************************************************
	before jump main.bin

	(after decrypt/loaded main.bin)
****************************************************************************/
void patch_main_bin(void)
{
	u32 *code = (u32 *)0x04000000;

	Kprintf("patch_main_bin\n");

	// TARP after reset DDR-SDRAM
	code[0x08/4] = 0x08000000 | ( ((u32)dump_mem & 0x0ffffffc)>>2);
	code[0x0c/4] = 0;

	// Change payload entrypoint to payloadex
	code[0xa758/4] = 0x3c198870; // entry = 88700000

	// Avoid TA-082 clockgen incompatibility 
	code[0x0858/4] = 0x1000ff07; // -> beq $zero, $zero, ...

	pspClearDcache();
	pspClearIcache();

	asm("lui		$25, 0x0400");
	asm("lui		$sp, 0x040F");
	asm("jr		$25");
	asm("ori		$sp,$sp, 0xFF00");
}

/****************************************************************************
	entry point
****************************************************************************/
int main(void)
{
	void (*ipl_bin_entry)(void);

	Kprintf("ENTER:%s\n",__title__);

	// init I/O
	pspSyscon_init();
	pspSysconCtrlLED(0,1);
	pspSysconCtrlLED(1,1);
	pspSysconCtrlMsPower(1);

	// MS/FAT system
	ms_fat_init();

	// load FW1.50 IPL for initialize DDR-SDRAM controller
	if(ms_load_ipl(PSP_IPL_FILENAME) == NULL)
	{
		Kprintf("can't load ipl.bin\n");
		while(1);
	}

	// hook jump main.bin
	u32 *code = (u32 *)0x040f0000;
	code[0xE0/4] = 0x08000000 | ( ((u32)patch_main_bin)>>2); // j patch_main_bin
	code[0xE4/4] = 0x00000000; // nop

	pspClearDcache();
	pspClearIcache();

	// goto IPL.bin
	ipl_bin_entry = (void *)code;
	ipl_bin_entry();

	return 0;
}
