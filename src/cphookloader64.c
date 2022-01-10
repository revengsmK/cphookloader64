#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "memtools.h"

/** Blumentals Program Protector v4.x CPHook64.dll / Explorer.exe *Loader* */


/* Banner generated at manytools.org */

void banner(void){

	puts("\n\n");
	puts("  ####  #####  ##  ##  ####   ####  ##  ##               ");
	puts(" ##  ## ##  ## ##  ## ##  ## ##  ## ## ##                ");
	puts(" ##     #####  ###### ##  ## ##  ## ####                 ");
	puts(" ##  ## ##     ##  ## ##  ## ##  ## ## ##                ");
	puts("  ####  ##     ##  ##  ####   ####  ##  ##               ");
	puts("                                                         ");
	puts(" ##      ####   ####  #####  ###### #####    ##       ## ");
	puts(" ##     ##  ## ##  ## ##  ## ##     ##  ##  ##    ##  ## ");
	puts(" ##     ##  ## ###### ##  ## ####   #####  #####  ###### ");
	puts(" ##     ##  ## ##  ## ##  ## ##     ##  ## ##  ##     ## ");
	puts(" ######  ####  ##  ## #####  ###### ##  ##  ####      ## ");
	puts("\n\t\t\tCoded by: Aleksandar");
	puts("\t\t\tVisit: https://github.com/revengsmK\n");
}



int main(void){
	
	unsigned int PIDs[100] = { 0 };
	unsigned int pid_count = 0;

	unsigned char pattern[] = { 0x0F, 0x95, 0xC3, 0x8B, 0xC3, 0xEB};
	unsigned char patch_bytes[] = { 0x0F, 0x95, 0xC3, 0xB0, 0x00, 0xEB};
	int i = 0;

	DWORD_PTR base_address;
	DWORD_PTR end_address;
	DWORD_PTR pattern_address;
	IMAGE_NT_HEADERS *hPE;
	HANDLE *hProc = NULL;
	HANDLE hProcLoader = NULL;

	
	banner();
	
	// OpenProcess() to loader itself
	
	hProcLoader = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,GetCurrentProcessId());
	
	if(hProcLoader == NULL){
		puts("Failed to open process!");
		return -1;
	}
	
	base_address = GetDLLBaseAddress("CPHook64.dll",PIDs[0]);
	
	if(base_address == 0)
	{
		puts("CPHook64.dll module NOT loaded!");
		return -1;
	}
	
	// Get explorer.exe PIDs and number of running instances
	
	pid_count = GetPIDByName(PIDs,"explorer.exe");
	
	if(pid_count == 0)
	{
		puts("No explorer.exe instances found!\nAborting...");
		return -1;
	}
	
	
	hProc = (HANDLE*)malloc(pid_count * sizeof(HANDLE));
	
	if(hProc == NULL)
	{
		puts("Failed to allocate memory!");
		return -1;
	}
	
	
	hPE = getPEHeader(hProcLoader,base_address);
	
	// Handle to the loader itself not needed anymore
	CloseHandle(hProcLoader);
	
	if(hPE == NULL)
	{
		puts("Failed to get PE header!\nAborting...");
		return -1;
	}
	
	end_address = base_address + hPE->OptionalHeader.SizeOfCode;
	
	printf("\nCPHook64.dll base address: %p\n",(PDWORD_PTR)base_address);
	
	printf("\nNumber of explorer.exe instances: %d\n\n\n",pid_count);
	
	puts("Trying to patch all explorer.exe processes...\n");
	
	for(i = 0 ; i < pid_count ; i++)
	{
		
		// OpenProcess() to all explorer.exe instances
		
		hProc[i] = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,FALSE,PIDs[i]);

		if(!hProc[i])
		{
			printf("Failed to open process for PID: %d!\n",PIDs[i]);
			continue;
		}
		
		pattern_address = seekbytes(hProc[i],pattern,base_address,end_address,6);
		
		
		if(pattern_address != 0)
		{
			// Patch CPHook64.dll module through running explorer.exe processes
			
			if(WriteProcessMemory(hProc[i],(char*)pattern_address,patch_bytes,6,NULL) == 0)
				puts("Cannot write to process memory!\n");
			else
			{
				printf("\n=> Process explorer.exe -> PID [%d] patched!\n",PIDs[i]);
		
				if(FlushInstructionCache(hProc[i],NULL,0) == 0)
					puts("Failed to flush instruction cache...\n");
			}
		}
		else
			puts("=> Pattern not found or process already patched!");
		
	}
	
	
	// Release all handles
	for(i = 0 ; i < pid_count ; i++)
		CloseHandle(hProc[i]);
	
	
	free(hProc);
	
	puts("\nPress any key to quit...");
	getch();
		
	return 0;
}