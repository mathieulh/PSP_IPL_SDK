/*
	PSP IPL exanple code for MS-MultiLoader
*/

#include <psptypes.h>

#include "syscon.h"
#include "patch.h"

/****************************************************************************
	title message for header
****************************************************************************/
const char __title__[] __attribute__ (( section (".init"))) = "MS-MultiLoader Example";

/****************************************************************************
	entry point
****************************************************************************/
int main(void)
{
	u32 sts;

	// init I/O
	pspSyscon_init();
	pspSysconCtrlLED(0,1);
	pspSysconCtrlLED(1,1);
	pspSysconCtrlMsPower(1);

	// flash LED
	sts=pspSysconNop();
	while(1)
	{
		// both LED flash
		REG32(0xbe24000c)  = 0xC0;
		Syscon_wait(250000/4);
		REG32(0xbe240008)  = 0xC0;
		Syscon_wait(250000/4);

		// POWER SW sense
		if( !(sts&SYSCON_STS_POWER_SW_ON) && ((sts=pspSysconNop()) & SYSCON_STS_POWER_SW_ON) )
		{
			// power off
			pspSysconPowerStandby();
		}
	}

	return 0;
}
