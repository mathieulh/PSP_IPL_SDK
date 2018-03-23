/****************************************************************************
	PSP IPL interrupt controll
****************************************************************************/
#include <pspkernel.h>

u32 irq_disable(void)
{
	asm("\n\
	.word 0x70020024;\n\
	.word 0x70000026;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	"::);
}

void irq_resume(u32 intval)
{
	asm("\n\
	sync;\n\
	nop;\n\
	sync;\n\
	nop;\n\
	.word 0x70040026;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	nop;\n\
	"::);
}

