#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <ctime>
#include <tchar.h>
#include <psapi.h>
#include <thread>
#define str std::string
// g++ hack.cpp -o hack -lpsapi

class Memory {
    public:
        static int Read(DWORD process_id, LPCVOID Address) {
            int buffer;
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, process_id);
            if(ReadProcessMemory(hProcess, (LPCVOID)Address, (LPVOID)&buffer, sizeof(buffer), 0) == 0) {
                return 0; CloseHandle(hProcess);
            }
            return buffer; CloseHandle(hProcess);
        }
        static bool Write(DWORD process_id, LPCVOID Address, int buffer) {
            HANDLE hProcess = OpenProcess((0x0008) | (0x0020) | (0x0400), 0, process_id);
            if (WriteProcessMemory(hProcess, (LPVOID)Address, (LPVOID)&buffer, sizeof(buffer), 0) == 0) {
                return 0; CloseHandle(hProcess);
            }
            return 1; CloseHandle(hProcess);
        }
};

void PrintProcessNameAndID( DWORD processID ) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess( 1024 | PROCESS_VM_READ,  FALSE, processID );

    if (NULL != hProcess ) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
    	}
    }
	_tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );

    CloseHandle( hProcess );
}

int toInt(str string) {
	try {
		int integer = std::stoi(string);
		return integer;
	} catch (std::invalid_argument const &error) { exit(EXIT_FAILURE);
		std::cout << "Bad input: std::invalid_argument thrown" << '\n'; 
	} catch (std::out_of_range const &error) { exit(EXIT_FAILURE);
		std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
	}
    return 0;
}

int main(int argc, char const *argv[])
{
    Memory Memory;
    // help
	if (argc <= 1 || str(argv[1]) == "help") {
		std::cout << "\nhack *process_id* where *address* = *value*" << std::endl;
		std::cout << "hack process" << std::endl;
        std::cout << "hack *process_id* in *address*" << std::endl;
    // process id
	} else if (str(argv[1]) == "process") {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        
        if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) ){ return 1; }
        cProcesses = cbNeeded / sizeof(DWORD);

        for (auto i = 0; i < cProcesses; i++ ) {
            if( aProcesses[i] != 0 ) { PrintProcessNameAndID( aProcesses[i] ); }
        }
    // write
    } else if (str(argv[2]) == "where" && str(argv[4]) == "="){
		int buffer;
		DWORD ProcessId = (DWORD)toInt(argv[1]);
		DWORD Address = std::stoi(argv[3], 0, 16);
		int MemoryValue = Memory.Read(ProcessId, (LPCVOID)Address);

		if (MemoryValue == 0) {
			DWORD error = GetLastError();
			std::cout << "Error is " << error << std::endl; exit(EXIT_FAILURE);
		}

		std::cout << "Process id: " << ProcessId << std::endl;
		std::cout << "Addres: " << Address << std::endl;
		std::cout << "Old vallue is: " << buffer << std::endl; 

		buffer = (DWORD)toInt(argv[5]);
		if (Memory.Write(ProcessId, (LPCVOID)Address, buffer) == 0)
		{
			DWORD error = GetLastError();
			std::cout << "Error is " << error << std::endl; exit(EXIT_FAILURE);
		}
		MemoryValue = Memory.Read(ProcessId, (LPCVOID)Address);
		if (MemoryValue == 0)
		{
			DWORD error = GetLastError();
			std::cout << "Error is " << error << std::endl; exit(EXIT_FAILURE);
		}
		std::cout << "New vallue: " << buffer << std::endl;	
    // read
	} else if (str(argv[2]) == "in") {
		int buffer;
		DWORD ProcessId = (DWORD)toInt(argv[1]);
		DWORD Address = std::stoi(argv[3], 0, 16);
		int MemoryValue = Memory.Read(ProcessId, (LPCVOID)Address);

        if (MemoryValue == 0) {
            DWORD error = GetLastError();
            std::cout << "Error is " << error << std::endl; exit(EXIT_FAILURE);
		}
        std::cout << buffer << std::endl;
    } 
    return EXIT_SUCCESS;
}