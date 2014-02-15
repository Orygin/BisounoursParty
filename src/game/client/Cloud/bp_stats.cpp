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
/// Cette classe est utilisée pour les stats de BP.
///-----------------------------------------------------------------------------------

#include "cbase.h"
#include "igameevents.h"
#include "bp_cloud.h"
#include "bp_stats.h"
#include "tinyxml.h"
#include "hl2mp/c_hl2mp_player.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
#include "steam\steam_api.h"
 #include "Filesystem.h"

bp_stats *g_pStats = NULL;

CBasePlayer* BP1UTIL_PlayerByUserId( int userID )
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

bp_stats::bp_stats()
{
	g_pStats = this;
}
void bp_stats::StartToListen()
{
	gameeventmanager->AddListener(this, "player_death", false);
}

void bp_stats::FireGameEvent(IGameEvent *pEvent)
{
	if (!strcmp("player_death", pEvent->GetName()))
	{
		if(pEvent->GetFloat("attacker") == C_BasePlayer::GetLocalPlayer()->GetUserID()) // We don't care about the others
		{
			const char *weapon = pEvent->GetString("weapon");		
			RegisterNewKill(BP1UTIL_PlayerByUserId(pEvent->GetFloat("userid")), weapon);
		}
	}
}
void bp_stats::StartStats(const char *map)
{
	m_cCurMap = new char[32];
	memcpy(m_cCurMap, map, 32);
	m_flStartTime = gpGlobals->curtime;
	StartToListen();
}
void bp_stats::RegisterNewKill(CBasePlayer *Victim, const char *weapon)
{
	int iQueueItem = m_pcKillList.AddToTail();
	Kill_t &New = m_pcKillList[iQueueItem];
	
	if(Victim->IsBot())
		return;

	New.x = Victim->GetAbsOrigin().x;
	New.z = Victim->GetAbsOrigin().z;
	New.y = Victim->GetAbsOrigin().y;
	New.p_weapon = new char[32];

	player_info_t pi;
	engine->GetPlayerInfo( Victim->GetUserID(), &pi );
	
	CSteamID steamIDForPlayer = CSteamID( pi.friendsID, 1, steamapicontext->SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );

	New.SteamID = steamIDForPlayer.ConvertToUint64();
	
	Q_strncpy(New.p_weapon, weapon, 32);
}
void bp_stats::Flush()
{
	return;
	if(m_pcKillList.Count() == 0)
		return;

	float finaltime = gpGlobals->curtime - m_flStartTime;
	char* cFinalTime = new char [256];

	sprintf(cFinalTime, "%2.0f", finaltime);
	TiXmlDocument doc;
	TiXmlElement *em;
	
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );

	char *oursteam = new char[17];
	sprintf(oursteam, "%I64d", steamapicontext->SteamUser()->GetSteamID().ConvertToUint64() );

	TiXmlElement * root = new TiXmlElement( "Stats" );  
	root->SetAttribute("map", m_cCurMap);
	root->SetAttribute("SteamID", oursteam);
	doc.LinkEndChild( root );

	TiXmlElement * msgs = new TiXmlElement( "Updates" );  
	root->LinkEndChild( msgs );
	
	em = new TiXmlElement( "Update" );
	em->SetAttribute("Field", "TimePlayed");
	em->LinkEndChild( new TiXmlText( cFinalTime ));  
	em->SetAttribute("Add", "1"); // If add is 1 then the value will be added to the database. Else, it's replaced by thir new value
	msgs->LinkEndChild( em );

	TiXmlElement * kills = new TiXmlElement("kills");
	root->LinkEndChild(kills);

	while (m_pcKillList.Count() != 0)
	{
		em = new TiXmlElement( "Kill" );
		kills->LinkEndChild(em);
	
		Kill_t &Current =  m_pcKillList[m_pcKillList.Head()];
		
		char *steamid = new char[17];
		sprintf(steamid, "%I64d", Current.SteamID );

		em->SetAttribute("victim",steamid);
		em->SetAttribute("xpos", Current.x);
		em->SetAttribute("ypos", Current.y);
		em->SetAttribute("zpos", Current.z);
		em->SetAttribute("weapon", Current.p_weapon);

		m_pcKillList.Remove(m_pcKillList.Head());
	}

	const char *pGameDir = engine->GetGameDirectory();
	char FullPath[256];
	strcpy(FullPath, pGameDir);

	strcat(FullPath, "\\Stats.xml");

	doc.SaveFile(FullPath);
	
	CUtlBuffer buf;
	if ( filesystem->ReadFile("Stats.xml","MOD",buf) )
	{
		char* Xml = new char[buf.Size() + 1];
		buf.GetString(Xml);
		Xml[buf.Size()] = 0; // null terminator
		if(Xml != NULL)
		{
			BPCloud()->SendStats(Xml);
		}
	}
}