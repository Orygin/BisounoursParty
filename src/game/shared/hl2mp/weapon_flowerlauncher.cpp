// BIISOUNOURS PARTY : FLOWERLAUNCHER

#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "c_te_effect_dispatch.h"
	#include "input.h"
#else
	#include "hl2mp_player.h"
	#include "te_effect_dispatch.h"
	#include "IEffects.h"
	#include "Sprite.h"
	#include "SpriteTrail.h"
	#include "beam_shared.h"
	#include "gamerules.h"
#endif

#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "effect_dispatch_data.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//#define BOLT_MODEL			"models/crossbow_bolt.mdl"
#define BOLT_MODEL	"models/weapons/w_missile_closed.mdl"

#define BOLT_VELOCITY 2000
#define	BOLT_SKIN_NORMAL	0
#define BOLT_SKIN_GLOW		1

#ifndef CLIENT_DLL

extern ConVar sv_gravity;

void TE_StickyBolt( IRecipientFilter& filter, float delay,	Vector vecDirection, const Vector *origin );

class CFlowerBolt : public CBaseCombatCharacter
{
	DECLARE_CLASS( CFlowerBolt, CBaseCombatCharacter );

public:
	CFlowerBolt() { };
	~CFlowerBolt();

	Class_T Classify( void ) { return CLASS_NONE; }

public:
	void Spawn( void );
	void Precache( void );
	void BubbleThink( void );
	void BoltTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CFlowerBolt *BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner = NULL );

protected:

	int		m_iDamage;
	CHandle<CSprite>		m_pGlowSprite;
	CHandle<CSpriteTrail>	m_pGlowTrail;
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
};

LINK_ENTITY_TO_CLASS( flowerlauncher_bolt, CFlowerBolt );

BEGIN_DATADESC( CFlowerBolt )
	// Function Pointers
	DEFINE_FIELD( m_pGlowSprite, FIELD_EHANDLE ),
	DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),
	DEFINE_FUNCTION( BubbleThink ),
	DEFINE_FUNCTION( BoltTouch ),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CFlowerBolt, DT_FlowerBolt )
END_SEND_TABLE()

CFlowerBolt *CFlowerBolt::BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner )
{
	CFlowerBolt *pBolt = (CFlowerBolt *)CreateEntityByName( "flowerlauncher_bolt" );
	UTIL_SetOrigin( pBolt, vecOrigin );
	pBolt->SetAbsAngles( angAngles );
	pBolt->Spawn();
	pBolt->SetCollisionGroup(COLLISION_GROUP_PROJECTILE);
	pBolt->SetOwnerEntity( pentOwner );
	pBolt->m_iDamage = iDamage;

	return pBolt;
}

CFlowerBolt::~CFlowerBolt( void )
{
}

bool CFlowerBolt::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, FSOLID_NOT_STANDABLE, false );

	return true;
}

unsigned int CFlowerBolt::PhysicsSolidMaskForEntity() const
{
	int mask = 0;
	#ifdef HL2MP
	if ( HL2MPRules()->IsTeamplay() )
	{
		if(GetOwnerEntity() && ToHL2MPPlayer(GetOwnerEntity()))
		{
			switch ( ToHL2MPPlayer(GetOwnerEntity())->GetTeamNumber() )
			{
			case TEAM_GREEN:
				mask = CONTENTS_TEAM1;
				break;

			case TEAM_PINK:
				mask = CONTENTS_TEAM2;
				break;
			}
		}
	}
	#endif
	return (mask | ( BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX ) & ~CONTENTS_GRATE);
}

void CFlowerBolt::Spawn( void )
{
	Precache( );

	SetModel( "models/crossbow_bolt.mdl" );
	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
	UTIL_SetSize( this, -Vector(1,1,1), Vector(1,1,1) );
	SetSolid( SOLID_BBOX );
	SetGravity( sv_gravity.GetFloat() / 450 );
	
	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch( &CFlowerBolt::BoltTouch );

	SetThink( &CFlowerBolt::BubbleThink );
	SetNextThink( gpGlobals->curtime + 0.1f );

	m_pGlowSprite = CSprite::SpriteCreate( "sprites/light_glow02_noz.vmt", GetLocalOrigin(), false );

	if ( m_pGlowSprite != NULL )
	{
		m_pGlowSprite->FollowEntity( this );
		m_pGlowSprite->SetTransparency( kRenderGlow, 255, 255, 255, 128, kRenderFxNoDissipation );
		m_pGlowSprite->SetScale( 0.2f );
		//m_pGlowSprite->TurnOff();
	}
	m_pGlowTrail = CSpriteTrail::SpriteTrailCreate( "sprites/grenade/grenade_laser.vmt", GetLocalOrigin(), false );

	if ( m_pGlowTrail != NULL )
	{
		m_pGlowTrail->FollowEntity( this );
		m_pGlowTrail->SetTransparency( kRenderTransAdd, 255, 255, 255, 70, kRenderFxNone );
		m_pGlowTrail->SetStartWidth( 8.0f );
		m_pGlowTrail->SetEndWidth( 1.0f );
		m_pGlowTrail->SetLifeTime( 0.5f );
		//m_pGlowTrail->TurnOff();
	}

	// Make us glow until we've hit the wall
	m_nSkin = BOLT_SKIN_GLOW;
}


void CFlowerBolt::Precache( void )
{
	PrecacheModel( BOLT_MODEL );

	// This is used by C_TEStickyBolt, despte being different from above!!!
	PrecacheModel( "models/crossbow_bolt.mdl" );
	PrecacheModel("sprites/grenade/grenade_laser.vmt");

	PrecacheModel( "sprites/light_glow02_noz.vmt" );
}

void CFlowerBolt::BoltTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsSolid() || pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS))
		return;

	if(pOther->GetCollisionGroup() == COLLISION_GROUP_WEAPON && !FClassnameIs(pOther, "weapon_pushball")) // Do collide with pushballs
		return;

	if ( pOther->m_takedamage != DAMAGE_NO && pOther->GetMoveType() != MOVETYPE_PUSH )
	{
		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector	vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize( vecNormalizedVel );

		if( GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC() )
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_iDamage, DMG_NEVERGIB );
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}
		else
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_iDamage, DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}

		ApplyMultiDamage();

		//Adrian: keep going through the glass.
		if ( pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS )
			 return;

		SetAbsVelocity( Vector( 0, 0, 0 ) );

		// play body "thwack" sound
		EmitSound( "Weapon_Crossbow.BoltHitBody" );

		Vector vForward;

		AngleVectors( GetAbsAngles(), &vForward );
		VectorNormalize ( vForward );

		UTIL_TraceLine( GetAbsOrigin(),	GetAbsOrigin() + vForward * 128, MASK_OPAQUE, pOther, COLLISION_GROUP_NONE, &tr2 );

		if ( tr2.fraction != 1.0f )
		{
//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

			if ( tr2.m_pEnt == NULL || ( tr2.m_pEnt && tr2.m_pEnt->GetMoveType() == MOVETYPE_NONE ) )
			{
				CEffectData	data;

				data.m_vOrigin = tr2.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = tr2.fraction != 1.0f;
			
				DispatchEffect( "BoltImpact", data );
			}
		}
		
		SetTouch( NULL );
		SetThink( NULL );

		UTIL_Remove( this );
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if ( (pOther->GetMoveType() == MOVETYPE_NONE || pOther->GetMoveType() == MOVETYPE_PUSH )&& !( tr.surface.flags & SURF_SKY ) )
		{
			EmitSound( "Weapon_flowerLauncher.BoltHitWorld" );

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();
			float speed = VectorNormalize( vecDir );

			// See if we should reflect off this surface
			float hitDot = DotProduct( tr.plane.normal, -vecDir );
			
			if (( ( hitDot < 0.8f ) && ( speed > 100 ) )) //|| pOther->GetCollisionGroup() == COLLISION_GROUP_WEAPON) //on se laisse passer a travers les armes
			{
				Vector vReflection = 2.0f * tr.plane.normal * hitDot + vecDir;
				
				QAngle reflectAngles;

				VectorAngles( vReflection, reflectAngles );

				SetLocalAngles( reflectAngles );

				SetAbsVelocity( vReflection * speed * 0.75f );

				// Start to sink faster
				SetGravity( 1.3f );
			}
			else
			{
				SetThink( &CFlowerBolt::SUB_Remove );
				SetNextThink( gpGlobals->curtime + 2.0f );
				
				//FIXME: We actually want to stick (with hierarchy) to what we've hit
				SetMoveType( MOVETYPE_NONE );
			
				Vector vForward;

				AngleVectors( GetAbsAngles(), &vForward );
				VectorNormalize ( vForward );

				CEffectData	data;

				data.m_vOrigin = tr.endpos;
				data.m_vNormal = vForward;
				data.m_nEntIndex = 0;
				data.m_nMaterial = 0; // Propriété inutilisée, on l'utilisera pour savoir quelle skin utiliser
			
				DispatchEffect( "BoltImpact", data );
				
				UTIL_ImpactTrace( &tr, DMG_BULLET );

				AddEffects( EF_NODRAW );
				SetTouch( NULL );
				SetThink( &CFlowerBolt::SUB_Remove );
				SetNextThink( gpGlobals->curtime + 2.0f );
			}
			
			// Shoot some sparks
			if ( UTIL_PointContents( GetAbsOrigin() ) != CONTENTS_WATER)
			{
				g_pEffects->Sparks( GetAbsOrigin() );
			}
		}
		else
		{
			// Put a mark unless we've hit the sky
			if ( ( tr.surface.flags & SURF_SKY ) == false )
			{
				UTIL_ImpactTrace( &tr, DMG_BULLET );
			}

			UTIL_Remove( this );
		}
	}

	if ( g_pGameRules->IsMultiplayer() )
	{
//		SetThink( &CFlowerBolt::ExplodeThink );
//		SetNextThink( gpGlobals->curtime + 0.1f );
	}
}

void CFlowerBolt::BubbleThink( void )
{
	QAngle angNewAngles;
	VectorAngles( GetAbsVelocity(), angNewAngles );
	SetAbsAngles( angNewAngles );

	SetNextThink( gpGlobals->curtime + 0.1f );

	if ( GetWaterLevel()  == 0 )
		return;

	UTIL_BubbleTrail( GetAbsOrigin() - GetAbsVelocity() * 0.1f, GetAbsOrigin(), 5 );
}

#endif

#ifdef CLIENT_DLL
#define CWeaponFlowerLauncher C_WeaponFlowerLauncher
#endif

class CWeaponFlowerLauncher : public CBaseHL2MPCombatWeapon
{
	DECLARE_CLASS( CWeaponFlowerLauncher, CBaseHL2MPCombatWeapon );
public:

	CWeaponFlowerLauncher( void );
	
	virtual void	Precache( void );
	virtual void	PrimaryAttack( void );
	virtual void	SecondaryAttack( void );
	virtual bool	Deploy( void );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	virtual bool	Reload( void );
	virtual void	ItemPostFrame( void );
	virtual void	ItemBusyFrame( void );
	virtual bool	SendWeaponAnim( int iActivity );

#ifndef CLIENT_DLL
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
#endif

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

private:
	
	void	SetSkin( int skinNum );
	void	FireBolt( void );
	
	DECLARE_ACTTABLE();

private:

	CNetworkVar( bool,	m_bInZoom );
	CNetworkVar( bool,	m_bMustReload );

	CWeaponFlowerLauncher( const CWeaponFlowerLauncher & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponFlowerLauncher, DT_WeaponFlowerLauncher )

BEGIN_NETWORK_TABLE( CWeaponFlowerLauncher, DT_WeaponFlowerLauncher )
#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_bMustReload ) ),
#else
	SendPropBool( SENDINFO( m_bMustReload ) ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponFlowerLauncher )
	DEFINE_PRED_FIELD( m_bMustReload, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_flowerlauncher, CWeaponFlowerLauncher );

PRECACHE_WEAPON_REGISTER( weapon_flowerlauncher );

acttable_t	CWeaponFlowerLauncher::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_CROSSBOW,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_CROSSBOW,				false },
	{ ACT_MP_RUN,						ACT_HL2MP_RUN_CROSSBOW,						false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_CROSSBOW,				false },
	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,	false },
	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,			false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW,			false },
	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_CROSSBOW,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_CROSSBOW,					false },
};

IMPLEMENT_ACTTABLE(CWeaponFlowerLauncher);

CWeaponFlowerLauncher::CWeaponFlowerLauncher( void )
{
	m_bReloadsSingly	= true;
	m_bFiresUnderwater	= true;
	m_bMustReload		= false;
}

void CWeaponFlowerLauncher::Precache( void )
{
#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "flowerlauncher_bolt" );
#endif

	PrecacheScriptSound( "Weapon_Crossbow.BoltHitBody" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltHitWorld" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltSkewer" );

	BaseClass::Precache();
}

void CWeaponFlowerLauncher::PrimaryAttack( void )
{
	FireBolt();

	m_bMustReload = true;

	SetWeaponIdleTime( gpGlobals->curtime + SequenceDuration( ACT_VM_PRIMARYATTACK ) );
}

void CWeaponFlowerLauncher::SecondaryAttack( void )
{
	//NOTE: The zooming is handled by the post/busy frames
}

bool CWeaponFlowerLauncher::Reload( void )
{
	if ( BaseClass::Reload() )
	{
		m_bMustReload = false;
		return true;
	}

	return false;
}

void CWeaponFlowerLauncher::ItemBusyFrame( void )
{
}

void CWeaponFlowerLauncher::ItemPostFrame( void )
{
	if ( m_bMustReload && HasWeaponIdleTimeElapsed() )
		Reload();

	BaseClass::ItemPostFrame();
}

void CWeaponFlowerLauncher::FireBolt( void )
{
	if ( m_iClip1 <= 0 )
	{
		if ( !m_bFireOnEmpty )
			Reload();
		else
		{
			WeaponSound( EMPTY );
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.15f;
		}

		return;
	}

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );	
	if ( pOwner == NULL )
		return;

#ifndef CLIENT_DLL
/*	Vector vecSrc = pOwner->Weapon_ShootPosition();
	Vector vecAiming = pOwner->GetAutoaimVector( 0 );	
	QAngle angAiming;

	VectorAngles(vecAiming, angAiming);
	angAiming.x += ((rand() % 100) / 100.0) * (rand() % 2 == 1 ? -1 : 1);
	angAiming.y += ((rand() % 100) / 100.0) * (rand() % 2 == 1 ? -1 : 1);
	AngleVectors(angAiming, &vecAiming);

	CFlowerBolt *pBolt = CFlowerBolt::BoltCreate( vecSrc, angAiming, GetHL2MPWpnData().m_iPlayerDamage, pOwner );*/
	Vector vecAiming	= pOwner->GetAutoaimVector( 0 );	
	Vector vecSrc		= pOwner->Weapon_ShootPosition();
	Vector	vForward, vRight, vUp;
	pOwner->EyeVectors( &vForward, &vRight, &vUp );

	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );

	CFlowerBolt *pBolt = CFlowerBolt::BoltCreate( vecSrc + vForward * 12.0f + vRight * 6.0f + vUp * -3.4f, angAiming, GetHL2MPWpnData().m_iPlayerDamage, pOwner );
	pBolt->SetAbsVelocity( vecAiming * BOLT_VELOCITY );
#endif

	m_iClip1--;
	pOwner->ViewPunch( QAngle( -2, 0, 0 ) );
	WeaponSound( SINGLE );
	WeaponSound( SPECIAL2 );

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	m_flNextPrimaryAttack = m_flNextSecondaryAttack	= gpGlobals->curtime + 0.75f;
}

bool CWeaponFlowerLauncher::Deploy( void )
{
	if ( m_iClip1 <= 0 )
		return Reload();
		//return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_CROSSBOW_DRAW_UNLOADED, (char*)GetAnimPrefix() );

	return BaseClass::Deploy();
}

bool CWeaponFlowerLauncher::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	return BaseClass::Holster( pSwitchingTo );
}

void CWeaponFlowerLauncher::SetSkin( int skinNum )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner == NULL )
		return;

	CBaseViewModel *pViewModel = pOwner->GetViewModel();
	if ( pViewModel == NULL )
		return;

	pViewModel->m_nSkin = skinNum;
}

#ifndef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pEvent - 
//			*pOperator - 
//-----------------------------------------------------------------------------
void CWeaponFlowerLauncher::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
}
#endif

bool CWeaponFlowerLauncher::SendWeaponAnim( int iActivity )
{
	int newActivity = iActivity;

	// The last shot needs a non-loaded activity
	if ( ( newActivity == ACT_VM_IDLE ) && ( m_iClip1 <= 0 ) )
		newActivity = ACT_VM_FIDGET;

	//For now, just set the ideal activity and be done with it
	return BaseClass::SendWeaponAnim( newActivity );
}
