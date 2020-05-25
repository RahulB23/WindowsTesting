#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	HMODULE library;
	FARPROC func;
	
	if(argc < 3)
	{
		printf("%s <Library Name> <Function Name>\n", argv[0]);
		exit(-1);
	}

	library = LoadLibrary(argv[1]);
	if(library == NULL)
	{
		printf("Error: could not load library!\n");
		exit(-1);
	}

	func = GetProcAddress(library,argv[2]);
	if(func == NULL)
	{
		printf("Error: could find the function in the library!\n");
		exit(-1);
	}

	printf("%s is located at 0x%08x in %s\n", argv[2], (unsigned int)func, argv[1]);
	return 0;
}
