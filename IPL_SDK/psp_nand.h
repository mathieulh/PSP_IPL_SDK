#ifndef __PSP_NAND_H__
#define __PSP_NAND_H__

int pspNandReadSector(int sector,u32 *databuf,u32 *extrabuf);
int pspNandReadIPLFat(void);
int pspNandReadIPLBlock(int block,u32 *databuf);

#endif
