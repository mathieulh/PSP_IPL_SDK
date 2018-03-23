#ifndef __PRE_IPL__
#define __PRE_IPL__

// PRE-IPL (BIOS ROM in PSP) routine
void (* piplEntry)(void) = (void *)0x80010000;

void (* piplDcache)(void) = (void *)0x800102d8;
void (* piplIcache)(void) = (void *)0x800102a0;

int (* piplNandRead)(u32 ppn, void *buf, void *extra) = (void *)0x80010334;

int (* piplMsInit)(void) = (void *)0x80010240;
int (* piplMsReadIplBlock)(int block, void *addr) = (void *)0x80010248;

int (* piplDecryptIplBLock)(void *dst,void *src) = (void *)0x80010620;

#endif
