//CSGO bhop script (KEEP SPACE PRESSED) (Compile with -fpermissive to neglect typecasting precision loss)
//Just for fun :P

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

class Game
{
public:
	DWORD ModuleBase;
	HWND window;
	DWORD player;
	DWORD pid;
	HANDLE handle;

	Game(const char* window_title);						//Constructor
	~Game();										//Destructor
	template <typename T> T* Read(DWORD addr);		//ReadProcessMemory template wrapper
	bool OnGroundCheck();							//Check if player is on the ground
	DWORD GetModuleBase(const char* module_name);	//Get base of loaded module (client.dll)

};

//Constructor
Game::Game(const char* window_title)
{
	while (this->window == NULL) 		//Loop till find Game (Game Process may not have started)
	{
		this->window = FindWindow(NULL, window_title);
		if(this->window == NULL)
		{
			cout<<"Error finding window"<<endl;
		}
	}

	//Get PID
	GetWindowThreadProcessId(window, &this->pid);

	//Open process
	this->handle = OpenProcess(PROCESS_VM_READ, 0, (DWORD)this->pid);
	if(this->handle == NULL)
	{
		cout<<"Error opening process"<<endl;
	}

	this->ModuleBase = this->GetModuleBase("client.dll");

}

//Destructor (Close handle to process)
Game::~Game()
{
	CloseHandle(this->handle);
}

//ReadProcMem template wrapper
template <typename T>
T* Game::Read(DWORD addr)
{
	T* value;
	ReadProcessMemory(this->handle, (LPVOID)addr, &value, sizeof(T), NULL);
	return value;
}

//Get base of client.dll
DWORD Game::GetModuleBase(const char* module_name)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);
	if(snapshot == INVALID_HANDLE_VALUE)
	{
		cout<<"Error creating snapshot"<<endl;
	}

	MODULEENTRY32 module_entry;
	module_entry.dwSize = sizeof(MODULEENTRY32);
	BYTE* module_base = 0;

	if (snapshot && Module32First(snapshot, &module_entry))
	{
		while (Module32Next(snapshot, &module_entry))
		{
			if (lstrcmpi(module_entry.szModule, (LPCSTR)module_name) == 0)
			{
				module_base = module_entry.modBaseAddr;
				break;
			}
		}
	}

	CloseHandle(snapshot);
	return (DWORD)module_base;
}

//Check if player is on ground
bool Game::OnGroundCheck()
{
	UINT16 flag = (UINT16)this->Read<UINT16>(this->player + 0x100);		//OnGround flag at 0x100 from player
	if(flag == 256)
	{
		return FALSE;
	}

	return TRUE;
}

Game CSGO = Game("Counter-Strike: Global Offensive");

int main(void)
{
	cout<< "PID: " <<CSGO.pid<<endl;
	while (1)
	{
		CSGO.player = (DWORD)CSGO.Read<DWORD>(CSGO.ModuleBase + 0x00A77D2C);	//Player-Base offset in client.dll
		
		if (GetAsyncKeyState(VK_SPACE) && CSGO.OnGroundCheck())
		{
			SendMessage(CSGO.window, WM_KEYDOWN, VK_SPACE, 0x390000);
			SendMessage(CSGO.window, WM_KEYUP, VK_SPACE, 0x390000);
		}
	}

	return 0;
}

