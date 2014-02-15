// BISOUNOURS PARTY : MINE (npc)

#include "cbase.h"
#include "shake.h"
#include "npc_mine.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "explode.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define MINE_DAMAGE 200
#define MINE_EXPLOSION_RADIUS 200

#define MINE_DELAY_POWERUP 2.0f
#define MINE_DELAY_DEATHWARNING 0.3f
#define MINE_DELAY_SKINCHANGE_DETECTION 1.0f

LINK_ENTITY_TO_CLASS( npc_mine, CNPCMine );

BEGIN_DATADESC( CNPCMine )
	DEFINE_FIELD( m_hOwner,		FIELD_EHANDLE ),
	DEFINE_FIELD( m_flPowerUp,	FIELD_TIME ),
	DEFINE_FIELD( m_flNextBeep,	FIELD_TIME ),
	DEFINE_FIELD( m_vecDir,		FIELD_VECTOR ),
	DEFINE_FIELD( m_vecEnd,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_posOwner,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_angleOwner,	FIELD_VECTOR ),
	DEFINE_FIELD( m_nSkin, FIELD_INTEGER),

	DEFINE_THINKFUNC( PowerupThink ),
	DEFINE_THINKFUNC( DetectionThink ),
	DEFINE_THINKFUNC( DelayDeathThink ),
END_DATADESC()

CNPCMine::CNPCMine()
{
	m_vecDir.Init();
	m_vecEnd.Init();
	m_posOwner.Init();
	m_angleOwner.Init();

	m_pConstraint = NULL;
	m_bAttached = false;
	m_hAttachEntity = NULL;
}

CNPCMine::~CNPCMine( void )
{
	if (m_pConstraint)
	{
		physenv->DestroyConstraint(m_pConstraint);
		m_pConstraint = NULL;
	}
}
void CNPCMine::Spawn( void )
{
	Precache( );

	SetMoveType( MOVETYPE_FLY );
	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_SOLID );
	SetModel( "models/Weapons/w_slam.mdl" );

	SetCollisionGroup( COLLISION_GROUP_WEAPON );

	SetCycle( 0.0f );
	m_nBody	= 2;
	m_flDamage = 100.0f;
	m_DmgRadius	= 50.0f;

	ResetSequenceInfo( );
	m_flPlaybackRate	= 0;
	
	UTIL_SetSize(this, Vector( -4, -4, -2), Vector(4, 4, 2));

	m_flPowerUp = gpGlobals->curtime + MINE_DELAY_POWERUP;
	m_flNextBeep = 0.0f;
	
	SetThink( &CNPCMine::PowerupThink );
	SetNextThink( gpGlobals->curtime + 0.2f );

	m_takedamage = DAMAGE_NO;

	m_iHealth = 1;

	EmitSound( "Weapon_Mine.Place" );
	SetDamage ( MINE_DAMAGE );

	QAngle angles = GetAbsAngles();
	angles.x -= 90;
	AngleVectors( angles, &m_vecDir );
	m_vecEnd = GetAbsOrigin() + m_vecDir * 2048;

	//On déclare la mine "live" dés qu'on la pose au cas où le joueur mourirait avant les 2s de powerup
	m_bIsLive = true;

	AddEffects( EF_NOSHADOW );
}

void CNPCMine::Precache( void )
{
	PrecacheModel("models/Weapons/w_slam.mdl"); 

	PrecacheScriptSound( "Weapon_Mine.Place" );
	PrecacheScriptSound( "Weapon_Mine.Activate" );
	PrecacheScriptSound( "Weapon_Mine.Detonate" );
}

void CNPCMine::PowerupThink( void  )
{
	if (gpGlobals->curtime > m_flPowerUp)
	{
		RemoveSolidFlags( FSOLID_NOT_SOLID );
		m_takedamage = DAMAGE_YES;

		//On enregistre la position de la mine
		if ( m_bAttached && m_hAttachEntity.Get() != NULL )
			m_vAttachedPosition = m_hAttachEntity.Get()->GetAbsOrigin();

		SetThink(&CNPCMine::DetectionThink);

		EmitSound( "Weapon_Mine.Activate" );
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CNPCMine::DetectionThink(void){
	if(gpGlobals->curtime > m_flNextBeep)
	{
		if((m_nSkin % 2) == 0)
			m_nSkin++;
		else
			m_nSkin--;

		m_flNextBeep = gpGlobals->curtime + MINE_DELAY_SKINCHANGE_DETECTION;
	}

	//On détecte si des entités susceptibles d'activer la mine sont dans la sphère de détection
	trace_t tr;
	CBaseEntity *pEntities[20];
	int iNumEntities = UTIL_EntitiesInSphere( pEntities, 20, GetAbsOrigin(), 150.0f, MASK_SHOT);
	for ( int i = 0; i < iNumEntities; i++ )
	{
		CBaseEntity *pSphereEntity = pEntities[i];
		if (!pSphereEntity)
			continue;

		CBaseCombatCharacter *pBCC  = ToBaseCombatCharacter( pSphereEntity );
		if(ShouldExplode(pBCC))
		{
			trace_t ptr;
			UTIL_TraceLine( GetAbsOrigin(), pBCC->GetAbsOrigin(), MASK_SOLID, this, COLLISION_GROUP_NONE, &ptr );
			if(ptr.fraction >= 1.0f)
			{
				Event_Killed( CTakeDamageInfo( (CBaseEntity*)m_hOwner, this, 100, GIB_NORMAL ) );
				return;
			}
		}
	}

	//On explose si le support de la mine est déplacé (caisse par exemple)
	if ( m_bAttached && m_hAttachEntity.Get() != NULL )
		if ( m_hAttachEntity.Get()->GetAbsOrigin() != m_vAttachedPosition )
			Explode();

	SetNextThink( gpGlobals->curtime + 0.05f );
}

bool CNPCMine::ShouldExplode(CBaseCombatCharacter *pBCC)
{
	if(pBCC == NULL)
		return false;

	if(!pBCC->IsNPC() && !pBCC->IsPlayer() && !pBCC->IsAlive())
		return false;

	CBasePlayer *pPlayer = ToBasePlayer(pBCC);
	CBasePlayer *pOwner = ToBasePlayer( m_hOwner.Get() );
	if(pPlayer == NULL || pOwner == NULL)
		return true;
	else
	{
		if(pPlayer == pOwner)
			return false;
		else
		{
			if(HL2MPRules()->GetGameType() == GAME_COOP)
				return false;
			if((pPlayer->GetTeamNumber() == pOwner->GetTeamNumber()) && pPlayer->GetTeamNumber() >= 2)
				return false;
			else
				return true;
		}
	}
}

void CNPCMine::Event_Killed( const CTakeDamageInfo &info )
{
	if (m_pConstraint)
		m_pConstraint->Deactivate();

	SetThink( &CNPCMine::DelayDeathThink );
	SetNextThink( gpGlobals->curtime + 0.1 );
	m_flNextBeep = gpGlobals->curtime + MINE_DELAY_DEATHWARNING;
	if(info.GetMaxDamage() != 200) // Maxdamage est a 200 quand la mine disparait, donc après son temps de détonation
		EmitSound( "Weapon_Mine.Detonate" );
}

void CNPCMine::DelayDeathThink( void )
{
	if(gpGlobals->curtime < m_flNextBeep)
	{
		if((m_nSkin % 2) == 0)
			m_nSkin++;
		else
			m_nSkin--;

		SetNextThink(gpGlobals->curtime + 0.1f);

		return;
	}

	Explode();
}

void CNPCMine::Explode( void ){
	UTIL_ScreenShake( GetAbsOrigin(), 25.0, 150.0, 1.0, 750, SHAKE_START );

	ExplosionCreate( GetAbsOrigin() + m_vecDir * 8, GetAbsAngles(), m_hOwner, GetDamage(), MINE_EXPLOSION_RADIUS, SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE, 0.0f, this);

	UTIL_Remove( this );
}

bool CNPCMine::MakeConstraint( CBaseEntity *pObject )
{
	IPhysicsObject *cMinePhysics = VPhysicsGetObject();

	Assert( cMinePhysics );

	IPhysicsObject *pAttached = pObject->VPhysicsGetObject();
	if ( !cMinePhysics || !pAttached )
		return false;

	// constraining to the world means object 1 is fixed
	if ( pAttached == g_PhysWorldObject )
		PhysSetGameFlags( cMinePhysics, FVPHYSICS_CONSTRAINT_STATIC );

	IPhysicsConstraintGroup *pGroup = NULL;

	constraint_fixedparams_t fixed;
	fixed.Defaults();
	fixed.InitWithCurrentObjectState( cMinePhysics, pAttached );
	fixed.constraint.Defaults();

	m_pConstraint = physenv->CreateFixedConstraint( cMinePhysics, pAttached, pGroup, fixed );
	
	if (!m_pConstraint)
		return false;

	m_pConstraint->SetGameData( (void *)this );

	return true;
}

void CNPCMine::AttachToEntity(CBaseEntity *pOther)
{
	if (!pOther)
		return;

	if ( !VPhysicsGetObject() )
		return;

	m_bAttached			= true;
	m_hAttachEntity		= pOther;

	SetMoveType			( MOVETYPE_NONE );

	if (pOther->GetSolid() == SOLID_VPHYSICS && pOther->VPhysicsGetObject() != NULL )
	{
		SetSolid(SOLID_BBOX); //Tony; switch to bbox solid instead of vphysics, because we've made the physics object non-solid
		MakeConstraint(pOther);
		SetMoveType	( MOVETYPE_VPHYSICS ); // use vphysics while constrained!!
	}
	//if it isnt vphysics or bsp, use SetParent to follow it.
	else if (pOther->GetSolid() != SOLID_BSP)
	{
		SetSolid(SOLID_BBOX); //Tony; switch to bbox solid instead of vphysics, because we've made the physics object non-solid
		SetParent( m_hAttachEntity.Get() );
	}
}