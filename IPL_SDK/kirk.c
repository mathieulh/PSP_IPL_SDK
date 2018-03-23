/****************************************************************************
	PSP IPL KIRK driver
****************************************************************************/
#include <pspkernel.h>

//////////////////////////////////////////////////////////////////////
// KIRK functioon 
//////////////////////////////////////////////////////////////////////
int pspKirkProc(void *dst,u32 dsize,void *src,u32 ssize,u32 cmd)
{
	u32 intval;
	u32 sts;

	// enable decrypter HW
	intval = irq_disable();
	(*(vu32 *)0xbc100050) |= 0x80;
	irq_resume(intval);

	// clear cache
//	sceKernelDcacheWritebackInvalidateAll();
	pspClearDcache();

#if 0
// memory address mask
	
  r2 = 0x1fffffff;
  r3 = r18 & r2; // arg1
  r2 = r16 & r2; // arg3
#endif

//Kprintf("irq_disable\n");

//	intval = irq_disable();

	// go KIRK
	*(u32 *)(0xbde00010) = (u32)cmd;
	*(u32 *)(0xbde0002c) = ((u32)src)&0x1fffffff;
	*(u32 *)(0xbde00030) = ((u32)dst)&0x1fffffff;
#if 0
	*(u32 *)(0xbde00020) = 0x33; // for IRQ mode
#endif

//Kprintf("go kirk\n");

	*(u32 *)(0xbde0000c) = 0x01;  // EXEXUTE

//	irq_resume(intval);

//Kprintf("wait sts\n");

	do
	{
		sts = *(vu32 *)0xbde0001c;
	}while( (sts & 0x11)==0);


	*(u32 *)(0xbde00028) = sts;
	if(sts & 0x10)
	{
		*(u32 *)(0xbde0000c) = 0x02;
		do
		{
			sts = *(vu32 *)0xbde0001c;
		}while( (sts & 0x02)==0);
		*(u32 *)(0xbde00028) = sts;
		asm("sync"::);
		return -1;
	}
#if 0
	// disable decrypter HW
//	intval = irq_disable();
	(*(vu32 *)0xbc100050) &= ~0x80;
//	irq_resume(intval);
#endif
	asm("sync"::);
	return *(u32 *)(0xbde00014);
}

