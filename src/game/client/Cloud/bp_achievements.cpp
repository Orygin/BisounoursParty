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

 #include "cbase.h"
 #include "bp_achievements.h"
 #include "Cloud\bp_stats.h"
 #include "tinyxml.h"
 #include <fstream>
 #include <istream>
 #include <iostream>
 #include "Filesystem.h"
 #include "Cloud\bp_cloud.h"

 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include <vgui_controls/TextEntry.h>
 #include "ienginevgui.h"
 #include "vgui/ILocalize.h"
 #include "vgui_bpachievements.h"
 #include "igameevents.h"
 #include "util_shared.h"
 #include "achievement_notification_panel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

bp_achievements *g_pAchievements = NULL;
CBasePlayer* BP2UTIL_PlayerByUserId( int userID )
{
	for (int i = 1; i<=gpGlobals->maxClients; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );
		
		if ( !pPlayer )
			continue;

		if ( pPlayer->GetUserID() == userID )
		{
			return pPlayer;
		}
	}
	return NULL;
}
bool IsInAir(C_BaseEntity *pPlayer) // Custom func to check the player state
{
	if (pPlayer == NULL)
		return false;

	trace_t tr;
	Vector	vecStart, vecStop, vecDir;

	// get the angles
	//AngleVectors( pPlayer->GetAbsAngles(), &vecDir );
	vecDir = Vector(0,0,-90);
	// get the vectors
	vecStart = pPlayer->GetAbsOrigin();
	vecStop = vecStart + Vector(0,0,-10);

	// do the traceline
	UTIL_TraceLine( vecStart, vecStop, MASK_PLAYERSOLID, pPlayer, COLLISION_GROUP_DEBRIS, &tr );
	
	// check to see if we hit a NPC
	if ( !tr.DidHitWorld() )
		return true;
	else
		return false;
}
bp_achievements::bp_achievements()
{
	g_pAchievements = this;
	OurListener = new bp_achievListener();
}
void bp_achievements::StoreAchievements(char *xml)
{
	if(!xml)
		return;

	FileHandle_t fh = filesystem->Open( "achievements.xml", "w+", "MOD");
 
	if (fh)
	{
		filesystem->FPrintf(fh, "%s", xml);
			filesystem->Close(fh);
		const char *pGameDir = engine->GetGameDirectory();
		char FullPath[256];

		strcpy(FullPath, pGameDir);
		strcat(FullPath, "\\achievements.xml");

		TiXmlDocument *File = new TiXmlDocument(FullPath);

		bool loadOkay = File->LoadFile();

			if ( !loadOkay )
			{
				Warning("Could not parse XML\n");
				return;
			}
		
			TiXmlElement* List = 0;
			TiXmlElement* Achievement = 0;

			List = File->FirstChildElement("achievements");
			Achievement = List->FirstChildElement();
			int i = 0;
			while(Achievement)
			{
				const char *Achiev_status = Achievement->GetText();
				
				m_pcAchievementList[i].Name = new char[32];

				Q_strncpy(m_pcAchievementList[i].Name, Achievement->Value(), 32);

				char *digit = new char [3];
				digit[0] = m_pcAchievementList[i].Name[1];
				digit[1] = m_pcAchievementList[i].Name[2];
				digit[2] = m_pcAchievementList[i].Name[3];

				m_pcAchievementList[i].ProgressMax = atoi(digit);

				int Achiev_stat = atoi(Achiev_status);

				if(Achiev_stat >= m_pcAchievementList[i].ProgressMax)
				{
					m_pcAchievementList[i].Progress = Achiev_stat;
					m_pcAchievementList[i].Achieved = true;
				}
				else
				{
					m_pcAchievementList[i].Achieved = false;
					m_pcAchievementList[i].Progress = Achiev_stat;
				}

				Achievement = Achievement->NextSiblingElement();
				i++;
			}
			m_iAchievementNumber = i;
			OurListener->StartToListen();
	}


}
bool bp_achievements::IsEarned(char *name)
{
	if(!name)
		return false;
	for(int i = 0; i < m_iAchievementNumber;i++)
	{
		if(!strcmp(m_pcAchievementList[i].Name, name)) // Are we on the right achievement ?
		{
			return m_pcAchievementList[i].Achieved;
		}
	}

	return false;
}
int bp_achievements::GetProgress(char *name)
{
	if(!name)
		return 0;
	for(int i = 0; i < m_iAchievementNumber;i++)
	{
		if(!strcmp(m_pcAchievementList[i].Name, name)) // Are we on the right achievement ?
		{
			return m_pcAchievementList[i].Progress;
		}
	}

}
bool bp_achievements::EarnAchievement(char *name)
{
	if(!name)
		return false;
	for(int i = 0; i < m_iAchievementNumber;i++)
	{
		if(!strcmp(m_pcAchievementList[i].Name, name)) // Are we on the right achievement ?
		{
			if(m_pcAchievementList[i].Achieved)
				return false; // No need to earn again
			else
			{
				m_pcAchievementList[i].Achieved = true; // Update locally to avoid being out of sync with the server
				m_pcAchievementList[i].Progress = m_pcAchievementList[i].ProgressMax;
					CAchievementNotificationPanel *pPanel = GET_HUDELEMENT( CAchievementNotificationPanel );
					if ( pPanel )
					{		
						pPanel->AddNotification( "HL2_KILL_ODESSAGUNSHIP", g_pVGuiLocalize->Find(BPAchievements()->GetFullName(name)), g_pVGuiLocalize->Find(BPAchievements()->GetFullDesc(name)) );
					}
					//BPCloud()->SendUpdateAchievement(name,m_pcAchievementList[i].ProgressMax);
				return true;
			}
		}
	}
	return true;
}
void bp_achievements::ShowNotification(char *name)
{
	CAchievementNotificationPanel *pPanel = GET_HUDELEMENT( CAchievementNotificationPanel );
	if ( pPanel )
	{		
		pPanel->AddNotification( "HL2_KILL_ODESSAGUNSHIP", g_pVGuiLocalize->Find(GetFullName(name)), g_pVGuiLocalize->Find(GetFullDesc(name)) );
	}
}
bool bp_achievements::EarnAchievement(const char *name)
{
	if(!name)
		return false;
	char *cName = new char[strlen(name)];
	strcpy(cName, name);
	for(int i = 0; i < m_iAchievementNumber;i++)
	{
		if(!strcmp(m_pcAchievementList[i].Name, name)) // Are we on the right achievement ?
		{
			if(m_pcAchievementList[i].Achieved)
				return false; // No need to earn again
			else
			{
				m_pcAchievementList[i].Achieved = true; // Update locally to avoid being out of sync with the server
				m_pcAchievementList[i].Progress = m_pcAchievementList[i].ProgressMax;

				//BPCloud()->SendUpdateAchievement(cName, m_pcAchievementList[i].ProgressMax);
				return true;
			}
		}


	}
	return true;
}
bool bp_achievements::UpdateAchievement(char *name, int amount)
{
	if(!name)
		return false;
	for(int i = 0; i < m_iAchievementNumber;i++)
	{
		if(!strcmp(m_pcAchievementList[i].Name, name)) // Are we on the right achievement ?
		{
			if(m_pcAchievementList[i].Achieved)
				return false; // No need to earn again
			else
			{
				m_pcAchievementList[i].Progress += amount; // Update locally to avoid being out of sync with the server
				if(m_pcAchievementList[i].Progress >= m_pcAchievementList[i].ProgressMax)
				{
					m_pcAchievementList[i].Achieved = true;
					//BPCloud()->SendUpdateAchievement(name, m_pcAchievementList[i].ProgressMax);

					ShowNotification(name);
				}
				//else
					//BPCloud()->SendUpdateAchievement(name, m_pcAchievementList[i].Progress);				
			}
		}


	}
	return true;
}
bool bp_achievements::UpdateAchievement(const char *name, int amount)
{
	if(!name)
		return false;
	char *cName = new char[strlen(name)];
	strcpy(cName, name);
	for(int i = 0; i < m_iAchievementNumber;i++)
	{
		if(!strcmp(m_pcAchievementList[i].Name, name)) // Are we on the right achievement ?
		{
			if(m_pcAchievementList[i].Achieved)
				return false; // No need to earn again
			else
			{
				m_pcAchievementList[i].Progress += amount; // Update locally to avoid being out of sync with the server
				if(m_pcAchievementList[i].Progress >= m_pcAchievementList[i].ProgressMax)
				{
					m_pcAchievementList[i].Achieved = true;
					//BPCloud()->SendUpdateAchievement(cName, m_pcAchievementList[i].ProgressMax);

					ShowNotification(cName);
				}
				//else
					//BPCloud()->SendUpdateAchievement(cName, m_pcAchievementList[i].Progress);
			}
		}

	}
	return true;
}
char* bp_achievements::GetFullName(char* name)
{
	if(!name)
		return "";
	char* prefix = "#ACH_NAME_";
		char FullName[256];

	strcpy(FullName, prefix);
	delete prefix;
	return strcat(FullName, name);
}
char* bp_achievements::GetFullDesc(char* name)
{
	if(!name)
		return "";
	char* prefix = "#ACH_DESC_";
	char *FullName = new char[strlen(name) + 10];

	strcpy(FullName, prefix);
	delete prefix;
	return strcat(FullName, name);
}
char* bp_achievements::GetLocalizedName(char *name)
{
	if(!name)
		return "";
	const wchar_t *localized = g_pVGuiLocalize->Find(BPAchievements()->GetFullName(name));
	if(!localized)
		return "Problem with localization";
	char *ToPrint = new char[wcslen(localized)];
	size_t i = 0;
	size_t b = wcslen(localized) + 1;
	wcstombs_s(&i, ToPrint, b, localized, b);

	return ToPrint;
}
char* bp_achievements::GetLocalizedDesc(char *name)
{
	if(!name)
		return "";
	const wchar_t *localized = g_pVGuiLocalize->Find(BPAchievements()->GetFullDesc(name));
	if(!localized)
		return "Problem with localization";
	char *ToPrint = new char[wcslen(localized)];
	size_t i = 0;
	size_t b = wcslen(localized) + 1;
	wcstombs_s(&i, ToPrint, b, localized, b);

	return ToPrint;
}

// Achievement Listener
void bp_achievListener::StartToListen()
{
	gameeventmanager->AddListener(this, "player_earn_achiev", false);
	gameeventmanager->AddListener(this, "player_update_achiev", false);
	gameeventmanager->AddListener(this, "player_death", false);
}
void bp_achievListener::FireGameEvent(IGameEvent *pEvent)
{
	ConVar *pCheats  = cvar->FindVar( "sv_cheats" );
 
 	if ( pCheats && pCheats->GetInt() != 0 ) 
		return;

	if (!strcmp("player_update_achiev", pEvent->GetName()))
	{
		if(pEvent->GetFloat("userid") == C_BasePlayer::GetLocalPlayer()->GetUserID()) // We don't care about the others
		{
			BPAchievements()->UpdateAchievement(pEvent->GetString("name"), pEvent->GetInt("amount"));
		}
	
	}
	if (!strcmp("player_death", pEvent->GetName()))
	{
		if(pEvent->GetFloat("attacker") == C_BasePlayer::GetLocalPlayer()->GetUserID() && pEvent->GetFloat("userid") == C_BasePlayer::GetLocalPlayer()->GetUserID() ) // on s'est suicidé
		{
			if(!strcmp("mine", pEvent->GetString("weapon")))
			{
				m_flRangeMineSuicide = gpGlobals->curtime + 3.0f;
			}
		}
		if(pEvent->GetFloat("attacker") == C_BasePlayer::GetLocalPlayer()->GetUserID() && pEvent->GetFloat("userid") != C_BasePlayer::GetLocalPlayer()->GetUserID() ) // on s'est suicidé
		{
			if(m_flRangeMineSuicide >= gpGlobals->curtime)
			{
				BPAchievements()->EarnAchievement("a001SuicideMineKiller");
			}
		}
		if(pEvent->GetFloat("userid") == C_BasePlayer::GetLocalPlayer()->GetUserID() && pEvent->GetFloat("userid") == pEvent->GetFloat("attacker") ) // Osef des autres tocards
		{
			if(!strcmp("weapon_bazooka", C_BasePlayer::GetLocalPlayer()->GetActiveWeapon()->GetName()))
			{
				BPAchievements()->UpdateAchievement("a010SuicideBazooka", 1);
			}
		}
		if(pEvent->GetFloat("attacker") == C_BasePlayer::GetLocalPlayer()->GetUserID()) // We don't care about the others
		{
			if(!strcmp("weapon_bazooka", C_BasePlayer::GetLocalPlayer()->GetActiveWeapon()->GetName())) // Air shot with the bazooka
			{
				C_BasePlayer *pplayer = BP2UTIL_PlayerByUserId(pEvent->GetInt("userid"));
				if(IsInAir(pplayer))
				{
					BPAchievements()->EarnAchievement("a001AirBazooka");
				}
			}
			if(!strcmp("weapon_flowerlauncher", C_BasePlayer::GetLocalPlayer()->GetActiveWeapon()->GetName())) //Air hot with theflowerlauncher
			{
				C_BasePlayer *pplayer = BP2UTIL_PlayerByUserId(pEvent->GetInt("userid"));
				if(IsInAir(pplayer))
				{
					BPAchievements()->EarnAchievement("a001AirFlowerLauncher");
				}
			}
		}
	}
}

