// BISOUNOURS PARTY : FLOWER

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
	#include "ilagcompensationmanager.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	FLOWER_RANGE 100.0f
#define	FLOWER_RANGE_COOP 65.0f
#define	FLOWER_REFIRE 0.6f

#define BLUDGEON_HULL_DIM 16
static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM,-BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM,BLUDGEON_HULL_DIM);

#ifdef CLIENT_DLL
#define CWeaponFlower C_WeaponFlower
#endif

extern ConVar sk_plr_dmg_flower;

class CWeaponFlower : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponFlower, CBaseHL2MPBludgeonWeapon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();


	CWeaponFlower();
	virtual void PrimaryAttack(void);
	void SecondaryAttack(void){ return;	}
	virtual float GetRange(void);
	virtual float GetFireRate(void){ return FLOWER_REFIRE; }
	void AddViewKick(void){ return; }
	float GetDamageForActivity(Activity hitActivity);
	void Drop(const Vector &vecVelocity);
	void Swing( int bIsSecondary );
	void Precache() { PrecacheScriptSound("Weapon_Melee.HitPlayer"); BaseClass::Precache(); }


	// Animation event
#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	void HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
#endif

	CWeaponFlower(const CWeaponFlower&);
};

IMPLEMENT_NETWORKCLASS_ALIASED(WeaponFlower, DT_WeaponFlower)

BEGIN_NETWORK_TABLE(CWeaponFlower, DT_WeaponFlower)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponFlower)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(weapon_flower, CWeaponFlower);
PRECACHE_WEAPON_REGISTER(weapon_flower);

acttable_t CWeaponFlower::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_MELEE,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_MELEE,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_MELEE,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_MELEE,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_MELEE,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_MELEE,					false },
};

IMPLEMENT_ACTTABLE(CWeaponFlower);

CWeaponFlower::CWeaponFlower( void )
{
}
float CWeaponFlower::GetRange( void )
{
	if(HL2MPRules()->GetGameType() == GAME_COOP)
		return	FLOWER_RANGE_COOP;
	else
		return FLOWER_RANGE;
}

void CWeaponFlower::PrimaryAttack()
{
#ifndef CLIENT_DLL
	CHL2MP_Player *pPlayer = ToHL2MPPlayer( GetPlayerOwner() );
	// Move other players back to history positions based on local player's lag
	lagcompensation->StartLagCompensation( pPlayer, pPlayer->GetCurrentCommand() );
#endif

	trace_t traceHit;
	// Try a ray
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	Vector swingStart = pOwner->Weapon_ShootPosition();
	Vector forward;

	pOwner->EyeVectors(&forward, NULL, NULL);

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine(swingStart, swingEnd, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit);

#ifndef CLIENT_DLL
	// Like bullets, bludgeon traces have to trace against triggers.
	CTakeDamageInfo triggerInfo(GetOwner(), GetOwner(), GetDamageForActivity( ACT_VM_PRIMARYATTACK ), DMG_CLUB);

	TraceAttackToTriggers(triggerInfo, traceHit.startpos, traceHit.endpos, vec3_origin);
#endif

	/*if(traceHit.fraction == 1.0)
	{
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull( swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pOwner, COLLISION_GROUP_NONE, &traceHit );
		if ( traceHit.fraction < 1.0 && traceHit.m_pEnt )
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize( vecToTarget );

			float dot = vecToTarget.Dot( forward );

			// YWB:  Make sure they are sort of facing the guy at least...
			if ( dot < 0.70721f )
				// Force amiss
				traceHit.fraction = 1.0f;
		}
	}*/
	
	WeaponSound(SINGLE);
	#ifndef CLIENT_DLL
	if(traceHit.m_pEnt){ // If it's a player, if it's DM or (he's not on our team and friendly fire is on)
		if(traceHit.m_pEnt->IsPlayer() && ( ( traceHit.m_pEnt->GetTeamNumber() != GetOwner()->GetTeamNumber() ) || cvar->FindVar("mp_friendlyfire")->GetBool() || traceHit.m_pEnt->GetTeamNumber() == TEAM_UNASSIGNED ) )
			EmitSound("Weapon_Melee.HitPlayer");
	}
	#endif

	if ( traceHit.fraction != 1.0f )
		Hit(traceHit, ACT_VM_PRIMARYATTACK);

	// Send the anim
	SendWeaponAnim(ACT_VM_PRIMARYATTACK);

	pOwner->SetAnimation(PLAYER_ATTACK1);
	ToHL2MPPlayer(pOwner)->DoAnimationEvent(PLAYERANIMEVENT_ATTACK_PRIMARY);

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + FLOWER_REFIRE;
	m_flNextSecondaryAttack = gpGlobals->curtime + FLOWER_REFIRE;

#ifndef CLIENT_DLL
	// Move other players back to history positions based on local player's lag
	lagcompensation->FinishLagCompensation( pPlayer );
#endif
}

float CWeaponFlower::GetDamageForActivity( Activity hitActivity )
{
	return 60;
}

#ifndef CLIENT_DLL
void CWeaponFlower::HandleAnimEventMeleeHit( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	// Trace up or down based on where the enemy is...
	// But only if we're basically facing that direction
	Vector vecDirection;
	AngleVectors( GetAbsAngles(), &vecDirection );

	Vector vecEnd;
	VectorMA( pOperator->Weapon_ShootPosition(), 50, vecDirection, vecEnd );
	CBaseEntity *pHurt = pOperator->CheckTraceHullAttack( pOperator->Weapon_ShootPosition(), vecEnd, 
		Vector(-16,-16,-16), Vector(36,36,36), GetDamageForActivity( GetActivity() ), DMG_CLUB, 0.75 );

	// did I hit someone?
	if ( pHurt )
	{
		// play sound
		WeaponSound( MELEE_MISS );

		// Fake a trace impact, so the effects work out like a player's crowbaw
		trace_t traceHit;
		UTIL_TraceLine( pOperator->Weapon_ShootPosition(), pHurt->GetAbsOrigin(), MASK_SHOT_HULL, pOperator, COLLISION_GROUP_NONE, &traceHit );
		ImpactEffect( traceHit );
	}
	else
		WeaponSound( MELEE_MISS );
}

void CWeaponFlower::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_MELEE_HIT:
		HandleAnimEventMeleeHit( pEvent, pOperator );
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;

	default:
		BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
		break;
	}
}
#endif

void CWeaponFlower::Drop( const Vector &vecVelocity )
{
#ifndef CLIENT_DLL
	UTIL_Remove( this );
#endif
}
