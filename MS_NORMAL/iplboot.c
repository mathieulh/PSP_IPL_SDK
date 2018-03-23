/*
	IPL exploit BOOT LOADER (MS to normal NAND boot)
*/

#include <psptypes.h>

#include "psp_uart.h"
#include "kprintf.h"
#include "patch.h"
#include "syscon.h"
#include "lcdc.h"
#include "cache.h"

#define UART_MESSAGE 0

const char title[] = "PSP NORMAL BOOT LOADER for pandora's battery";

/****************************************************************************
	IPL exploit entry point
****************************************************************************/
void entry(void)
{
#if UART_MESSAGE
	// SYNC UART
	psp_uart_init(115200);
	Kprintf("MS BOOT PROGRAM\n");
#endif

#if 1
	// SYSCON SPI,GPIO & I/O enable
	REG32(0xbc100058) |= 0x00800002;
	REG32(0xbc10007c) |= (1<<26) | 0xc8;
	REG32(0xbe240000) |= 0xc8;
	REG32(0xbc100050) |= 0x0000608E;
	REG32(0xbc10004c) &= 0xFFFFFBF7;
	REG32(0xbc100078) &= 0x00000002;
#endif

	// SYSCON initialize
	pspSyscon_init();
#if 0
	// power control
	pspSysconCtrlLED(0,1);
	pspSysconCtrlLED(1,1);
	pspSysconCtrlHRPower(1);
	pspSysconCtrlMsPower(1);
#endif

	// reset reboot
#if UART_MESSAGE
	Kprintf("Reset for NAND Flash BOOT\n\n                    ");
#endif
	// RE-RESET to boot NAND Flash ROM
	pspSysconResetDevice(1);

#if UART_MESSAGE
	Kprintf("MS LOAD/BOOT ERROR\n");
#endif

#if UART_MESSAGE
	// message to UART when boot error
	Kprintf("Power OFF\n\n                    ");
#endif
	// power down
	while(1)
		pspSysconPowerStandby();
}
