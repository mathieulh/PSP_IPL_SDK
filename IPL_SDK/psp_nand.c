/****************************************************************************
	PSP IPL NAND Flash Driver
****************************************************************************/
#include <psptypes.h>
#include "psp_nand.h"

// 8MB buffer for data
#define BUFFER_SIZE (8*1024*1024)
unsigned char buf_data[BUFFER_SIZE];

// 1MB for all area of EXTRA data
unsigned char buf_extra[0x10 * 0x20 * 0x800];

/****************************************************************************
NAND_CTRL(0xbd101004):bit assign

R/-B : 0x01
???  : 0x02
-CLE : 0x04
-WE  : 0x08
CE   : 0x10
???  : 0x20
???  : 0x40
WP   : 0x80
-RE  : 0x100 or 0x200
ALE  : 0x200 or 0x100

****************************************************************************/

/* NAND I/F register s */


#define NAND_STS  (volatile u32 *)(0xbd101004)
#define NAND_CTRL (volatile u32 *)(0xbd101004)
#define NAND_CMD  (volatile u32 *)(0xbd101008)
#define NAND_ADR  (volatile u32 *)(0xbd10100c)
#define NAND_END  (volatile u32 *)(0xbd101014)

#define NAND_AUTO_ADR (volatile u32 *)(0xbd101020)
#define NAND_AUTO_CMD (volatile u32 *)(0xbd101024)
#define NAND_AUTO_STS (volatile u32 *)(0xbd101024)
#define NAND_AUTO_ERR (volatile u32 *)(0xbd101028)

#define NAND_MODE (volatile u32 *)(0xbd101000)
#define NAND_DAT  (volatile u32 *)(0xbd101300)

#define NAND_DATA  (volatile u32 *)(0xbff00000)
#define NAND_EXTRA (volatile u32 *)(0xbff00900)

/****************************************************************************
	reset CMD
****************************************************************************/
static void pspNandReset(void)
{
	*NAND_CMD = 0xff;
	while( *NAND_STS &0x01 ==0);
	*NAND_END = 0x01;
}

/****************************************************************************
****************************************************************************/
int pspNandReadSector(int sector,u32 *databuf,u32 *extrabuf)
{
	u32 *ptr;
	int size;

	while( *NAND_STS &0x01 ==0);

	// start command
	*NAND_AUTO_ADR = sector<<10;
	*NAND_AUTO_CMD = 0x0301;

	while(*NAND_AUTO_STS & 1);
	if(*NAND_AUTO_ERR) return -1;
;
	ptr = NAND_DATA;

	extrabuf[0] = ptr[0x900/4];
	extrabuf[1] = ptr[0x904/4];
	extrabuf[2] = ptr[0x908/4];
;
	for(size=0x200;size>0;size-=4)
		*databuf++ = *ptr++;
	return 0;
}

/****************************************************************************
****************************************************************************/
static u16 iplfat_buf[0x200/2];
static u32 extra_buf[3];

int pspNandReadIPLFat(void)
{
	int sector;
	pspNandReset();

	sector=0x80;
	do{
		if( pspNandReadSector(sector,iplfat_buf,extra_buf) < 0) return 0x8001018c;
		sector+=0x20;
	}while(extra_buf[1] != 0x6dc64a38);
}

/****************************************************************************
****************************************************************************/
int pspNandReadIPLBlock(int block,u32 *databuf)
{
	int sector;
	int size;

	sector = ( (iplfat_buf[block>>2]<<2) | (block & 3) ) * 0x08;
	for(size=0;size<8;size++)
	{
		if( pspNandReadSector(sector,databuf,extra_buf) < 0) return -1;
		if(extra_buf[1] != 0x6dc64a38) return -1;
		databuf += 0x200/4;
		sector++;
	}
	return 0;
}

