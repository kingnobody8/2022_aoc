
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

struct aoc_result
{
	int Res1;
	int Res2;
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

struct aoc_result aocday01()
{
	struct aoc_result Result = { 0 };

	struct memory_buffer FileMemory = PlatformReadEntireFile("day01.txt");

	if (FileMemory.Memory)
	{
		const char* FileTxt = (const char*)FileMemory.Memory;

		struct ad1_input_stat Stat = DetermineInputStat(FileTxt);
		int* FoodA = PlatformAlloc(sizeof(int) * Stat.NumberOfFoods);
		struct ad1_elf* ElfA = (struct ad1_elf*)PlatformAlloc(sizeof(struct ad1_elf) * Stat.NumberOfElves);

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
					Iter = Atoi(Iter, &Value);
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

		PlatformFree(ElfA);
		PlatformFree(FoodA);
		PlatformFree(FileMemory.Memory);
	}

	return Result;
}
