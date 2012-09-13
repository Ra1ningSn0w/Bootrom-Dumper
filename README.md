/**
  * Bootrom-Dumper
  * Copyright (C) 2010 pod2g
  * Copyright (C) 2012 Ra1ningSn0w
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
 
 This is a Windows port of pod2g's Bootrom-Dumper for Windows
 
 Credits:
	[-] Bootrom-Dumper - pod2g
	[-] Syringe - Chronic-Dev-Team
	[-] Ported by Ra1ningSn0w
	
 What you need:
	[-] MinGW
	[-] MSYS
	[-] libusb-1.0
	[-] libreadline5-dev
	[-] arm-elf-toolchain (you can use the one from http://www.gnuarm.com/ , but only if you add the path to your Windows-Environment Variables afterwards)
	
 How to build:
	[-] Uncomment the variables for your device in bdu.c and payload.s
	[-] Run make
	
 How to use:
	[-] Connect your device in DFU-Mode via USB
	[-] Run ./bdu.exe
	[-] A file called bootrom.bin (about 64 KB) should be in directory afterwards