//BP : PEDOBEAR

#include "cbase.h"
#include "ai_basenpc.h"
#include "ai_default.h"
#include "ai_schedule.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "game.h"
#include "npc_headcrab.h"
#include "npcevent.h"
#include "entitylist.h"
#include "ai_task.h"
#include "activitylist.h"
#include "engine/IEngineSound.h"
#include "npc_BaseZombie.h"
#include "hl2mp_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BREATH_VOL_MAX  0.6
#define ZOMBIE_ENEMY_BREATHE_DIST		300	// How close we must be to our enemy before we start breathing hard.
#define ZOMBIE_REFIRE_TIME 0.7f

envelopePoint_t envPedobearMoanVolumeFast[] =
{
	{	1.0f, 1.0f,
		0.1f, 0.1f,
	},
	{	0.0f, 0.0f,
		0.2f, 0.3f,
	},
};

// Turns the breathing off for a second, then back on.
envelopePoint_t envPedobearVolumeOffShort[] =
{
	{	0.0f, 0.0f,
		0.1f, 0.1f,
	},
	{	0.0f, 0.0f,
		2.0f, 2.0f,
	},
	{	BREATH_VOL_MAX, BREATH_VOL_MAX,
		1.0f, 1.0f,
	},
};

static const char *pMoanSounds[] =
{
	"NPC_PoisonZombie.Moan1",
};

//=========================================================
// Conditions
//=========================================================
enum
{
	COND_BLOCKED_BY_DOOR = LAST_BASE_ZOMBIE_CONDITION,
	COND_DOOR_OPENED,
	COND_ZOMBIE_CHARGE_TARGET_MOVED,
};

//=========================================================
// Schedules
//=========================================================
enum
{
	SCHED_ZOMBIE_BASH_DOOR = LAST_BASE_ZOMBIE_SCHEDULE,
	SCHED_ZOMBIE_WANDER_ANGRILY,
	SCHED_ZOMBIE_CHARGE_ENEMY,
	SCHED_ZOMBIE_FAIL,
};

//=========================================================
// Tasks
//=========================================================
enum
{
	TASK_ZOMBIE_EXPRESS_ANGER = LAST_BASE_ZOMBIE_TASK,
	TASK_ZOMBIE_YAW_TO_DOOR,
	TASK_ZOMBIE_ATTACK_DOOR,
	TASK_ZOMBIE_CHARGE_ENEMY,
};

int ACT_PEDOBEAR_TANTRUM;
int ACT_PEDOBEAR_WALLPOUND;

//enum{SCHED_PEDOBEAR_RANGE_ATTACK2 = LAST_BASE_ZOMBIE_SCHEDULE, SCHED_PEDOBEAR_RANGE_ATTACK1};

/*int AE_PEDOBEAR_THROW_WARN_SOUND;
int AE_PEDOBEAR_PICKUP_CRAB;
int AE_PEDOBEAR_THROW_SOUND;
int AE_PEDOBEAR_THROW_CRAB;
int AE_PEDOBEAR_SPIT;*/

ConVar sk_pedobear_health( "sk_pedobear_health", "0");
ConVar sk_pedobear_dmg_one_slash( "sk_pedobear_dmg_one_slash","0");

class CNPC_Pedobear : public CAI_BlendingHost<CNPC_BaseZombie>
{
	DECLARE_CLASS( CNPC_Pedobear, CAI_BlendingHost<CNPC_BaseZombie> );

public:

	bool ShouldBecomeTorso( const CTakeDamageInfo &info, float flDamageThreshold ){ return false; } //BP Pas de découpage de zombie
	virtual bool IsChopped( const CTakeDamageInfo &info ){ return false; }

	virtual float MaxYawSpeed( void );

	virtual int RangeAttack1Conditions( float flDot, float flDist );
	virtual int RangeAttack2Conditions( float flDot, float flDist );

	virtual float GetClawAttackRange() const { return 70; }

	virtual CBaseEntity *ClawAttack( float flDist, int iDamage, QAngle qaViewPunch, Vector vecVelocityPunch, int BloodOrigin );

	virtual void PrescheduleThink( void );
	virtual void BuildScheduleTestBits( void );
	virtual int SelectSchedule( void );
	virtual int SelectFailSchedule( int nFailedSchedule, int nFailedTask, AI_TaskFailureCode_t eTaskFailCode );
	virtual int TranslateSchedule( int scheduleType );

	virtual bool ShouldPlayIdleSound( void );

	virtual void HandleAnimEvent( animevent_t *pEvent );

	virtual void Spawn( void );
	virtual void Precache( void );
	virtual void SetZombieModel( void );

	virtual Class_T Classify( void );
	virtual void Event_Killed( const CTakeDamageInfo &info );
	virtual int OnTakeDamage_Alive( const CTakeDamageInfo &inputInfo );

	DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

	void PainSound( const CTakeDamageInfo &info );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void AttackHitSound( void );
	void AttackMissSound( void );
	void FootstepSound( bool fRightFoot );
	void FootscuffSound( bool fRightFoot ) {};

	virtual void StopLoopingSounds( void );

protected:

	virtual void MoanSound( envelopePoint_t *pEnvelope, int iEnvelopeSize );
	virtual bool MustCloseToAttack( void );

	virtual const char *GetMoanSound( int nSoundIndex );
	virtual const char *GetLegsModel( void ){ return "models/zombie/classic_legs.mdl"; }
	virtual const char *GetTorsoModel( void ){ return "models/zombie/classic_torse.mdl"; }
	virtual const char *GetHeadcrabClassname( void ){ return "npc_headcrab_poison"; }
	virtual const char *GetHeadcrabModel( void ){ return "models/headcrabblack.mdl"; }

private:

	void BreatheOffShort( void );

	void EnableCrab( int nCrab, bool bEnable );
	int RandomThrowCrab( void );
	void EvacuateNest( bool bExplosion, float flDamage, CBaseEntity *pAttacker );

	CSoundPatch *m_pFastBreathSound;
	CSoundPatch *m_pSlowBreathSound;

	float m_flNextPainSoundTime;

	bool m_bNearEnemy;
};

LINK_ENTITY_TO_CLASS( npc_pedobear, CNPC_Pedobear );

BEGIN_DATADESC( CNPC_Pedobear )
	DEFINE_SOUNDPATCH( m_pFastBreathSound ),
	DEFINE_SOUNDPATCH( m_pSlowBreathSound ),
	DEFINE_FIELD( m_flNextPainSoundTime, FIELD_TIME ),
	DEFINE_FIELD( m_bNearEnemy, FIELD_BOOLEAN ),
END_DATADESC()

//=========================================================
// Schedules
//=========================================================
enum
{
	SCHED_PEDOBEAR_WANDER_ANGRILY,
	SCHED_PEDOBEAR_CHARGE_ENEMY,
};

//=========================================================
// Tasks
//=========================================================
enum
{
	TASK_PEDOBEAR_CHARGE_ENEMY,
};

void CNPC_Pedobear::Precache( void )
{
	PrecacheModel("models/zombie/poison.mdl");

	PrecacheScriptSound( "NPC_PoisonZombie.Die" );
	PrecacheScriptSound( "NPC_PoisonZombie.ThrowWarn" );
	PrecacheScriptSound( "NPC_PoisonZombie.Throw" );
	PrecacheScriptSound( "NPC_PoisonZombie.Idle" );
	PrecacheScriptSound( "NPC_PoisonZombie.Pain" );
	PrecacheScriptSound( "NPC_PoisonZombie.Alert" );
	PrecacheScriptSound( "NPC_PoisonZombie.FootstepRight" );
	PrecacheScriptSound( "NPC_PoisonZombie.FootstepLeft" );
	PrecacheScriptSound( "NPC_PoisonZombie.Attack" );
	PrecacheScriptSound( "NPC_PoisonZombie.FastBreath" );
	PrecacheScriptSound( "NPC_PoisonZombie.Moan1" );

	PrecacheScriptSound( "Zombie.AttackHit" );
	PrecacheScriptSound( "Zombie.AttackMiss" );

	BaseClass::Precache();
}

CBaseEntity *CNPC_Pedobear::ClawAttack( float flDist, int iDamage, QAngle qaViewPunch, Vector vecVelocityPunch, int BloodOrigin  )
{
	if(m_flRefireTime > gpGlobals->curtime)
		return NULL; // Eviter de violer le jouer trop vite

	m_flRefireTime = gpGlobals->curtime + ZOMBIE_REFIRE_TIME;
	// Added test because claw attack anim sometimes used when for cases other than melee
	int iDriverInitialHealth = -1;
	CBaseEntity *pDriver = NULL;
	if ( GetEnemy() )
	{
		trace_t	tr;
		AI_TraceHull( WorldSpaceCenter(), GetEnemy()->WorldSpaceCenter(), -Vector(8,8,8), Vector(8,8,80), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr );

		if ( tr.fraction < 1.0f )
			return NULL;
	}

	//
	// Trace out a cubic section of our hull and see what we hit.
	//
	Vector vecMins = GetHullMins();
	Vector vecMaxs = GetHullMaxs();
	vecMins.z = vecMins.x;
	vecMaxs.z = vecMaxs.x;

	CBaseEntity *pHurt = NULL;
	if ( GetEnemy() && GetEnemy()->Classify() == CLASS_BULLSEYE )
	{ 
		// We always hit bullseyes we're targeting
		pHurt = GetEnemy();
		CTakeDamageInfo info( this, this, vec3_origin, GetAbsOrigin(), iDamage, DMG_SLASH );
		pHurt->TakeDamage( info );
	}
	else 
	{
		// Try to hit them with a trace
		//pHurt = CheckTraceHullAttack( flDist, vecMins, vecMaxs, iDamage, DMG_SLASH );
		pHurt = GetEnemy();
	}

	if ( pDriver && iDriverInitialHealth != pDriver->GetHealth() )
	{
		pHurt = pDriver;
	}

	if ( !pHurt && m_hPhysicsEnt != NULL && IsCurSchedule(SCHED_ZOMBIE_ATTACKITEM) )
	{
		pHurt = m_hPhysicsEnt;

		Vector vForce = pHurt->WorldSpaceCenter() - WorldSpaceCenter(); 
		VectorNormalize( vForce );

		vForce *= 5 * 24;

		CTakeDamageInfo info( this, this, vForce, GetAbsOrigin(), iDamage, DMG_SLASH );
		pHurt->TakeDamage( info );

		pHurt = m_hPhysicsEnt;
	}

	if ( pHurt )
	{
		AttackHitSound();

		CBasePlayer *pPlayer = ToBasePlayer( pHurt );

		if ( pPlayer != NULL && !(pPlayer->GetFlags() & FL_GODMODE ) )
		{
			pPlayer->ViewPunch( qaViewPunch );			
			pPlayer->VelocityPunch( vecVelocityPunch );
			//CTakeDamageInfo *dmg = new CTakeDamageInfo(pHurt,this, iDamage, DMG_SLASH);
			pPlayer->TakeDamage(CTakeDamageInfo(pHurt,this, iDamage, DMG_SLASH));
			//BP Une coup fait remonter la vie du zombie (entre 15% et 20% de sa vie de départ) NOMNOMNOM
		}
		else if( !pPlayer && UTIL_ShouldShowBlood(pHurt->BloodColor()) )
		{
			// Hit an NPC. Bleed them!
			Vector vecBloodPos;

			switch( BloodOrigin )
			{
			case ZOMBIE_BLOOD_LEFT_HAND:
				if( GetAttachment( "blood_left", vecBloodPos ) )
					SpawnBlood( vecBloodPos, g_vecAttackDir, pHurt->BloodColor(), min( iDamage, 30 ) );
				break;

			case ZOMBIE_BLOOD_RIGHT_HAND:
				if( GetAttachment( "blood_right", vecBloodPos ) )
					SpawnBlood( vecBloodPos, g_vecAttackDir, pHurt->BloodColor(), min( iDamage, 30 ) );
				break;

			case ZOMBIE_BLOOD_BOTH_HANDS:
				if( GetAttachment( "blood_left", vecBloodPos ) )
					SpawnBlood( vecBloodPos, g_vecAttackDir, pHurt->BloodColor(), min( iDamage, 30 ) );

				if( GetAttachment( "blood_right", vecBloodPos ) )
					SpawnBlood( vecBloodPos, g_vecAttackDir, pHurt->BloodColor(), min( iDamage, 30 ) );
				break;

			case ZOMBIE_BLOOD_BITE:
				// No blood for these.
				break;
			}
		}
	}
	else 
	{
		AttackMissSound();
	}

	if ( pHurt == m_hPhysicsEnt && IsCurSchedule(SCHED_ZOMBIE_ATTACKITEM) )
	{
		m_hPhysicsEnt = NULL;
		m_flNextSwat = gpGlobals->curtime + random->RandomFloat( 2, 4 );
	}

	return pHurt;
}
void CNPC_Pedobear::Spawn( void )
{
	Precache();

	m_fIsTorso = m_fIsHeadless = false;

	SetBloodColor( BLOOD_COLOR_RED );

	m_iHealth = sk_pedobear_health.GetFloat();
	m_flFieldOfView = 0.2;

	CapabilitiesClear();
	CapabilitiesAdd( bits_CAP_MOVE_GROUND | bits_CAP_INNATE_MELEE_ATTACK1 | bits_CAP_INNATE_RANGE_ATTACK1 | bits_CAP_INNATE_RANGE_ATTACK2 );

	BaseClass::Spawn();

	CPASAttenuationFilter filter( this, ATTN_IDLE );
	m_pFastBreathSound = ENVELOPE_CONTROLLER.SoundCreate( filter, entindex(), CHAN_ITEM, "NPC_PoisonZombie.FastBreath", ATTN_IDLE );
	ENVELOPE_CONTROLLER.Play( m_pFastBreathSound, 0.0f, 100 );

	CPASAttenuationFilter filter2( this );
	m_pSlowBreathSound = ENVELOPE_CONTROLLER.SoundCreate( filter2, entindex(), CHAN_ITEM, "NPC_PoisonZombie.Moan1", ATTN_NORM );
	ENVELOPE_CONTROLLER.Play( m_pSlowBreathSound, BREATH_VOL_MAX, 100 );
}

const char *CNPC_Pedobear::GetMoanSound( int nSound )
{
	return pMoanSounds[nSound % ARRAYSIZE( pMoanSounds )];
}

void CNPC_Pedobear::StopLoopingSounds( void )
{
	ENVELOPE_CONTROLLER.SoundDestroy( m_pFastBreathSound );
	m_pFastBreathSound = NULL;

	ENVELOPE_CONTROLLER.SoundDestroy( m_pSlowBreathSound );
	m_pSlowBreathSound = NULL;

	BaseClass::StopLoopingSounds();
}

void CNPC_Pedobear::Event_Killed( const CTakeDamageInfo &info )
{
	m_nSkin = 2; //ded
	if ( !( info.GetDamageType() & ( DMG_BLAST | DMG_ALWAYSGIB) ) ) 
		EmitSound( "NPC_PoisonZombie.Die" );
	if(info.GetAttacker()->IsPlayer())
	{
		ToHL2MPPlayer(info.GetAttacker())->EarnAchievement("Kill_Pedobear");
	}
	BaseClass::Event_Killed( info );
}

int CNPC_Pedobear::OnTakeDamage_Alive( const CTakeDamageInfo &inputInfo )
{
	const int pourcent20 = (20*sk_pedobear_health.GetInt())/100;
	const int pourcent40 = (40*sk_pedobear_health.GetInt())/100;

	if(m_iHealth > pourcent20 && m_iHealth <= pourcent40)
		m_nSkin = 3; // -_-
	if(m_iHealth <= pourcent20)
		m_nSkin = 1; // *O_O
	if (m_iHealth <= 0)
		m_nSkin = 2; // X_X

	return BaseClass::OnTakeDamage_Alive( inputInfo );
}

float CNPC_Pedobear::MaxYawSpeed( void )
{
	return BaseClass::MaxYawSpeed();
}

Class_T	CNPC_Pedobear::Classify( void )
{
	return CLASS_PEDOBEAR;
}

void CNPC_Pedobear::SetZombieModel( void )
{
	Hull_t lastHull = GetHullType();

	SetModel( "models/zombie/poison.mdl" );
	SetHullType(HULL_HUMAN);

	SetBodygroup( ZOMBIE_BODYGROUP_HEADCRAB, !m_fIsHeadless );

	SetHullSizeNormal( true );
	SetDefaultEyeOffset();
	SetActivity( ACT_IDLE );

	if ( lastHull != GetHullType() )
		if ( VPhysicsGetObject() )
			SetupVPhysicsHull();
}


int CNPC_Pedobear::RangeAttack1Conditions( float flDot, float flDist )
{
		float range = GetClawAttackRange();

	if (flDist > range )
	{
		return COND_TOO_FAR_TO_ATTACK;
	}

	if (flDot < 0.7)
	{
		return COND_NOT_FACING_ATTACK;
	}

	// Build a cube-shaped hull, the same hull that ClawAttack() is going to use.
	Vector vecMins = GetHullMins();
	Vector vecMaxs = GetHullMaxs();
	vecMins.z = vecMins.x;
	vecMaxs.z = vecMaxs.x;

	Vector forward, Upward;
	GetVectors( &forward, NULL, &Upward );

	trace_t	tr;
	CTraceFilterNav traceFilter( this, false, this, COLLISION_GROUP_NONE );
	AI_TraceHull( WorldSpaceCenter(), WorldSpaceCenter() + forward * GetClawAttackRange() + Upward * GetClawAttackRange(), -Vector(8,8,8), Vector(8,8,80), MASK_NPCSOLID, &traceFilter, &tr );

	if( tr.fraction == 1.0 || !tr.m_pEnt )
	{


		// If our trace was unobstructed but we were shooting 
		if ( GetEnemy() && GetEnemy()->Classify() == CLASS_BULLSEYE )
			return COND_CAN_MELEE_ATTACK1;


		// This attack would miss completely. Trick the zombie into moving around some more.
		return COND_TOO_FAR_TO_ATTACK;
	}
	Vector vecTrace = tr.endpos - tr.startpos;
	float lenTraceSq = vecTrace.Length2DSqr();

	if ( GetEnemy() && GetEnemy()->MyCombatCharacterPointer() && tr.m_pEnt == static_cast<CBaseCombatCharacter *>(GetEnemy())->GetVehicleEntity() )
	{
		if ( lenTraceSq < Square( GetClawAttackRange() * 0.75f ) )
		{
			return COND_CAN_MELEE_ATTACK1;
		}
	}

	if( tr.m_pEnt->IsBSPModel() )
	{
		// The trace hit something solid, but it's not the enemy. If this item is closer to the zombie than
		// the enemy is, treat this as an obstruction.
		Vector vecToEnemy = GetEnemy()->WorldSpaceCenter() - WorldSpaceCenter();

		if( lenTraceSq < vecToEnemy.Length2DSqr() )
		{
			return COND_ZOMBIE_LOCAL_MELEE_OBSTRUCTION;
		}
	}


	if ( !tr.m_pEnt->IsWorld() && GetEnemy() && GetEnemy()->GetGroundEntity() == tr.m_pEnt )
	{
		//Try to swat whatever the player is standing on instead of acting like a dill.
		return COND_CAN_MELEE_ATTACK1;
	}

	// Bullseyes are given some grace on if they can be hit
	if ( GetEnemy() && GetEnemy()->Classify() == CLASS_BULLSEYE )
		return COND_CAN_MELEE_ATTACK1;


	// Move around some more
	return COND_TOO_FAR_TO_ATTACK;
	//return COND_CAN_RANGE_ATTACK1;
}

int CNPC_Pedobear::RangeAttack2Conditions( float flDot, float flDist )
{
	return RangeAttack1Conditions( flDot,flDist );
}

// Purpose: Turns off our breath so we can play another vocal sound.
void CNPC_Pedobear::BreatheOffShort( void )
{
	if ( m_bNearEnemy )
		ENVELOPE_CONTROLLER.SoundPlayEnvelope( m_pFastBreathSound, SOUNDCTRL_CHANGE_VOLUME, envPedobearVolumeOffShort, ARRAYSIZE(envPedobearVolumeOffShort) );
	else
		ENVELOPE_CONTROLLER.SoundPlayEnvelope( m_pSlowBreathSound, SOUNDCTRL_CHANGE_VOLUME, envPedobearVolumeOffShort, ARRAYSIZE(envPedobearVolumeOffShort) );
}

void CNPC_Pedobear::HandleAnimEvent( animevent_t *pEvent )
{
	BaseClass::HandleAnimEvent( pEvent );
}

void CNPC_Pedobear::PrescheduleThink( void )
{
	bool bNearEnemy = false;
	if ( GetEnemy() != NULL )
	{
		float flDist = (GetEnemy()->GetAbsOrigin() - GetAbsOrigin()).Length();
		if ( flDist < ZOMBIE_ENEMY_BREATHE_DIST )
		{
			bNearEnemy = true;
		}
	}

	if ( bNearEnemy )
	{
		if ( !m_bNearEnemy )
		{
			// Our enemy is nearby. Breathe faster.
			float duration = random->RandomFloat( 1.0f, 2.0f );
			ENVELOPE_CONTROLLER.SoundChangeVolume( m_pFastBreathSound, BREATH_VOL_MAX, duration );
			ENVELOPE_CONTROLLER.SoundChangePitch( m_pFastBreathSound, random->RandomInt( 100, 120 ), random->RandomFloat( 1.0f, 2.0f ) );

			ENVELOPE_CONTROLLER.SoundChangeVolume( m_pSlowBreathSound, 0.0f, duration );

			m_bNearEnemy = true;
		}
	}
	else if ( m_bNearEnemy )
	{
		// Our enemy is far away. Slow our breathing down.
		float duration = random->RandomFloat( 2.0f, 4.0f );
		ENVELOPE_CONTROLLER.SoundChangeVolume( m_pFastBreathSound, BREATH_VOL_MAX, duration );
		ENVELOPE_CONTROLLER.SoundChangeVolume( m_pSlowBreathSound, 0.0f, duration );
//		ENVELOPE_CONTROLLER.SoundChangePitch( m_pBreathSound, random->RandomInt( 80, 100 ), duration );

		m_bNearEnemy = false;
	}

	BaseClass::PrescheduleThink();
}

void CNPC_Pedobear::BuildScheduleTestBits( void )
{
	BaseClass::BuildScheduleTestBits();

	if ( IsCurSchedule( SCHED_CHASE_ENEMY ) )
	{
		SetCustomInterruptCondition( COND_LIGHT_DAMAGE );
		SetCustomInterruptCondition( COND_HEAVY_DAMAGE );
	}
	else if ( IsCurSchedule( SCHED_RANGE_ATTACK1 ) || IsCurSchedule( SCHED_RANGE_ATTACK2 ) )
	{
		ClearCustomInterruptCondition( COND_LIGHT_DAMAGE );
		ClearCustomInterruptCondition( COND_HEAVY_DAMAGE );
	}
}

int CNPC_Pedobear::SelectFailSchedule( int nFailedSchedule, int nFailedTask, AI_TaskFailureCode_t eTaskFailCode )
{
	int nSchedule = BaseClass::SelectFailSchedule( nFailedSchedule, nFailedTask, eTaskFailCode );

	if ( nSchedule == SCHED_CHASE_ENEMY_FAILED )
		return SCHED_ESTABLISH_LINE_OF_FIRE;

	return nSchedule;
}

int CNPC_Pedobear::SelectSchedule( void )
{
	int nSchedule = BaseClass::SelectSchedule();

	if ( nSchedule == SCHED_SMALL_FLINCH )
		 m_flNextFlinchTime = gpGlobals->curtime + random->RandomFloat( 1, 3 );

	return nSchedule;
}

int CNPC_Pedobear::TranslateSchedule( int scheduleType )
{
	/*if ( scheduleType == SCHED_RANGE_ATTACK2 )
	{
		return SCHED_PEDOBEAR_RANGE_ATTACK2;
	}

	if ( scheduleType == SCHED_RANGE_ATTACK1 )
	{
		return SCHED_PEDOBEAR_RANGE_ATTACK1;
	}

	if ( scheduleType == SCHED_COMBAT_FACE && IsUnreachable( GetEnemy() ) )
		return SCHED_TAKE_COVER_FROM_ENEMY;
*/
	// We'd simply like to shamble towards our enemy
	if ( scheduleType == SCHED_MOVE_TO_WEAPON_RANGE )
		return SCHED_CHASE_ENEMY;

	return BaseClass::TranslateSchedule( scheduleType );
}

bool CNPC_Pedobear::ShouldPlayIdleSound( void )
{
	return CAI_BaseNPC::ShouldPlayIdleSound();
}

void CNPC_Pedobear::AttackHitSound( void )
{
	EmitSound( "Zombie.AttackHit" );
}

void CNPC_Pedobear::AttackMissSound( void )
{
	EmitSound( "Zombie.AttackMiss" );
}

void CNPC_Pedobear::AttackSound( void )
{
	EmitSound( "NPC_PoisonZombie.Attack" );
}

void CNPC_Pedobear::IdleSound( void )
{
	// HACK: base zombie code calls IdleSound even when not idle!
	if ( m_NPCState != NPC_STATE_COMBAT )
	{
		BreatheOffShort();
		EmitSound( "NPC_PoisonZombie.Idle" );
		MakeAISpookySound( 360.0f );
	}
}

void CNPC_Pedobear::PainSound( const CTakeDamageInfo &info )
{
	// Don't make pain sounds too often.
	if ( m_flNextPainSoundTime <= gpGlobals->curtime )
	{	
		BreatheOffShort();
		EmitSound( "NPC_PoisonZombie.Pain" );
		m_flNextPainSoundTime = gpGlobals->curtime + random->RandomFloat( 4.0, 7.0 );
	}
}

void CNPC_Pedobear::AlertSound( void )
{
	BreatheOffShort();

	EmitSound( "NPC_PoisonZombie.Alert" );
}

void CNPC_Pedobear::FootstepSound( bool fRightFoot )
{
	if( fRightFoot )
		EmitSound( "NPC_PoisonZombie.FootstepRight" );
	else
		EmitSound( "NPC_PoisonZombie.FootstepLeft" );

	if( ShouldPlayFootstepMoan() )
	{
		m_flNextMoanSound = gpGlobals->curtime;
		MoanSound( envPedobearMoanVolumeFast, ARRAYSIZE( envPedobearMoanVolumeFast ) );
	}
}

bool CNPC_Pedobear::MustCloseToAttack(void)
{
	return true; //BP Pas de lancer de headcrab donc on attaque au corps à corps direct
}

void CNPC_Pedobear::MoanSound( envelopePoint_t *pEnvelope, int iEnvelopeSize )
{
	if( !m_pMoanSound )
	{
		// Don't set this up until the code calls for it.
		const char *pszSound = GetMoanSound( m_iMoanSound );
		m_flMoanPitch = random->RandomInt( 98, 110 );

		CPASAttenuationFilter filter( this, 1.5 );
		//m_pMoanSound = ENVELOPE_CONTROLLER.SoundCreate( entindex(), CHAN_STATIC, pszSound, ATTN_NORM );
		m_pMoanSound = ENVELOPE_CONTROLLER.SoundCreate( filter, entindex(), CHAN_STATIC, pszSound, 1.5 );

		ENVELOPE_CONTROLLER.Play( m_pMoanSound, 0.5, m_flMoanPitch );
	}

	envPedobearMoanVolumeFast[ 1 ].durationMin = 0.1;
	envPedobearMoanVolumeFast[ 1 ].durationMax = 0.4;

	if ( random->RandomInt( 1, 2 ) == 1 )
		IdleSound();

	float duration = ENVELOPE_CONTROLLER.SoundPlayEnvelope( m_pMoanSound, SOUNDCTRL_CHANGE_VOLUME, pEnvelope, iEnvelopeSize );

	float flPitchShift = random->RandomInt( -4, 4 );
	ENVELOPE_CONTROLLER.SoundChangePitch( m_pMoanSound, m_flMoanPitch + flPitchShift, 0.3 );

	m_flNextMoanSound = gpGlobals->curtime + duration + 9999;
}

/*
int ACT_PEDOBEAR_THREAT;

AI_BEGIN_CUSTOM_NPC( npc_pedobear, CNPC_Pedobear )

	DECLARE_ACTIVITY( ACT_PEDOBEAR_THREAT )

	//Adrian: events go here
	DECLARE_ANIMEVENT( AE_PEDOBEAR_THROW_WARN_SOUND )
	DECLARE_ANIMEVENT( AE_PEDOBEAR_PICKUP_CRAB )
	DECLARE_ANIMEVENT( AE_PEDOBEAR_THROW_SOUND )
	DECLARE_ANIMEVENT( AE_PEDOBEAR_THROW_CRAB )
	DECLARE_ANIMEVENT( AE_PEDOBEAR_SPIT )

	DEFINE_SCHEDULE
	(
		SCHED_PEDOBEAR_RANGE_ATTACK2,

		"	Tasks"
		"		TASK_STOP_MOVING						0"
		"		TASK_FACE_IDEAL							0"
		"		TASK_PLAY_PRIVATE_SEQUENCE_FACE_ENEMY	ACTIVITY:ACT_PEDOBEAR_THREAT"
		"		TASK_FACE_IDEAL							0"
		"		TASK_RANGE_ATTACK2						0"

		"	Interrupts"
		"		COND_NO_PRIMARY_AMMO"
	)

	DEFINE_SCHEDULE
	(
		SCHED_PEDOBEAR_RANGE_ATTACK1,

		"	Tasks"
		"		TASK_STOP_MOVING		0"
		"		TASK_FACE_ENEMY			0"
		"		TASK_ANNOUNCE_ATTACK	1"	// 1 = primary attack
		"		TASK_RANGE_ATTACK1		0"
		""
		"	Interrupts"
		"		COND_NO_PRIMARY_AMMO"
	)

AI_END_CUSTOM_NPC()
*/

AI_BEGIN_CUSTOM_NPC( npc_pedobear, CNPC_Pedobear )

	DECLARE_CONDITION( COND_BLOCKED_BY_DOOR )
	DECLARE_CONDITION( COND_DOOR_OPENED )
	DECLARE_CONDITION( COND_ZOMBIE_CHARGE_TARGET_MOVED )

	DECLARE_TASK( TASK_ZOMBIE_EXPRESS_ANGER )
	DECLARE_TASK( TASK_ZOMBIE_YAW_TO_DOOR )
	DECLARE_TASK( TASK_ZOMBIE_ATTACK_DOOR )
	DECLARE_TASK( TASK_ZOMBIE_CHARGE_ENEMY )
	
	DECLARE_ACTIVITY( ACT_PEDOBEAR_TANTRUM );
	DECLARE_ACTIVITY( ACT_PEDOBEAR_WALLPOUND );

	DEFINE_SCHEDULE
	( 
		SCHED_ZOMBIE_BASH_DOOR,

		"	Tasks"
		"		TASK_SET_ACTIVITY				ACTIVITY:ACT_ZOMBIE_TANTRUM"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_TAKE_COVER_FROM_ENEMY"
		"		TASK_ZOMBIE_YAW_TO_DOOR			0"
		"		TASK_FACE_IDEAL					0"
		"		TASK_ZOMBIE_ATTACK_DOOR			0"
		""
		"	Interrupts"
		"		COND_ZOMBIE_RELEASECRAB"
		"		COND_ENEMY_DEAD"
		"		COND_NEW_ENEMY"
		"		COND_DOOR_OPENED"
	)

	DEFINE_SCHEDULE
	(
		SCHED_ZOMBIE_WANDER_ANGRILY,

		"	Tasks"
		"		TASK_WANDER						480420" // 48 units to 240 units.
		"		TASK_WALK_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			4"
		""
		"	Interrupts"
		"		COND_ZOMBIE_RELEASECRAB"
		"		COND_ENEMY_DEAD"
		"		COND_NEW_ENEMY"
		"		COND_DOOR_OPENED"
	)

	DEFINE_SCHEDULE
	(
		SCHED_ZOMBIE_CHARGE_ENEMY,


		"	Tasks"
		"		TASK_ZOMBIE_CHARGE_ENEMY		0"
		"		TASK_WALK_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_PLAY_SEQUENCE				ACTIVITY:ACT_ZOMBIE_TANTRUM" /* placeholder until frustration/rage/fence shake animation available */
		""
		"	Interrupts"
		"		COND_ZOMBIE_RELEASECRAB"
		"		COND_ENEMY_DEAD"
		"		COND_NEW_ENEMY"
		"		COND_DOOR_OPENED"
		"		COND_ZOMBIE_CHARGE_TARGET_MOVED"
	)

	DEFINE_SCHEDULE
	(
		SCHED_ZOMBIE_FAIL,

		"	Tasks"
		"		TASK_STOP_MOVING		0"
		"		TASK_SET_ACTIVITY		ACTIVITY:ACT_ZOMBIE_TANTRUM"
		"		TASK_WAIT				1"
		"		TASK_WAIT_PVS			0"
		""
		"	Interrupts"
		"		COND_CAN_RANGE_ATTACK1 "
		"		COND_CAN_RANGE_ATTACK2 "
		"		COND_CAN_MELEE_ATTACK1 "
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_GIVE_WAY"
		"		COND_DOOR_OPENED"
	)


AI_END_CUSTOM_NPC()