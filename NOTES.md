# Notes/Interesting Findings

- Once you update to the latest version, you won't be able  
to install the same firmware file. (there is a workaround)  

- You can modify the header to set the  
firmware update version. When I incremented tje number by  
one, I could update a new firmware, but the new version doesn't  
update the internal number, which is a plus.  
- Additionally, the firmware numbers (X.XX) are printed in hex on  
the camera. The firmware is invalid if the hex number has any letters.  
- Both firmware numbers cannot be bigger than `0x99`.  

- Fujifilm used to offer a firmware update utility. It is available  
via the [internet archive](https://web.archive.org/web/20110318083436if_/http://www.fujifilm.com/support/digital_cameras/software/firmware/sdhc/exe/index/FPUpdateV.exe).  
It seems to include many things that could help reverse engineer  
Fujifilm's PTP commands.  

- Code is 32 bit ARM (not thumb)  

- The firmware includes SQLite and some sql code. Not sure why.  

- Unlike the cameras described in https://chdk.setepontos.com/index.php?topic=6484.0, the camera I have  
does not appear to have any batch scripting or hidden menu, as far as I know.  

- The code includes "A:\DCAA\auto_act.scr". Is this some kind of script? Making the file on the SD  
card doesn't seem to do anything.  
EDIT: the fujifilm firmware update utility seems to contain `AUTO_ACT.SCR`  
next to `FPUPDATE.DAT`. Still creating the file does not appear to do anything.  
I assume it means "Automatically activated script".  
EDIT: SCR could mean screen. It's used several other times in the firmware.  
Also, I thought it would be a SQLite script, but AUTO_ACT.SCR is on the Fujifilm Z3,  
which doesn't have SQLite or much code at all (4mb firmware).  

- There is a "Happy Birthday!" string in the firmware.  

- These strings suggest there may be a secret menu:  
```
"OSD DEBUG MODE SCREEN SELECT"  
"SCREEN   <UP/DOWN>    : "  
"WARNING  <RIGHT/LEFT> : "  
```
I tried calling the function that uses it and nothing happened.

- https://www.fujix-forum.com/threads/strange-folder-on-card.14120/

- Diagnostic software from repair manual is long gone, but I have found  
some artifacts: 
https://web.archive.org/web/*/fujifilm-di.intranets.com/*
https://web.archive.org/web/20080413055031/http://fujifilm-di.webexone.com/login.asp?loc=&link=