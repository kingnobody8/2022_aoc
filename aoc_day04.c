

struct ad4_input_stat
{
	int NumberOfPairs;
};

struct ad4_range
{
	int Min;
	int Max;
};

struct ad4_pair
{
	struct ad4_range R1;
	struct ad4_range R2;
	//	bool Overlaps;
	//	bool FullContain;
};

struct ad4_input_stat Ad4DetermineInputStat(const char* aTxt)
{
	struct ad4_input_stat Result = { 0 };

	const char* Iter = aTxt;
	while (*Iter != 0)
	{
		char C = *Iter;
		if (C == '\n')
		{
			++Result.NumberOfPairs;
		}
		++Iter;
	}

	return Result;
}

bool ContainsOtherPair(struct ad4_pair aPair)
{
	bool Result = false;


	if (aPair.R2.Min >= aPair.R1.Min && aPair.R2.Min <= aPair.R1.Max &&
		aPair.R2.Max >= aPair.R1.Min && aPair.R2.Max <= aPair.R1.Max)
	{
		//check if R1 contains R2
		Result = true;
	}
	else if (aPair.R1.Min >= aPair.R2.Min && aPair.R1.Min <= aPair.R2.Max &&
		aPair.R1.Max >= aPair.R2.Min && aPair.R1.Max <= aPair.R2.Max)
	{
		//check if R2 contains R1
		Result = true;
	}

	return Result;
}

bool HasOverlap(struct ad4_pair aPair)
{
	bool Result = false;

	if ((aPair.R2.Min >= aPair.R1.Min && aPair.R2.Min <= aPair.R1.Max) ||
		(aPair.R2.Max >= aPair.R1.Min && aPair.R2.Max <= aPair.R1.Max))
	{
		Result = true;
	}
	else if ((aPair.R1.Min >= aPair.R2.Min && aPair.R1.Min <= aPair.R2.Max) ||
		(aPair.R1.Max >= aPair.R2.Min && aPair.R1.Max <= aPair.R2.Max))
	{
		Result = true;
	}

	return Result;
}

int Ad4Part1(struct ad4_input_stat aStat, struct ad4_pair* aPairA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfPairs; ++i)
	{
		struct ad4_pair* Iter = &aPairA[i];

		if (ContainsOtherPair(*Iter))
		{
			Result++;
		}
	}

	return Result;
}

int Ad4Part2(struct ad4_input_stat aStat, struct ad4_pair* aPairA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfPairs; ++i)
	{
		struct ad4_pair* Iter = &aPairA[i];

		if (HasOverlap(*Iter))
		{
			Result++;
		}
	}

	return Result;
}


struct aoc_result aocday04()
{
	struct aoc_result Result = { 0 };

	struct memory_buffer FileMemory = PlatformReadEntireFile("day04.txt");

	if (FileMemory.Memory)
	{
		const char* FileTxt = (const char*)FileMemory.Memory;

		//allocate working memory based on input data
		struct ad4_input_stat Stat = Ad4DetermineInputStat(FileTxt);
		struct ad4_pair* PairA = (struct ad4_pair*)PlatformAlloc(sizeof(struct ad4_pair) * Stat.NumberOfPairs);

		//fill out the data arrays
		{
			int PairIndex = 0;

			struct ad4_pair PairData = { 0 };

			const char* Iter = FileTxt;
			while (*Iter != 0)
			{
				char C = *Iter;
				if (IsDigit(C))
				{
					int Value = 0;
					Iter = Atoi(Iter, &Value);
					if (PairData.R1.Min == 0)
					{
						PairData.R1.Min = Value;
					}
					else if (PairData.R1.Max == 0)
					{
						PairData.R1.Max = Value;
					}
					else if (PairData.R2.Min == 0)
					{
						PairData.R2.Min = Value;
					}
					else
					{
						PairData.R2.Max = Value;
					}
				}

				if (C == '\n')
				{
					PairA[PairIndex++] = PairData;
					PairData = (const struct ad4_pair){ 0 };
				}

				Iter++;
			}
		}

		Result.Res1 = Ad4Part1(Stat, PairA);
		Result.Res2 = Ad4Part2(Stat, PairA);

		//memory cleanup
		PlatformFree(PairA);
		PlatformFree(FileMemory.Memory);
	}

	return Result;
}
