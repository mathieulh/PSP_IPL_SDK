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

#define BIOS_FILE_NAME "/psp_bios.bin"

/****************************************************************************
	title message for header
****************************************************************************/
const char __title__[] __attribute__ (( section (".init"))) = "PSP BIOS DUMPER";

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
	entry point
****************************************************************************/
int main(void)
{
	Kprintf("ENTER:%s\n",__title__);

	// init I/O
	pspSyscon_init();
	pspSysconCtrlLED(0,1);
	pspSysconCtrlLED(1,1);
	pspSysconCtrlMsPower(1);

	// MS/FAT system
	ms_fat_init();

	ms_save_file(BIOS_FILE_NAME,(void *)0xbfc00000,0x1000);

	// reset reboot
	reboot_psp();

	return 0;
}
