#include "cbase.h"
#include "GameEventListener.h"
#include "hl2orange.spa.h"
#include "Cloud\Nebuleuse.h"
#include "AchievementListener.h"
#include "hl2mp/hl2mp_hud_chat.h"
#include "c_playerresource.h"
#include <vgui/ILocalize.h>
#include "iachievementmgr.h"
#include "fmtstr.h"
#include "achievement_notification_panel.h"
#include "engine/ienginesound.h"

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
	if (pPlayer == NULL || !pPlayer)
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
void bp_achievListener::SendEarnEvent(char *name)
{
	IGameEvent* pEvent = gameeventmanager->CreateEvent("player_announce_achiev", true);
	if(pEvent)
	{
		pEvent->SetInt("userid", C_BasePlayer::GetLocalPlayer()->GetUserID());
		pEvent->SetString("name", name);
		gameeventmanager->FireEvent(pEvent);
	}
}
void bp_achievListener::SendEarnEvent(const char *name)
{
	IGameEvent* pEvent = gameeventmanager->CreateEvent("player_announce_achiev", true);
	if(pEvent)
	{
		pEvent->SetInt("userid", C_BasePlayer::GetLocalPlayer()->GetUserID());
		pEvent->SetString("name", name);
		gameeventmanager->FireEvent(pEvent);
	}
}
// Achievement Listener
void bp_achievListener::StartToListen()
{
	gameeventmanager->AddListener(this, "player_earn_achiev", false);
	gameeventmanager->AddListener(this, "player_announce_achiev", false);
	gameeventmanager->AddListener(this, "player_update_achiev", false);
	gameeventmanager->AddListener(this, "player_death", false);
	gameeventmanager->AddListener(this, "game_newmap", false);
}

void bp_achievListener::FireGameEvent(IGameEvent *pEvent)
{
	ConVar *pCheats  = cvar->FindVar( "sv_cheats" );
 
	if ( pCheats && pCheats->GetInt() != 0 ) 
		return;

	if(engine->IsPlayingDemo() || engine->IsPlayingTimeDemo())
		return;

	if(!C_BasePlayer::GetLocalPlayer())
		return;

	if(!strcmp("game_newmap", pEvent->GetName()))
	{
		PlayerStat st = g_Nebuleuse->GetPlayerStats("mapsLoaded");
		st.Value += 1;
		g_Nebuleuse->SetPlayerStats(st);
	}

	if (!strcmp("player_update_achiev", pEvent->GetName()))
	{
		if(pEvent->GetFloat("userid") == C_BasePlayer::GetLocalPlayer()->GetUserID()) // We don't care about the others
		{
			bool completed = g_Nebuleuse->GetAchievementData(pEvent->GetString("name")).IsCompleted();
			g_Nebuleuse->UpdateAchievementProgress(pEvent->GetString("name"), pEvent->GetInt("amount"));
			if(g_Nebuleuse->GetAchievementData(pEvent->GetString("name")).IsCompleted() && !completed)
				SendEarnEvent(pEvent->GetString("name"));
		}
	
	}
	if (!strcmp("player_earn_achiev", pEvent->GetName()))
	{
		if(pEvent->GetFloat("userid") == C_BasePlayer::GetLocalPlayer()->GetUserID()) // We don't care about the others
		{
			if(g_Nebuleuse->GetAchievementData(pEvent->GetString("name")).IsCompleted())
				return;
			g_Nebuleuse->EarnAchievement(pEvent->GetString("name"));
			SendEarnEvent(pEvent->GetString("name"));
		}
	}
	if (!strcmp("player_announce_achiev", pEvent->GetName()))
	{
		C_BasePlayer *pPlayer = BP2UTIL_PlayerByUserId(pEvent->GetInt("userid")); // This player earned an achievement
		if(!pPlayer)
			return;

		DispatchParticleEffect("Awesome_Glow", PATTACH_ABSORIGIN_FOLLOW, pPlayer); // Make a pretty particle and have it follow the player
		
		CLocalPlayerFilter filter;
		enginesound->EmitSound(	filter, SOUND_FROM_WORLD, CHAN_STATIC, "gameplay/achievement_earned.wav", 1.0f, 0.2, 0, 100, &pPlayer->GetAbsOrigin()); // Play a sound where the player is

		CBaseHudChat *hudChat = (CBaseHudChat *)GET_HUDELEMENT( CHudChat ); // And add a chat line for it
		
		if(!hudChat)
			return;

		wchar_t wszPlayerName[MAX_PLAYER_NAME_LENGTH];
		g_pVGuiLocalize->ConvertANSIToUnicode( g_PR->GetPlayerName( pPlayer->entindex() ), wszPlayerName, sizeof( wszPlayerName ) );
	
		const wchar_t *pchLocalizedAchievement = ACHIEVEMENT_LOCALIZED_NAME_FROM_STR( g_Nebuleuse->GetAchievementData(pEvent->GetString("name")).Name );
		if ( pchLocalizedAchievement )
		{
			wchar_t wszLocalizedString[128];
			g_pVGuiLocalize->ConstructString( wszLocalizedString, sizeof( wszLocalizedString ), g_pVGuiLocalize->Find( "#Achievement_Earned" ), 2, wszPlayerName, pchLocalizedAchievement );
		
			char szLocalized[128];
			g_pVGuiLocalize->ConvertUnicodeToANSI( wszLocalizedString, szLocalized, sizeof( szLocalized ) );
			hudChat->ChatPrintf( pEvent->GetFloat("userid"), CHAT_FILTER_SERVERMSG, "%s", szLocalized );
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
			if(m_flRangeMineKill >= gpGlobals->curtime)
			{
				if(g_Nebuleuse->GetAchievementData(pEvent->GetString("SuicideMineKiller")).IsCompleted())
					return;
				g_Nebuleuse->EarnAchievement("SuicideMineKiller");
				SendEarnEvent(pEvent->GetString("name"));
			}
		}
		if(pEvent->GetFloat("attacker") == C_BasePlayer::GetLocalPlayer()->GetUserID() && pEvent->GetFloat("userid") != C_BasePlayer::GetLocalPlayer()->GetUserID() )
		{
			PlayerStat st = g_Nebuleuse->GetPlayerStats("kills");
			st.Value += 1;
			g_Nebuleuse->SetPlayerStats(st);

			C_BasePlayer *pPlayer = BP2UTIL_PlayerByUserId(pEvent->GetInt("userid"));
			if(!pPlayer)
				return;

			char * b = new char[strlen(pEvent->GetString("weapon")) + 1 ];
			strcpy(b,pEvent->GetString("weapon"));
			g_Nebuleuse->AddKill(new nKillInfo(b, pPlayer->GetAbsOrigin()));

			if(!strcmp("mine", pEvent->GetString("weapon")))
			{
				m_flRangeMineKill = gpGlobals->curtime + 3.0f;
			}
			if(m_flRangeMineSuicide >= gpGlobals->curtime)
			{
				if(g_Nebuleuse->GetAchievementData(pEvent->GetString("SuicideMineKiller")).IsCompleted())
					return;
				g_Nebuleuse->EarnAchievement("SuicideMineKiller");
				SendEarnEvent(pEvent->GetString("name"));
			}
		}
		if(pEvent->GetFloat("userid") == C_BasePlayer::GetLocalPlayer()->GetUserID() && pEvent->GetFloat("userid") == pEvent->GetFloat("attacker") ) // Osef des autres tocards
		{
			CBasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
			if(!pPlayer)
				return;
			CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
			if (!pWeapon)
				return;
			if(!strcmp("weapon_bazooka", pWeapon->GetName()))
			{
				if(g_Nebuleuse->GetAchievementData(pEvent->GetString("SuicideBazooka")).IsCompleted())
					return;
				g_Nebuleuse->UpdateAchievementProgress("SuicideBazooka", 1);
				if(g_Nebuleuse->GetAchievementData(pEvent->GetString("name")).IsCompleted())
					SendEarnEvent(pEvent->GetString("name"));
			}
		}
		if(pEvent->GetFloat("attacker") == C_BasePlayer::GetLocalPlayer()->GetUserID()) // We don't care about the others
		{
			CBasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
			if(!pPlayer)
				return;
			CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
			if (!pWeapon)
				return;
			if(!strcmp("weapon_bazooka", pWeapon->GetName())) // Air shot with the bazooka
			{
				C_BasePlayer *pOther = BP2UTIL_PlayerByUserId(pEvent->GetInt("userid"));
				if(!pOther)
					return;
				if(IsInAir(pOther))
				{
					if(g_Nebuleuse->GetAchievementData(pEvent->GetString("AirBazooka")).IsCompleted())
						return;
					g_Nebuleuse->EarnAchievement("AirBazooka");
					SendEarnEvent(pEvent->GetString("name"));
				}
			}
			if(!strcmp("weapon_flowerlauncher", pWeapon->GetName())) //Air hot with theflowerlauncher
			{
				C_BasePlayer *pOther = BP2UTIL_PlayerByUserId(pEvent->GetInt("userid"));
				if(!pOther)
					return;
				if(IsInAir(pOther))
				{
					if(g_Nebuleuse->GetAchievementData(pEvent->GetString("AirFlowerLauncher")).IsCompleted())
						return;
					g_Nebuleuse->EarnAchievement("AirFlowerLauncher");
					SendEarnEvent(pEvent->GetString("name"));
				}
			}
		}
	}
}