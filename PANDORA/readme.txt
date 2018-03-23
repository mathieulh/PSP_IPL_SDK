        ____       __      ____
       /\  _`\    /\ \    /\  _`\
       \ \ \/\_\  \_\ \___\ \ \/\ \
        \ \ \/_/_/\___  __\\ \ \ \ \
         \ \ \L\ \/__/\ \_/ \ \ \_\ \
          \ \____/   \ \_\   \ \____/
           \/___/     \/_/    \/___/

              PANDORA'S BATTERY
              =================

A universal PSP unbricker/downgrader by the Prometheus project, also
known as Team C+D.

REQUIREMENTS
- A spare PSP Battery. Any make will do. Using a spare is best, as it
  will not be possible to boot the PSP in its normal state with the
  modified battery.
- A spare Memory Stick Pro Duo of less than 4Gb. (256Mb should be
  enough for all applications).
- A PSP with a Custom Firmware such as OE, M33 or WC, or a PSP with
  the 1.5 original kernel.

INSTALLATION PROCEDURE
- Connect the PSP to the PC with the Memory Stick inside via the USB
  Connection.
- Format the Memory Stick using any utility.
- Use mspformat to format the Memory Stick logically.
- Remove the Memory Stick and disable the USB connection.
- Connect the Memory Stick again via the USB connection of the PSP.
- Manually create the PSP/GAME (optionally PSP/GAME150) folders on the
  Memory Stick.
- Copy the "battery" and "installer" programs to the PSP/GAME
  (optionally PSP/GAME150) folder.
- Copy the 1.50 official updater as UPDATE.PBP to the root of the
  Memory Stick.
- Remove the Memory Stick and disable the USB connection.
- Run "installer" through the firmware on the PSP. This application
  will create the firmware files on the root of the Memory Stick, as
  well as msipl.bin.
- Connect the Memory Stick again via the USB connection of the PSP. 
- Execute "msipl" application on the PC with the msipl.bin to write
  the IPL to memory stick.
- Remove the Memory Stick and disable the USB connection.
- Execute "battery" application on the PSP to modify the battery.
- Congratulation, you are now in posession of a "Magic Memory Stick"
  and a "JigKick Battery".

USAGE
- Insert Memory Stick into the PSP
- Remove power cord and insert the JigKick Battery into the PSP.
- A menu appears when the PSP boots. Follow the onscreen information
  and instructions.

NOTES AND RESTRICTIONS
- The firmware in the Memory Stick is not a full 1.50, but a minimal
  subset. Therefore, not all applications (such as flashers, or
  recovery apps) will load.
- Only Memory Stick Pro Duo are compatible. Memory Stick Duo
  (e.g. 32MB sticks) are not supported.
- The IDStorage cannot be totally restored, as there is no known way
  to regenerate it properly.
- It is strongly RECOMMENDED to make a flash dump of the PSP before
  any downgrading operation.
- There might be a BSOD at the end of the downgrade. This is normal,
  and happens on the standard downgraders.
- All hardware revisions known at this day are supported. 
- This release contains no Sony-copyrighted material.  All required
  Sony data is generated from the v1.50 update files.

CREDITS
- Please do not edit or remove these credits... ;)
- All work done by the Prometheus team aka Team C+D:

  Adrahil (VoidPointer)
  Booster
  Cswindle (Caretaker)
  Dark_AleX (Malyot)
  Ditlew
  Fanjita (FullerMonty)
  Joek2100 (CosmicOverSoul)
  Jim
  Mathieulh (WiseFellow)
  Nem (h1ckeyph0rce)
  Psp250
  Skylark
  TyRaNiD (bockscar)

- With thanks to everyone who has contributed to the PSPSDK, without which
  nothing would have been possible.
