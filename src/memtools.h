#ifndef MEMTOOLS_H
#define MEMTOOLS_H

IMAGE_NT_HEADERS *getPEHeader(HANDLE hProcess, DWORD_PTR baseAddr);
DWORD_PTR seekbytes (HANDLE hProc, unsigned char pattern[] , DWORD_PTR base_addr , DWORD_PTR end_addr, SIZE_T numOfBytes);
DWORD_PTR GetDLLBaseAddress(char moduleName[],DWORD pid);
int GetPIDByName(unsigned int *buf , char processName[]);

#endif /*MEMTOOLS_H*/