## cphookLoader64

A [Blumentals Program Protector](https://www.blumentals.net/protector/) v4.x protection bypass implemented as a memory loader.

### Screenshot & demo

![cphookloader64](img/cphookscr.png) 

### Overview

_CPHook64.dll_ is a module located in the Program Protector installation directory.

It hooks process creation functions ( CreateProcessA, CreateProcessW ), by jumping into this DLL, thus checking if the program is protected or not. If you supply correct password it continues normally with process creation, if you click Cancel it calls _kernel32.SetLastError()_ with ERROR_ACCESS_DENIED (0x00000005) parameter.

This DLL is loaded by every executable on the system, therefore it is also loaded by **explorer.exe**, which is a Windows shell.

**cphookLoader64** works by modifying CPHook64.dll in memory of explorer.exe processes, so that it bypasses password check, allowing you to run any protected program.


### Usage

Simply run **cphookloader64.exe**.   
Note that it will patch only currently loaded explorer.exe processes. Any newly created explorer.exe processes will require running cphookloader64 again.

### How to build

Install either GCC or MSVC compiler and set PATH environment variable correctly to point to your chosen compiler, then run **build.bat** script. The script prefers GCC,but if it is not available it will try to build using MSVC.

This loader is intended to be compiled as a **64-bit** binary, so make sure you have set your compiler to output **64-bit** image.

### Tested / Affected versions

Following Program Protector versions are tested/ affected:
 
**4.8, 4.11, 4.12, 4.13**


### Note on antivirus

Your antivirus software may flag this loader as a malware or potentially unwanted program (PUP).   
Please ignore this, and add it to your antivirus exclusion list.

### Support

[![bitcoin](https://img.shields.io/badge/donate-bitcoin-EF8E19)](bitcoin:bc1qjwedefp3xruv7qak72l89hzx5mx0c0ftq8qem7)

If you would like to see more projects like this one, please consider supporting me :)  

Donate bitcoin to the following address:

![btcqrcode](img/bcqrcode.png)

```
bc1qjwedefp3xruv7qak72l89hzx5mx0c0ftq8qem7
```