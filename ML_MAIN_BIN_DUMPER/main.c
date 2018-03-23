/*
	PSP IPL main.bin dumper
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

#define DUMP_MAIN_BIN 1

// load IPL from ms file (or NAND Flash)
#define LOAD_IPL_FILE_NAME "/ipl.bin"

// load IPL from ms file (or NAND Flash)
#define MAIN_BIN_FILE_NAME "/ipl_main.bin"
#define MAIN_BIN_SIZE 0x010000

/****************************************************************************
	title message for header
****************************************************************************/
const char __title__[] __attribute__ (( section (".init"))) = "IPL main.bin DUMPER";

/****************************************************************************
	reset PSP
****************************************************************************/
void reboot_psp(void)
{
	// reset reboot
	Kprintf("Reset PSP\n\n");
	pspSysconResetDevice(1);
	while(1);
}

/****************************************************************************
	INTERRIGENT SEARCH
****************************************************************************/
static u32 mips_regs[32];
static void parge_regs(void)
{
	int i;
	for(i=0;i<32;i++) mips_regs[i] = 0;
}

u32 *search_long_jump(void *start,void *end,u32 jump_address)
{
	u32 *ptr;
	int reg;

	parge_regs();
	for(ptr=(u32 *)start;ptr<(u32 *)end;ptr++)
	{
		if( (ptr[0] & 0xffe00000) == MIPS_LUI(0,0) )
		{
			reg = (ptr[0]>>16) & 31;
			mips_regs[reg]=ptr[0]<<16;
Kprintf("%08X:LUI R%2d,%08X\n",(u32)ptr,reg,mips_regs[reg]);
		}

		if( (ptr[0] & 0xfc1fffff) == MIPS_JR(0) )
		{
			reg = (ptr[0]>>21)&31;
Kprintf("%08X:JR r%2d[%08X]\n",(u32)ptr,reg,mips_regs[reg]);
			if(mips_regs[reg]==jump_address)
			{
Kprintf("Found main.bin call %08X , entry %08X\n",(u32)ptr,mips_regs[reg]);
				// main_bin_entry = mips_regs[reg];
				return ptr;
			}
			parge_regs();
		}

		if( ptr[0] & 0xfc000000 == MIPS_J(0) )
		{
Kprintf("%08X:J\n");
			parge_regs();
		}
	}
	return 0;
}

/****************************************************************************
	before jump main.bin

	(after decrypt/loaded main.bin)
****************************************************************************/
void patch_main_bin(void)
{
	u32 *code = (u32 *)0x04000000;

Kprintf("dump main.bin\n");
	ms_save_file(MAIN_BIN_FILE_NAME,(void *)0x04000000,MAIN_BIN_SIZE);

Kprintf("\nMAIN.BIN DUMPED!\n");

	// reset reboot
	reboot_psp();
}

/****************************************************************************
	entry point
****************************************************************************/
int main(void)
{
	void (*ipl_bin_entry)(void);
	u32 *patch_point;

	Kprintf("ENTER:%s\n",__title__);

	// init I/O
	pspSyscon_init();
	pspSysconCtrlLED(0,1);
	pspSysconCtrlLED(1,1);
	pspSysconCtrlMsPower(1);

	// MS/FAT system
	ms_fat_init();

#ifdef LOAD_IPL_FILE_NAME
	// load FW1.50 IPL for initialize DDR-SDRAM controller
	if(ms_load_ipl(LOAD_IPL_FILE_NAME) == NULL)
	{
		Kprintf("can't load %s\n",LOAD_IPL_FILE_NAME);
		while(1);
	}
#else
	// load IPL from NAND flash
	nand_load_ipl();
#endif

	Kprintf("IPL loaded\n");

	// interrigent HOOK , "jump main.bin"
	patch_point = search_long_jump(0x040f0000,0x040f1000,0x04000000);
	if(!patch_point)
	{
		reboot_psp();
	}
	// hook JR instruction
	patch_point[0] = MIPS_J(patch_main_bin);

	// clear main.bin area
	u32 *ptr;
	for(ptr=(u32 *)0x04000000;ptr<(u32 *)(0x04080000+MAIN_BIN_SIZE);ptr++) *ptr=0;

	pspClearDcache();
	pspClearIcache();

	// goto IPL.bin
	ipl_bin_entry = (u32 *)0x040f0000;
	ipl_bin_entry();

	return 0;
}
