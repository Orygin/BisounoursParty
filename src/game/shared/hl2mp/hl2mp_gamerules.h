//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2MP_GAMERULES_H
#define HL2MP_GAMERULES_H
#pragma once

#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "gamevars_shared.h"

#ifndef CLIENT_DLL
#include "hl2mp_player.h"
#endif

#define VEC_CROUCH_TRACE_MIN	HL2MPRules()->GetHL2MPViewVectors()->m_vCrouchTraceMin
#define VEC_CROUCH_TRACE_MAX	HL2MPRules()->GetHL2MPViewVectors()->m_vCrouchTraceMax

enum
{
	TEAM_PINK = 2,
	TEAM_GREEN
};

enum
{
	FORCE_NONE = -1,
	FORCE_OFF = 0,
	FORCE_ON = 1,
	
};

enum { GAME_DM = 0, GAME_TDM, GAME_CTB, GAME_FORTS, GAME_DOM, GAME_KOTH, GAME_PUSH, GAME_COOP }; // GAME_COOP MUST BE THE LAST ITEM

#define FORTS_MAX_INFOS 2
#define DOM_MAX_ZONES 10

#ifdef CLIENT_DLL
	#define CHL2MPRules C_HL2MPRules
	#define CHL2MPGameRulesProxy C_HL2MPGameRulesProxy
#endif

class CHL2MPGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CHL2MPGameRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();
};

class HL2MPViewVectors : public CViewVectors
{
public:
	HL2MPViewVectors( 
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight,
		Vector vCrouchTraceMin,
		Vector vCrouchTraceMax ) :
			CViewVectors( 
				vView,
				vHullMin,
				vHullMax,
				vDuckHullMin,
				vDuckHullMax,
				vDuckView,
				vObsHullMin,
				vObsHullMax,
				vDeadViewHeight )
	{
		m_vCrouchTraceMin = vCrouchTraceMin;
		m_vCrouchTraceMax = vCrouchTraceMax;
	}

	Vector m_vCrouchTraceMin;
	Vector m_vCrouchTraceMax;	
};

class CHL2MPRules : public CTeamplayRules
{
public:
	DECLARE_CLASS( CHL2MPRules, CTeamplayRules );

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif
	
	CHL2MPRules();
	virtual ~CHL2MPRules();

	virtual void Precache( void );
	virtual bool ShouldCollide( int collisionGroup0, int collisionGroup1 );
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );

	virtual float FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon );
	virtual float FlWeaponTryRespawn( CBaseCombatWeapon *pWeapon );
	virtual Vector VecWeaponRespawnSpot( CBaseCombatWeapon *pWeapon );
	virtual int WeaponShouldRespawn( CBaseCombatWeapon *pWeapon );
	virtual void Think( void );
	virtual void CreateStandardEntities( void );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget );
	virtual void GoToIntermission( void );
	virtual void GoToHumiliation( void );
	virtual void GoToFortsInterphase ( void );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual const char *GetGameDescription( void );
	// derive this function if you mod uses encrypted weapon info files
	virtual const unsigned char *GetEncryptionKey( void ) { return (unsigned char *)"x9Ke0BY7"; }
	virtual const CViewVectors* GetViewVectors() const;
	const HL2MPViewVectors* GetHL2MPViewVectors() const;

	//bp
	//virtual float FlPlayerFallDamage( CBasePlayer *pPlayer );

	float GetMapRemainingTime();
	void CleanUpMap();
	void CheckRestartGame();
	void RestartGame();
	
#ifndef CLIENT_DLL
	virtual Vector VecItemRespawnSpot( CItem *pItem );
	virtual QAngle VecItemRespawnAngles( CItem *pItem );
	virtual float	FlItemRespawnTime( CItem *pItem );
	virtual bool	CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem );
	virtual bool FShouldSwitchWeapon( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon );

	void	AddLevelDesignerPlacedObject( CBaseEntity *pEntity );
	void	RemoveLevelDesignerPlacedObject( CBaseEntity *pEntity );
	void	ManageObjectRelocation( void );
	void    CheckChatForReadySignal( CHL2MP_Player *pPlayer, const char *chatmsg );
	const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer );

	int UpdateTransmitState()
	{
		return FL_EDICT_ALWAYS;
	}

#endif
	virtual void ClientDisconnected( edict_t *pClient );

	bool CheckGameOver( void );
	bool IsIntermission( void );
	bool IsHumiliation( void );

	void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	
	void	CheckAllPlayersReady( void );

	bool	IsForceValid(int force) { return (force >= 0) && (force <= 1);}

	bool	IsTeamplay( void ) const { return m_bTeamPlayEnabled;	}
	void	SetTeamplay( bool teamplayValue ) { m_bTeamPlayEnabled = teamplayValue; }

	int		GetGameType( void ) const { return m_iGameType; }
	void	SetGameType( int type ) { if(type >= 0 && type <= GAME_COOP) m_iGameType = type; }

	int		GetForceBunny( void ) const { return m_iForceBunnyhop; }
	void	SetForceBunny( int bunny ) { if(IsForceValid(bunny)) m_iForceBunnyhop = bunny; }

	int		GetForcePogo( void ) const { return m_iForcePogo; }
	void	SetForcePogo( int pogo ) { if(IsForceValid(pogo)) m_iForcePogo = pogo;}

	int		GetForceFallDamage( void ) const { return m_iForceFallDamage; }
	void	SetForceFallDamage( int falldamage ) { m_iForceFallDamage = (falldamage >= 0 && falldamage <= 2)?falldamage:-1; }

	int		GetForceFragLimit( void ) const { return m_iForceFragLimit; }
	void	SetForceFragLimit( int limit ) { m_iForceFragLimit = limit; }

	int		GetForceTimeLimit( void ) const { return m_iForceTimeLimit; }
	void	SetForceTimeLimit( int limit ) { m_iForceTimeLimit = limit; }

	int		GetForceDomScoreMultiplier( void ) const { return m_iForceDomScoreMultiplier; }
	void	SetForceDomScoreMultiplier( int multiplier ) { m_iForceDomScoreMultiplier = multiplier; }

	float	GetForceDomScoreTimer( void ) const { return m_flForceDomScoreTimer; }
	void	SetForceDomScoreTimer( float timer ) { m_flForceDomScoreTimer = timer; }

	int		GetForceStartArmor( void ) const { return m_iForceStartArmor; }
	void	SetForceStartArmor( int armor ) { if(armor >= 0 && armor <= 200) m_iForceStartArmor = armor; }

	bool	IsFortsBuildPhase( void ) const { return m_bIsFortsBuildPhase; }
	bool	IsFortsFirstBuildPhase( void ) const { return m_bIsFirstBuildPhase; }
	void	SetFortsBuildPhase ( bool phase ) { m_bIsFortsBuildPhase = phase; }

	int		GetForceFlashlight( void ) const { return m_iForceFlashlight; }
	void	SetForceFlashlight( int force ) {if(IsForceValid(force)) m_iForceFlashlight = force;}

	float	GetLastFortsPhaseTime(void) const { return m_flLastPhaseTime; }

	void	ResetDomTimer(void) { m_flLastDomScoreTime = gpGlobals->curtime; }
	float	GetLastDomScoreTime(void) const { return m_flLastDomScoreTime; }

	bool	DontDropWeapons(void) const { return m_bDontDropWeapons; }
	void	SetDontDropWeapons(bool dontDropWeapons) { m_bDontDropWeapons = dontDropWeapons; }

	bool	OnlyGiveFlower(void) const { return m_bOnlyGiveFlower; }
	void	SetOnlyGiveFlower(bool onlyGiveFlower) { m_bOnlyGiveFlower = onlyGiveFlower;}

	float	GetTimeLimitMod() const { return m_flTimeLimitMod; }
	void	SetTimeLimitMod(float mod) { m_flTimeLimitMod = mod; }
	void	AddTimeLimitMod(float mod) { m_flTimeLimitMod += mod; }

	int		GetWinningTeamNumber(void) {return m_nWinningTeam;}
	CBasePlayer* GetBestPlayer(void) {return m_pBestPlayer;}

	void	PushTeamScored(int team, bool respawn, float delay);

	void	ReRollTeamplay(void);
	void	ResetStatics(void);

	float	GetHumiliationEndTime() { return m_flHumiliationEndTime; }

	//BP
	float	GetPhaseTimeLeft(void) { return m_flPhaseTimeLeft; }
	CBasePlayer*	m_apFortsInfosGreen[FORTS_MAX_INFOS];
	CBasePlayer*	m_apFortsInfosPink[FORTS_MAX_INFOS];
	int				m_iDomZones[DOM_MAX_ZONES];
	CNetworkVar( float, m_flPhaseTimeLeft );

	friend void RecvProxy_HL2MPRules( const RecvProp *pProp, void **pOut, void *pData, int objectID );

private:
	
	 #ifndef CLIENT_DLL 
 void InitDefaultAIRelationships( void );
 #endif

	CNetworkVar( bool, m_bTeamPlayEnabled );
	CNetworkVar( float, m_flGameStartTime );
	CUtlVector<EHANDLE> m_hRespawnableItemsAndWeapons;
	float m_tmNextPeriodicThink;
	float m_flRestartGameTime;
	bool m_bCompleteReset;
	bool m_bAwaitingReadyRestart;
	bool m_bHeardAllPlayersReady;
	/*	 #ifdef CLIENT_DLL 
	bool m_bIsFirstBuildPhase;
	int m_iGameType;
		#endif*/

	bool m_bDomDidPointCheck;
	float m_flLastPhaseTime;
	float m_flLastDomScoreTime;
	CNetworkVar(float, m_flHumiliationEndTime);
	int	m_nWinningTeam;
	float m_fPushRespawnDelay;
	float m_flPushLastBallCheck;
	bool m_bPushRespawn;
	CNetworkVar(float, m_flTimeLimitMod);
	bool m_bAlreadySetSkill;

	CBasePlayer* m_pBestPlayer;

	CNetworkVar( bool, m_bIsFortsBuildPhase );
	//#ifndef CLIENT_DLL 
	CNetworkVar( bool, m_bIsFirstBuildPhase );
	 //#endif
	CNetworkVar( int, m_iForceFlashlight );  // 0 : osef ; 1 : flashlight ; 2 : disable flashlight
	CNetworkVar( int, m_iForcePogo);
	CNetworkVar( int, m_iForceBunnyhop);
	//	#ifndef CLIENT_DLL 
	CNetworkVar( int, m_iGameType );
	//#endif

	CNetworkVar( int, m_iForceTimeLimit );
	CNetworkVar( int, m_iForceFragLimit );
	CNetworkVar( int, m_iForceStartArmor );
	CNetworkVar( int, m_iForceFallDamage );

	CNetworkVar( bool, m_bDontDropWeapons );
	CNetworkVar( bool, m_bOnlyGiveFlower );

	

	CNetworkVar( int, m_iForceDomScoreMultiplier );
	CNetworkVar( float, m_flForceDomScoreTimer );
};

inline CHL2MPRules* HL2MPRules()
{
	return static_cast<CHL2MPRules*>(g_pGameRules);
}

#endif //HL2MP_GAMERULES_H
