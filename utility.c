
bool IsDigit(char aChar)
{
	//NOTE (dhabig): this will fail if the OS defines a non-standard order for the ASCII character set.
	bool Result = (aChar >= '0' && aChar <= '9');
	return Result;
}

//returns string end
const char* Atoi(const char* aString, int* aoutResult)
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

#define StackAlloc(aSize) (_alloca(aSize));

void Swap(void* aAlpha, void* aBeta, size_t aSize)
{
	//allocate a local buffer on the stack to use for the swap
	char* Buffer = (char*)StackAlloc(aSize);

	MemCpy(Buffer, aAlpha, aSize);
	MemCpy(aAlpha, aBeta, aSize);
	MemCpy(aBeta, Buffer, aSize);
}

#define SORT_FUNC(name) int (name) (const void* aLhs, const void* aRhs)
typedef SORT_FUNC(sort_func);

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

