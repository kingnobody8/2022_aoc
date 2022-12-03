
enum en_rps
{
	Rock = 1,
	Paper = 2,
	Scissors = 3
};

enum en_outcome
{
	Win = 6,
	Draw = 3,
	Loss = 0
};

enum en_rps DetermineAction(char aValue)
{
	enum en_rps Result = { 0 };
	switch (aValue)
	{
	case 'A': /*fallthrough*/
	case 'X':
		Result = Rock;
		break;
	case 'B': /*fallthrough*/
	case 'Y':
		Result = Paper;
		break;
	case 'C': /*fallthrough*/
	case 'Z':
		Result = Scissors;
		break;
	}
	return Result;
}

enum en_rps DetermineAction2(enum en_rps aOp, char aMy)
{
	enum en_rps Result = { 0 };

	enum en_outcome Outcome = { 0 };

	switch (aMy)
	{
	case 'X': Outcome = Loss; break;
	case 'Y': Outcome = Draw; break;
	case 'Z': Outcome = Win; break;
	}

	switch (Outcome)
	{
	case Loss:
	{
		switch (aOp)
		{
		case Rock: Result = Scissors; break;
		case Paper: Result = Rock; break;
		case Scissors: Result = Paper; break;
		}
	} break;
	case Draw:
	{
		Result = aOp;
	} break;
	case Win:
	{
		switch (aOp)
		{
		case Rock: Result = Paper; break;
		case Paper: Result = Scissors; break;
		case Scissors: Result = Rock; break;
		}
	} break;
	}

	return Result;
}

int DetermineRoundResult(enum en_rps aOpponent, enum en_rps aMine)
{
	int Result = 0;

	if (aOpponent == aMine)
	{
		Result += Draw;
	}
	else if ((aMine == Rock && aOpponent == Scissors) ||
		(aMine == Paper && aOpponent == Rock) ||
		(aMine == Scissors && aOpponent == Paper))
	{
		Result += Win;
	}
	else
	{
		Result += Loss;
	}

	Result += (int)aMine;

	return Result;
}

struct ad2_input_stat
{
	int NumberOfRounds;
};

struct ad2_round
{
	int Score;
	enum en_rps Opponent;
	enum en_rps Mine;
};


struct ad2_input_stat Ad2DetermineInputStat(const char* aTxt)
{
	struct ad2_input_stat Result = { 0 };

	const char* Iter = aTxt;
	while (*Iter != 0)
	{
		char C = *Iter;
		if (C == '\n')
		{
			++Result.NumberOfRounds;
		}

		++Iter;
	}

	return Result;
}

int Ad2Part1(struct ad2_input_stat aStat, struct ad2_round* aRoundA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfRounds; ++i)
	{
		Result += aRoundA[i].Score;
	}

	return Result;
}

int Ad2Part2(struct ad2_input_stat aStat, struct ad2_round* aRoundA)
{
	int Result = 0;

	for (int i = 0; i < aStat.NumberOfRounds; ++i)
	{
		Result += aRoundA[i].Score;
	}

	return Result;
}


struct aoc_result aocday02()
{
	struct aoc_result Result = { 0 };

	struct memory_buffer FileMemory = PlatformReadEntireFile("day02.txt");

	if (FileMemory.Memory)
	{
		const char* FileTxt = (const char*)FileMemory.Memory;

		//allocate working memory based on input data
		struct ad2_input_stat Stat = Ad2DetermineInputStat(FileTxt);
		struct ad2_round* Round1A = (struct ad2_round*)PlatformAlloc(sizeof(struct ad2_round) * Stat.NumberOfRounds);
		struct ad2_round* Round2A = (struct ad2_round*)PlatformAlloc(sizeof(struct ad2_round) * Stat.NumberOfRounds);

		//fill out the data arrays
		{
			int RoundIndex = 0;

			struct ad2_round RoundData = { 0 };

			const char* Iter = FileTxt;
			while (*Iter != 0)
			{
				char Op = Iter[0];
				char My = Iter[2];

				{
					struct ad2_round* Round = &Round1A[RoundIndex];

					Round->Opponent = DetermineAction(Op);
					Round->Mine = DetermineAction(My);
					Round->Score = DetermineRoundResult(Round->Opponent, Round->Mine);
				}

				{
					struct ad2_round* Round = &Round2A[RoundIndex];
					Round->Opponent = DetermineAction(Op);
					Round->Mine = DetermineAction2(Round->Opponent, My);
					Round->Score = DetermineRoundResult(Round->Opponent, Round->Mine);
				}

				RoundIndex++;
				Iter += 5;
			}
		}

		Result.Res1 = Ad2Part1(Stat, Round1A);
		Result.Res2 = Ad2Part2(Stat, Round2A);

		//memory cleanup
		PlatformFree(Round1A);
		PlatformFree(Round2A);
		PlatformFree(FileMemory.Memory);
	}

	return Result;
}
