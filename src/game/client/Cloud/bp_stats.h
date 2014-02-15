/*
Bisounours Party Cloud.
Original file by Louis Orygin Geuten.

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must
	not claim that you wrote the original software. If you use this
	software in a product, an acknowledgment in the product documentation
	would be appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#ifndef GAME_STATS_H
#define GAME_STATS_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "GameEventListener.h"

struct Kill_t 
{
	uint64 SteamID;
	Vector *pos;
	int x;
	int z;
	int y;
	char *p_weapon;
};

///-----------------------------------------------------------------------------------
/// This class is used by BPCloud : It register kills, and keep track of stats. Once Level_shutdown is fired, it upload them.
///-----------------------------------------------------------------------------------
class bp_stats : public CGameEventListener
{
public:
	DECLARE_CLASS_NOBASE( bp_stats );
	bp_stats();

	void Flush();
	void StartStats(const char *map);
	void StartToListen();
	void FireGameEvent(IGameEvent *event);	

private:
	void RegisterNewKill(CBasePlayer *player, const char *weapon);
	char *m_cCurMap;
	float m_flStartTime;
	CUtlLinkedList<Kill_t> m_pcKillList;
};

extern bp_stats *g_pStats;

inline bp_stats* BPStats()
{
	if(g_pStats == NULL)
		return new bp_stats();
	else
		return g_pStats;
};

#endif