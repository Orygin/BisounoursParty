// BISOUNOURS PARTY : GRENADE (weapon)

#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "c_te_effect_dispatch.h"
#else
	#include "hl2mp_player.h"
	#include "te_effect_dispatch.h"
	#include "hl2mp/grenade_boing.h"
	#include "Sprite.h"
#endif

#include "weapon_ar2.h"
#include "effect_dispatch_data.h"
#include "weapon_hl2mpbasehlmpcombatweapon.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define GRENADE_TIMER	4.0f
#define GRENADE_RADIUS 4.0f
#define GRENADE_DAMAGE_RADIUS 250.0f

#ifdef CLIENT_DLL
#define CWeaponGrenade C_WeaponGrenade
#endif

class CWeaponGrenade: public CBaseHL2MPCombatWeapon
{
	DECLARE_CLASS( CWeaponGrenade, CBaseHL2MPCombatWeapon );
public:

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponGrenade();
	void	Precache( void );
	void	PrimaryAttack( void );
	void    PrimaryAttackThrow( void );
	void	DecrementAmmo( CBaseCombatCharacter *pOwner );
	void	ItemPostFrame( void );
	bool	Deploy( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );	
	bool	Reload( void );
	void	ThrowGrenade( CBasePlayer *pPlayer );
	
private:

	// check a throw from vecSrc.  If not valid, move the position back along the line to vecEye
	void	CheckThrowPosition( CBasePlayer *pPlayer, const Vector &vecEye, Vector &vecSrc );

	CNetworkVar( bool,	m_bRedraw );
	CNetworkVar( bool,	m_bAttackPaused );

	CWeaponGrenade( const CWeaponGrenade & );

	DECLARE_ACTTABLE();
};

acttable_t	CWeaponGrenade::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_GRENADE,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_GRENADE,			false },
	{ ACT_MP_RUN,						ACT_HL2MP_RUN_GRENADE,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_GRENADE,			false },
	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_GRENADE,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_GRENADE,	false },
	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_GRENADE,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_GRENADE,		false },
	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_GRENADE,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_GRENADE,					false },
};

IMPLEMENT_ACTTABLE(CWeaponGrenade);

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponGrenade, DT_WeaponGrenade )

BEGIN_NETWORK_TABLE( CWeaponGrenade, DT_WeaponGrenade )

#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_bRedraw ) ),
	RecvPropInt( RECVINFO( m_bAttackPaused ) ),
#else
	SendPropBool( SENDINFO( m_bRedraw ) ),
	SendPropInt( SENDINFO( m_bAttackPaused ) ),
#endif
	
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponGrenade )
	DEFINE_PRED_FIELD( m_bRedraw, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bAttackPaused, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_grenade, CWeaponGrenade );

PRECACHE_WEAPON_REGISTER(grenade);

CWeaponGrenade::CWeaponGrenade( void ) : CBaseHL2MPCombatWeapon()
{
	m_bRedraw = false;
	Precache();
}

void CWeaponGrenade::Precache( void )
{
	BaseClass::Precache();

	PrecacheScriptSound( "Weapon_Grenade.Throw" );
	PrecacheModel("models/weapons/v_grenade.mdl");
#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "npc_grenade_frag" );
#endif
}

bool CWeaponGrenade::Deploy( void )
{
	m_bRedraw = false;

	return BaseClass::Deploy();
}

bool CWeaponGrenade::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	m_bRedraw = false;

	return BaseClass::Holster( pSwitchingTo );
}

bool CWeaponGrenade::Reload( void )
{
	if ( !HasPrimaryAmmo() )
		return false;

	if ( m_bRedraw && m_flNextPrimaryAttack <= gpGlobals->curtime )
	{
		SendWeaponAnim( ACT_VM_DRAW );
		m_flNextPrimaryAttack	= m_flTimeWeaponIdle = gpGlobals->curtime + SequenceDuration();
		m_bRedraw = false;
	}

	return true;
}

void CWeaponGrenade::PrimaryAttack( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();
	if ( pOwner == NULL )
		return;

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );;
	if ( !pPlayer )
		return;

	SendWeaponAnim( ACT_VM_PULLBACK_HIGH );	
	m_flTimeWeaponIdle = m_flNextPrimaryAttack = FLT_MAX;

	m_bAttackPaused = true;
}

void CWeaponGrenade::PrimaryAttackThrow( void ){
	CBaseCombatCharacter *pOwner  = GetOwner();
	if ( pOwner == NULL )
		return;

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );;
	if ( !pPlayer )
		return;

	SendWeaponAnim( ACT_VM_THROW );
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();

	ThrowGrenade(pPlayer);
	DecrementAmmo(pOwner);

	m_bAttackPaused = false;

	if(!HasPrimaryAmmo())
		pPlayer->SwitchToNextBestWeapon( this );
}

void CWeaponGrenade::DecrementAmmo( CBaseCombatCharacter *pOwner )
{
	pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );
}

void CWeaponGrenade::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (pOwner)
	{
		//Si on est pas en attente de lancer et que le nouton est appuyé
		if(!m_bAttackPaused && (pOwner->m_nButtons & IN_ATTACK) && HasPrimaryAmmo() && gpGlobals->curtime > m_flNextPrimaryAttack)
			PrimaryAttack();
		//Sinon si on est en attente de lancer et que le bouton n'est plus appuyé
		else if(m_bAttackPaused && !(pOwner->m_nButtons & IN_ATTACK))
			PrimaryAttackThrow();
	}

	BaseClass::ItemPostFrame();

	if ( m_bRedraw )
		if ( IsViewModelSequenceFinished() )
			Reload();

	if(!HasPrimaryAmmo())
		pOwner->SwitchToNextBestWeapon( NULL );
}

// check a throw from vecSrc.  If not valid, move the position back along the line to vecEye
void CWeaponGrenade::CheckThrowPosition( CBasePlayer *pPlayer, const Vector &vecEye, Vector &vecSrc )
{
	trace_t tr;
	UTIL_TraceHull( vecEye, vecSrc, -Vector(GRENADE_RADIUS+2,GRENADE_RADIUS+2,GRENADE_RADIUS+2), Vector(GRENADE_RADIUS+2,GRENADE_RADIUS+2,GRENADE_RADIUS+2), 
		pPlayer->PhysicsSolidMaskForEntity(), pPlayer, pPlayer->GetCollisionGroup(), &tr );
	
	if ( tr.DidHit() )
		vecSrc = tr.endpos;
}

void CWeaponGrenade::ThrowGrenade( CBasePlayer *pPlayer )
{
#ifndef CLIENT_DLL
	Vector	vecEye = pPlayer->EyePosition();
	Vector	vForward, vRight;

	pPlayer->EyeVectors( &vForward, &vRight, NULL );
	Vector vecSrc = vecEye + vForward * 18.0f + vRight * 8.0f;
	CheckThrowPosition( pPlayer, vecEye, vecSrc );
	vForward[2] += 0.1f;

	Vector vecThrow;
	pPlayer->GetVelocity( &vecThrow, NULL );
	vecThrow += vForward * 1200;
	CBaseGrenade *pGrenade = GrenadeBoing_Create( vecSrc, vec3_angle, vecThrow, AngularImpulse(600,600,0), pPlayer, GRENADE_TIMER, false );
	//AngularImpulse(600,random->RandomInt(-1200,1200),0)
	if ( pGrenade )
	{
		if ( pPlayer && pPlayer->m_lifeState != LIFE_ALIVE )
		{
			pPlayer->GetVelocity( &vecThrow, NULL );

			IPhysicsObject *pPhysicsObject = pGrenade->VPhysicsGetObject();
			if ( pPhysicsObject )
				pPhysicsObject->SetVelocity( &vecThrow, NULL );
		}
		
		pGrenade->SetDamage( GetHL2MPWpnData().m_iPlayerDamage );
		pGrenade->SetDamageRadius( GRENADE_DAMAGE_RADIUS );
	}
#endif
	WeaponSound( SINGLE );
}