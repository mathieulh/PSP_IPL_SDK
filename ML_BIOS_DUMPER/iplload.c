/****************************************************************************
	PSP IPL loader from NAND Flash
****************************************************************************/

#include <psptypes.h>
#include "kprintf.h"
#include "cache.h"
#include "kirk.h"

#define READ_BUF ((volatile u32 *)0xbfd00000)

/****************************************************************************
	NAND transmit block
****************************************************************************/
static u32 trasnmit_iplblock(u32 *dst,u32 *src,int size)
{
	u32 sum=0;

	do{
		sum += *src;
		*dst++ = *src++;
	}while( (size-=4) >0);
	return sum;
}

/****************************************************************************
	NAND transmit block
****************************************************************************/
void *nand_load_ipl(void)
{
	int block;
	u32 *dest;
	u32 size;
	void *entry;
	u32 sum;

Kprintf("Read NAND IPL FAT\n");
	if( pspNandReadIPLFat() < 0) return 0;

	block = 0;
	sum = 0;
	do{
		if( pspNandReadIPLBlock(block,READ_BUF) < 0)
		{
Kprintf("pspNandReadIPLBlock error\n");
			while(1);
		}
		// decrypt 1000H block
		if( pspKirkProc(READ_BUF,0x1000,READ_BUF,0x1000,0x01) !=0)
		{
Kprintf("pspKirkProc error\n");
			while(1);
		}

		// +000c : check sum of last block
		if(sum != READ_BUF[3])
		{
Kprintf("checksum error %08X != %08X\n",sum,READ_BUF[3]);
			while(1);
		}
;
		// +0000 : destination address
		// +0004 : block size
		dest = READ_BUF[0];
		size = READ_BUF[1];
		if(dest)
		{
// transmit BLOCK body & calc sum32
			sum = trasnmit_iplblock(dest,&READ_BUF[0x10/4],size);
		}

		entry = (void *)(READ_BUF[2]);
		block++;
	}while(entry==0);

	pspClearIcache();
	pspClearDcache();

	return entry;
}
