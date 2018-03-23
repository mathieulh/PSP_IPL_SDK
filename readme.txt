-----------------------------------------------------------------------------
PSP-IPL-SDK Rev.0.5 (2007.10.9) by BOOSTER.
Contact: https://twitter.com/Mathieulh
-----------------------------------------------------------------------------

This is an open SoftwareDevelopmentKit for PSP IPL code.
And example.

-----------------------------------------------------------------------------
directories
-----------------------------------------------------------------------------

-- driver / library --

IPL_SDK/              --- PSP driver and libs to build IPL/boot code.
LIBC/                 --- poor stdio libs.
tff/                  --- ChaN's open source FAT File System Module.
PANDORA/              --- PANDORA'S BATTERY (readme.txt only)

-- PSP RAW IPL code example --

MS_NORMAL/           --- Normal NAND boot for MS-IPL.
MS_MULTI_LOADER/     --- MS MultiLoader for MS-IPL.

-- boot code example for MS MultiLoader --

ML_FLASH_LED/         --- The simplest exsample.
ML_BIOS_DUMPER/       --- 'pre-ipl' BIOS ROM dumper.
ML_MAIN_BIN_DUMPER/   --- IPL "main.bin" dumper.
ML_DDR_DUMPER/        --- DDR-SDRAM dumper at last power off.
ML_RECOVERY_LOADER/   --- PANDORA'S BATTERY recovery menu bootloader.

-- tools --

msinst/               --- MS IPL code installer for Windows PC.

-- misc code / tool --

iplex/                --- 2nd IPL,"ipl.bin" loader with patch (for CFW/downgrader)
installer/            --- IPL patch & boot code installer to NAND Flash (for CFW)

-----------------------------------------------------------------------------
note
-----------------------------------------------------------------------------

----------------------------------------
kprintf message

Connect a serial port via HPRemote with 115Kbps.
The output of Kprintf() out there.
Plug mini-jack because SYSCON cuts power automatically when pulling out a plug.

--------------------------------------
MultiLoader file format

see MS_MULTI_LOADER/readme.txt

----------------------------------------
Top address of MultiLoader bootcode

"PROVIDE (__executable_start = 0x040e0000); . = 0x040e0000;"
in "IPL_SDK/pspipl.x"

----------------------------------------
DDR-SDRAM

The DDR-SDRAM can not use because it isn't initialized.
The simple way of using SDRAM is to be in the hook after main.bin is initialized.
see the "ML_DDRDUMP" example.

finally,the original SDRAM driver should be made.

----------------------------------------
Changelog (2018.3.23)

- Moved to GPLv3 License (some crt0 code has "PANDORA'S BATTERY" code.
Check readme.txt of "PANDORA'S BATTERY" for license.)
- Added compatibility to GCC newer than 4.0.2
