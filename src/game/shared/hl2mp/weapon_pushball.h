#ifndef	WEAPONPUSH_H
#define	WEAPONPUSH_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "weapon_hl2mpbasebasebludgeon.h"

#ifdef CLIENT_DLL
#define CWeaponPushBall C_WeaponPushBall
#endif

class CWeaponPushBall : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponPushBall, CBaseHL2MPBludgeonWeapon );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	DECLARE_ACTTABLE();

	CWeaponPushBall();
	float		GetRange( void );
	float		GetFireRate( void )		{	return	1;	}
	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );
	void		SecondaryAttack( void )	{	return;	}
	void		Drop( const Vector &vecVelocity );
	void		OnPickedUp( CBaseCombatCharacter *pNewOwner );
	void		Precache();
	bool		CanHolster();
	void		Think();
	void		DropBall();
	void		Delete();
	void		SetShouldRepop(bool should);

	// Animation event
#ifndef CLIENT_DLL
	void	PrimaryAttack();
#endif

	CWeaponPushBall( const CWeaponPushBall & );
private:
	bool m_Dying;
	bool m_bShouldRepop;
	bool m_bHasParticles;
	float m_flCreationTime;
};

LINK_ENTITY_TO_CLASS( weapon_PushBall, CWeaponPushBall );

#endif