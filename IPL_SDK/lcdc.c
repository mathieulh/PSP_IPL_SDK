/****************************************************************************
	PSP IPL LCDC driver

note:
	Work in progress.
	It doesn't work yet.

****************************************************************************/
#include <psptypes.h>
#include "Kprintf.h"

#include "patch.h"
#include "syscon.h"
#include "lcdc.h"

#define BYPASS_ERR_CHECK 1

u32 pspLcdcInit(void *frame_buffer)
{
	// LCD POWER
	pspSysconCtrlPower(SYSCON_DEV_LCD|SYSCON_DEV_ON);

	// sceSysreg_driver_Unkonow_e88b77ed
	REG32(0xbc10003c) |= 0x01;

	//sceSysreg_driver_Unkonow_dca57573(1)
	//REG32(0xbc100068) |= ?

	// sceSysregDmacplusBusClockEnable : 00000010
	REG32(0xbc100050) |= 0x00000010;

	// sceSysregLcdcClkEnable : 00040000
	// sceSysregPwmClkEnable  : 00080000
	//REG32(0xBC100058)=0x01AD2601;
	REG32(0xbc100058) |= 0x000c0000;

	// select : bit 22-20
	REG32(0xBC100060)=0x00030070;

	// sceSysregLcdcIoEnable : 00000040
	// sceSysregPwmIoEnable : 00002000
	//REG32(0xBC100078)=0x01182AFA;
	REG32(0xbc100078) |= 0x00002040;

	// GPIO.0 (LCD ON)
	REG32(0xbe240000) |= 0x00000001;
	REG32(0xbe240008) = 0x00000001;

	// PwmStart(0,18,19,0)
	REG32(0xBE300000)=0x00000018;
	REG32(0xBE300004)=0x00000001;
	REG32(0xBE300008)=0x00000000;
	REG32(0xBE300010)=0x00000019;
	REG32(0xBE300014)=0x00000000; // 6

	// PwmStart(1,18,19,19)
	REG32(0xBE300020)=0x00000018;
	REG32(0xBE300024)=0x00000001;
	REG32(0xBE300028)=0x00000000;
	REG32(0xBE300030)=0x00000019;
	REG32(0xBE300034)=0x00000019;

	// LCDC
	REG32(0xBE140000)=0x03;
	REG32(0xBE140004)=0;
	REG32(0xBE140008)=0;
	REG32(0xBE140010)=0x00000029;
	REG32(0xBE140014)=0x02;
	REG32(0xBE140018)=0x02;
	REG32(0xBE14001C)=480;			// LCD X 
	REG32(0xBE140020)=0x0A;
	REG32(0xBE140024)=0x02;
	REG32(0xBE140028)=0x02;
	REG32(0xBE14002C)=272;			// LCD Y
	REG32(0xBE140040)=0x00000000;
	REG32(0xBE140044)=0x00000000;
	REG32(0xBE140048)=480;
	REG32(0xBE14004C)=272;
	REG32(0xBE140050)=0x00000001;

	// sceDmacplusLcdcSetBaseAddr
	REG32(0xBC800100)=(u32 *)frame_buffer;

	// sceDmacplusLcdcSetFormat
	REG32(0xBC800104)=0x00000000;
	REG32(0xBC800108)=0x000001E0; // X display
	REG32(0xBC80010C)=0x00000200; // X vram

	// sceDmacplusLcdcEnable
	REG32(0xBC800110)=0x00000003;

	// LCDC reset
	REG32(0xBE140000)&=~1;
	REG32(0xBE140000)|= 1;

	return 0;
}
