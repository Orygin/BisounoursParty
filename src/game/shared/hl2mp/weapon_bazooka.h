// BISOUNOURS PARTY : BAZOOKA AND MISSILE

#ifndef WEAPON_BAZOOKA_H
#define WEAPON_BAZOOKA_H

#ifdef _WIN32
#pragma once
#endif

#include "weapon_hl2mpbasehlmpcombatweapon.h"

#ifndef CLIENT_DLL
#include "npcevent.h"
#include "basegrenade_shared.h"
#include "particle_parse.h"

class CWeaponBazooka;
#if defined( _LINUX ) && !defined( _WIN32 )
class CBazookaMissile : public CBaseGrenade
{
	DECLARE_CLASS( CBazookaMissile, CBaseGrenade );
#else
class CBazookaMissile : public CBaseCombatCharacter
{
	DECLARE_CLASS( CBazookaMissile, CBaseCombatCharacter );
#endif

public:
	CBazookaMissile();
	~CBazookaMissile();

#ifdef HL1_DLL
	Class_T Classify( void ) { return CLASS_NONE; }
#else
	Class_T Classify( void ) { return CLASS_MISSILE; }
#endif
	
	void	Spawn( void );
	void	Precache( void );
	void	MissileTouch( CBaseEntity *pOther );
	void	Explode();
	void	DumbThink();
	void	IgniteThink();
	void	SetGracePeriod( float flGracePeriod );
	void	DoRocketJump(void);
	//int		OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void	Event_Killed( const CTakeDamageInfo &info );
	
	virtual float	GetDamage() { return m_flDamage; }
	virtual void	SetDamage(float flDamage) { m_flDamage = flDamage; }

	unsigned int PhysicsSolidMaskForEntity( void ) const;

	CHandle<CWeaponBazooka>		m_hOwner;

	static CBazookaMissile *Create( const Vector &vecOrigin, const QAngle &vecAngles, edict_t *pentOwner );

protected:
	virtual void DoExplosion();	

	float					m_flMarkDeadTime;
	float					m_flDamage;

private:
	float					m_flGracePeriodEndsAt;
	DECLARE_DATADESC();
};

#endif

//-----------------------------------------------------------------------------
// RPG
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponBazooka C_WeaponBazooka
#endif

class CWeaponBazooka : public CBaseHL2MPCombatWeapon
{
	DECLARE_CLASS( CWeaponBazooka, CBaseHL2MPCombatWeapon );
public:

	CWeaponBazooka();
	~CWeaponBazooka();

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	void	Precache( void );

	void	PrimaryAttack( void );
	void	SecondaryAttack( void );
	virtual float GetFireRate( void ) { return 1; };
	void	ItemPostFrame( void );

	void	Activate( void );
	void	DecrementAmmo( CBaseCombatCharacter *pOwner );

	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	bool	Reload( void );
	bool	WeaponShouldBeLowered( void );

	virtual void Drop( const Vector &vecVelocity );

	int		GetMinBurst() { return 1; }
	int		GetMaxBurst() { return 1; }
	float	GetMinRestTime() { return 4.0; }
	float	GetMaxRestTime() { return 4.0; }

	void	SuppressGuiding( bool state = true );

	
#ifdef CLIENT_DLL

	// We need to render opaque and translucent pieces
	virtual RenderGroup_t	GetRenderGroup( void ) {	return RENDER_GROUP_TWOPASS;	}

	virtual void	NotifyShouldTransmit( ShouldTransmitState_t state );
	virtual int		DrawModel( int flags );
	virtual void	ViewModelDrawn( C_BaseViewModel *pBaseViewModel );
	virtual bool	IsTranslucent( void );
	void			GetWeaponAttachment( int attachmentId, Vector &outVector, Vector *dir = NULL );

	CMaterialReference	m_hSpriteMaterial;	// Used for the laser glint
	CMaterialReference	m_hBeamMaterial;	// Used for the laser beam
	Beam_t				*m_pBeam;			// Laser beam temp entity

	//Tony; third person check thing, to destroy/reinitialize the beam ( swapping first -> third or back, etc )
	virtual void ThirdPersonSwitch( bool bThirdPerson );

#endif	//CLIENT_DLL

	CBaseEntity *GetMissile( void ) { return m_hMissile; }

	DECLARE_ACTTABLE();
	
protected:

	CNetworkVar( bool, m_bInitialStateUpdate );
	CNetworkHandle( CBaseEntity,	m_hMissile );

private:
	CWeaponBazooka( const CWeaponBazooka & );
	float m_fSecondRefire;
};

#endif