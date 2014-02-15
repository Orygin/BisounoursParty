#include "cbase.h"
#include "bp_random.h"
#include <time.h>

#define PI 3.1415926

bool BPRandom::m_bInited = false;
MTRand_int32 BPRandom::mt(time(NULL));

int BPRandom::GetInt(int min, int max)
{
	unsigned long res = mt();

	return (res % max) + min;
}

float BPRandom::GetFloat(int min, int max)
{
	return GetInt(min,max) / PI;
}