/****************************************************************************
	MS-FAT PSP bootfile loader
****************************************************************************/

#include <psptypes.h>
#include "kprintf.h"
#include "cache.h"
#include "syscon.h"
#include "kirk.h"
//#include "preipl.h"

#include "memstk.h"
#include "tff.h"		/* Tiny-FatFs declarations */

#include "patch.h"
#include "patch.h"

#define __VERBOSE__ 0

/****************************************************************************
	IPL load BOOT
****************************************************************************/
static int check_ipl_address(u32 address)
{
	if(address >= 0x04000000 && address < 0x04200000) return 1;
	return 0;
}

static int check_ipl_address2(u32 address,u32 offset)
{
	return check_ipl_address(address+offset);
}

/****************************************************************************
****************************************************************************/
int ms_poll_access(int cnt)
{
	// flip LED
	if(cnt&1)
		REG32(0xbe24000c)  = 0x40;
	else
		REG32(0xbe240008)  = 0x40;
	if( (cnt%32)==0)
	{
		// clear WDT
		pspSysconNop();
		return 1;
	}
	return 0;
}

/****************************************************************************
	IPL load BOOT
****************************************************************************/
static FATFS  FileSystem;
static FIL FileObject;

extern int _ms_init(void);

int ms_load_file(const char *path,void *top_addr)
{
	FRESULT result;
	WORD readed;
	BYTE *load_addr;
	int ttl_read;
	int block_cnt = 0;

	load_addr = (BYTE *)top_addr;

	result = f_mount(0,&FileSystem);
	if(result!=0)
	{
//Kprintf("f_mount error %08X\n",result);
		return -1;
	}

	result = f_open(&FileObject,path,FA_READ|FA_OPEN_EXISTING);
	if(result!=0)
	{
//Kprintf("f_open %s error\n",path);
		return -1;
	}

	ttl_read = 0;
	do{
		result = f_read(&FileObject,load_addr,0x8000,&readed);
		if(result!=0)
		{
//Kprintf("f_read error\n");
			return -1;
		}
//Kprintf("f_read addr=%08X size=%08X\n",(int)load_addr,readed);
		load_addr += readed;
		ttl_read  += readed;

		ms_poll_access(block_cnt++);

	}while(readed!=0);
//Kprintf("readed %d bytes\n",ttl_read);

	f_close(&FileObject);
	REG32(0xbe24000c)  = 0x40;

	return ttl_read;
}

#if 0
/****************************************************************************
	save file
****************************************************************************/
int ms_save_file(const char *path,const void *data,int size)
{
	FRESULT result;
	WORD writed;
	int num_write;
	int block_cnt = 0;

	//
	result = f_mount(0,&FileSystem);
	if(result!=0)
	{
#if __VERBOSE__
Kprintf("f_mount error %08X\n",result);
#endif
		return -1;
	}

	result = f_open(&FileObject,path,FA_WRITE|FA_CREATE_ALWAYS);
	if(result!=0)
	{
#if __VERBOSE__
Kprintf("f_open %s error\n",path);
#endif
		return -1;
	}

	while(size)
	{
		num_write = size>0x8000 ? 0x8000 : size;
		result = f_write(&FileObject,data,num_write,&writed);
		if(result!=0 || num_write!=writed)
		{
#if __VERBOSE__
Kprintf("f_write error\n");
#endif
			return -1;
		}
//Kprintf("f_write %08X:%04X\n",(int)data,writed);
		data += num_write;
		size -= num_write;

		if( ms_poll_access(block_cnt++) )
		{
#if __VERBOSE__
Kprintf("Left %08X\n",size);
#endif
		}
	}
//Kprintf("readed %d bytes\n",ttl_read);
	f_close(&FileObject);
	REG32(0xbe24000c)  = 0x40;

	return 0;
}
#endif

/****************************************************************************
	PSP_IPL loader

1.encrypted IPL BLOCK DATA (1000h BLOCK)
2.decrypted IPL BLOCK DATA (1000h BLOCK)

****************************************************************************/
static DWORD *ipl_buf = (DWORD *)0xbfd00000;

void *ms_load_ipl(const char *path)
{
	FRESULT result;
	WORD readed;
	DWORD *src;

	DWORD *top;
	DWORD size;
	void *entry;
	DWORD sum;
	int binary_type = 0;

	result = f_mount(0,&FileSystem);
	if(result!=0)
	{
#if __VERBOSE__
Kprintf("f_mount error %08X\n",result);
#endif
		return 0;
	}

	result = f_open(&FileObject,path,FA_READ|FA_OPEN_EXISTING);
	if(result!=0)
	{
#if __VERBOSE__
Kprintf("f_open %s error\n",path);
#endif
		return 0;
	}

	do{
		result = f_read(&FileObject,ipl_buf,0x1000,&readed);
		if(result!=0)
		{
#if __VERBOSE__
Kprintf("f_read error\n");
#endif
			entry = 0;
			goto error;
		}

		if(binary_type==0)
		{
			// chech decrypted HEADER
			top  = (DWORD *)(ipl_buf[0]);
			size = (ipl_buf[1]);
			entry= (void *)(ipl_buf[2]);
			sum  = (ipl_buf[3]);
			if(
				check_ipl_address(top) &&
				check_ipl_address(top+size) && 
				(entry==0 || ( entry >= top && entry<=(top+size))) &&
				sum == 0
			){
				// decrypted with header
				binary_type = 2;
			} else if(
				ipl_buf[0x60/4]==0x01 &&
				ipl_buf[0x64/4]==0    &&
				ipl_buf[0x68/4]==0
			)
			{
				// encrypted IPL
				binary_type = 1;
			}
		}

		// decrypt block
		if(binary_type==1)
		{
			if( pspKirkProc(ipl_buf,0x1000,ipl_buf,0x1000,0x01) < 0)
			{
#if __VERBOSE__
Kprintf("Decrypt error\n");
#endif
				entry = 0;
				goto error;
			}
		}

		// load BLOCK
		top  = (ipl_buf[0]);
		size = (ipl_buf[1]);
		entry= (ipl_buf[2]);
		sum  = (ipl_buf[3]);
		src  = &(ipl_buf[4]);
#if __VERBOSE__
Kprintf("TOP %08X SIZE %08X ENTRY %08X SUM %08X\n",top,size,entry,sum);
#endif
		while(size)
		{
			*top++ = *src++;
			size -= 4;
		}

	}while(entry==0);
//Kprintf("readed %d bytes\n",ttl_read);
error:
	f_close(&FileObject);

	return entry;
}

/****************************************************************************
	universal IPL/file  loader

1.encrypted IPL BLOCK DATA
2.decrypted IPL BLOCK DATA
3.binary included header of single decrypted IPL BLOCK DATA.

****************************************************************************/
void *ms_load_bootloader(const char *path)
{
	FRESULT result;
	WORD readed;
	DWORD *src;

	DWORD *top;
	DWORD size;
	void *entry;
	DWORD sum;
	int file_type;

	result = f_mount(0,&FileSystem);
	if(result!=0)
	{
#if __VERBOSE__
Kprintf("f_mount error %08X\n",result);
#endif
		return 0;
	}

	// check file format
	result = f_open(&FileObject,path,FA_READ|FA_OPEN_EXISTING);
	if(result!=0)
	{
#if __VERBOSE__
Kprintf("f_open %s error\n",path);
#endif
		return 0;
	}

	// check 1st block
	result = f_read(&FileObject,ipl_buf,0x10,&readed);

	// chech decrypted IPL HEADER
	top  = (DWORD *)(ipl_buf[0]);
	size = (ipl_buf[1]);
	entry= (void *)(ipl_buf[2]);
	sum  = (ipl_buf[3]);
	if(
		check_ipl_address(top) &&
		check_ipl_address(top+size) && 
		(entry==0 || ( entry >= top && entry<=(top+size))) &&
		sum == 0
	){
		// decrypted with header
Kprintf("Decrypted Format\n");
		if(entry==NULL)
		{
			// multi header == decrypted IPL
			f_close(&FileObject);
			return ms_load_ipl(path);
		}

		// single decrypted binary
#if __VERBOSE__
Kprintf("Single Binary Format\n");
#endif
		while(size>0)
		{
			result = f_read(&FileObject,top,0x8000,&readed);
			if(result!=0)
			{
//Kprintf("f_read error\n");
				return -1;
			}
#if __VERBOSE__
Kprintf("f_read addr=%08X size=%08X\n",(int)top,readed);
#endif
			top  += readed/4;
			size -= readed;
		}
		f_close(&FileObject);
		return entry;
	}

	// check encrypted IPL format
	result = f_read(&FileObject,ipl_buf+0x10/4,0x1000-0x10,&readed);
	f_close(&FileObject);

	if( pspKirkProc(ipl_buf,0x1000,ipl_buf,0x1000,0x01) ==0)
	{
Kprintf("Encrypted Format\n");
		return ms_load_ipl(path);
	}

	//
Kprintf("Unsuported Format\n");
	return NULL;
}


/****************************************************************************
	initialize FAT system
****************************************************************************/
int ms_fat_init(void)
{
	pspMsInit();
}
