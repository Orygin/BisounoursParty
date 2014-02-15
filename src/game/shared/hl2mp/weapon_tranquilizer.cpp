//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "input.h"
	#include "model_types.h"
#else
	#include "hl2mp_player.h"
	#include "te_effect_dispatch.h"
	#include "IEffects.h"
	#include "Sprite.h"
	#include "SpriteTrail.h"
	#include "beam_shared.h"
	#include "func_break.h"
	#include "autovocals.h"
	#include "Sprite.h"
	#include "beam_shared.h"
#include "particle_parse.h"
#endif

#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "weapon_rpg.h"
#include "effect_dispatch_data.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BOLT_MODEL			"models/Weapons/flechette.mdl"

#define BOLT_AIR_VELOCITY	3100
#define BOLT_WATER_VELOCITY	750

#define FIRE_DELAY 0.8f
#define HEAL_AMOUNT 45.0f

#ifndef CLIENT_DLL

extern ConVar sk_plr_dmg_flechette;
extern ConVar sk_npc_dmg_flechette;

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------

class CFlechette : public CBaseCombatCharacter
{
public:
	DECLARE_CLASS( CFlechette, CBaseCombatCharacter );
	DECLARE_DATADESC();

	Class_T Classify( void ) { return CLASS_NONE; }
	CFlechette( void ) { m_flFirstCallTime = 0; }
	void Spawn( void );
	void Precache( void );
	void BubbleThink( void );
	void RemoveMeThink( void );
	void BoltTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CFlechette *BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner = NULL );
	void DoAOEDamage();

protected:

	int		m_iDamage;
	float	m_flFirstCallTime;

	CHandle<CSpriteTrail>	m_pGlowTrail;
};


LINK_ENTITY_TO_CLASS( flechette, CFlechette );

BEGIN_DATADESC( CFlechette )
	// Function Pointers
	DEFINE_FUNCTION( BubbleThink ),
	DEFINE_FUNCTION( BoltTouch ),
	DEFINE_FUNCTION( RemoveMeThink ),
	DEFINE_FIELD( m_flFirstCallTime, FIELD_FLOAT ),
	DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),
END_DATADESC()

CFlechette *CFlechette::BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, int iDamage, CBasePlayer *pentOwner )
{
	// Create a new entity with CFlechette private data
	CFlechette *pBolt = (CFlechette *)CreateEntityByName( "flechette" );
	UTIL_SetOrigin( pBolt, vecOrigin );
	pBolt->SetAbsAngles( angAngles );
	pBolt->SetCollisionGroup(COLLISION_GROUP_PROJECTILE);
	pBolt->SetOwnerEntity( pentOwner );
	pBolt->Spawn();

	pBolt->m_iDamage = iDamage;

	return pBolt;
}
void CFlechette::RemoveMeThink( void )
{
	if(m_flFirstCallTime == 0)
		m_flFirstCallTime = gpGlobals->curtime;

	if(gpGlobals->curtime < m_flFirstCallTime + 30)
		SetNextThink(gpGlobals->curtime + 5);
	else
	{
		SetThink( NULL );
		SetTouch( NULL );
		m_flFirstCallTime = 0;
		UTIL_Remove(this);
	}
}
//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CFlechette::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, FSOLID_NOT_STANDABLE, false );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CFlechette::PhysicsSolidMaskForEntity() const
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
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlechette::Spawn( void )
{
	Precache( );

	SetModel( BOLT_MODEL );
	SetMoveType( MOVETYPE_FLY, MOVECOLLIDE_FLY_CUSTOM );
	UTIL_SetSize( this, -Vector(1,1,1), Vector(1,1,1) );
	SetSolid( SOLID_BBOX );
	
	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch( &CFlechette::BoltTouch );

	SetThink( &CFlechette::BubbleThink );
	SetNextThink( gpGlobals->curtime + 0.1f );

	CBasePlayer *pPlayer = ToBasePlayer(GetOwnerEntity());
	if(pPlayer != NULL && HL2MPRules()->IsTeamplay())
	{
		if(pPlayer->GetTeamNumber() == TEAM_PINK)
			m_nSkin = 1;
		else if(pPlayer->GetTeamNumber() == TEAM_GREEN)
			m_nSkin = 2;
		else
			m_nSkin = 0;
	}

	m_pGlowTrail = CSpriteTrail::SpriteTrailCreate( "sprites/grenade/grenade_laser.vmt", GetLocalOrigin(), false );
	if ( m_pGlowTrail != NULL )
	{
		m_pGlowTrail->FollowEntity( this );
		if(m_nSkin == 1)
			m_pGlowTrail->SetTransparency( kRenderTransAdd, 255, 50, 255, 70, kRenderFxNone );
		else if(m_nSkin == 2)
			m_pGlowTrail->SetTransparency( kRenderTransAdd, 0, 255, 0, 70, kRenderFxNone );
		else
			m_pGlowTrail->SetTransparency( kRenderTransAdd, 255, 255, 255, 70, kRenderFxNone );
		m_pGlowTrail->SetStartWidth( 3.0f );
		m_pGlowTrail->SetEndWidth( 0.5f );
		m_pGlowTrail->SetLifeTime( 0.4f );
	}
}


void CFlechette::Precache( void )
{
	PrecacheModel( BOLT_MODEL );
	PrecacheScriptSound("Tranqu.Heal");
	PrecacheModel("sprites/grenade/grenade_laser.vmt");
	PrecacheParticleSystem( "gasgrenade_pink" );
	PrecacheParticleSystem( "gasgrenade_green" );
	PrecacheParticleSystem( "gasgrenade_red" );
}
ConVar flechette_radius("bp_flechette_radius","60");
void CFlechette::DoAOEDamage()
{
	CBaseEntity *ppEnts[256];
	Vector vecCenter = WorldSpaceCenter();
	 
	float flRadius = flechette_radius.GetFloat();
	vecCenter.z -= flRadius * 0.8f;
	int nEntCount = UTIL_EntitiesInSphere( ppEnts, 256, vecCenter, flRadius, 0 );
	int i;
	for ( i = 0; i < nEntCount; i++ )
	{
		if ( ppEnts[i] == NULL )
			continue;
		bool bDoDamage = true;
		if(ppEnts[i]->IsPlayer() || ppEnts[i]->IsNPC())
		{
			CBasePlayer *pOtherPlayer  = ToBasePlayer(ppEnts[i]);
			CBasePlayer *pPlayer  = ToBasePlayer(GetOwnerEntity());
			if(pOtherPlayer != NULL && pPlayer != NULL)
			{
				if((HL2MPRules()->IsTeamplay() && (pPlayer->GetTeamNumber() == pOtherPlayer->GetTeamNumber()))
					|| HL2MPRules()->GetGameType() == GAME_COOP)
				{
					const int oldHealth = pOtherPlayer->GetHealth();
					pOtherPlayer->TakeHealth( HEAL_AMOUNT, DMG_GENERIC );
					bDoDamage = false;
					if(oldHealth <= HEAL_AMOUNT)
						pOtherPlayer->PlayAutovocal(HEALED,0);
					if(oldHealth != pOtherPlayer->GetHealth()) // Si on a vraiment heal
						pOtherPlayer->EmitSound("Tranqu.Heal");
				}
			}
			if(bDoDamage && ppEnts[i] != GetOwnerEntity())
			{
				int iApplyDamage = 0;
				if(ppEnts[i]->IsNPC())
					iApplyDamage = 70;
				else
				{
					if(ppEnts[i]->GetHealth() < m_iDamage + 10)
						iApplyDamage = 100;
					else
						iApplyDamage = ppEnts[i]->GetHealth() - m_iDamage;
				}

				CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), GetOwnerEntity(), iApplyDamage, DMG_POISON | DMG_NEVERGIB );
				//CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
				dmgInfo.SetDamagePosition( vecCenter );
				ppEnts[i]->TakeDamage(dmgInfo);
				//ppEnts[i]->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
			}
		}

		ApplyMultiDamage();

	}
}
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CFlechette::BoltTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsSolid() || pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS) || pOther->GetCollisionGroup() == COLLISION_GROUP_WEAPON )
		return;

	DoAOEDamage();

		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if ( !( tr.surface.flags & SURF_SKY ) )
		{
			if( (pOther->GetMoveType() == MOVETYPE_NONE || pOther->GetMoveType() == MOVETYPE_PUSH ) && !pOther->IsPlayer() && !pOther->IsNPC() )
			EmitSound( "Weapon_Crossbow.BoltHitWorld" );

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();

			SetThink( &CFlechette::SUB_Remove );
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
			data.m_nMaterial = m_nSkin; // Propriété inutilisée, on l'utilisera pour savoir quelle skin utiliser

			if(!pOther->IsPlayer() && !pOther->IsNPC())
			{
				DispatchEffect( "FlechetteImpact", data );
			}

			if(HL2MPRules()->IsTeamplay())
			{
				if(GetOwnerEntity()->GetTeamNumber() == TEAM_PINK)
					DispatchParticleEffect( "gasgrenade_pink", tr.endpos, data.m_vAngles, this );
				else
					DispatchParticleEffect( "gasgrenade_green", tr.endpos, data.m_vAngles, this );
			}
			else
				DispatchParticleEffect( "gasgrenade_red", tr.endpos, data.m_vAngles, this );

			UTIL_ImpactTrace( &tr, DMG_BULLET );

			AddEffects( EF_NODRAW );
			SetTouch( NULL );
			SetThink( &CFlechette::SUB_Remove );
			SetNextThink( gpGlobals->curtime + 2.0f );
			
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
	//}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlechette::BubbleThink( void )
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

//-----------------------------------------------------------------------------
// CWeaponTranquilizer
//-----------------------------------------------------------------------------

#ifdef CLIENT_DLL
#define CWeaponTranquilizer C_WeaponTranquilizer
#endif

class CWeaponTranquilizer : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponTranquilizer, CBaseHL2MPCombatWeapon );

	CWeaponTranquilizer();
	~CWeaponTranquilizer();


	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	
	void	Precache( void );
	void	AddViewKick( void );

	int		GetMinBurst() { return 2; }
	int		GetMaxBurst() { return 5; }

	void	PrimaryAttack( void );
	void	FireBolt( void );
	void	ItemPostFrame( void );
	bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	bool	Deploy( void );

	virtual void Equip( CBaseCombatCharacter *pOwner );
	bool	Reload( void );

	Activity	GetPrimaryAttackActivity( void );

	const WeaponProficiencyInfo_t *GetProficiencyValues();
	
	static void DoMachineGunKick( CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime );
	
	void UpdateLaserPosition( Vector vecMuzzlePos = vec3_origin, Vector vecEndPos = vec3_origin );
	void CreateLaserPointer();

protected:

	Vector	m_vecTossVelocity;
	float	m_flNextGrenadeCheck;
	
private:
	
	DECLARE_ACTTABLE();

	CWeaponTranquilizer( const CWeaponTranquilizer & );
	CNetworkVar( bool,	m_bInZoom );
	CNetworkVar( bool,	m_bLockUnZoom );
#ifndef CLIENT_DLL
	CHandle<CLaserDot>	m_hLaserDot;
#endif
	float	m_fSecondRefire;

	void	CheckZoomToggle( void );
	void	ToggleZoom( void );
	void	Zoom( void );
	void	UnZoom( void );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponTranquilizer, DT_WeaponTranquilizer )

BEGIN_NETWORK_TABLE( CWeaponTranquilizer, DT_WeaponTranquilizer )
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponTranquilizer )
	DEFINE_PRED_FIELD( m_bInZoom, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bLockUnZoom, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_tranquilizer, CWeaponTranquilizer );
PRECACHE_WEAPON_REGISTER(weapon_tranquilizer);


acttable_t	CWeaponTranquilizer::m_acttable[] = 
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

IMPLEMENT_ACTTABLE(CWeaponTranquilizer);


//=========================================================
CWeaponTranquilizer::CWeaponTranquilizer( )
{
	m_fMinRange1		= 0;
	m_fMaxRange1		= 4096;
	m_iPrimaryAmmoType = 7;
}
CWeaponTranquilizer::~CWeaponTranquilizer()
{
#ifndef CLIENT_DLL
	if ( m_hLaserDot != NULL )
	{
		UTIL_Remove( m_hLaserDot );
		m_hLaserDot = NULL;
	}
#endif
}
void CWeaponTranquilizer::ItemPostFrame( void )
{
	CheckZoomToggle();
	UpdateLaserPosition();
	BaseClass::ItemPostFrame();
}
void CWeaponTranquilizer::UpdateLaserPosition( Vector vecMuzzlePos, Vector vecEndPos )
{

#ifndef CLIENT_DLL
	if ( vecMuzzlePos == vec3_origin || vecEndPos == vec3_origin )
	{
		CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
		if ( !pPlayer )
			return;

		vecMuzzlePos = pPlayer->Weapon_ShootPosition();
		Vector	forward;
		pPlayer->EyeVectors( &forward );
		vecEndPos = vecMuzzlePos + ( forward * MAX_TRACE_LENGTH );
	}

	//Move the laser dot, if active
	trace_t	tr;
	
	// Trace out for the endpoint
	UTIL_TraceLine( vecMuzzlePos, vecEndPos, (MASK_SHOT & ~CONTENTS_WINDOW), GetOwner(), COLLISION_GROUP_NONE, &tr );

	// Move the laser sprite
	if ( m_hLaserDot != NULL )
	{
		Vector	laserPos = tr.endpos;
		m_hLaserDot->SetLaserPosition( laserPos, tr.plane.normal );
				
		if ( tr.DidHitNonWorldEntity() )
		{
			CBaseEntity *pHit = tr.m_pEnt;

			if ( ( pHit != NULL ) && ( pHit->m_takedamage ) )
			{
				m_hLaserDot->SetTargetEntity( pHit );
			}
			else
			{
				m_hLaserDot->SetTargetEntity( NULL );
			}
		}
		else
		{
			m_hLaserDot->SetTargetEntity( NULL );
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponTranquilizer::CreateLaserPointer( void )
{
#ifndef CLIENT_DLL
	if ( m_hLaserDot != NULL )
		return;

	CBaseCombatCharacter *pOwner = GetOwner();
	
	if ( pOwner == NULL )
		return;

	if ( pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
		return;

	m_hLaserDot = CLaserDot::Create( GetAbsOrigin(), GetOwner() );
	m_hLaserDot->TurnOff();
	m_hLaserDot->SetRenderMode(kRenderWorldGlow);
	if(HL2MPRules()->IsTeamplay())
	{
		if(pOwner->GetTeamNumber() == TEAM_PINK)
			m_hLaserDot->SetRenderColor(255,100,255,255);
		else
			m_hLaserDot->SetRenderColor(153,255,153,255);
	}
	else
		m_hLaserDot->SetRenderColor(255,0,0,255);

	UpdateLaserPosition();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponTranquilizer::Precache( void )
{
	PrecacheModel( BOLT_MODEL );
	PrecacheScriptSound("Tranqu.Heal");
	BaseClass::Precache();
}
bool CWeaponTranquilizer::Holster( CBaseCombatWeapon *pSwitchingTo)
{
	UnZoom();
#ifndef CLIENT_DLL
	if ( m_hLaserDot != NULL )
	{
		m_hLaserDot->TurnOff();
		UTIL_Remove( m_hLaserDot );
		m_hLaserDot = NULL;
	}
#endif
	return BaseClass::Holster( pSwitchingTo );
}
bool CWeaponTranquilizer::Deploy( void )
{
	UnZoom();
	// Petit bordel pour conserver le m_flNextPrimaryAttack
	// Cette connerie de Deploy le modifie...
	const float tmpNextattack = m_flNextPrimaryAttack;
	const bool reployRet = BaseClass::Deploy();
	m_flNextPrimaryAttack = tmpNextattack;

	CreateLaserPointer();

	// Ca semble pas marcher. On se fait pas chier on recalcule (hyper compliqué)

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.15f;
	return reployRet;
}

void CWeaponTranquilizer::CheckZoomToggle( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer == NULL )
		return;

	if ( pPlayer->m_afButtonPressed & IN_ATTACK2 )
		ToggleZoom();
}
void CWeaponTranquilizer::ToggleZoom( void )
{
	if ( m_bInZoom )
		UnZoom();
	else
		Zoom();
}
void CWeaponTranquilizer::UnZoom( void )
{
#ifndef CLIENT_DLL
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer == NULL )
		return;

	pPlayer->SetFOV( this, 0, 0.0f );
	m_bInZoom = false;
	pPlayer->LockSpeed(false);
	pPlayer->SetMaxSpeed( 275 );
	pPlayer->ShowViewModel(true);
#endif
}
void CWeaponTranquilizer::Zoom( void )
{
#ifndef CLIENT_DLL
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer == NULL )
		return;

	if ( pPlayer->SetFOV( this, 20, 0.1f ) )
	{
		m_bInZoom = true;
		pPlayer->SetMaxSpeed( 80 );
		pPlayer->LockSpeed(true);
		pPlayer->ShowViewModel(false);
	}
#endif
}
//-----------------------------------------------------------------------------
// Purpose: Give this weapon longer range when wielded by an ally NPC.
//-----------------------------------------------------------------------------
void CWeaponTranquilizer::Equip( CBaseCombatCharacter *pOwner )
{
	m_fMaxRange1 = 1400;

	BaseClass::Equip( pOwner );
}
void CWeaponTranquilizer::PrimaryAttack( void )
{
	if(m_flNextPrimaryAttack > gpGlobals->curtime)
		return;

	FireBolt();

	SetWeaponIdleTime( gpGlobals->curtime + FIRE_DELAY );
	
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = m_fSecondRefire = gpGlobals->curtime + FIRE_DELAY;

	#define	EASY_DAMPEN			0.5f
	#define	MAX_VERTICAL_KICK	1.0f	//Degrees
	#define	SLIDE_LIMIT			2.0f	//Seconds
	
	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( pPlayer == NULL )
		return;

	DoMachineGunKick( pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT );
}
void CWeaponTranquilizer::DoMachineGunKick( CBasePlayer *pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime )
{
	#define	KICK_MIN_X			0.2f	//Degrees
	#define	KICK_MIN_Y			0.2f	//Degrees
	#define	KICK_MIN_Z			0.1f	//Degrees

	QAngle vecScratch;
	int iSeed = CBaseEntity::GetPredictionRandomSeed() & 255;
	
	//Find how far into our accuracy degradation we are
	float duration	= ( fireDurationTime > slideLimitTime ) ? slideLimitTime : fireDurationTime;
	float kickPerc = duration / slideLimitTime;

	// do this to get a hard discontinuity, clear out anything under 10 degrees punch
	pPlayer->ViewPunchReset( 10 );

	//Apply this to the view angles as well
	vecScratch.x = -( KICK_MIN_X + ( maxVerticleKickAngle * kickPerc ) );
	vecScratch.y = -( KICK_MIN_Y + ( maxVerticleKickAngle * kickPerc ) ) / 3;
	vecScratch.z = KICK_MIN_Z + ( maxVerticleKickAngle * kickPerc ) / 8;

	RandomSeed( iSeed );

	//Wibble left and right
	if ( RandomInt( -1, 1 ) >= 0 )
		vecScratch.y *= -1;

	iSeed++;

	//Wobble up and down
	if ( RandomInt( -1, 1 ) >= 0 )
		vecScratch.z *= -1;

	//Clip this to our desired min/max
	UTIL_ClipPunchAngleOffset( vecScratch, pPlayer->m_Local.m_vecPunchAngle, QAngle( 24.0f, 3.0f, 1.0f ) );

	//Add it to the view punch
	// NOTE: 0.5 is just tuned to match the old effect before the punch became simulated
	pPlayer->ViewPunch( vecScratch * 0.5 );
}
void CWeaponTranquilizer::FireBolt( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

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

#ifndef CLIENT_DLL
	Vector vecAiming	= pOwner->GetAutoaimVector( 0 );	
	Vector vecSrc		= pOwner->Weapon_ShootPosition();
	Vector	vForward;
	pOwner->EyeVectors( &vForward, NULL, NULL );

	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );

	CFlechette *pBolt = CFlechette::BoltCreate( vecSrc + vForward * 12.0f, angAiming, GetHL2MPWpnData().m_iPlayerDamage, pOwner );

	if ( pOwner->GetWaterLevel() == 3 )
		pBolt->SetAbsVelocity( vecAiming * BOLT_WATER_VELOCITY );
	else
		pBolt->SetAbsVelocity( vecAiming * BOLT_AIR_VELOCITY );

#endif

	m_iClip1--;

	pOwner->ViewPunch( QAngle( -2, 0, 0 ) );

	WeaponSound( SINGLE );
	WeaponSound( SPECIAL2 );

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	if ( !m_iClip1 && pOwner->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pOwner->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	m_flNextPrimaryAttack = m_flNextSecondaryAttack	= gpGlobals->curtime + FIRE_DELAY;
}
//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CWeaponTranquilizer::GetPrimaryAttackActivity( void )
{
		return ACT_VM_PRIMARYATTACK;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CWeaponTranquilizer::Reload( void )
{
	bool fRet;
	float fCacheTime      = m_flNextSecondaryAttack;
	bool ResetPrimaryFire = m_flNextPrimaryAttack < gpGlobals->curtime;

	fRet = DefaultReload( GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD );
	if ( fRet )
	{
		// Undo whatever the reload process has done to our secondary
		// attack timer. We allow you to interrupt reloading to fire
		// a grenade.
		m_flNextSecondaryAttack = GetOwner()->m_flNextAttack = fCacheTime;
		
		WeaponSound( RELOAD );

		if(ResetPrimaryFire)
			m_flNextPrimaryAttack = gpGlobals->curtime;
	}

	return fRet;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponTranquilizer::AddViewKick( void )
{
	#define	EASY_DAMPEN			0.5f
	#define	MAX_VERTICAL_KICK	1.0f	//Degrees
	#define	SLIDE_LIMIT			2.0f	//Seconds
	
	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( pPlayer == NULL )
		return;
	pPlayer->ViewPunch(QAngle(1,1,1));
	//DoMachineGunKick( pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT );
}

const WeaponProficiencyInfo_t *CWeaponTranquilizer::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.75	},
		{ 5.00,		0.75	},
		{ 10.0/3.0, 0.75	},
		{ 5.0/3.0,	0.75	},
		{ 1.00,		1.0		},
	};

	COMPILE_TIME_ASSERT( ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}