

struct ad5_input_stat
{
	int NumberOfBoxes;
	int NumberOfStacks;
	int HighestStartingStack;
	int NumberOfActions;
};

struct ad5_action
{
	int Count;
	int StackSource;
	int StackDest;
};

struct ad5_stack
{
	char* StackA;
	int StackALength;
};

struct ad5_input_stat Ad5DetermineInputStat(const char* aTxt)
{
	struct ad5_input_stat Result = { 0 };

	const char* Iter = aTxt;

	//Starting conditions
	bool HasOnlyNewline = true;
	while (*Iter != 0)
	{
		char C = *Iter;
		if (C != '\n' && C != '\r')
		{
			HasOnlyNewline = false;
		}
		if (C == '[')
		{
			Result.NumberOfBoxes++;
		}
		else if (IsDigit(C))
		{
			int Value = 0;
			Iter = Atoi(Iter, &Value);
			if (Value > Result.NumberOfStacks)
			{
				Result.NumberOfStacks = Value;
			}
		}
		else if (C == '\n')
		{
			if (HasOnlyNewline)
			{
				++Iter;
				break; //finished the starting condition section, move onto the actions section
			}
			Result.HighestStartingStack++;
			HasOnlyNewline = true;
		}
		++Iter;
	}
	Result.HighestStartingStack--; //subtract 1 to remove the indexing line

	//Actions
	while (*Iter != 0)
	{
		char C = *Iter;
		if (C == '\n')
		{
			Result.NumberOfActions++;
		}
		++Iter;
	}

	return Result;
}

void PlaceBoxInStack(struct ad5_stack* aStack, char aBox)
{
	aStack->StackA[aStack->StackALength++] = aBox;
}
char PopBoxFromStack(struct ad5_stack* aStack)
{
	char Result = aStack->StackA[--aStack->StackALength];
	aStack->StackA[aStack->StackALength] = 0; //to make it easier to read in debug
	return Result;
}

int Ad5Part1(struct ad5_input_stat aStat, struct ad5_action* aActionA, struct ad5_stack* aStackA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfActions; ++i)
	{
		struct ad5_action* Act = &aActionA[i];

		struct ad5_stack* Src = &aStackA[Act->StackSource - 1];
		struct ad5_stack* Dst = &aStackA[Act->StackDest - 1];

		for (int c = Act->Count; c > 0; --c)
		{
			char Box = PopBoxFromStack(Src);
			PlaceBoxInStack(Dst, Box);
		}
	}

	char Buffer[1024] = { 0 };
	for (int i = 0; i < aStat.NumberOfStacks; ++i)
	{
		Buffer[i] = aStackA[i].StackA[aStackA[i].StackALength - 1];
	}

	return Result;
}

int Ad5Part2(struct ad5_input_stat aStat, struct ad5_action* aActionA, struct ad5_stack* aStackA)
{
	int Result = 0;

	char* BoxScratch = (char*)PlatformAlloc(sizeof(char) * aStat.NumberOfBoxes);
	int BSIndex = 0;

	for (int i = 0; i < aStat.NumberOfActions; ++i)
	{
		BSIndex = 0;
		struct ad5_action* Act = &aActionA[i];

		struct ad5_stack* Src = &aStackA[Act->StackSource - 1];
		struct ad5_stack* Dst = &aStackA[Act->StackDest - 1];

		for (int c = Act->Count; c > 0; --c)
		{
			char Box = PopBoxFromStack(Src);
			BoxScratch[BSIndex++] = Box;
		}

		for (int c = Act->Count; c > 0; --c)
		{
			char Box = BoxScratch[c-1];
			PlaceBoxInStack(Dst, Box);
		}
	}

	PlatformFree(BoxScratch);

	char Buffer[1024] = { 0 };
	for (int i = 0; i < aStat.NumberOfStacks; ++i)
	{
		Buffer[i] = aStackA[i].StackA[aStackA[i].StackALength - 1];
	}

	return Result;
}


struct aoc_result aocday05()
{
	struct aoc_result Result = { 0 };

	struct memory_buffer FileMemory = PlatformReadEntireFile("day05.txt");

	if (FileMemory.Memory)
	{
		const char* FileTxt = (const char*)FileMemory.Memory;

		//allocate working memory based on input data
		struct ad5_input_stat Stat = Ad5DetermineInputStat(FileTxt);
		struct ad5_action* ActionA = (struct ad5_action*)PlatformAlloc(sizeof(struct ad5_action) * Stat.NumberOfActions);
		
		struct ad5_stack* StackA = (struct ad5_stack*)PlatformAlloc(sizeof(struct ad5_stack) * Stat.NumberOfStacks);
		char* BoxA = (char*)PlatformAlloc(sizeof(char) * Stat.NumberOfBoxes);
		struct ad5_stack* StackA2 = (struct ad5_stack*)PlatformAlloc(sizeof(struct ad5_stack) * Stat.NumberOfStacks);
		char* BoxA2 = (char*)PlatformAlloc(sizeof(char) * Stat.NumberOfBoxes);



		//fill out the data arrays
		{
			for (int i = 0; i < Stat.NumberOfStacks; ++i)
			{
				//worst case of having every box in one stack
				StackA[i].StackA = BoxA + (i * Stat.NumberOfBoxes);
				StackA[i].StackALength = 0;

				StackA2[i].StackA = BoxA2 + (i * Stat.NumberOfBoxes);
				StackA2[i].StackALength = 0;
			}

			int CharactersOnOneLine = 0;
			const char* Iter = FileTxt;
			while (true)
			{
				char C = *Iter;
				CharactersOnOneLine++;
				if (C == '\n')
				{
					break;
				}
				++Iter;
			}

			const int Offset = 1; // '[A' how far to get to the character
			const int Stride = 4; // '[A] ' => 4 chars

			for (int i = 0; i < Stat.NumberOfStacks; ++i)
			{
				for (int r = Stat.HighestStartingStack; r >= 1; --r)
				{
					int Index = (r-1) * CharactersOnOneLine + Offset + Stride * i;
					char C = FileTxt[Index];
					if (IsAlpha(C))
					{
						PlaceBoxInStack(&StackA[i], FileTxt[Index]);
						PlaceBoxInStack(&StackA2[i], FileTxt[Index]);
					}
					else
					{
						break;
					}
				}
			}

			//move along to the actions sections
			Iter += CharactersOnOneLine * Stat.HighestStartingStack;
			while (*Iter == '\r' || *Iter == '\n')
			{
				++Iter;
			}

			//fill in the actions array
			int ActionIndex = 0;

			//const char* Iter = FileTxt;
			while (*Iter != 0)
			{
				struct ad5_action* Act = &ActionA[ActionIndex++];

				Iter += 5; //skip the 'move ' chars
				Iter = Atoi(Iter, &Act->Count);
				Iter += 6; //skip the ' from ' chars
				Iter = Atoi(Iter, &Act->StackSource);
				Iter += 4; //skip the ' to ' chars
				Iter = Atoi(Iter, &Act->StackDest);

				while (*Iter == '\r' || *Iter == '\n')
				{
					++Iter;
				}
			}
		}

		Result.Res1 = Ad5Part1(Stat, ActionA, StackA);
		Result.Res2 = Ad5Part2(Stat, ActionA, StackA2);

		//memory cleanup
		PlatformFree(ActionA);
		PlatformFree(StackA);
		PlatformFree(BoxA);
		PlatformFree(StackA2);
		PlatformFree(BoxA2);
		PlatformFree(FileMemory.Memory);
	}

	return Result;
}
