#include <string.h>
#include "tff.h"		/* Tiny-FatFs declarations */
#include "diskio.h"		/* Include file for user provided disk functions */
#include "memstk.h"

//#include "Kprintf.h"

extern int _ms_init(void);
extern int _ms_read_sector(int unk1, int addr);

DSTATUS disk_initialize (BYTE Drive)
{
//Kprintf("disk_initialize %d\n",Drive);
	return 0;
}

/*
物理ドライブの状態が次のフラグの組み合わせの値で返されます。

STA_NOINIT
    ドライブが初期化されていないことを示すフラグ。システム・リセットやメディアの取り外し等でセットされ、disk_initialize() の正常終了でクリア、失敗でセットされます。

STA_NODISK
    メディアがセットされていないことを示すフラグ。メディアが取り外されている間はセットされ、メディアがセットされている間はクリアされます。固定ディスクでは常にクリアされています。
STA_PROTECTED
*/
DSTATUS disk_status (BYTE Drive)
{
//Kprintf("disk_status %d\n",Drive);
	return 0;
}

DRESULT disk_read (
  BYTE Drive,          /* 物理ドライブ番号 */
  BYTE* Buffer,        /* 読み出しバッファへのポインタ */
  DWORD SectorNumber,  /* 読み出し開始セクタ番号 */
  BYTE SectorCount     /* 読み出しセクタ数 */
)
{
//Kprintf("disk_read %d\n",Drive);
	while(SectorCount--)
	{
		if( pspMsReadSector(SectorNumber,Buffer) < 0) return RES_ERROR;
		SectorNumber++;
		Buffer+=0x200;
	}
	return RES_OK;
}

#if _FS_READONLY == 0

DRESULT disk_write (
  BYTE Drive,
  const BYTE* Buffer,
  DWORD SectorNumber,
  BYTE SectorCount
)
{
//Kprintf("disk_write %d\n",Drive);
	while(SectorCount--)
	{
		if( pspMsWriteSector(SectorNumber,Buffer) < 0) return RES_ERROR;
		SectorNumber++;
		Buffer+=0x200;
	}
	return RES_OK;
}

DRESULT disk_ioctl (BYTE Drive, BYTE cmd, void* ptr)
{
// CTRL_SYNC : Clean-up cached data      
	return RES_OK;
}

DWORD get_fattime (void)
{
#if 0
	// do not RTC time
	u32 clk;
	pspSysconReadClock(&clk);
	Kprintf("pspSysconReadClock %08X\n",clk);
	pspSysconWriteClock(0x001FCEA4);
	pspSysconReadClock(&clk);
	Kprintf("pspSysconReadClock %08X\n",clk);

#endif

	return 0x00000000;

}

#endif


