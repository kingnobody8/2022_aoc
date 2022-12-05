
#include <stdbool.h>

#include "crt_alt.c"
#include "utility.h"
#include "utility.c"
#include "platform.c"

struct aoc_result
{
	int Res1;
	int Res2;
};

#include "aoc_day01.c"
#include "aoc_day02.c"
#include "aoc_day03.c"
#include "aoc_day04.c"
#include "aoc_day05.c"

int main()
{
	HANDLE  hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	struct aoc_result Result = aocday01(); //65912 & 195625
	Result = aocday02(); //12535 & 15457
	Result = aocday03(); //8088 & 2522
	Result = aocday04(); //644 & 926
	Result = aocday05(); //SHMSDGZVC & VRZGHDFBQ

	DWORD Written = 0;
	WriteConsole(hConsoleOutput, "Hello World!", 12, &Written, 0);

	return (int)Written;
}

void __stdcall mainCRTStartup()
{
	int Result = main();
	ExitProcess(Result);
}