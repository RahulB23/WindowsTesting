#include<iostream>
#include<windows.h>
#include<tlhelp32.h>

using namespace std;

int main(){
	
	BOOL dbg;
	dbg = IsDebuggerPresent();
	cout<<"dbg : "<<dbg<<endl;

	DWORD me = GetCurrentProcessId();
  	cout<<me<<endl;

	HANDLE procsnap, hproc;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	procsnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if(procsnap == INVALID_HANDLE_VALUE){
    	cout<<"Invalid handle CreateToolhelp32Snapshot"<<endl;
    	return 1;
  	}

  	Process32First(procsnap, &pe32);
  	cout<<pe32.th32ProcessID<<endl;
  	DWORD i;
  	while(Process32Next(procsnap, &pe32)){
  	  	cout<<pe32.szExeFile<<endl;
  	  	if(pe32.szExeFile == "cmd.exe"){
  	  		cout<<"found";
  	  		hproc = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
  	  		BOOL tproc = TerminateProcess(hproc, 1234);
  	  		CloseHandle(hproc);
  	  		exit(0);
  	  	}
  	}

	return 0;
}