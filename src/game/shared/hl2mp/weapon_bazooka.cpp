// BISOUNOURS PARTY : BAZOOKA AND MISSILE

#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"
#include "weapon_bazooka.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "model_types.h"
	#include "view.h"
	#include "input.h"
	#include "dlight.h"
	#include "iefx.h"
#else
	#include "basecombatcharacter.h"
	#include "movie_explosion.h"
	#include "soundent.h"
	#include "player.h"
	#include "rope.h"
	#include "vstdlib/random.h"
	#include "engine/IEngineSound.h"
	#include "explode.h"
	#include "util.h"
	#include "in_buttons.h"
	#include "shake.h"
	#include "te_effect_dispatch.h"
	#include "triggers.h"
	#include "collisionutils.h"
	#include "hl2_shareddefs.h"
#endif

#include "debugoverlay_shared.h"
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define	BAZOOKA_SPEED 1800
#define BAZOOKA_EXPLOSION_RADIUS 250
extern ConVar mp_jumpfactor_rpg("mp_jumpfactor_rpg", "3", FCVAR_REPLICATED|FCVAR_CHEAT, "RocketJump height factor", true, 0, false, 999);
#ifndef CLIENT_DLL

BEGIN_DATADESC( CBazookaMissile )

	DEFINE_FIELD( m_hOwner,					FIELD_EHANDLE ),
	DEFINE_FIELD( m_flMarkDeadTime,			FIELD_TIME ),
	DEFINE_FIELD( m_flGracePeriodEndsAt,	FIELD_TIME ),
	DEFINE_FIELD( m_flDamage,				FIELD_FLOAT ),
	
	// Function Pointers
	DEFINE_FUNCTION( MissileTouch ),
	DEFINE_FUNCTION( DumbThink ),
	DEFINE_FUNCTION( IgniteThink ),

END_DATADESC()

LINK_ENTITY_TO_CLASS( bazooka_missile, CBazookaMissile );

class CWeaponBazooka;

CBazookaMissile::CBazookaMissile()
{
}

CBazookaMissile::~CBazookaMissile()
{
}

void CBazookaMissile::Precache( void )
{
	PrecacheModel( "models/weapons/w_missile.mdl" );

	//PrecacheParticleSystem( "obus_trail" );
}

extern ConVar sv_gravity;

void CBazookaMissile::Spawn( void )
{
	Precache();

	SetSolid( SOLID_BBOX );
	SetModel("models/weapons/w_missile.mdl");
	UTIL_SetSize( this, -Vector(4,4,4), Vector(4,4,4) );

	SetTouch( &CBazookaMissile::MissileTouch );
	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE );
	SetThink( &CBazookaMissile::IgniteThink );
	
	SetNextThink( gpGlobals->curtime );

	//DispatchParticleEffect
	
	
	m_takedamage = DAMAGE_YES;
	m_iHealth = m_iMaxHealth = 100;
//	m_bloodColor = DONT_BLEED;
	m_flGracePeriodEndsAt = 0;

	SetGravity(sv_gravity.GetFloat()/800);

	AddFlag( FL_OBJECT );
}

void CBazookaMissile::Event_Killed( const CTakeDamageInfo &info )
{
	m_takedamage = DAMAGE_NO;

	Explode();
}

unsigned int CBazookaMissile::PhysicsSolidMaskForEntity( void ) const
{ 
	return BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX;
}

/*int CBazookaMissile::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	return BaseClass::OnTakeDamage_Alive( info );
}*/

void CBazookaMissile::SetGracePeriod( float flGracePeriod )
{
	m_flGracePeriodEndsAt = gpGlobals->curtime + flGracePeriod;

	// Go non-solid until the grace period ends
	AddSolidFlags( FSOLID_NOT_SOLID );
}

void CBazookaMissile::DoExplosion( void )
{
	ExplosionCreate( GetAbsOrigin(), GetAbsAngles(), GetOwnerEntity(), GetDamage(), BAZOOKA_EXPLOSION_RADIUS, SF_ENVEXPLOSION_RND_ORIENT, 0.0f, this);
	DoRocketJump();
}

void CBazookaMissile::DoRocketJump(void)
{
	for ( int i = 0; i < gpGlobals->maxClients; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i+1 );

		if ( pPlayer == NULL )
			return;
		if ( !pPlayer->IsAlive() || pPlayer->IsObserver())
			return;

		Vector playerOrigin = pPlayer->GetAbsOrigin();
		Vector vel = GetAbsOrigin() - playerOrigin; // Vecteur Player->Rocket

		float multiplier = (playerOrigin.DistTo(GetAbsOrigin()) / -13.0f)+10.5f;
		multiplier = clamp(multiplier, 0, 10);

		vel.Negate(); // Pousse en arrière
		VectorNormalize(vel); // Utilisation du vecteur comme un "angle"

		multiplier *= mp_jumpfactor_rpg.GetFloat();

		if ( m_hOwner != NULL )
		{
			CBasePlayer *pOwner = ToBasePlayer(m_hOwner->GetOwner());
			if(pOwner != NULL)
			{
				if(pOwner != pPlayer)
				{
					if(g_pGameRules->IsTeamplay())
					{
						if(pOwner->GetTeamNumber() == pPlayer->GetTeamNumber())
							multiplier *= 3;
						else
							multiplier *= 2;
					}
					else
						multiplier *= 2;
				}
			}
		}

		vel *= multiplier * 10;
		pPlayer->SetAbsVelocity(pPlayer->GetAbsVelocity() + vel);
	}
}


void CBazookaMissile::Explode( void )
{
	// Don't explode against the skybox. Just pretend that the missile flies off into the distance.
	Vector forward;
	GetVectors( &forward, NULL, NULL );

	trace_t tr;
	UTIL_TraceLine( GetAbsOrigin(), GetAbsOrigin() + forward * 16, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );

	m_takedamage = DAMAGE_NO;
	SetSolid( SOLID_NONE );

	DoExplosion();

	if ( m_hOwner != NULL )
		m_hOwner = NULL;

	StopSound( "Missile.Ignite" );
	UTIL_Remove( this );
}

void CBazookaMissile::MissileTouch( CBaseEntity *pOther )
{
	Assert( pOther );
	
	// Don't touch triggers (but DO hit weapons)
	if ( pOther->IsSolidFlagSet(FSOLID_TRIGGER|FSOLID_VOLUME_CONTENTS) && pOther->GetCollisionGroup() != COLLISION_GROUP_WEAPON )
		return;

	Explode();
}

void CBazookaMissile::IgniteThink( void )
{
	SetMoveType( MOVETYPE_FLYGRAVITY );
	SetModel("models/weapons/w_missile.mdl");
	UTIL_SetSize( this, vec3_origin, vec3_origin );
 	RemoveSolidFlags( FSOLID_NOT_SOLID );

	EmitSound( "Missile.Ignite" );
	//DispatchParticleEffect("obus_trail", PATTACH_ABSORIGIN_FOLLOW, this);
	Vector vecForward;
	AngleVectors( GetLocalAngles(), &vecForward );
	SetAbsVelocity( vecForward * BAZOOKA_SPEED );

	SetThink( &CBazookaMissile::DumbThink );
	SetNextThink( gpGlobals->curtime );

		if ( m_hOwner && m_hOwner->GetOwner() )
	{
		CBasePlayer *pPlayer = ToBasePlayer( m_hOwner->GetOwner() );

		color32 white = { 255,225,205,64 };
		UTIL_ScreenFade( pPlayer, white, 0.1f, 0.0f, FFADE_IN );
	}
}

void CBazookaMissile::DumbThink( void )
{   
	CBasePlayer *pOwner = ToBasePlayer( GetOwnerEntity() );
	if (!pOwner)
		return;

	if(pOwner->m_bMarkedBazookaMissileDeletion)
		Explode();

    // If we have a grace period, go solid when it ends
    if ( m_flGracePeriodEndsAt )
        if ( m_flGracePeriodEndsAt < gpGlobals->curtime ){
            RemoveSolidFlags( FSOLID_NOT_SOLID );
            m_flGracePeriodEndsAt = 0;
        }

    //Fait pointer le missile dans la bonne direction
    QAngle angNewAngles;
    VectorAngles( GetAbsVelocity(), angNewAngles );
    SetAbsAngles( angNewAngles );

	//DispatchParticleEffect("obus_trail", PATTACH_ABSORIGIN_FOLLO, this, "trainee");

    SetNextThink( gpGlobals->curtime + 0.1f );
}

CBazookaMissile *CBazookaMissile::Create( const Vector &vecOrigin, const QAngle &vecAngles, edict_t *pentOwner = NULL )
{
	CBazookaMissile *pMissile = (CBazookaMissile *) CBaseEntity::Create( "bazooka_missile", vecOrigin, vecAngles, CBaseEntity::Instance( pentOwner ) );
	pMissile->SetOwnerEntity( Instance( pentOwner ) );
	pMissile->Spawn();
	pMissile->AddEffects( EF_NOSHADOW );

	Vector vecForward;
	AngleVectors( vecAngles, &vecForward );

	pMissile->SetAbsVelocity( vecForward * 320 + Vector( 0,0, 128 ) );
	return pMissile;
}

#endif

LINK_ENTITY_TO_CLASS( weapon_bazooka, CWeaponBazooka );

PRECACHE_WEAPON_REGISTER(weapon_bazooka);

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponBazooka, DT_WeaponBazooka )

BEGIN_NETWORK_TABLE( CWeaponBazooka, DT_WeaponBazooka )
#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_bInitialStateUpdate ) ),
#else
	SendPropBool( SENDINFO( m_bInitialStateUpdate ) ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL

BEGIN_PREDICTION_DATA( CWeaponBazooka )
	DEFINE_PRED_FIELD( m_bInitialStateUpdate, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()

#endif

acttable_t	CWeaponBazooka::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_RPG,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_RPG,			false },
	{ ACT_MP_RUN,						ACT_HL2MP_RUN_RPG,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_RPG,			false },
	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_RPG,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_RPG,	false },
	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_RPG,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_RPG,		false },
	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_RPG,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_RPG,					false },
};

IMPLEMENT_ACTTABLE(CWeaponBazooka);

CWeaponBazooka::CWeaponBazooka()
{
	m_bReloadsSingly = true;
	m_bInitialStateUpdate= false;

	m_fMinRange1 = m_fMinRange2 = 40*12;
	m_fMaxRange1 = m_fMaxRange2 = 500*12;
}

CWeaponBazooka::~CWeaponBazooka()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponBazooka::Precache( void )
{
	BaseClass::Precache();

	PrecacheScriptSound( "Missile.Ignite" );
	PrecacheScriptSound( "Missile.Accelerate" );

	PrecacheParticleSystem("muzzleflash_final_bazooka");

#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "rpg_missile" );
#endif
}

void CWeaponBazooka::Activate( void )
{
	BaseClass::Activate();
}

bool CWeaponBazooka::WeaponShouldBeLowered( void )
{
	if ( !HasAnyAmmo() )
		return true;
	
	return BaseClass::WeaponShouldBeLowered();
}
void CWeaponBazooka::SecondaryAttack( void )
{
#ifndef CLIENT_DLL
			CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;
	pPlayer->m_bMarkedBazookaMissileDeletion = true;
/*	CBazookaMissile *m_missile = dynamic_cast<CBazookaMissile*>(GetMissile());

	if(m_missile)
		m_missile->MissileTouch(this);*/
#endif
}
void CWeaponBazooka::PrimaryAttack( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;
	if(m_fSecondRefire > gpGlobals->curtime)
		return;

	// Can't be reloading
	if ( GetActivity() == ACT_VM_RELOAD )
		return;

	m_flNextPrimaryAttack = gpGlobals->curtime + 1.2f;
	m_fSecondRefire = gpGlobals->curtime + 1.4f;
	m_flNextSecondaryAttack = gpGlobals->curtime;
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );	
	if ( pOwner == NULL )
		return;

	Vector	vForward, vRight, vUp;
	pOwner->EyeVectors( &vForward, &vRight, &vUp );

	Vector	muzzlePoint = pOwner->Weapon_ShootPosition() + vForward * 12.0f + vRight * 6.0f + vUp * -3.0f;
	
#ifndef CLIENT_DLL
	QAngle vecAngles;
	VectorAngles( vForward, vecAngles );

	CBazookaMissile *pMissile = CBazookaMissile::Create( muzzlePoint, vecAngles, GetOwner()->edict() );
	pMissile->m_hOwner = this;

	// If the shot is clear to the player, give the missile a grace period
	trace_t	tr;
	Vector vecEye = pOwner->EyePosition();
	UTIL_TraceLine( vecEye, vecEye + vForward * 128, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );
	if ( tr.fraction == 1.0 )
		pMissile->SetGracePeriod( 0.3 );

	pMissile->SetDamage( GetHL2MPWpnData().m_iPlayerDamage );
	m_hMissile = pMissile;
#else
	dlight_t *el = effects->CL_AllocDlight( LIGHT_INDEX_MUZZLEFLASH + index );

	el->origin = muzzlePoint;
	el->radius = random->RandomInt( 64, 128 ); 
	el->decay = el->radius / 0.05f;
	el->die = gpGlobals->curtime + 0.15f;
	el->color.r = 255;
	el->color.g = 192;
	el->color.b = 64;
	el->color.exponent = 5;
#endif

	//BP muzzlefash de la mort
/*	Vector vecShootOrigin;
	QAngle angShootDir;
	GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);*/
	DispatchParticleEffect("muzzleflash_final_bazooka", muzzlePoint, pOwner->EyeAngles());

	DecrementAmmo( GetOwner() );
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	WeaponSound( SINGLE );

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );
	ToHL2MPPlayer(pPlayer)->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );
}

void CWeaponBazooka::DecrementAmmo( CBaseCombatCharacter *pOwner )
{
	pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );
}

void CWeaponBazooka::ItemPostFrame( void )
{
	BaseClass::ItemPostFrame();

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer == NULL )
		return;

	//If we're pulling the weapon out for the first time, wait to draw the laser
	if ( ( m_bInitialStateUpdate ) && ( GetActivity() != ACT_VM_DRAW ) )
		m_bInitialStateUpdate = false;
}

bool CWeaponBazooka::Deploy( void )
{
	m_bInitialStateUpdate = true;

	return BaseClass::Deploy();
}

bool CWeaponBazooka::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	return BaseClass::Holster( pSwitchingTo );
}

void CWeaponBazooka::Drop( const Vector &vecVelocity )
{
	BaseClass::Drop( vecVelocity );
}

bool CWeaponBazooka::Reload( void )
{
	CBaseCombatCharacter *pOwner = GetOwner();	
	if ( pOwner == NULL )
		return false;

	if ( pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
		return false;

	WeaponSound( RELOAD );	
	SendWeaponAnim( ACT_VM_RELOAD );

	return true;
}

#ifdef CLIENT_DLL

extern void FormatViewModelAttachment( Vector &vOrigin, bool bInverse );

//-----------------------------------------------------------------------------
// Purpose: Returns the attachment point on either the world or viewmodel
//			This should really be worked into the CBaseCombatWeapon class!
//-----------------------------------------------------------------------------
void CWeaponBazooka::GetWeaponAttachment( int attachmentId, Vector &outVector, Vector *dir /*= NULL*/ )
{
	QAngle	angles;

	//Tony; third person attachment
	if ( IsCarriedByLocalPlayer() && !::input->CAM_IsThirdPerson())
	{
		CBasePlayer *pOwner = ToBasePlayer( GetOwner() );		
		if ( pOwner != NULL )
		{
			pOwner->GetViewModel()->GetAttachment( attachmentId, outVector, angles );
			::FormatViewModelAttachment( outVector, true );
		}
	}
	else
	{
		// We offset the IDs to make them correct for our world model
		BaseClass::GetAttachment( attachmentId, outVector, angles );
	}

	// Supply the direction, if requested
	if ( dir != NULL )
	{
		AngleVectors( angles, dir, NULL, NULL );
	}
}

//Tony; added so when the rpg switches to third person, the beam etc is re-created.
void CWeaponBazooka::ThirdPersonSwitch( bool bThirdPerson )
{
}

//-----------------------------------------------------------------------------
// Purpose: Called on third-person weapon drawing
//-----------------------------------------------------------------------------
int	CWeaponBazooka::DrawModel( int flags )
{
	// Only render these on the transparent pass
	if ( flags & STUDIO_TRANSPARENCY )
		return 1;

	// Draw the model as normal
	return BaseClass::DrawModel( flags );
}

//-----------------------------------------------------------------------------
// Purpose: Called after first-person viewmodel is drawn
//-----------------------------------------------------------------------------
void CWeaponBazooka::ViewModelDrawn( C_BaseViewModel *pBaseViewModel )
{
	BaseClass::ViewModelDrawn( pBaseViewModel );
}

//-----------------------------------------------------------------------------
// Purpose: Used to determine sorting of model when drawn
//-----------------------------------------------------------------------------
bool CWeaponBazooka::IsTranslucent( void )
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Turns off effects when leaving the PVS
//-----------------------------------------------------------------------------
void CWeaponBazooka::NotifyShouldTransmit( ShouldTransmitState_t state )
{
	BaseClass::NotifyShouldTransmit(state);
}

#endif	//CLIENT_DLL