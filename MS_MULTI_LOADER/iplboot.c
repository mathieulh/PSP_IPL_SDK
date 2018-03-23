/*
	IPL exploit BOOT LOADER (MS ver)
*/

#include <psptypes.h>

#include "psp_uart.h"
#include "kprintf.h"
#include "patch.h"
#include "syscon.h"
#include "lcdc.h"
#include "cache.h"

#include "fatload.h"

#define UART_MESSAGE 0

/****************************************************************************
	load boot file
****************************************************************************/

#define SENSE_MASK (\
  SYSCON_CTRL_TRIANGLE|SYSCON_CTRL_CIRCLE|SYSCON_CTRL_CROSS|SYSCON_CTRL_RECTANGLE|\
  SYSCON_CTRL_SELECT|SYSCON_CTRL_LTRG|SYSCON_CTRL_RTRG|\
  SYSCON_CTRL_START|SYSCON_CTRL_HOME|SYSCON_CTRL_VOL_UP|SYSCON_CTRL_VOL_DN|\
  SYSCON_CTRL_LCD|SYSCON_CTRL_NOTE|\
  SYSCON_CTRL_ALLOW_UP|SYSCON_CTRL_ALLOW_RT|SYSCON_CTRL_ALLOW_DN|SYSCON_CTRL_ALLOW_LT\
)

const char *select_boot_file(void)
{
	u32 ctrl;

	// get PSP buttons
#if 1
	_pspSysconGetCtrl1(&ctrl);
	Kprintf("\nCONTROL VALUE=%08X\n",ctrl);
#else
	u8 lx,ly;
	_pspSysconCtrlAStickPower(1);
	_pspSysconGetCtrl2(ctrl,&lx,&ly);
	Kprintf("\nCONTROL %08X:%02X:%02X\n",ctrl,lx,ly);
#endif

	// positive
	ctrl = ~ctrl;

	// LTRG + ALLOW ?
	switch(ctrl&SENSE_MASK)
	{
	case SYSCON_CTRL_LTRG|SYSCON_CTRL_ALLOW_UP: return "ipl/ipl_up.bin";
	case SYSCON_CTRL_LTRG|SYSCON_CTRL_ALLOW_RT: return "ipl/ipl_rt.bin";
	case SYSCON_CTRL_LTRG|SYSCON_CTRL_ALLOW_DN: return "ipl/ipl_dn.bin";
	case SYSCON_CTRL_LTRG|SYSCON_CTRL_ALLOW_LT: return "ipl/ipl_lt.bin";
	case SYSCON_CTRL_LTRG:                      return "ipl/ipl.bin";
	}
	return NULL;
}

/****************************************************************************
	IPL exploit entry point
****************************************************************************/
void entry(void)
{
	int flip_led = 0;
	void (*entry)(void);
	const char *name;
	u32 sts;

#if UART_MESSAGE
	// SYNC UART
	psp_uart_init(115200);
	Kprintf("MS BOOT PROGRAM\n");
#endif

	// GPIO enable
	REG32(0xbc100058) |= 0x02;
	REG32(0xbc10007c) |= (1<<26) | 0xc8;
	REG32(0xbe240000) |= 0xc8;

	REG32(0xbc100058) |= 0x00800000;
	REG32(0xbc100050) |= 0x0000608E;
	REG32(0xbc10004c) &= 0xFFFFFBF7;
	REG32(0xbc100078) &= 0x00000002;
#if 0
	dl	0D500010,00000001	; [$bd500010] = 00000001
	dl	3D500010,00000001	; while( [$bd500010] & 1)
	dl	0D500040,00000001	; [$bd500040] = 00000001
	dl	F0000000			; end

#endif

	// SYSCON power controll
	pspSyscon_init();
	pspSysconCtrlLED(0,1);
	pspSysconCtrlLED(1,1);
	pspSysconCtrlHRPower(1);
	pspSysconCtrlMsPower(1);

	// MS/FAT init
	ms_fat_init();

	// laod boot file
	name = select_boot_file();
	if(name==NULL)
	{
		// reset reboot
		Kprintf("Reset for NAND Flash BOOT\n\n                    ");
		pspSysconResetDevice(1);
	}
#if UART_MESSAGE
	Kprintf("Loading %s\n",name);
#endif
	entry = (void *)0x040e0000;
	if( (entry = ms_load_bootloader(name)) != 0 )
	{
#if UART_MESSAGE
		Kprintf("EXECUTE %s(%08x)\n",name,entry);
#endif
		// goto loaded PROGRAM
		pspClearDcache();
		pspClearIcache();
		entry();
	}

#if UART_MESSAGE
	Kprintf("MS LOAD/BOOT ERROR\n");
#endif

	// message to UART when boot error
	sts = pspSysconNop();
	while(1)
	{
		// both LED flash
		REG32(0xbe24000c)  = 0xC0;
		Syscon_wait(125000/4);
		REG32(0xbe240008)  = 0xC0;
		Syscon_wait(125000/4);

		if( !(sts&SYSCON_STS_POWER_SW_ON) && ((sts=pspSysconNop()) & SYSCON_STS_POWER_SW_ON) )
		{
#if UART_MESSAGE
			Kprintf("Power OFF\n\n                    ");
#endif
			pspSysconPowerStandby();
		}
	}
	// restart PRE-IPL
}
