PSP MS MultiLoader IPL by BOOSTER.
Contact: https://twitter.com/Mathieulh
--------------------------------------
What is it?

This is IPL to support "JigKick Battery" of PANDORA'S BATTERY.
It can start up the FAT file which was chosen at the button.
And , it includes a normal boot function.

It will not need mspformat with most case.

--------------------------------------
How to install.

1.Double click "install_psp_ms_multi_loader_ipl.cmd" on Windows2K/XP (admin).
2.Connect MsProDuo to PC via USB MemoryReaderWriter(or PSP).
3.Press 'Y' for "Are You Sure ?[Y]" message to install the IPL to MsProDuo.

--------------------------------------
How to use.

1.Copy bootfile to "ms0:/ipl/ipl_xxx.bin" of MsProDuo which installed MultiLoader.
  (see "RECOVERY_LOADER/readme.txt" to boot padnora's "Magic Memory Stick")

2.Power off the PSP.

3.Hold LTRG and allow key to select boot file,or LTRG off to normal boot.
  LTRG+UP    -> load/run "ms0:/ipl/ipl_up.bin"
  LTRG+LEFT  -> load/run "ms0:/ipl/ipl_lt.bin"
  LTRG+RIGHT -> load/run "ms0:/ipl/ipl_rt.bin"
  LTRG+DOWN  -> load/run "ms0:/ipl/ipl_dn.bin"
  LTRG       -> load/run "ms0:/ipl/ipl.bin"
  other      -> normal boot from internal NAND flash.

4.Insert "JigKick Battery".

  The MS and WLAN LED are flashing when boot file are missing or boot error.

--------------------------------------
How to build boot file.

File format are binary included 16bytes PSP IPL likely header.

|offset|size| specification
|+0000 |  4 | top of load address
|+0004 |  4 | data size
|+0008 |  4 | entry point (0x0000000 then continue to next block)
|+000C |  4 | check sum of last block(0x00000000 for top block)
|+0010 |  ? | data body

Usage space are 0x04000000 to 0x041DFFFF,
because DDR-SDRAM was not initialized,
and the Multi loader uses 0x041E0000 to 0x041FFFFF.

Wait for PSP_IPL_SDK which will be immediately released.

--------------------------------------
Note.

This uses the IPL exploit block of the PANDORA'S BATTERY.
Please read the readme of PANDORA'S BATTERY.

