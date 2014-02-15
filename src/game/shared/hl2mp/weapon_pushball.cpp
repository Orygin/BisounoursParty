// BISOUNOURS PARTY : PushBall

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
#include "weapon_pushball.h"
#include "particle_parse.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "autovocals.h"
	#include "playerSkins.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



void Game_RespawnPushBall();

#define BALLVELOCITY 700
#define BALLVELOCITYZ BALLVELOCITY * 0.8f
#define BALLPICKUPTIME 0.2f
#define PI 3.14159265
#define BALLRESPTIME 25


IMPLEMENT_NETWORKCLASS_ALIASED( WeaponPushBall, DT_WeaponPushBall )

BEGIN_NETWORK_TABLE( CWeaponPushBall, DT_WeaponPushBall )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponPushBall )
END_PREDICTION_DATA()

PRECACHE_WEAPON_REGISTER( weapon_PushBall );

acttable_t	CWeaponPushBall::m_acttable[] = 
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

IMPLEMENT_ACTTABLE(CWeaponPushBall);

CWeaponPushBall::CWeaponPushBall( void )
{
	m_flCreationTime = gpGlobals->curtime;
#ifndef CLIENT_DLL
	AddSpawnFlags(SF_NORESPAWN);
#endif
	SetMoveType(MOVETYPE_FLYGRAVITY);
	Precache();
	m_bShouldRepop = true;
	m_bHasParticles = false;
}
void CWeaponPushBall::SetShouldRepop(bool should)
{
	m_bShouldRepop = should;
}
void CWeaponPushBall::Precache()
{
	PrecacheScriptSound("Gameplay.Push.BallRespawn");
	BaseClass::Precache();
}

float CWeaponPushBall::GetRange( void )
{
return 1;
}

float CWeaponPushBall::GetDamageForActivity( Activity hitActivity )
{
	return 0;
}

void CWeaponPushBall::OnPickedUp( CBaseCombatCharacter *pNewOwner )
{
#ifndef CLIENT_DLL
	CBasePlayer *pPlayer = ToBasePlayer(pNewOwner);
	if(pPlayer == NULL)
		return;
	pPlayer->Weapon_Switch(this);
	pPlayer->PlayAutovocal(PUSHBALLPICKUP,0);

	if(pPlayer->GetTeamNumber() == TEAM_PINK)
		pPlayer->m_nSkin = PLAYER_BONUS_NROSE;
	else
		pPlayer->m_nSkin = PLAYER_BONUS_NVERT;
#endif
	m_Dying = false;
	BaseClass::OnPickedUp(pNewOwner);
}
void CWeaponPushBall::Think()
{
#ifndef CLIENT_DLL
	if( ( m_flCreationTime + BALLRESPTIME < gpGlobals->curtime ) && !GetOwner() && m_bShouldRepop)
	{
		Game_RespawnPushBall();
		Remove();
		CBaseEntity::EmitSound("Gameplay.Push.BallRespawn");
	}
	if(!m_bHasParticles)
	{
		StopParticleEffects( this );
		DispatchParticleEffect( "ball", PATTACH_ABSORIGIN_FOLLOW, this, 0, false );
		m_bHasParticles = true;
	}
#endif

	return BaseClass::Think();
}
void CWeaponPushBall::AddViewKick( void )
{
}
bool CWeaponPushBall::CanHolster()
{
	return m_Dying;
}

#ifndef CLIENT_DLL
void CWeaponPushBall::PrimaryAttack()
{
	DropBall();
}
#endif
void CWeaponPushBall::DropBall()
{
#ifndef CLIENT_DLL
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if(pOwner)
	{
		Vector Pos = pOwner->EyePosition();
		QAngle ang = pOwner->GetAbsAngles();
		Vector Vec = pOwner->GetAbsVelocity();

		// We add velocity based on where the player is currently looking at
		Vec.x = Vec.x * 1.2f + ( BALLVELOCITY * cos(ang.y*PI/180) * cos((-ang.x)*PI/180) );
		Vec.y = Vec.y * 1.2f + ( BALLVELOCITY * sin(ang.y*PI/180) * cos((-ang.x)*PI/180) );
		Vec.z = Vec.z * 1.2f + ( BALLVELOCITYZ * sin((-ang.x)*PI/180) );

		CWeaponPushBall *newball = (CWeaponPushBall*)CBaseEntity::Create("weapon_pushball", Pos, GetOwner()->GetAbsAngles(), GetOwner());
		newball->m_NextPickAllowed = gpGlobals->curtime + BALLPICKUPTIME;
		newball->m_NextPickAllowedPlayer = pOwner;

		newball->SetAbsVelocity(Vec);
		newball->SetBaseVelocity(Vec);
		newball->ApplyAbsVelocityImpulse(Vec);

		m_Dying = true;
		pOwner->SwitchToNextBestWeapon(NULL);

		if(pOwner->GetTeamNumber() == TEAM_PINK)
			pOwner->m_nSkin = toSkinId(PLAYER_SKIN_ROSE);
		else
			pOwner->m_nSkin = toSkinId(PLAYER_SKIN_VERT);
		
	}
	UTIL_Remove( this );
#endif
}
void CWeaponPushBall::Drop( const Vector &vecVelocity )
{
#ifndef CLIENT_DLL
	m_Dying = true;
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if(pOwner)
	{
		if(pOwner->GetTeamNumber() == TEAM_PINK)
			pOwner->m_nSkin = toSkinId(PLAYER_SKIN_ROSE);
		else
			pOwner->m_nSkin = toSkinId(PLAYER_SKIN_VERT);
	}
	return BaseClass::Drop(vecVelocity);
	//UTIL_Remove(this);
	#endif
}
void CWeaponPushBall::Delete()
{
#ifndef CLIENT_DLL
	m_Dying = true;
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if(pOwner)
	{
		if(pOwner->GetTeamNumber() == TEAM_PINK)
			pOwner->m_nSkin = toSkinId(PLAYER_SKIN_ROSE);
		else
			pOwner->m_nSkin = toSkinId(PLAYER_SKIN_VERT);
	}
	UTIL_Remove(this);
#endif
}