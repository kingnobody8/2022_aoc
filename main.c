#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <stdbool.h>

//when calling alloca msvc will embedd a call to the crt function 'chkstk' to make sure that the alloca doesn't overwrite program memory. we need to define it so that we don't get linker errors
void __chkstk(void)
{
	/*NO-OP*/
}

void DoAssertion()
{
	*(int*)(0) = 0; //force a crash - via write to null
}

struct memory_buffer
{
	void* Memory;
	size_t Size;
};

void* MyAlloc(size_t aSize)
{
	return VirtualAlloc(0, aSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void MyFree(void* aPtr)
{
	VirtualFree(aPtr, 0, MEM_RELEASE);
}

struct memory_buffer ReadEntireFile(const char* aFilepath)
{
	struct memory_buffer Result = { 0 };

	HANDLE FileHandle = CreateFileA(aFilepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER FileSize;
		if (GetFileSizeEx(FileHandle, &FileSize))
		{
			size_t FileSize32 = FileSize.QuadPart;
			size_t MemorySize32 = FileSize32 + 1;//NOTE (dhabig): need one extra byte for null terminator (in case file is a string of characters)
			Result.Memory = MyAlloc(MemorySize32);

			if (Result.Memory)
			{
				DWORD BytesRead;
				if (ReadFile(FileHandle, Result.Memory, (DWORD)FileSize32, &BytesRead, 0) && (FileSize32 == BytesRead))
				{
					Result.Size = FileSize32;
				}
				else
				{
					MyFree(Result.Memory);
					Result.Memory = 0;
				}
			}
			else
			{
				DoAssertion();
			}
		}
		else
		{
			DoAssertion();
		}

		CloseHandle(FileHandle);
	}
	else
	{
		DoAssertion();
	}

	return Result;
}

bool IsDigit(char aChar)
{
	//NOTE (dhabig): this will fail if the OS defines a non-standard order for the ASCII character set.
	bool Result = (aChar >= '0' && aChar <= '9');
	return Result;
}

struct ad1_input_stat
{
	int NumberOfElves;
	int NumberOfFoods;
};

struct ad1_elf
{
	int Total;
	int* FoodA;
	int FoodALength;
};

struct ad1_input_stat DetermineInputStat(const char* aTxt)
{
	struct ad1_input_stat Result = { 0 };

	const char* Iter = aTxt;
	bool LineHasValue = false;
	while (*Iter != 0)
	{
		char C = *Iter;
		if (IsDigit(C))
		{
			LineHasValue = true;
		}
		else if (C == '\n')
		{
			if (!LineHasValue)
			{
				++Result.NumberOfElves;
			}
			else
			{
				++Result.NumberOfFoods;
			}
			LineHasValue = false;
		}

		++Iter;
	}

	return Result;
}

//returns string end
const char* MyAtoi(const char* aString, int* aoutResult)
{
	*aoutResult = 0;
	int Negative = 0;
	if (*aString == '-') { Negative = 1; aString++; }
	if (*aString == '+') { aString++; }
	int V = 0;
	while (*aString >= '0' && *aString <= '9')
	{
		V = (V * 10) + (*aString++ - '0');
	}
	*aoutResult = Negative ? -V : V;
	return aString;
}

int aocday01_part1(struct ad1_input_stat aStat, int* aFoodA, struct ad1_elf* aElfA)
{
	int Result = 0;

	//determine which elf has the most calories
	{
		struct ad1_elf* HighestElf = &aElfA[0];
		for (int i = 1; i < aStat.NumberOfElves; ++i)
		{
			struct ad1_elf* IterElf = &aElfA[i];
			if (IterElf->Total > HighestElf->Total)
			{
				HighestElf = IterElf;
			}
		}

		Result = HighestElf->Total;
	}

	return Result;
}

#define SORT_FUNC(name) int (name) (const void* aLhs, const void* aRhs)
typedef SORT_FUNC(sort_func);

#define StackAlloc(aSize) (_alloca(aSize));

void* MemCpy(void* aDest, const void* const aSource, size_t aSize)
{
	void* Result = aDest;

	// Typecast src and dest addresses to (char *)
	char* SrcIter = (char*)aSource;
	char* DestIter = (char*)aDest;

	// Copy contents of src[] to dest[]
	for (size_t i = 0; i < aSize; i++)
	{
		DestIter[i] = SrcIter[i];
	}

	return Result;
}


void Swap(void* aAlpha, void* aBeta, size_t aSize)
{
	//allocate a local buffer on the stack to use for the swap
	char* Buffer = (char*)StackAlloc(aSize);

	MemCpy(Buffer, aAlpha, aSize);
	MemCpy(aAlpha, aBeta, aSize);
	MemCpy(aBeta, Buffer, aSize);
}


void Sort(void* aBase, int aElementNum, size_t aElementSize, sort_func* _CompareFunction)
{
	for (int i = 0; i < aElementNum - 1; i++)
	{
		for (int j = 0; j < aElementNum - i - 1; j++)
		{
			void* jthing = (char*)aBase + j * aElementSize;
			void* nthing = (char*)aBase + (j + 1) * aElementSize;

			if (_CompareFunction(jthing, nthing) > 0)
			{
				Swap(jthing, nthing, aElementSize);
			}
		}
	}
}

SORT_FUNC(ElfSortFunc)
{
	struct ad1_elf* LElf = (struct ad1_elf*)(aLhs);
	struct ad1_elf* RElf = (struct ad1_elf*)(aRhs);
	return RElf->Total - LElf->Total;
}

int aocday01_part2(struct ad1_input_stat Stat, int* aFoodA, struct ad1_elf* aElfA)
{
	int Result = 0;

	//determine the total of the top 3 elves
	{
		Sort(aElfA, Stat.NumberOfElves, sizeof(struct ad1_elf), &ElfSortFunc);

		for (int i = 0; i < 3; ++i)
		{
			Result += aElfA[i].Total;
		}
	}

	return Result;
}

struct aoc_result
{
	int Res1;
	int Res2;
};

struct aoc_result aocday01()
{
	struct aoc_result Result = { 0 };

	struct memory_buffer FileMemory = ReadEntireFile("day01.txt");

	if (FileMemory.Memory)
	{
		const char* FileTxt = (const char*)FileMemory.Memory;

		struct ad1_input_stat Stat = DetermineInputStat(FileTxt);
		int* FoodA = MyAlloc(sizeof(int) * Stat.NumberOfFoods);
		struct ad1_elf* ElfA = (struct ad1_elf*)MyAlloc(sizeof(struct ad1_elf) * Stat.NumberOfElves);

		//fill out the data arrays
		{
			int FoodIndex = 0;
			int ElfIndex = 0;

			struct ad1_elf ElfData = { 0 };
			ElfData.FoodA = &FoodA[FoodIndex];
			bool LineHasValue = false;

			const char* Iter = FileTxt;
			while (*Iter != 0)
			{
				char C = *Iter;
				if (IsDigit(C))
				{
					LineHasValue = true;
					int Value = 0;
					Iter = MyAtoi(Iter, &Value);
					FoodA[FoodIndex++] = Value;
					ElfData.Total += Value;
					++ElfData.FoodALength;
				}
				else if (C == '\n')
				{
					if (!LineHasValue)
					{
						ElfA[ElfIndex++] = ElfData;
						ElfData = (const struct ad1_elf){ 0 };
						ElfData.FoodA = &FoodA[FoodIndex];
					}
					LineHasValue = false;
				}
				++Iter;
			}
		}

		Result.Res1 = aocday01_part1(Stat, FoodA, ElfA);
		Result.Res2 = aocday01_part2(Stat, FoodA, ElfA);

		MyFree(ElfA);
		MyFree(FoodA);
		MyFree(FileMemory.Memory);
	}

	return Result;
}

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