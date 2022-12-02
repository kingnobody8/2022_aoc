
#include <stdbool.h>

#include "crt_alt.c"
#include "utility.h"
#include "utility.c"
#include "platform.c"

#include "aoc_day01.c"

int main()
{
	HANDLE  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	struct aoc_result Result = aocday01();

	DWORD Written = 0;
	WriteConsole(hConsoleOutput, "Hello World!", 12, &Written, 0);

	return (int)Written;
}

void __stdcall mainCRTStartup()
{
	int Result = main();
	ExitProcess(Result);
}