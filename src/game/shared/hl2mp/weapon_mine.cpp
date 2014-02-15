// BISOUNOURS PARTY : MINE (weapon)

#include "cbase.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "hl2mp/npc_mine.h"
	#include "entitylist.h"
	#include "eventqueue.h"
#endif

#include "hl2mp/weapon_mine.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define MINE_DISTANCE 16

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponMine, DT_WeaponMine )

BEGIN_NETWORK_TABLE( CWeaponMine, DT_WeaponMine )
#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_bNeedReload ) ),
	RecvPropBool( RECVINFO( m_bAttachMine ) ),
#else
	SendPropBool( SENDINFO( m_bNeedReload ) ),
	SendPropBool( SENDINFO( m_bAttachMine ) ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL

BEGIN_PREDICTION_DATA( CWeaponMine )
	DEFINE_PRED_FIELD( m_bNeedReload, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bAttachMine, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()

#endif

LINK_ENTITY_TO_CLASS( weapon_mine, CWeaponMine );

PRECACHE_WEAPON_REGISTER(weapon_mine);

#ifndef CLIENT_DLL

BEGIN_DATADESC( CWeaponMine )
	DEFINE_FIELD( m_bNeedReload, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bAttachMine, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flWallSwitchTime, FIELD_TIME ),

	DEFINE_FUNCTION( MineTouch ),
END_DATADESC()
#endif

acttable_t	CWeaponMine::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_SLAM,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_SLAM,				false },
	{ ACT_MP_RUN,						ACT_HL2MP_RUN_SLAM,						false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_SLAM,				false },
	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SLAM,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SLAM,	false },
	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_SLAM,			false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_SLAM,			false },
	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_SLAM,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_SLAM,					false },
};

IMPLEMENT_ACTTABLE(CWeaponMine);

void CWeaponMine::Spawn( )
{
	BaseClass::Spawn();

	Precache( );

	FallInit();// get ready to fall down

	m_flWallSwitchTime	= 0;

	m_iClip2 = 1;
}

void CWeaponMine::Precache( void )
{
	BaseClass::Precache();

#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "npc_mine" );
#endif
}

void CWeaponMine::SetPickupTouch( void )
{
	SetTouch(&CWeaponMine::MineTouch);
}

void CWeaponMine::MineTouch( CBaseEntity *pOther )
{
#ifdef GAME_DLL
	CBaseCombatCharacter* pBCC = ToBaseCombatCharacter( pOther );
	if ( pBCC && !pBCC->IsAllowedToPickupWeapons() )
		return;
#endif

	BaseClass::DefaultTouch(pOther);
}

bool CWeaponMine::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	SetThink(NULL);

	return BaseClass::Holster(pSwitchingTo);
}

bool CWeaponMine::Reload( void )
{
	WeaponIdle( );

	return true;
}

void CWeaponMine::PrimaryAttack( void )
{
	CBaseCombatCharacter *pOwner = GetOwner();
	if (!pOwner)
		return;

	if (pOwner->GetAmmoCount(m_iSecondaryAmmoType) <= 0)
		return;

	if (!m_bAttachMine)
		StartAttach();
}

//Lance l'animation de pose de la mine
void CWeaponMine::StartAttach( void ){
	SendWeaponAnim(ACT_SLAM_TRIPMINE_ATTACH);

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
	m_bAttachMine = true;
}

//Pose la mine sur son support une fois l'animation terminée
void CWeaponMine::FinishAttach( void ){
	CHL2MP_Player *pOwner = ToHL2MPPlayer( GetOwner() );
	if (!pOwner)
		return;

	Vector vecSrc, vecAiming;
	vecSrc = pOwner->EyePosition();
	QAngle angles = pOwner->GetLocalAngles();
	AngleVectors( angles, &vecAiming );
	trace_t tr;
	UTIL_TraceLine( vecSrc, vecSrc + (vecAiming * 60), MASK_SOLID, pOwner, COLLISION_GROUP_NONE, &tr );
	
	if (tr.fraction < 1.0)
	{		
		if (tr.m_pEnt)
		{
			//On attache pas la mine sur une entité vivante
			CBaseEntity *pEntity = tr.m_pEnt;
			CBaseCombatCharacter *pBCC = ToBaseCombatCharacter( pEntity );
			if (pBCC){
				m_bAttachMine  = false;
				m_bNeedReload = true;
				return;
			}

#ifndef CLIENT_DLL
			//On vérifie qu'il n 'y a pas déjà une mine sur le support visé
			CBaseEntity* pResult = gEntList.FindEntityByClassname(NULL,"npc_mine");
			while (pResult)
			{
				if((pResult->GetAbsOrigin() - tr.endpos).Length() < MINE_DISTANCE){
					m_bAttachMine  = false;
					m_bNeedReload = true;
					return;
				}

				pResult = gEntList.FindEntityByClassname(pResult,"npc_mine");
			}

			if (pEntity && !(pEntity->GetFlags() & FL_CONVEYOR))
			{
				QAngle angles;
				VectorAngles(tr.plane.normal, angles);
				angles.x += 90;

				CBaseEntity *pEnt = CBaseEntity::Create( "npc_mine", tr.endpos + tr.plane.normal * 3, angles, NULL );
				CNPCMine *pMine = (CNPCMine *)pEnt;
				pMine->m_hOwner = GetOwner();
				ChooseMineColor(pMine);
				pMine->AttachToEntity( pEntity );

				pOwner->RemoveAmmo( 1, m_iSecondaryAmmoType );
			}
#endif
		}
	}

	m_bAttachMine  = false;
	m_bNeedReload = true;
}

#ifndef CLIENT_DLL
void CWeaponMine::ChooseMineColor( CNPCMine * pMine ){
	CBasePlayer *pOwner = ToBasePlayer(pMine->m_hOwner.Get());
	if(pOwner == NULL)
		pMine->m_nSkin = 0;

	switch(pOwner->GetTeamNumber())
	{
		case TEAM_GREEN:
			pMine->m_nSkin = 4;
			break;
		case TEAM_PINK:
			pMine->m_nSkin = 2;
			break;
		default:
			pMine->m_nSkin = 0;
	};
}
#endif

void CWeaponMine::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner)
		return;

	if (!m_bNeedReload && (pOwner->m_nButtons & IN_ATTACK) && m_flNextPrimaryAttack <= gpGlobals->curtime && !((pOwner->m_nButtons & IN_ATTACK) && m_bAttachMine))
		PrimaryAttack();
	else 
		WeaponIdle();
}

void CWeaponMine::WeaponIdle( void )
{
	if(gpGlobals->curtime > m_flNextPrimaryAttack){
		CBaseCombatCharacter *pOwner  = GetOwner();
		if (!pOwner)
			return;

		//Si on a une mine à finir de poser
		if(m_bAttachMine)
			FinishAttach();
		//Sinon si on doit recharger
		if( m_bNeedReload )
		{	
			if (pOwner->GetAmmoCount(m_iSecondaryAmmoType) > 0){
				SendWeaponAnim(ACT_SLAM_TRIPMINE_DRAW);
				m_bNeedReload = false;
			}
#ifndef CLIENT_DLL
			//Sinon on a pas assez de munitions et on drop l'arme
			else
			{
				pOwner->Weapon_Drop(this);
				UTIL_Remove(this);
			}
#endif
		}
		//Sinon on idle
		else
		{
			SendWeaponAnim(ACT_SLAM_TRIPMINE_IDLE);
			m_flWallSwitchTime = 0;
		}
	}
}

bool CWeaponMine::Deploy( void )
{
	m_bNeedReload = false;

	return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_SLAM_TRIPMINE_DRAW, (char*)GetAnimPrefix() );
}

CWeaponMine::CWeaponMine(void)
{
	m_bNeedReload = false;
	m_bAttachMine = false;
}