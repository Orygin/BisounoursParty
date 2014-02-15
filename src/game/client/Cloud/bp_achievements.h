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

///-----------------------------------------------------------------------------------
/// This class is used by BPCloud : It register and keep track of achievements and periodically uploade them.
///-----------------------------------------------------------------------------------
#ifndef GAME_ACHIEVEMENTS_H
#define GAME_ACHIEVEMENTS_H

#ifdef _WIN32
#pragma once
#endif

	struct Achievement_struct
	{
		char *Name;
		bool Achieved;
		int ProgressMax;
		int Progress;
		bool NeedUpdate;
	};

#include "GameEventListener.h"
#include "hl2orange.spa.h"

class bp_achievListener : public CGameEventListener // Will listen the earn and updates of achievements.
{
	public:
	DECLARE_CLASS_NOBASE( bp_achievListener );

	DECLARE_SIMPLE_DATADESC();

	bp_achievListener() 
	{
		m_iHealthTaken = 0;
		m_flRangeMineSuicide = 0;
	};
	void StartToListen();
	void FireGameEvent(IGameEvent *event);
	void Think();
protected:
	float m_flRangeMineSuicide;
	int m_iHealthTaken;

private:
};

class bp_achievements
{
public:
	bp_achievements();
	void StoreAchievements(char *xml);

	bool EarnAchievement(char *name);
	bool EarnAchievement(const char *name);
	bool UpdateAchievement(char *name, int amount);
	bool UpdateAchievement(const char *name, int amount);

	int GetProgress(char *name);
	bool IsEarned(char *name);
	
	char* GetFullName(char *name);
	char* GetFullDesc(char *name);

	char* GetLocalizedName(char *name);
	char* GetLocalizedDesc(char *name);

	void ShowNotification(char *name);

private:
 Achievement_struct m_pcAchievementList[64];
	int m_iAchievementNumber;
	bp_achievListener* OurListener;
};

extern bp_achievements *g_pAchievements;

inline bp_achievements* BPAchievements()
{
	if(g_pAchievements == NULL)
		return new bp_achievements();
	else
		return g_pAchievements;
}
#endif