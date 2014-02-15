// BISOUNOURS PARTY : GRENADE (npc)

#include "cbase.h"
#include "basegrenade_shared.h"
#include "grenade_boing.h"
#include "Sprite.h"
#include "SpriteTrail.h"
#include "soundent.h"
#include "hl2mp_gamerules.h"
#include "autovocals.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const float GRENADE_COEFFICIENT_OF_RESTITUTION = 2.0f;

#define GRENADE_EXPLOSION_RADIUS 180
#define GRENADE_MODEL "models/Weapons/w_grenade.mdl"

class CGrenadeBoing : public CBaseGrenade
{
	DECLARE_CLASS( CGrenadeBoing, CBaseGrenade );

#if !defined( CLIENT_DLL )
	DECLARE_DATADESC();
#endif
					
	~CGrenadeBoing( void );

public:
	void	Spawn( void );
	void	OnRestore( void );
	void	Precache( void );
	bool	CreateVPhysics( void );
	void	CreateEffects( void );
	void	SetTimer( float detonateDelay, float warnDelay );
	void	SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity );
	void	DelayThink();
	void	VPhysicsUpdate( IPhysicsObject *pPhysics );

protected:
	CHandle<CSprite>		m_pMainGlow;
	CHandle<CSpriteTrail>	m_pGlowTrail;

	bool m_inSolid;
};

LINK_ENTITY_TO_CLASS( grenade_boing, CGrenadeBoing );

BEGIN_DATADESC( CGrenadeBoing )
	// Fields
	DEFINE_FIELD( m_pMainGlow, FIELD_EHANDLE ),
	DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),
	DEFINE_FIELD( m_inSolid, FIELD_BOOLEAN ),
	
	// Function Pointers
	DEFINE_THINKFUNC( DelayThink ),
END_DATADESC()

CGrenadeBoing::~CGrenadeBoing( void )
{
}

void CGrenadeBoing::Spawn( void )
{
	Precache( );
	
	SetModel( GRENADE_MODEL );

	SetSize( -Vector(4,4,4), Vector(4,4,4) );
	SetCollisionGroup( COLLISION_GROUP_WEAPON );
	CreateVPhysics();
	AddSolidFlags( FSOLID_NOT_STANDABLE );

	m_DmgRadius	 = GRENADE_EXPLOSION_RADIUS;

	BaseClass::Spawn();
}

void CGrenadeBoing::OnRestore( void )
{
	// If we were primed and ready to detonate, put FX on us.
	if (m_flDetonateTime > 0)
		CreateEffects();

	BaseClass::OnRestore();
}

void CGrenadeBoing::CreateEffects( void )
{
	// Start up the eye glow
	//m_pMainGlow = CSprite::SpriteCreate( "sprites/grenade/grenade_glow.vmt", GetLocalOrigin(), false );

	int	nAttachment = LookupAttachment( "fuse" );

	if ( m_pMainGlow != NULL )
	{
		m_pMainGlow->FollowEntity( this );
		m_pMainGlow->SetAttachment( this, nAttachment );
		m_pMainGlow->SetTransparency( kRenderGlow, 255, 255, 255, 200, kRenderFxNoDissipation );
		m_pMainGlow->SetScale( 0.2f );
		m_pMainGlow->SetGlowProxySize( 4.0f );
	}
	

	// Start up the eye trail
	m_pGlowTrail	= CSpriteTrail::SpriteTrailCreate( "sprites/grenade/grenade_laser.vmt", GetLocalOrigin(), false );

	if ( m_pGlowTrail != NULL )
	{
		m_pGlowTrail->FollowEntity( this );
		m_pGlowTrail->SetAttachment( this, nAttachment );

		CBasePlayer *pPlayer = ToBasePlayer(GetOwnerEntity());
		if(pPlayer != NULL && HL2MPRules()->IsTeamplay())
		{
			if(pPlayer->GetTeamNumber() == TEAM_PINK)
				m_pGlowTrail->SetTransparency( kRenderTransAdd, 255, 50, 255, 70, kRenderFxNone );
			else
				m_pGlowTrail->SetTransparency( kRenderTransAdd, 0, 255, 0, 70, kRenderFxNone );
		}
		else
				m_pGlowTrail->SetTransparency( kRenderTransAdd, 255, 255, 255, 70, kRenderFxNone );

		m_pGlowTrail->SetStartWidth( 8.0f );
		m_pGlowTrail->SetEndWidth( 1.0f );
		m_pGlowTrail->SetLifeTime( 0.5f );
	}
}

bool CGrenadeBoing::CreateVPhysics()
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, 0, false );
	return true;
}

// this will hit only things that are in newCollisionGroup, but NOT in collisionGroupAlreadyChecked
class CTraceFilterCollisionGroupDelta : public CTraceFilterEntitiesOnly
{
public:
	// It does have a base, but we'll never network anything below here..
	DECLARE_CLASS_NOBASE( CTraceFilterCollisionGroupDelta );
	
	CTraceFilterCollisionGroupDelta( const IHandleEntity *passentity, int collisionGroupAlreadyChecked, int newCollisionGroup )
		: m_pPassEnt(passentity), m_collisionGroupAlreadyChecked( collisionGroupAlreadyChecked ), m_newCollisionGroup( newCollisionGroup )
	{
	}
	
	virtual bool ShouldHitEntity( IHandleEntity *pHandleEntity, int contentsMask )
	{
		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt ) )
			return false;
		CBaseEntity *pEntity = EntityFromEntityHandle( pHandleEntity );

		if ( pEntity )
		{
			if ( g_pGameRules->ShouldCollide( m_collisionGroupAlreadyChecked, pEntity->GetCollisionGroup() ) )
				return false;
			if ( g_pGameRules->ShouldCollide( m_newCollisionGroup, pEntity->GetCollisionGroup() ) )
				return true;
		}

		return false;
	}

protected:
	const IHandleEntity *m_pPassEnt;
	int	m_collisionGroupAlreadyChecked;
	int	m_newCollisionGroup;
};

void CGrenadeBoing::VPhysicsUpdate( IPhysicsObject *pPhysics )
{
	BaseClass::VPhysicsUpdate( pPhysics );
	Vector vel;
	AngularImpulse angVel;
	pPhysics->GetVelocity( &vel, &angVel );
	
	Vector start = GetAbsOrigin();
	// find all entities that my collision group wouldn't hit, but COLLISION_GROUP_NONE would and bounce off of them as a ray cast
	CTraceFilterCollisionGroupDelta filter( this, GetCollisionGroup(), COLLISION_GROUP_PLAYER );
	trace_t tr;

	// UNDONE: Hull won't work with hitboxes - hits outer hull.  But the whole point of this test is to hit hitboxes.
#if 0
	UTIL_TraceHull( start, start + vel * gpGlobals->frametime, CollisionProp()->OBBMins(), CollisionProp()->OBBMaxs(), CONTENTS_HITBOX|CONTENTS_MONSTER|CONTENTS_SOLID, &filter, &tr );
#else
	UTIL_TraceLine( start, start + vel * gpGlobals->frametime, CONTENTS_HITBOX|CONTENTS_MONSTER|CONTENTS_SOLID, &filter, &tr );
#endif

	if ( tr.startsolid )
	{
		if ( !m_inSolid )
		{
			// UNDONE: Do a better contact solution that uses relative velocity?
			vel *= -GRENADE_COEFFICIENT_OF_RESTITUTION; // bounce backwards
			pPhysics->SetVelocity( &vel, NULL );
		}
		m_inSolid = true;
		return;
	}
	m_inSolid = false;
	if ( tr.DidHit() )
	{
		Vector dir = vel;
		VectorNormalize(dir);
		// send a tiny amount of damage so the character will react to getting bonked
		CTakeDamageInfo info( this, GetThrower(), pPhysics->GetMass() * vel, GetAbsOrigin(), 0.1f, DMG_CRUSH );
		tr.m_pEnt->TakeDamage( info );
		CBasePlayer *pVictim = ToBasePlayer(tr.m_pEnt);
		Msg(tr.m_pEnt->GetClassname());
		if(pVictim != NULL)
		{
			Detonate();
		}
		if(tr.m_pEnt->IsNPC() || tr.m_pEnt->IsPlayerSimulated() )
		{
			Detonate();
		}
		// reflect velocity around normal
		vel = -2.0f * tr.plane.normal * DotProduct(vel,tr.plane.normal) + vel;
		
		// absorb 80% in impact
		vel *= GRENADE_COEFFICIENT_OF_RESTITUTION;
		angVel *= -0.5f;
		pPhysics->SetVelocity( &vel, &angVel );

		//BP Autovocal
		CBasePlayer *pPlayer = ToBasePlayer(GetOwnerEntity());
		//CBasePlayer *pVictim = ToBasePlayer(tr.m_pEnt);
		if(pPlayer == NULL)
			return;
		if(pVictim == NULL)
			pPlayer->PlayAutovocal(GRENADEHIT, 0.5f);
		else if(!((pPlayer->GetTeamNumber() == pVictim->GetTeamNumber()) && HL2MPRules()->IsTeamplay())) // Si c'est pas un TK
				pPlayer->PlayAutovocal(GRENADEHIT, 0.5f);
	}
}

void CGrenadeBoing::Precache( void )
{
	PrecacheModel( GRENADE_MODEL );

	PrecacheModel( "sprites/grenade/grenade_glow.vmt" );
	m_pMainGlow = CSprite::SpriteCreate( "sprites/grenade/grenade_glow.vmt", GetLocalOrigin(), false );
	PrecacheModel( "sprites/grenade/grenade_laser.vmt" );

	BaseClass::Precache();
}

void CGrenadeBoing::SetTimer( float detonateDelay, float warnDelay )
{
	m_flDetonateTime = gpGlobals->curtime + detonateDelay;
	m_flWarnAITime = gpGlobals->curtime + warnDelay;
	SetThink( &CGrenadeBoing::DelayThink );
	SetNextThink( gpGlobals->curtime );

	CreateEffects();
}

void CGrenadeBoing::DelayThink() 
{
	if( gpGlobals->curtime > m_flDetonateTime )
	{
		Detonate();
		return;
	}

	SetNextThink( gpGlobals->curtime + 0.1 );
}

void CGrenadeBoing::SetVelocity( const Vector &velocity, const AngularImpulse &angVelocity )
{
	IPhysicsObject *pPhysicsObject = VPhysicsGetObject();
	if ( pPhysicsObject )
		pPhysicsObject->AddVelocity( &velocity, &angVelocity );
}

CBaseGrenade *GrenadeBoing_Create( const Vector &position, const QAngle &angles, const Vector &velocity, const AngularImpulse &angVelocity, CBaseEntity *pOwner, float timer, bool combineSpawned )
{
	// Don't set the owner here, or the player can't interact with grenades he's thrown
	CGrenadeBoing *pGrenade = (CGrenadeBoing *)CBaseEntity::Create( "grenade_boing", position, angles, pOwner );
	
	pGrenade->SetTimer( timer, timer - 1.5f );
	pGrenade->SetVelocity( velocity, angVelocity );
	pGrenade->SetThrower( ToBaseCombatCharacter( pOwner ) );
	pGrenade->m_takedamage = DAMAGE_NO;

	return pGrenade;
}