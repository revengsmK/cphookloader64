#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>

#include "memtools.h"

/* cphookLoader64 memory helper functions */

IMAGE_NT_HEADERS *getPEHeader(HANDLE hProcess, DWORD_PTR baseAddr){
	
	IMAGE_DOS_HEADER DosHead;
	static IMAGE_NT_HEADERS hPE;
	BOOL rVal;
	BOOL rVal2;
	
	rVal = ReadProcessMemory(hProcess,(char*)baseAddr,&DosHead,sizeof(IMAGE_DOS_HEADER),NULL);
	rVal2 = ReadProcessMemory(hProcess,(char*)baseAddr + DosHead.e_lfanew,&hPE,sizeof(IMAGE_NT_HEADERS),NULL);
	
	if(rVal == 0 || rVal2 == 0)
		return NULL;
	
	
	return &hPE;
		
} 

/* Gets Process ID from specified process name, and stores them in array in case of multiple instances
 * Return value: Number of PID values stored
 * Caller provides memory allocation
 **/
 
int GetPIDByName(unsigned int *buf,char processName[]){
	
	HANDLE hSnapshot;
	PROCESSENTRY32 procEntry;
	DWORD sessionID = 0;
	int numOfPIDs = 0;
	
	// create snapshot of running processes in the system
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	
	// set size of PROCESSENTRY32 structure before call to Process32First
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	
	// get first process in the list
	Process32First(hSnapshot,&procEntry);
	
	// search for specific process name, and store PID value of all instances 
	while(Process32Next(hSnapshot,&procEntry)){
		
		// avoid other users processes
		if(!ProcessIdToSessionId(procEntry.th32ProcessID,&sessionID))
			continue;
		
		// printf("PID : %d\tSession ID:%d\n",(int)procEntry.th32ProcessID,(int)sessionID);
		
		if(strcmp(procEntry.szExeFile,processName) == 0){  
				// printf("Session ID:%d\n",(int)sessionID);
				buf[numOfPIDs] = procEntry.th32ProcessID;
				numOfPIDs++;
		}
}
	CloseHandle(hSnapshot);
	
	return numOfPIDs;
}


DWORD_PTR GetDLLBaseAddress(char moduleName[], DWORD pid){
	
	HANDLE hSnapshot;
	MODULEENTRY32 modEntry;
	DWORD_PTR base_address = 0;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
	
	// Initialize dwSize member before calling Module32First
	
	modEntry.dwSize = sizeof(MODULEENTRY32);
	
	// Get first module in the snapshot
	
	if(!Module32First(hSnapshot,&modEntry)){
		puts("Failed to load first module!\nAborting...");
		CloseHandle(hSnapshot);
		return 0;
	}
	
	// Search for specified module in the process
	
	while(Module32Next(hSnapshot,&modEntry)){
		
		if(strcmp(moduleName,modEntry.szModule) == 0)
		{
			base_address = (DWORD_PTR)modEntry.modBaseAddr;
			break;
		}
	}
	
	// Clean up snapshot object
	
	CloseHandle(hSnapshot);
	
	return base_address;
}



/* Searches for a specified unmasked byte pattern in a process */

DWORD_PTR seekbytes(HANDLE hProc, unsigned char pattern[] , DWORD_PTR base_addr , DWORD_PTR end_addr, SIZE_T numOfBytes){
	
	unsigned char buffer[4096];
	SIZE_T bytesRead = 0;
	DWORD_PTR block = base_addr;
	int i;
	
	while(block <= end_addr)
	{
		
		if(ReadProcessMemory(hProc,(char*)block,buffer,4096,&bytesRead) == 0){
			puts("Cannot read process memory!\nAborting...");
			break;
		}
		
		if(bytesRead == 0)
			return 0;
		
		for( i = 0 ; i < bytesRead; i++){
		
			if(memcmp(pattern,buffer+i,numOfBytes) == 0)
				return block + i;
		}
		
		block = block + bytesRead;
	}
	
	return 0;
}
