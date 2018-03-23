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
�����h���C�u�̏�Ԃ����̃t���O�̑g�ݍ��킹�̒l�ŕԂ���܂��B

STA_NOINIT
    �h���C�u������������Ă��Ȃ����Ƃ������t���O�B�V�X�e���E���Z�b�g�⃁�f�B�A�̎��O�����ŃZ�b�g����Adisk_initialize() �̐���I���ŃN���A�A���s�ŃZ�b�g����܂��B

STA_NODISK
    ���f�B�A���Z�b�g����Ă��Ȃ����Ƃ������t���O�B���f�B�A�����O����Ă���Ԃ̓Z�b�g����A���f�B�A���Z�b�g����Ă���Ԃ̓N���A����܂��B�Œ�f�B�X�N�ł͏�ɃN���A����Ă��܂��B
STA_PROTECTED
*/
DSTATUS disk_status (BYTE Drive)
{
//Kprintf("disk_status %d\n",Drive);
	return 0;
}

DRESULT disk_read (
  BYTE Drive,          /* �����h���C�u�ԍ� */
  BYTE* Buffer,        /* �ǂݏo���o�b�t�@�ւ̃|�C���^ */
  DWORD SectorNumber,  /* �ǂݏo���J�n�Z�N�^�ԍ� */
  BYTE SectorCount     /* �ǂݏo���Z�N�^�� */
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


