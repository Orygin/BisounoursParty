// BISOUNOURS PARTY : MINE (npc)

#ifndef NPCMINE_H
#define NPCMINE_H
#ifdef _WIN32
#pragma once
#endif

#include "basegrenade_shared.h"

// for constraints
#include "vphysics/constraints.h"

class CNPCMine : public CBaseGrenade
{
public:
	DECLARE_CLASS( CNPCMine, CBaseGrenade );

	CNPCMine();
	~CNPCMine();

	void Spawn( void );
	void Precache( void );
	
	void PowerupThink( void );
	void DetectionThink( void );
	void DelayDeathThink( void );
	void Explode( void );
	bool ShouldExplode(CBaseCombatCharacter *pBCC);
	void Event_Killed( const CTakeDamageInfo &info );

	// Added to create a constraint
	void AttachToEntity( CBaseEntity *pOther );
	bool MakeConstraint( CBaseEntity *pOther );

public:
	EHANDLE		m_hOwner;
	EHANDLE		m_hAttachEntity;

private:
	float		m_flPowerUp;
	float		m_flNextBeep;
	Vector		m_vecDir;
	Vector		m_vecEnd;

	Vector		m_posOwner;
	Vector		m_angleOwner;

	// signifies if we're attached to something, and need to update slightly differently.
	bool	    m_bAttached;
	IPhysicsConstraint	*m_pConstraint;
	Vector		m_vAttachedPosition;	// if the attached position changes, we need to detonate

	DECLARE_DATADESC();
};

#endif // GRENADE_TRIPMINE_H
