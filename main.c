
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

int main()
{
	HANDLE  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD Written = 0;
	WriteConsole(hConsoleOutput, "Hello World!", 12, &Written, 0);

	return (int)Written;
}

void __stdcall mainCRTStartup()
{
	int Result = main();
	ExitProcess(Result);
}