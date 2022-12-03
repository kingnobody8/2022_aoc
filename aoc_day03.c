

struct ad3_input_stat
{
	int NumberOfPacks;
	int NumberOfItems;
};

struct ad3_pack
{
	int Size;
	char* Comp1;
	char* Comp2;
};


struct ad3_input_stat Ad3DetermineInputStat(const char* aTxt)
{
	struct ad3_input_stat Result = { 0 };

	const char* Iter = aTxt;
	while (*Iter != 0)
	{
		char C = *Iter;
		if (C == '\n')
		{
			++Result.NumberOfPacks;
		}
		else if (C != '\r')
		{
			++Result.NumberOfItems;
		}

		++Iter;
	}

	return Result;
}

bool IsLowercase(char aChar)
{
	return aChar >= 'a' && aChar <= 'z';
}

int Ad3Part1(struct ad3_input_stat aStat, struct ad3_pack* aPackA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfPacks; ++i)
	{
		struct ad3_pack* Pack = &aPackA[i];

		char SharedItem = 0;

		for (int s1 = 0; s1 < Pack->Size && SharedItem == 0; ++s1)
		{
			char s1Item = Pack->Comp1[s1];
			for (int s2 = 0; s2 < Pack->Size && SharedItem == 0; ++s2)
			{
				char s2Item = Pack->Comp2[s2];

				if (s1Item == s2Item)
				{
					SharedItem = s1Item;
				}
			}
		}


		if (SharedItem)
		{
			if (IsLowercase(SharedItem))
			{
				Result += SharedItem - 'a' + 1;
			}
			else
			{
				Result += SharedItem - 'A' + 27;
			}
		}
		else
		{
			int x = 0;
			x++;
		}

	}

	return Result;
}

int Ad3Part2(struct ad3_input_stat aStat, struct ad3_pack* aPackA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfPacks; i += 3)
	{
		struct ad3_pack* Pack1 = &aPackA[i];
		struct ad3_pack* Pack2 = &aPackA[i+1];
		struct ad3_pack* Pack3 = &aPackA[i+2];

		char SharedItem = 0;

		for (int s1 = 0; s1 < Pack1->Size * 2 && SharedItem == 0; ++s1)
		{
			char s1Item = Pack1->Comp1[s1];

			for (int s2 = 0; s2 < Pack2->Size * 2 && SharedItem == 0; ++s2)
			{
				char s2Item = Pack2->Comp1[s2];

				if (s1Item == s2Item)
				{
					for (int s3 = 0; s3 < Pack3->Size * 2 && SharedItem == 0; ++s3)
					{
						char s3Item = Pack3->Comp1[s3];
						if (s2Item == s3Item)
						{
							SharedItem = s1Item;
						}
					}
				}
			}
		}


		if (SharedItem)
		{
			if (IsLowercase(SharedItem))
			{
				Result += SharedItem - 'a' + 1;
			}
			else
			{
				Result += SharedItem - 'A' + 27;
			}
		}
		else
		{
			int x = 0;
			x++;
		}

	}


	return Result;
}


struct aoc_result aocday03()
{
	struct aoc_result Result = { 0 };

	struct memory_buffer FileMemory = PlatformReadEntireFile("day03.txt");

	if (FileMemory.Memory)
	{
		const char* FileTxt = (const char*)FileMemory.Memory;

		//allocate working memory based on input data
		struct ad3_input_stat Stat = Ad3DetermineInputStat(FileTxt);
		char* ItemA = (char*)PlatformAlloc(sizeof(char) * Stat.NumberOfItems);
		struct ad3_pack* PackA = (struct ad3_pack*)PlatformAlloc(sizeof(struct ad3_pack) * Stat.NumberOfPacks);

		//fill out the data arrays
		{
			int PackIndex = 0;
			int ItemIndex = 0;

			struct ad3_pack PackData = { 0 };

			const char* Iter = FileTxt;
			while (*Iter != 0)
			{
				char C = *Iter;
				if (C == '\n')
				{
					PackData.Size /= 2;
					PackData.Comp2 = PackData.Comp1 + PackData.Size;
					PackA[PackIndex++] = PackData;
					PackData = (const struct ad3_pack){ 0 };
				}
				else if (C != '\r')
				{
					ItemA[ItemIndex++] = C;
					if (PackData.Comp1 == 0)
					{
						PackData.Comp1 = &ItemA[ItemIndex-1];
					}
					PackData.Size++;
				}

				Iter++;
			}
		}

		Result.Res1 = Ad3Part1(Stat, PackA);
		Result.Res2 = Ad3Part2(Stat, PackA);

		//memory cleanup
		PlatformFree(PackA);
		PlatformFree(ItemA);
		PlatformFree(FileMemory.Memory);
	}

	return Result;
}
