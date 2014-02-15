#ifndef BPRAND_H
#define BPRAND_H

#include "mtrand.h"

class BPRandom
{
public:
	static int GetInt(int min, int max);
	static float GetFloat(int min, int max);

private:
	static void Init();
	static bool m_bInited;
	BPRandom();
	static MTRand_int32 mt;
};

#endif