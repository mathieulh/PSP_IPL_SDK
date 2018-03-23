PSP normal boot loader for pandora's battery.

--------------------
What is it?

This is IPL code to boot PSP normally with pandora's battery.
Because the code is small, it doesn't need mspformat with most case.

This code send a reset request to the SYSCON.
Then, the PSP does Normal reboot by the SYSCON.

You doesn't need to restore battery serial code to normal operation.
Keep your pandora's battery!

--------------------
How to install

1.execute "msinst ipl_ms.bin" on command prompt of Windows XP/2K.
2.Connect MsProDuo to PC via USB.
 It is possible to use PSP USB Connection,or any USB MsProDuo Reader.
3.press 'Y' to install.

Or,
Write the ipl_ms.bin to absolute sectors 0x10-0x19 of MsProDuo.

--------------------
next

The multi_boot_loader which does boot any IPL by buttons select comes soon.

