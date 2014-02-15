#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

#ifdef _WIN32
#pragma once
#endif

#include "maprules.h"
#include "props.h"
#include "entitylist.h"
#include "entityoutput.h"
#include "networkvar.h"
#include "collisionproperty.h"
#include "ServerNetworkProperty.h"
#include "shareddefs.h"

#define DEFAULT_COUNTER_DELAY 1

class CGameFortsInfoZone : public CTriggerMultiple
{
public:
	DECLARE_CLASS( CGameFortsInfoZone, CTriggerMultiple );
	DECLARE_DATADESC();

	CGameFortsInfoZone(void);
	void Activate(void);
	void InfoZoneTouch(CBaseEntity *pEntity);
	void HandleSameTeam(CBasePlayer *pPlayer);
	void HandleOtherTeam(CBasePlayer *pPlayer);
	bool IsPlayerCarryingInfos(CBasePlayer *pPlayer);
	bool AreInfosSlotsFilled(int team);
	void AddPlayerToInfosList(CBasePlayer *pPlayer);
	void ResetAllInfos(bool bPlayLostDataSound = false);
	void Precache(void);

private:
	int m_iTeam;
	float m_flLastHit;

	COutputEvent m_OnPinkScore;
	COutputEvent m_OnGreenScore;
};

LINK_ENTITY_TO_CLASS( game_forts_infozone, CGameFortsInfoZone );

class CGameDomCaptureZone : public CTriggerMultiple
{
public:
	DECLARE_CLASS( CGameDomCaptureZone, CTriggerMultiple );
	DECLARE_DATADESC();

	CGameDomCaptureZone(void);
	void Activate(void);
	void CaptureZoneTouch(CBaseEntity *pEntity);
	void SoundThink(void);
	int CGameDomCaptureZone::DetermineZoneID(void);
	void ResetScoreTimer(void);
	void Capture(int team);

private:
	int m_iZoneID;
	float m_flLastCapture;
	float m_flLastScore;
	COutputEvent m_OnPinkCapture;
	COutputEvent m_OnGreenCapture;
};

LINK_ENTITY_TO_CLASS( game_dom_capturezone, CGameDomCaptureZone );

class CGameRespawnPlayers : public CRulePointEntity
{
public:
	DECLARE_CLASS( CGameRespawnPlayers, CRulePointEntity );
	DECLARE_DATADESC();

	void InputRespawnPlayers(inputdata_t &inputData);
	void InputKillAndRespawnPlayers(inputdata_t &inputData);
};

LINK_ENTITY_TO_CLASS( game_respawnplayers, CGameRespawnPlayers );
class CPushCapture : public CTriggerMultiple	
{
public:
	DECLARE_CLASS( CPushCapture, CTriggerMultiple);
	DECLARE_DATADESC();
	
	CPushCapture() { Precache(); };
	void Activate();
	void Precache(void);
	void Spawn();
	void Toggle();

	void HandleTouch( CBaseEntity *pOther );
	COutputEvent m_OnCaptured;
private:
	float m_fNextAllowedTouch;
	bool m_IsActive;
	int m_ihTeam;

};
LINK_ENTITY_TO_CLASS( game_push_capturezone, CPushCapture );


class CPushBall : public CPhysicsProp
{
public:
	DECLARE_CLASS( CPushBall, CPhysicsProp);


	DECLARE_SIMPLE_DATADESC();
	DECLARE_SERVERCLASS_NOBASE();
	virtual datamap_t *GetDataDescMap( void );
	

	int UpdateTransmitState()
	{
		return SetTransmitState( FL_EDICT_ALWAYS );
	}

	CPushBall()
	{
		m_bActive = true;
	}

	void Activate( void );
	void Spawn( void );
	void Precache( void );
	void OnPhysGunPickup(CBasePlayer *pPlayer, PhysGunPickup_t reason);
	void OnPhysGunDrop(CBasePlayer *pPlayer, PhysGunDrop_t reason);
	bool IsActive(){ return m_bActive; };
	void TimeThink();
	int OnTakeDamage(const CTakeDamageInfo &info);
	void ResetBall();
	
	int GetTeamHolder() { return m_iHoldTeam; }

	void Resp();
	void Captured();

	virtual void SetGlow( bool state, Color glowColor = Color(255,255,255) );

	// Input function
	void InputToggle( inputdata_t &inputData );

	CNetworkVar( bool, m_bEnableGlow );
	CNetworkVar( color32, m_GlowColor );

	bool m_bRespawnOnCap;

	string_t m_sModelName;

	COutputEvent m_OnCaptured;
	COutputEvent m_OnReset;
	
	int		m_iResetTime;

private:

	int		m_iHoldTeam;
	bool	m_bShouldTimer;
	bool	m_bActive;
	float	m_fTimeToReset;
	Vector m_vOriginalSpawnOrigin;
	QAngle m_vOriginalSpawnAngles;
};

LINK_ENTITY_TO_CLASS( game_push_ball, CPushBall );

void Game_RespawnPushBall();

#endif // ifndef GAME_ENTITIES_H
