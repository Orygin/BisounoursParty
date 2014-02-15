// BISOUNOURS PARTY : BAT

#include "cbase.h"
#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "weapon_hl2mpbasebasebludgeon.h"
#include "vstdlib/random.h"
#include "npcevent.h"
#include "hl2mp_gamerules.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "autovocals.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	BAT_RANGE	55.0f
#define	BAT_RANGE_COOP	40.0f
#define BAT_RANGE_HUMILIATION 70.0f
#define	BAT_REFIRE	0.8f

#ifdef CLIENT_DLL
#define CWeaponBat C_WeaponBat
#endif

extern ConVar sk_plr_dmg_bat;

class CWeaponBat : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponBat, CBaseHL2MPBludgeonWeapon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

	CWeaponBat();
	float		GetRange( void );
	float		GetFireRate( void )		{	return	BAT_REFIRE;	}
	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );
	void		SecondaryAttack( void )	{	return;	}
	void		Drop( const Vector &vecVelocity );
	void		OnPickedUp( CBaseCombatCharacter *pNewOwner );

	// Animation event
#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	void AttemptToMaterialize(void);
#endif

	CWeaponBat( const CWeaponBat & );

private:
		
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponBat, DT_WeaponBat )

BEGIN_NETWORK_TABLE( CWeaponBat, DT_WeaponBat )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponBat )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_bat, CWeaponBat );

PRECACHE_WEAPON_REGISTER( weapon_bat );

#ifndef CLIENT_DLL

acttable_t	CWeaponBat::m_acttable[] = 
{
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_SLAM, true },
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_MELEE,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_MELEE,					false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_MELEE,			false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_MELEE,			false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_MELEE,					false },
};

IMPLEMENT_ACTTABLE(CWeaponBat);

#endif

CWeaponBat::CWeaponBat( void )
{
}


float CWeaponBat::GetRange( void )
{
	if(HL2MPRules()->IsHumiliation())
		return BAT_RANGE_HUMILIATION;
	else if(HL2MPRules()->GetGameType() == GAME_COOP)
        return	BAT_RANGE_COOP;
	else
		return BAT_RANGE;
}

float CWeaponBat::GetDamageForActivity( Activity hitActivity )
{
	return sk_plr_dmg_bat.GetFloat();
}

void CWeaponBat::OnPickedUp( CBaseCombatCharacter *pNewOwner )
{
#ifndef CLIENT_DLL
	CBasePlayer *pPlayer = ToBasePlayer(pNewOwner);
	if(pPlayer == NULL)
		return;

	pPlayer->PlayAutovocal(BATPICKUP,0);
#endif

	BaseClass::OnPickedUp(pNewOwner);
}

void CWeaponBat::AddViewKick( void )
{
}


#ifndef CLIENT_DLL
void CWeaponBat::HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	// Trace up or down based on where the enemy is...
	// But only if we're basically facing that direction
	Vector vecDirection;
	AngleVectors( GetAbsAngles(), &vecDirection );

	Vector vecEnd;
	VectorMA( pOperator->Weapon_ShootPosition(), 50, vecDirection, vecEnd );
	CBaseEntity *pHurt = pOperator->CheckTraceHullAttack( pOperator->Weapon_ShootPosition(), vecEnd, Vector(-16,-16,-16), Vector(36,36,36), GetDamageForActivity( GetActivity() ), DMG_CLUB, 0.75 );
	
	// did I hit someone?
	if ( pHurt )
	{
		// play sound
		WeaponSound( MELEE_HIT );

		// Fake a trace impact, so the effects work out like a player's crowbaw
		trace_t traceHit;
		UTIL_TraceLine( pOperator->Weapon_ShootPosition(), pHurt->GetAbsOrigin(), MASK_SHOT_HULL, pOperator, COLLISION_GROUP_NONE, &traceHit );
		ImpactEffect( traceHit );
	}
	else
		WeaponSound( MELEE_MISS );
}

void CWeaponBat::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( pEvent, pOperator );
		break;

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}

void CWeaponBat::AttemptToMaterialize(void)
{
	// On va check toutes les armes de tous les joueurs en vie de la partie
	// si un de ces enfoirés possède la batte, on empêche de respawn une autre
	for( int i=1; i<=gpGlobals->maxClients; ++i )
	{
		CBasePlayer *pPlayer = static_cast<CBasePlayer *>( UTIL_PlayerByIndex( i ) );
		if (pPlayer == NULL)
			continue;
		if (FNullEnt( pPlayer->edict() ))
			continue;
		if (!pPlayer->IsPlayer())
			continue;
		if(!pPlayer->IsAlive())
			continue;

		for ( int i = 0 ; i < pPlayer->WeaponCount() ; i++ )
		{
			CBaseCombatWeapon *pWeapon = pPlayer->GetWeapon(i);
			if ( pWeapon == NULL )
				continue;
			if ( FClassnameIs(pWeapon, "weapon_bat"))
			{
				SetNextThink( gpGlobals->curtime + 1.0f );
				return;
			}
		}
	}

	return BaseClass::AttemptToMaterialize();
}

#endif

void CWeaponBat::Drop( const Vector &vecVelocity )
{
#ifndef CLIENT_DLL
	UTIL_Remove( this );
#endif
}