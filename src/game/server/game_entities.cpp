#include "cbase.h"
#include "hl2mp_gamerules.h"
#include "player.h"
#include "team.h"
#include "triggers.h"
#include "playerSkins.h"
#include "EnvMessage.h"
#include "eventqueue.h"
#include "game_entities.h"
#include "autovocals.h"
#include "particle_parse.h"
#include "weapon_pushball.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


CGameFortsInfoZone::CGameFortsInfoZone(void)
{
	m_iTeam = 0;
	m_flLastHit = 0;
	Precache();
}
void CGameFortsInfoZone::Precache(void)
{
	BaseClass::Precache();

	PrecacheScriptSound("Gameplay.Score.Friend");
	PrecacheScriptSound("Gameplay.Score.Ennemy");
	PrecacheScriptSound("Gameplay.Forts.StoleData");
	PrecacheScriptSound("Gameplay.Forts.LostData");
}
// Le GameType est défini par un game_config, comme ce con là a pas forcément
// été spawn avant l'infozone, il faut faire les vérifications après
// le spawn de toutes les entités, ::Activate() donc
void CGameFortsInfoZone::Activate(void)
{
	BaseClass::Activate();

	SetTouch( &CGameFortsInfoZone::InfoZoneTouch );

	if(HL2MPRules()->GetGameType() != GAME_FORTS)
	{
		Warning("game_forts_infozone in a non-forts game\n");
		SetTouch( NULL );
	}
	if(m_iTeam != TEAM_PINK && m_iTeam != TEAM_GREEN)
	{
		Warning("Invalid team for game_forts_infozone\n");
		SetTouch( NULL );
	}
	BaseClass::Activate();
}

void CGameFortsInfoZone::HandleSameTeam(CBasePlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	if(IsPlayerCarryingInfos(pPlayer))
	{
		g_Teams[pPlayer->GetTeamNumber()]->AddScore(1);
		pPlayer->IncrementFragCount(3);
		/*IGameEvent *event = gameeventmanager->CreateEvent( "player_death" );
		if( event )
		{
			event->SetInt("userid", 0 );
			event->SetInt("attacker", pPlayer->GetUserID() );
			event->SetString("weapon", "fortsscore" );
			gameeventmanager->FireEvent( event );
		}*/
	IGameEvent *event = gameeventmanager->CreateEvent( "forts_event" );
	if ( event )
	{
		event->SetInt( "userid", pPlayer->GetUserID() );
		event->SetInt( "team", m_iTeam );
		event->SetInt( "action", 0 );
		gameeventmanager->FireEvent( event );
	}	

		ResetAllInfos(true);

		if(pPlayer->GetTeamNumber() == TEAM_PINK)
			m_OnPinkScore.FireOutput(pPlayer, this);
		else
			m_OnGreenScore.FireOutput(pPlayer, this);

		CRecipientFilter allyFilter, ennemyFilter;
		allyFilter.AddRecipientsByTeam(pPlayer->GetTeam());
		ennemyFilter.AddRecipientsByTeam(g_Teams[pPlayer->GetTeamNumber()==TEAM_PINK?TEAM_GREEN:TEAM_PINK]);

		allyFilter.MakeReliable();
		ennemyFilter.MakeReliable();
		CBaseEntity::EmitSound(allyFilter, NULL, "Gameplay.Score.Friend");
		CBaseEntity::EmitSound(ennemyFilter, NULL, "Gameplay.Score.Ennemy");
	}
}
void CGameFortsInfoZone::ResetAllInfos(bool bPlayLostDataSound)
{
	for(int i = 0; i < FORTS_MAX_INFOS; i++)
	{
		if(HL2MPRules()->m_apFortsInfosPink[i] != NULL)
		{
			HL2MPRules()->m_apFortsInfosPink[i]->m_nSkin = toSkinId(PLAYER_SKIN_ROSE);
			HL2MPRules()->m_apFortsInfosPink[i]->EmitSound("Gameplay.Forts.LostData");
			HL2MPRules()->m_apFortsInfosPink[i] = NULL;
		}
		if(HL2MPRules()->m_apFortsInfosGreen[i] != NULL)
		{
			HL2MPRules()->m_apFortsInfosGreen[i]->m_nSkin = toSkinId(PLAYER_SKIN_VERT);
			HL2MPRules()->m_apFortsInfosGreen[i]->EmitSound("Gameplay.Forts.LostData");
			HL2MPRules()->m_apFortsInfosGreen[i] = NULL;
		}
	}
}
void CGameFortsInfoZone::HandleOtherTeam(CBasePlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	// Si on a les infos, passer dans le trigger ne fait rien
	if(IsPlayerCarryingInfos(pPlayer))
		return;
	// Les places sont prises §
	if(AreInfosSlotsFilled(pPlayer->GetTeamNumber()))
		return;

	// Bah, on peut prendre maintenant non ?
	AddPlayerToInfosList(pPlayer);
	pPlayer->IncrementFragCount(2);
	
	if(pPlayer->GetTeamNumber() == TEAM_PINK)
		pPlayer->m_nSkin = PLAYER_BONUS_NROSE;
	else
		pPlayer->m_nSkin = PLAYER_BONUS_NVERT;

	/*IGameEvent *event = gameeventmanager->CreateEvent( "player_death" );
	if( event )
	{
		event->SetInt("userid", 0 );
		event->SetInt("attacker", pPlayer->GetUserID() );
		event->SetString("weapon", "fortssteal" );
		gameeventmanager->FireEvent( event );
	}*/
	IGameEvent *event = gameeventmanager->CreateEvent( "forts_event" );
	if ( event )
	{
		event->SetInt( "userid", pPlayer->GetUserID() );
		event->SetInt( "team", pPlayer->GetTeamNumber() );
		event->SetInt( "action", 1 );
		gameeventmanager->FireEvent( event );
	}	
	pPlayer->PlayAutovocal(TAKEFLAG);
		CRecipientFilter allyFilter, ennemyFilter;
		allyFilter.AddRecipientsByTeam(pPlayer->GetTeam());
		ennemyFilter.AddRecipientsByTeam(g_Teams[pPlayer->GetTeamNumber()==TEAM_PINK?TEAM_GREEN:TEAM_PINK]);

		allyFilter.MakeReliable();
		ennemyFilter.MakeReliable();

		CBaseEntity::EmitSound(allyFilter, NULL, "Gameplay.Forts.Stoledata");
		CBaseEntity::EmitSound(ennemyFilter, NULL, "Gameplay.Forts.Lostdata");
}
void CGameFortsInfoZone::AddPlayerToInfosList(CBasePlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	if(pPlayer->GetTeamNumber() == TEAM_PINK)
	{
		for(int i = 0; i < FORTS_MAX_INFOS; i++)
		{
			if(HL2MPRules()->m_apFortsInfosPink[i] == NULL)
			{
				HL2MPRules()->m_apFortsInfosPink[i] = pPlayer;
				break;
			}
		}
	}
	else
	{
		for(int i = 0; i < FORTS_MAX_INFOS; i++)
		{
			if(HL2MPRules()->m_apFortsInfosGreen[i] == NULL)
			{
				HL2MPRules()->m_apFortsInfosGreen[i] = pPlayer;
				break;
			}
		}
	}
}
bool CGameFortsInfoZone::AreInfosSlotsFilled(int team)
{
	if(team != TEAM_PINK && team != TEAM_GREEN)
		return false;

	if(team == TEAM_PINK)
		for(int i = 0; i < FORTS_MAX_INFOS; i++)
		{
			if(HL2MPRules()->m_apFortsInfosPink[i] == NULL)
				return false;
		}
	else
		for(int i = 0; i < FORTS_MAX_INFOS; i++)
		{
			if(HL2MPRules()->m_apFortsInfosGreen[i] == NULL)
				return false;
		}

	return true;

}
bool CGameFortsInfoZone::IsPlayerCarryingInfos(CBasePlayer *pPlayer)
{
	if(pPlayer == NULL)
		return false;

	if(pPlayer->GetTeamNumber() == TEAM_PINK)
	{
		for(int i = 0; i < FORTS_MAX_INFOS; i++)
		{
			if(HL2MPRules()->m_apFortsInfosPink[i] == pPlayer)
				return true;
		}
	}
	else
	{
		for(int i = 0; i < FORTS_MAX_INFOS; i++)
		{
			if(HL2MPRules()->m_apFortsInfosGreen[i] == pPlayer)
				return true;
		}
	}
	return false;
}

extern ConVar mp_dom_scoretime;
void CGameFortsInfoZone::InfoZoneTouch(CBaseEntity *pEntity)
{
	if(HL2MPRules()->IsFortsBuildPhase())
		return; // Les objectifs fonctionnent uniquement en mode fight

	CBasePlayer *pPlayer = ToBasePlayer(pEntity);
	if(pPlayer == NULL)
		return;

	if(gpGlobals->curtime < m_flLastHit + mp_dom_scoretime.GetFloat()/2)
		return;
	m_flLastHit = gpGlobals->curtime;
	
	if(pPlayer->m_lifeState != LIFE_ALIVE) // On sait jamais
		return;

	if(pPlayer->GetTeamNumber() == m_iTeam)
		HandleSameTeam(pPlayer);
	else
		HandleOtherTeam(pPlayer);
}

BEGIN_DATADESC( CGameFortsInfoZone )
	DEFINE_KEYFIELD( m_iTeam, FIELD_INTEGER, "team" ),

	DEFINE_OUTPUT( m_OnPinkScore, "OnPinkScore"),
	DEFINE_OUTPUT( m_OnGreenScore, "OnGreenScore"),
END_DATADESC()

CGameDomCaptureZone::CGameDomCaptureZone(void)
{
	m_iZoneID = -1;
	m_flLastCapture = 0;
	m_flLastScore = 0;
	PrecacheScriptSound("Gameplay.Domscore");
	PrecacheScriptSound("Gameplay.Domambient");
}
void CGameDomCaptureZone::Activate(void)
{
	BaseClass::Activate();

	SetTouch( &CGameDomCaptureZone::CaptureZoneTouch );

	if(HL2MPRules()->GetGameType() != GAME_DOM)
	{
		Warning("game_dom_capturezone in a non-dom game\n");
		SetTouch(NULL);
		return;
	}

	m_iZoneID = DetermineZoneID();
	if(m_iZoneID == -1)
	{
		Warning("Max game_dom_capturezone count reached\n");
		SetTouch(NULL);
		return;
	}
	SetThink( &CGameDomCaptureZone::SoundThink );
	SetNextThink(gpGlobals->curtime);
}
extern ConVar mp_scorelimit_dom;
extern ConVar mp_dom_scoretime;
void CGameDomCaptureZone::SoundThink(void)
{
	SetNextThink(gpGlobals->curtime + 0.1f);

	// Si ça appartient à aucune équipe, pas de son (genre points neutres en début de map)
	if(HL2MPRules()->m_iDomZones[m_iZoneID] != TEAM_PINK && HL2MPRules()->m_iDomZones[m_iZoneID] != TEAM_GREEN)
		return;

	// Score toutes les 5 secondes, faut synchroniser le son avec
	// float flDomScoreTimer = (HL2MPRules()->GetForceDomScoreTimer() != -1)?HL2MPRules()->GetForceDomScoreTimer():mp_dom_scoretime.GetFloat();
	if(gpGlobals->curtime < (m_flLastScore + 5))
		return;
	m_flLastScore = gpGlobals->curtime;

	int iScoreLeft = mp_scorelimit_dom.GetInt() - g_Teams[HL2MPRules()->m_iDomZones[m_iZoneID]]->GetScore();
	int pitch = (pow(1.019f,-iScoreLeft+260))+30;
	pitch = clamp(pitch, 30, 163);

	CPASAttenuationFilter filter( this, 0.8 );
	EmitSound_t ep;
	ep.m_nChannel = CHAN_STATIC;
	ep.m_pSoundName = "gameplay/domscore.wav";
	ep.m_flVolume = 1.0f;
	ep.m_SoundLevel = SNDLVL_80dB;
	ep.m_nPitch = pitch;

	EmitSound( filter, entindex(), ep );
}

int CGameDomCaptureZone::DetermineZoneID(void)
{
	for(int i = 0; i < DOM_MAX_ZONES; i++)
	{
		if(HL2MPRules()->m_iDomZones[i] == -1)
		{
			HL2MPRules()->m_iDomZones[i] = 0;
			return i;
		}
	}
	return -1;
}

void CGameDomCaptureZone::ResetScoreTimer(void)
{
	m_flLastScore = gpGlobals->curtime;
}
void CGameDomCaptureZone::Capture(int team)
{
	HL2MPRules()->ResetDomTimer();
	EmitSound("Gameplay.Domambient");

	// Petite boucle moche pour reset toutes les zones
	CBaseEntity *pEntity2 = gEntList.FirstEnt();
	while( pEntity2 )
	{
		if(FClassnameIs(pEntity2, "game_dom_capturezone"))
		{
			CGameDomCaptureZone *pZone = dynamic_cast<CGameDomCaptureZone*>(pEntity2);
			pZone->ResetScoreTimer();
		}

		pEntity2 = gEntList.NextEnt( pEntity2 );
	}

	HL2MPRules()->m_iDomZones[m_iZoneID] = team;
	if(team == TEAM_PINK)
		m_OnPinkCapture.FireOutput(this, this);
	else
		m_OnGreenCapture.FireOutput(this, this);

	DevMsg("Zone %d captured by team %d\n", m_iZoneID, team);

	IGameEvent *event = gameeventmanager->CreateEvent( "cp_caped" );
	if( event )
	{
		event->SetInt("id", m_iZoneID );
		event->SetInt("team", HL2MPRules()->m_iDomZones[m_iZoneID] );
		if(HL2MPRules()->m_iDomZones[2] == -1)
			event->SetInt("thirdexist", 0 );
		else
			event->SetInt("thirdexist", 1 );
		gameeventmanager->FireEvent( event );
	}
}
void CGameDomCaptureZone::CaptureZoneTouch(CBaseEntity *pEntity)
{
	CBasePlayer *pPlayer = ToBasePlayer(pEntity);
	if(!pPlayer)
		return;
	if(pPlayer->m_lifeState != LIFE_ALIVE)
		return;
	if(m_iZoneID == -1)
		return;

	if(HL2MPRules()->m_iDomZones[m_iZoneID] == pPlayer->GetTeamNumber())
		return;

	if(gpGlobals->curtime < m_flLastCapture + 2.0f)
		return;
	m_flLastCapture = gpGlobals->curtime;
	HL2MPRules()->ResetDomTimer();
	EmitSound("Gameplay.Domambient");

	// Petite boucle moche pour reset toutes les zones
	CBaseEntity *pEntity2 = gEntList.FirstEnt();
	while( pEntity2 )
	{
		if(FClassnameIs(pEntity2, "game_dom_capturezone"))
		{
			CGameDomCaptureZone *pZone = dynamic_cast<CGameDomCaptureZone*>(pEntity2);
			pZone->ResetScoreTimer();
		}

		pEntity2 = gEntList.NextEnt( pEntity2 );
	}

	HL2MPRules()->m_iDomZones[m_iZoneID] = pPlayer->GetTeamNumber();
	if(pPlayer->GetTeamNumber() == TEAM_PINK)
		m_OnPinkCapture.FireOutput(pPlayer, this);
	else
		m_OnGreenCapture.FireOutput(pPlayer, this);

	DevMsg("Zone %d captured by team %d\n", m_iZoneID, pPlayer->GetTeamNumber());

	IGameEvent *event = gameeventmanager->CreateEvent( "cp_caped" );
	if( event )
	{
		event->SetInt("userid", pPlayer->GetUserID() );
		event->SetInt("id", m_iZoneID );
		event->SetInt("team", HL2MPRules()->m_iDomZones[m_iZoneID] );
		if(HL2MPRules()->m_iDomZones[2] == -1)
			event->SetInt("thirdexist", 0 );
		else
			event->SetInt("thirdexist", 1 );
		gameeventmanager->FireEvent( event );
	}
}

BEGIN_DATADESC( CGameDomCaptureZone )
	DEFINE_OUTPUT( m_OnPinkCapture, "OnPinkCapture" ),
	DEFINE_OUTPUT( m_OnGreenCapture, "OnGreenCapture" ),
	DEFINE_THINKFUNC( SoundThink ),
END_DATADESC()

void CGameRespawnPlayers::InputRespawnPlayers(inputdata_t &inputData)
{
	for ( int i = 0; i < MAX_PLAYERS; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );

		if ( !pPlayer )
			continue;

		pPlayer->Respawn(false);
	}
}

void CGameRespawnPlayers::InputKillAndRespawnPlayers(inputdata_t &inputData)
{
	for ( int i = 0; i < MAX_PLAYERS; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );

		if ( !pPlayer )
			continue;

		pPlayer->Respawn(true);
	}
}

BEGIN_DATADESC( CGameRespawnPlayers )
	DEFINE_INPUTFUNC( FIELD_VOID, "RespawnPlayers", InputRespawnPlayers ),
	DEFINE_INPUTFUNC( FIELD_VOID, "KillAndRespawnPlayers", InputKillAndRespawnPlayers ),
END_DATADESC()

//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ========
//
// Purpose: Simple model entity that randomly moves and changes direction
//			when activated.
//
//=============================================================================

// Start of our data description for the class
BEGIN_DATADESC( CPushBall )
	
	// Save/restore our active state
	DEFINE_KEYFIELD( m_sModelName, FIELD_STRING, "model"),
	DEFINE_KEYFIELD( m_bRespawnOnCap, FIELD_BOOLEAN, "respawn"),
	DEFINE_KEYFIELD( m_iResetTime, FIELD_INTEGER, "resettime"),
	// Links our input name from Hammer to our input member function
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
	DEFINE_OUTPUT( m_OnCaptured, "OnCapture"),
	DEFINE_OUTPUT( m_OnReset,"OnReset"),
	DEFINE_THINKFUNC( TimeThink ),

END_DATADESC()
BEGIN_NETWORK_TABLE( CPushBall, DT_PushBall )
	SendPropBool( SENDINFO(m_bEnableGlow) ),
	SendPropInt( SENDINFO(m_GlowColor), 32, SPROP_UNSIGNED, SendProxy_Color32ToInt ),
END_NETWORK_TABLE()


//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void CPushBall::Precache( void )
{
	PrecacheModel( STRING( m_sModelName ) );
	PrecacheScriptSound("Gameplay.Push.Pickup");
	PrecacheScriptSound("Gameplay.Push.BallReset");
	BaseClass::Precache();
}
void CPushBall::Activate(void)
{
	if(HL2MPRules()->GetGameType() != GAME_PUSH)
	{
		Warning("game_push_ball in a non-push game\n");
		SetTouch( NULL );
	}

	SetThink(	&CPushBall::TimeThink	);
	SetNextThink( gpGlobals->curtime + 0.1f );

	m_bShouldTimer = false;

	BaseClass::Activate();
}
void CPushBall::TimeThink()
{
	SetNextThink( gpGlobals->curtime + 0.1f );

	if(!m_bShouldTimer)
		return;
	if(gpGlobals->curtime > m_fTimeToReset)
		this->ResetBall();
}
void CPushBall::OnPhysGunPickup(CBasePlayer *pPlayer, PhysGunPickup_t reason)
{
	CRecipientFilter allyFilter;

	allyFilter.AddRecipient(pPlayer);	
	allyFilter.MakeReliable();

	CBaseEntity::EmitSound(allyFilter,NULL, "Gameplay.Push.Pickup");
	pPlayer->EmitSound(allyFilter, NULL, "Gameplay.Push.Pickup",&GetAbsOrigin(),0,(float *)0);
	BaseClass::OnPhysGunPickup(pPlayer, reason);

	m_iHoldTeam = pPlayer->GetTeamNumber();
	BaseClass::OnPhysGunPickup(pPlayer, reason);

	m_bShouldTimer = false;
}
void CPushBall::OnPhysGunDrop(CBasePlayer *pPlayer, PhysGunDrop_t reason)
{
	if( reason != LAUNCHED_BY_CANNON )
		m_iHoldTeam = 0;
	m_fTimeToReset = gpGlobals->curtime + m_iResetTime;
	m_bShouldTimer = true;
	BaseClass::OnPhysGunDrop(pPlayer, reason);	
}

int CPushBall::OnTakeDamage(const CTakeDamageInfo &info)
{
	m_fTimeToReset = gpGlobals->curtime + m_iResetTime;
	m_bShouldTimer = true;
	return BaseClass::OnTakeDamage(info);
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CPushBall::Spawn( void )
{
	Precache();
	return;
	char *szModel = (char *)STRING( m_sModelName );
	SetModel( szModel );
	SetSolid( SOLID_VPHYSICS );
	SetMoveType( MOVETYPE_VPHYSICS );
	BaseClass::Spawn();
	m_bActive = true;
	m_vOriginalSpawnAngles = GetAbsAngles();
	m_vOriginalSpawnOrigin = GetAbsOrigin();
}
void CPushBall::Resp()
{
	//Respawn();	
	//this->Teleport(&m_vOriginalSpawnOrigin, NULL,NULL);
	Teleport(&m_vOriginalSpawnOrigin, &m_vOriginalSpawnAngles, NULL);
		m_bShouldTimer = false;
	SetAbsVelocity(Vector(0,0,-1));
	SetMoveType(MOVETYPE_VPHYSICS);
	DispatchUpdateTransmitState();
}
void CPushBall::ResetBall()
{
	Resp();
	m_OnReset.FireOutput(this, this);
	CBaseEntity::EmitSound("Gameplay.Push.BallReset");
}
void CPushBall::Captured()
{
	Resp();
	m_OnCaptured.FireOutput(this,this);
}
//-----------------------------------------------------------------------------
// Purpose: Toggle the movement of the entity
//-----------------------------------------------------------------------------
void CPushBall::InputToggle( inputdata_t &inputData )
{
		if(m_bActive)
			m_bActive = false;
		else
			m_bActive = true;	
}

BEGIN_DATADESC( CPushCapture )
	
	DEFINE_KEYFIELD( m_ihTeam, FIELD_INTEGER, "team"),
	DEFINE_OUTPUT( m_OnCaptured, "OnCaptured"),

END_DATADESC()
void CPushCapture::Spawn()
{
		BaseClass::Spawn();
		m_fNextAllowedTouch = 0;
	SetTouch( &CPushCapture::HandleTouch );
}
void CPushCapture::Precache()
{
	//DO precache
	PrecacheScriptSound("Gameplay.Score.Friend");
	PrecacheScriptSound("Gameplay.Score.Ennemy");
	BaseClass::Precache();
}
void CPushCapture::HandleTouch(CBaseEntity *pOther)
{
	if(m_fNextAllowedTouch > gpGlobals->curtime)
		return;

	CBasePlayer *pPlayer = ToBasePlayer(pOther);
	if(!pPlayer)
		return;

	if (!FClassnameIs(pPlayer->GetActiveWeapon(), "weapon_pushball"))
		return;

	if(pPlayer->GetTeamNumber() == m_ihTeam)
		return;
	
	pPlayer->GetActiveWeapon()->Delete();
 	pPlayer->SwitchToNextBestWeapon(pPlayer->GetActiveWeapon());

	m_OnCaptured.FireOutput(pOther,this);
		

	CRecipientFilter allyFilter, ennemyFilter;
	allyFilter.AddRecipientsByTeam(pPlayer->GetTeam());
	ennemyFilter.AddRecipientsByTeam(g_Teams[pPlayer->GetTeamNumber()==TEAM_PINK?TEAM_GREEN:TEAM_PINK]);

	allyFilter.MakeReliable();
	ennemyFilter.MakeReliable();
	CBaseEntity::EmitSound(allyFilter, NULL, "Gameplay.Score.Friend");
	CBaseEntity::EmitSound(ennemyFilter, NULL, "Gameplay.Score.Ennemy");
	
	HL2MPRules()->PushTeamScored(m_ihTeam, true, 3);
	Game_RespawnPushBall();
	m_fNextAllowedTouch = gpGlobals->curtime + 10;
	
	IGameEvent *event = gameeventmanager->CreateEvent( "push_caped" );
	if( event )
	{
		event->SetInt("userid", pPlayer->GetUserID() );
		event->SetInt("team", pPlayer->GetTeamNumber() );
		gameeventmanager->FireEvent( event );
	}
}

void CPushCapture::Activate()
{
	SetTouch( &CPushCapture::HandleTouch );
	m_fNextAllowedTouch = 0;
	BaseClass::Activate();
}
void CPushBall::SetGlow( bool state, Color glowColor /*=Color(255,255,255)*/ )
{
	m_bEnableGlow = state;
	color32 col32 = { glowColor.r(), glowColor.g(), glowColor.b(), glowColor.a() };
 
	m_GlowColor.Set( col32 );
	m_bEnableGlow.Set( state );
}

void Game_RespawnPushBall()
{
	CBaseEntity *Target = gEntList.FindEntityByName(NULL, "game_pushball_spawn");
	if(!Target)
	{
		Warning("Could not find pushball respwan target");
		return;
	}
	CWeaponPushBall *pBall = (CWeaponPushBall*)CBaseEntity::Create("weapon_pushball", Target->GetAbsOrigin(), Target->GetAbsAngles(), Target);
	pBall->CollisionRulesChanged();
	pBall->SetShouldRepop(false);
}