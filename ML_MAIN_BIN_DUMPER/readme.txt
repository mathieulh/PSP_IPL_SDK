main.bin dumper.

1.copy raw 'encrypted' IPL file to "ms0:./ipl.bin"
2.copy "pspboot.bin" to "ms0:/ipl/ipl_up.bin"
3.install "ipl_ms.bin" multi IPL loader in PSP_IPL_SDK_04
4.hold LTRG+UP and power on from pandora's battery.

tested psp_ipl.bin(150) and nandipl_psp1000.bin(371) on PSP-1000.
nandipl_psp2000.bin(371) can't work on PSP-1000.

