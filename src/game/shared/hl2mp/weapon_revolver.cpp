// BISOUNOURS PARTY : REVOLVER

#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"
#include "particle_parse.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "dlight.h"
	#include "iefx.h"
#else
	#include "hl2mp_player.h"
#endif

#include "weapon_hl2mpbasehlmpcombatweapon.h"


#ifdef CLIENT_DLL
#define CWeaponRevolver C_WeaponRevolver
#endif

class CWeaponRevolver : public CBaseHL2MPCombatWeapon
{
	DECLARE_CLASS( CWeaponRevolver, CBaseHL2MPCombatWeapon );

public:

	CWeaponRevolver( void );
	void Precache( void );
	void PrimaryAttack( void );
	bool Deploy();
	bool Reload();
	void	ItemPostFrame( void );
	void	ItemPreFrame( void );
	void	ItemBusyFrame( void );
	void Think();
	void UpdatePenaltyTime( void );
	void AddViewKick( void );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();
	
	CNetworkVar( float,	m_flSoonestPrimaryAttack );
	CNetworkVar( float,	m_flLastAttackTime );
	CNetworkVar( float,	m_flAccuracyPenalty );

private:
	
	CWeaponRevolver( const CWeaponRevolver & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponRevolver, DT_WeaponRevolver )

BEGIN_NETWORK_TABLE( CWeaponRevolver, DT_WeaponRevolver )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponRevolver )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_revolver, CWeaponRevolver );
PRECACHE_WEAPON_REGISTER( weapon_revolver );


acttable_t CWeaponRevolver::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_PISTOL,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_PISTOL,			false },
	{ ACT_MP_RUN,						ACT_HL2MP_RUN_PISTOL,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_PISTOL,			false },
	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_PISTOL,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_PISTOL,					false },

	//==================
	//AI Patch Addition.
	//==================	
	{ ACT_IDLE,						ACT_IDLE_PISTOL,				true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_PISTOL,			true },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_PISTOL,		true },
	{ ACT_RELOAD,					ACT_RELOAD_PISTOL,				true },
	{ ACT_WALK_AIM,					ACT_WALK_AIM_PISTOL,			true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_PISTOL,				true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_PISTOL,true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_PISTOL_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_PISTOL_LOW,	false },
	{ ACT_COVER_LOW,				ACT_COVER_PISTOL_LOW,			false },
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_PISTOL_LOW,		false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_WALK,						ACT_WALK_PISTOL,				false },
	{ ACT_RUN,						ACT_RUN_PISTOL,					false },
	//===== End Of Patch Addition =====
};

IMPLEMENT_ACTTABLE( CWeaponRevolver );

CWeaponRevolver::CWeaponRevolver( void )
{
	m_bReloadsSingly	= false;
	m_bFiresUnderwater	= false;
	SetThink( &CWeaponRevolver::Think );
	SetNextThink( gpGlobals->curtime + 0.1 );
	Deploy();
}

void CWeaponRevolver::Precache( void )
{
	BaseClass::Precache();

	PrecacheParticleSystem("muzzleflash_final_revolver");
	PrecacheScriptSound("Weapon_Revolver.Draw");
}
bool CWeaponRevolver::Deploy()
{
	//EmitSound("Weapon_Revolver.Draw");
	WeaponSound( SPECIAL1 );
	return	BaseClass::Deploy();
}
bool CWeaponRevolver::Reload()
{
	CBaseCombatCharacter *pOwner = GetOwner();
	if (!pOwner)
		return false;

	// If I don't have any spare ammo, I can't reload
	if ( pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
		return false;

	bool bReload = false;

	// If you don't have clips, then don't try to reload them.
	if ( UsesClipsForAmmo1() )
	{
		// need to reload primary clip?
		int primary	= min(GetMaxClip1() - m_iClip1, pOwner->GetAmmoCount(m_iPrimaryAmmoType));
		if ( primary != 0 )
		{
			bReload = true;
		}
	}

	if ( UsesClipsForAmmo2() )
	{
		// need to reload secondary clip?
		int secondary = min(GetMaxClip2() - m_iClip2, pOwner->GetAmmoCount(m_iSecondaryAmmoType));
		if ( secondary != 0 )
		{
			bReload = true;
		}
	}

	if ( !bReload )
		return false;
	/*float flSequenceEndTime = gpGlobals->curtime + 0.75f;
	pOwner->SetNextAttack( flSequenceEndTime );
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = flSequenceEndTime;*/
			m_flLastAttackTime = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.4f;
		//Allow a refire as fast as the player can click
	m_bInReload = true;	

	return true;
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRevolver::ItemPreFrame( void )
{
	UpdatePenaltyTime();

	BaseClass::ItemPreFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRevolver::ItemBusyFrame( void )
{
	UpdatePenaltyTime();

	BaseClass::ItemBusyFrame();
}
//-----------------------------------------------------------------------------
// Purpose: Allows firing as fast as button is pressed
//-----------------------------------------------------------------------------
void CWeaponRevolver::ItemPostFrame( void )
{
	BaseClass::ItemPostFrame();

	if ( m_bInReload )
		return;
	
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if ( pOwner == NULL )
		return;
		
	//Allow a refire as fast as the player can click
	if ( ( ( pOwner->m_nButtons & IN_ATTACK ) == false ) && ( m_flSoonestPrimaryAttack < gpGlobals->curtime ) )
	{
		m_flNextPrimaryAttack = gpGlobals->curtime - 1.0f;
	}
}
void CWeaponRevolver::Think()
{
	BaseClass::Think();

	if ( m_bInReload )
		return;
	
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if ( pOwner == NULL )
		return;

		//Allow a refire as fast as the player can click
	//if ( ( pOwner->m_nButtons & IN_ATTACK ) == false )
	//{
		//float flSequenceEndTime = gpGlobals->curtime;
		//pOwner->SetNextAttack( gpGlobals->curtime - 0.1f );
	//	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime - 0.1f;
	//}
//	SetNextThink( gpGlobals->curtime + 0.1 );
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponRevolver::UpdatePenaltyTime( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if ( pOwner == NULL )
		return;
	// Check our penalty time decay
	if ( ( ( pOwner->m_nButtons & IN_ATTACK ) == false ) && ( m_flSoonestPrimaryAttack < gpGlobals->curtime ) )
	{
		m_flNextPrimaryAttack = gpGlobals->curtime - 1.0f;
	}
}

void CWeaponRevolver::PrimaryAttack( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
		return;

	if ( m_iClip1 <= 0 )
	{
		if ( !m_bFireOnEmpty )
			Reload();
		else
		{
			//WeaponSound( EMPTY );
			//m_flNextPrimaryAttack = 0.05;
		}

		return;
	}

	//BP muzzlefash de la mort
	/*Vector vecShootOrigin;
	QAngle angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	DispatchParticleEffect("muzzleflash_final_revolver", vecShootOrigin, angShootDir);*/
	//pPlayer->DoMuzzleFlash();
	
	WeaponSound( SINGLE );	
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pPlayer->SetAnimation( PLAYER_ATTACK1 );
	ToHL2MPPlayer(pPlayer)->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );


	//m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + 0.1;

	m_iClip1--;
	m_flLastAttackTime = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.25f;
	m_flSoonestPrimaryAttack = gpGlobals->curtime + 0.25f;
	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	QAngle angAiming;
	
	VectorAngles(vecAiming, angAiming);
	angAiming.x += ((rand() % 100) / 100.0) * (rand() % 2 == 1 ? -1 : 1);
	angAiming.y += ((rand() % 100) / 100.0) * (rand() % 2 == 1 ? -1 : 1);
	AngleVectors(angAiming, &vecAiming);
	AddViewKick();
	FireBulletsInfo_t info( 1, vecSrc, vecAiming, vec3_origin, MAX_TRACE_LENGTH, m_iPrimaryAmmoType );
	info.m_pAttacker = pPlayer;

	Vector	vForward, vRight, vUp;
	Vector vecShootOrigin;
	QAngle angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	pPlayer->EyeVectors( &vForward, &vRight, &vUp );
#ifdef CLIENT_DLL
	Vector	muzzlePoint = pPlayer->Weapon_ShootPosition() + vForward * 12.0f + vRight * 6.0f + vUp * -3.0f;
#else
	Vector	muzzlePoint = vecShootOrigin;
#endif
	DispatchParticleEffect("muzzleflash_final_revolver", muzzlePoint, angShootDir);

#ifdef CLIENT_DLL
	dlight_t *el = effects->CL_AllocDlight( LIGHT_INDEX_MUZZLEFLASH + index );

	el->origin = muzzlePoint;
	el->radius = random->RandomInt( 40, 70 ); 
	el->decay = el->radius / 0.05f;
	el->die = gpGlobals->curtime + 0.1f;
	el->color.r = 255;
	el->color.g = 192;
	el->color.b = 64;
	el->color.exponent = 5;
#endif

	pPlayer->FireBullets( info );
	Reload();
	//m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + 0.1;
}
void CWeaponRevolver::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	int iSeed = CBaseEntity::GetPredictionRandomSeed() & 255;
	RandomSeed( iSeed );
	
	QAngle	viewPunch;

	viewPunch.x = random->RandomFloat( 0.25f, 0.5f );
	viewPunch.y = random->RandomFloat( -.6f, .6f );
	viewPunch.z = 0.0f;

	//Add it to the view punch
	pPlayer->ViewPunch( viewPunch );
}
