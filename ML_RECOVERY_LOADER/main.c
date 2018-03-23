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

// path of PANDORA'S IPL CODE with patcher
#define LOAD_IPL_FILE_NAME "/ipl/150ex.bin"

/****************************************************************************
	title message for header
****************************************************************************/
const char __title__[] __attribute__ (( section (".init"))) = "Pandora's Recovery Menu LOADER for MS-Multi Loader";

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

	// load PANDORA'S IPL from MS FAT file
	if(ms_load_ipl(LOAD_IPL_FILE_NAME) == NULL)
	{
		Kprintf("can't load %s\n",LOAD_IPL_FILE_NAME);
		while(1);
	}
	Kprintf("OK\n");

	// hook jump main.bin
	u32 *code = (u32 *)0x040f0000;
	code[0xE0/4] = 0x08000000 | ( (0x040e0000)>>2); // j patch_main_bin
	code[0xE4/4] = 0x00000000; // nop

	pspClearDcache();
	pspClearIcache();

	// goto IPL.bin
	ipl_bin_entry = (void *)code;
	ipl_bin_entry();

	return 0;
}
