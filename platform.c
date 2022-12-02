#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

inline void PlatformDebugBreak()
{
	if (IsDebuggerPresent())
	{
		__debugbreak(); //this is a compiler intrinisc
	}
}

void PlatformAssert(const char* aExpression, const char* aFileName, int aLineNumber, const char* aMessage)
{
	const char* AssertMessage =
		"Assertion occured\n"
		"Press 'Abort' to exit application.\n"
		"Press 'Retry' to break.\n"
		"'Ignore' does nothing.\n";

	const int ret = MessageBoxA(NULL, AssertMessage, "Eko - Assertion Error", MB_ABORTRETRYIGNORE);

	// Break and debug
	if (ret == IDRETRY)
	{
		PlatformDebugBreak();
	}
	else if (ret == IDIGNORE)
	{
	}
	// Abort
	else
	{
		ExitProcess(1);
	}
}

#define Assert(CONDITION, MESSAGE)	\
	if(!(CONDITION))	\
	{	\
		PlatformAssert(#CONDITION, __FILE__, __LINE__, MESSAGE);	\
	}

#define AssertAlways(MESSAGE)	Assert(false, MESSAGE)

void* PlatformAlloc(size_t aSize)
{
	return VirtualAlloc(0, aSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void PlatformFree(void* aPtr)
{
	VirtualFree(aPtr, 0, MEM_RELEASE);
}

struct memory_buffer PlatformReadEntireFile(const char* aFilepath)
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
			Result.Memory = PlatformAlloc(MemorySize32);

			if (Result.Memory)
			{
				DWORD BytesRead;
				if (ReadFile(FileHandle, Result.Memory, (DWORD)FileSize32, &BytesRead, 0) && (FileSize32 == BytesRead))
				{
					Result.Size = FileSize32;
				}
				else
				{
					PlatformFree(Result.Memory);
					Result.Memory = 0;
				}
			}
			else
			{
				AssertAlways("");
			}
		}
		else
		{
			AssertAlways("");
		}

		CloseHandle(FileHandle);
	}
	else
	{
		AssertAlways("");
	}

	return Result;
}
