// BISOUNOURS PARTY : BIGSHOTGUN

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

#define BIGSHOTGUN_PRIMARY_PELLETS 12
#define BIGSHOTGUN_SECONDARY_PELLETS 22

#ifdef CLIENT_DLL
#define CWeaponBigshotgun C_WeaponBigshotgun
#endif

class CWeaponBigshotgun : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponBigshotgun, CBaseHL2MPCombatWeapon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

private:
	CNetworkVar(bool, m_bNeedPump );		// When emptied completely
	CNetworkVar(bool, m_bReloadEmpty)		// Rechargement d'un chargeur entièrement vidé
	CNetworkVar(bool, m_bDelayedFire1 );	// Fire primary when finished reloading
	CNetworkVar(bool, m_bDelayedFire2 );	// Fire secondary when finished reloading
	CNetworkVar(bool, m_bDelayedReload );	// Reload when finished pump

public:
	virtual const Vector& GetBulletSpread( void )
	{
		static Vector cone = VECTOR_CONE_15DEGREES;
		return cone;
	}


	CWeaponBigshotgun(void);
		bool Deploy();
	virtual void Precache( void );
	virtual int	GetMinBurst() { return 1; }
	virtual int	GetMaxBurst() { return 3; }
	bool StartReload( void );
	bool Reload( void );
	void FillClip( void );
	void FinishReload( void );
	void CheckHolsterReload( void );
	void Pump( void );
	void ItemHolsterFrame( void );
	void ItemPostFrame( void );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	virtual float GetFireRate( void ) { return 0.7; };

private:

	CNetworkVar( bool, m_bEnableGlow );
	CNetworkVar( color32, m_GlowColor );

	DECLARE_ACTTABLE();

	CWeaponBigshotgun( const CWeaponBigshotgun & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponBigshotgun, DT_WeaponBigshotgun )

BEGIN_NETWORK_TABLE( CWeaponBigshotgun, DT_WeaponBigshotgun )
#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO( m_bNeedPump ) ),
	RecvPropInt(RECVINFO(m_bReloadEmpty)),
	RecvPropBool( RECVINFO( m_bDelayedFire1 ) ),
	RecvPropBool( RECVINFO( m_bDelayedFire2 ) ),
	RecvPropBool( RECVINFO( m_bDelayedReload ) ),
#else
	SendPropBool( SENDINFO( m_bNeedPump ) ),
	SendPropInt(SENDINFO(m_bReloadEmpty)),
	SendPropBool( SENDINFO( m_bDelayedFire1 ) ),
	SendPropBool( SENDINFO( m_bDelayedFire2 ) ),
	SendPropBool( SENDINFO( m_bDelayedReload ) ),
#endif
	#ifdef CLIENT_DLL
	RecvPropBool( RECVINFO(m_bEnableGlow) ),
	RecvPropInt( RECVINFO(m_GlowColor), 0, RecvProxy_IntToColor32 ),
#else
	SendPropBool( SENDINFO(m_bEnableGlow) ),
	SendPropInt( SENDINFO(m_GlowColor), 32, SPROP_UNSIGNED, SendProxy_Color32ToInt ),
#endif

END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponBigshotgun )
	DEFINE_PRED_FIELD( m_bNeedPump, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bReloadEmpty, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bDelayedFire1, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bDelayedFire2, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bDelayedReload, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_bigshotgun, CWeaponBigshotgun );
PRECACHE_WEAPON_REGISTER(weapon_bigshotgun);

acttable_t	CWeaponBigshotgun::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_SHOTGUN,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_SHOTGUN,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_SHOTGUN,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_SHOTGUN,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SHOTGUN,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SHOTGUN,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_SHOTGUN,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_SHOTGUN,		false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_SHOTGUN,					false },
	{ ACT_MP_SWIM,						ACT_HL2MP_IDLE_SHOTGUN,					false },
};

IMPLEMENT_ACTTABLE(CWeaponBigshotgun);

void CWeaponBigshotgun::Precache(void)
{
	CBaseCombatWeapon::Precache();
	PrecacheParticleSystem("muzzleflash_final_bigshotgun");
}
bool CWeaponBigshotgun::StartReload( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();	
	if ( pOwner == NULL )
		return false;

	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		return false;

	if (m_iClip1 >= GetMaxClip1())
		return false;

	if (min(1, pOwner->GetAmmoCount(m_iPrimaryAmmoType)) <= 0)
		return false;

	SendWeaponAnim( ACT_SHOTGUN_RELOAD_START );

	//Tony; BUG BUG BUG!!! shotgun does one shell at a time!!! -- player model only has a single reload!!! so I'm just going to dispatch the singular for now.
	ToHL2MPPlayer( pOwner )->DoAnimationEvent( PLAYERANIMEVENT_RELOAD );

	// Make shotgun shell visible
	SetBodygroup(1,0);

	pOwner->m_flNextAttack = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();

	m_bInReload = true;

	// Pompage à la L4D pour que Klems soit content
	if(m_iClip1 == 0)
		m_bReloadEmpty = true;

	return true;
}
bool CWeaponBigshotgun::Reload( void )
{
	// Check that StartReload was called first
	if (!m_bInReload)
		Warning("ERROR: Shotgun Reload called incorrectly!\n");

	CBaseCombatCharacter *pOwner  = GetOwner();	
	if ( pOwner == NULL )
		return false;

	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		return false;

	if (m_iClip1 >= GetMaxClip1())
		return false;

	if (min(1, pOwner->GetAmmoCount(m_iPrimaryAmmoType)) <= 0)
		return false;

	FillClip();

	// Play reload on different channel as otherwise steals channel away from fire sound
	WeaponSound(RELOAD);
	SendWeaponAnim( ACT_VM_RELOAD );

	pOwner->m_flNextAttack = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();

	return true;
}

void CWeaponBigshotgun::FinishReload( void )
{
	// Make shotgun shell invisible
	SetBodygroup(1,1);

	CBaseCombatCharacter *pOwner  = GetOwner();	
	if ( pOwner == NULL )
		return;

	m_bInReload = false;

	// Finish reload animation
	SendWeaponAnim( ACT_SHOTGUN_RELOAD_FINISH );

	pOwner->m_flNextAttack = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();

	Pump();
}

void CWeaponBigshotgun::FillClip( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();	
	if ( pOwner == NULL )
		return;

	if ( pOwner->GetAmmoCount( m_iPrimaryAmmoType ) > 0 )
		if ( Clip1() < GetMaxClip1() )
		{
			m_iClip1++;
			pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );
		}
}

void CWeaponBigshotgun::Pump( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();
	if ( pOwner == NULL )
		return;
	
	m_bNeedPump = false;
	if ( m_bDelayedReload )
	{
		m_bDelayedReload = false;
		StartReload();
	}
	
	WeaponSound( SPECIAL1 );

	// Finish reload animation
	SendWeaponAnim( ACT_SHOTGUN_PUMP );

	pOwner->m_flNextAttack	= gpGlobals->curtime + 0.1;
	m_flNextPrimaryAttack	= gpGlobals->curtime + 0.1;
}

void CWeaponBigshotgun::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;

	// MUST call sound before removing a round from the clip of a CMachineGun
	WeaponSound(SINGLE);

	//BP muzzlefash de la mort
	//Vector vecShootOrigin;
	//QAngle angShootDir;
	//GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
	//DispatchParticleEffect("muzzleflash_final_bigshotgun", PATTACH_POINT_FOLLOW, this, LookupAttachment("muzzle"));
	Vector	vForward, vRight, vUp;
	pPlayer->EyeVectors( &vForward, &vRight, &vUp );
#ifdef CLIENT_DLL
	Vector	muzzlePoint = pPlayer->Weapon_ShootPosition() + vForward * 12.0f + vRight * 6.0f + vUp * -3.0f;
#else
	Vector muzzlePoint;
	QAngle angShootDir;
	GetAttachment(LookupAttachment("muzzle"), muzzlePoint, angShootDir);
#endif
	
	DispatchParticleEffect("muzzleflash_final_bigshotgun", muzzlePoint, pPlayer->EyeAngles());
	//pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);

	// Don't fire again until fire animation has completed
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.36f + 0.56f;
	m_iClip1 -= 1;

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );
	ToHL2MPPlayer(pPlayer)->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);	


	FireBulletsInfo_t info(BIGSHOTGUN_PRIMARY_PELLETS, vecSrc, vecAiming, GetBulletSpread(), MAX_TRACE_LENGTH, m_iPrimaryAmmoType);
	info.m_pAttacker = pPlayer;

	// Fire the bullets, and force the first shot to be perfectly accuracy
	pPlayer->FireBullets(info);

	AddRecoil(210, 210, 150, 0.5);

	QAngle angles = pPlayer->GetLocalAngles();

	angles.x += random->RandomInt( -1, 1 );
	angles.y += random->RandomInt( -1, 1 );
	angles.z = 0;

#ifndef CLIENT_DLL
	pPlayer->SnapEyeAngles( angles );
#else
	dlight_t *el = effects->CL_AllocDlight( LIGHT_INDEX_MUZZLEFLASH + index );

	el->origin = muzzlePoint;
	el->radius = random->RandomInt( 32, 64 ); 
	el->decay = el->radius / 0.05f;
	el->die = gpGlobals->curtime + 0.15f;
	el->color.r = 255;
	el->color.g = 192;
	el->color.b = 64;
	el->color.exponent = 5;
#endif

	pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );
}
bool CWeaponBigshotgun::Deploy()
{
	if(m_bInReload)
		StartReload();
	Pump();
	return BaseClass::Deploy();
}
void CWeaponBigshotgun::SecondaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;

	pPlayer->m_nButtons &= ~IN_ATTACK2;
	// MUST call sound before removing a round from the clip of a CMachineGun
	WeaponSound(WPN_DOUBLE);

	//BP muzzlefash de la mort
	Vector	vForward, vRight, vUp;
	pPlayer->EyeVectors( &vForward, &vRight, &vUp );
#ifdef CLIENT_DLL
	Vector	muzzlePoint = pPlayer->Weapon_ShootPosition() + vForward * 12.0f + vRight * 6.0f + vUp * -3.0f;
#else
	Vector muzzlePoint;
	QAngle angShootDir;
	GetAttachment(LookupAttachment("muzzle"), muzzlePoint, angShootDir);
#endif
	DispatchParticleEffect("muzzleflash_final_bigshotgun", muzzlePoint, pPlayer->EyeAngles());

	SendWeaponAnim( ACT_VM_SECONDARYATTACK );

	// Don't fire again until fire animation has completed
	//m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration() + 0.56f;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration() + 0.6f;
	m_iClip1 -= 2;	// Shotgun uses same clip for primary and secondary attacks

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );
	//Tony; does shotgun have a second anim?
	ToHL2MPPlayer(pPlayer)->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	Vector vecSrc	 = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );	

	FireBulletsInfo_t info( BIGSHOTGUN_SECONDARY_PELLETS, vecSrc, vecAiming, GetBulletSpread(), MAX_TRACE_LENGTH, m_iPrimaryAmmoType );
	info.m_pAttacker = pPlayer;

	// Fire the bullets, and force the first shot to be perfectly accuracy
	pPlayer->FireBullets( info );

	AddRecoil(300, 300, 250, 0.3);
			QAngle angles = pPlayer->GetLocalAngles();

	angles.x += random->RandomInt( -2, 3 );
	angles.y += random->RandomInt( -3, 2 );
	angles.z = 0;

#ifndef CLIENT_DLL
	pPlayer->SnapEyeAngles( angles );
#else
	dlight_t *el = effects->CL_AllocDlight( LIGHT_INDEX_MUZZLEFLASH + index );

	el->origin = muzzlePoint;
	el->radius = random->RandomInt( 64, 100 ); 
	el->decay = el->radius * 10;
	el->die = gpGlobals->curtime + 0.2f;
	el->color.r = 255;
	el->color.g = 192;
	el->color.b = 64;
	el->color.exponent = 5;
#endif

	pPlayer->ViewPunch( QAngle( -8, random->RandomFloat( -2, 2 ), 0 ) );
}

void CWeaponBigshotgun::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner)
		return;

	//Doit-on pomper?
	if(!m_bInReload && m_bReloadEmpty && m_iClip1 > 0){
		m_bNeedPump = true;
		m_bReloadEmpty = false;
	}

	if ( m_bNeedPump && ( pOwner->m_nButtons & IN_RELOAD ) )
		m_bDelayedReload = true;

	if (m_bInReload)
	{
		// If I'm primary firing and have one round stop reloading and fire
		if ((pOwner->m_nButtons & IN_ATTACK ) && (m_iClip1 >=1) && !m_bNeedPump)
		{
			m_bInReload		= false;
			m_bDelayedFire1 = true;

			//m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
		}
		// If I'm secondary firing and have two rounds stop reloading and fire
		else if ((pOwner->m_nButtons & IN_ATTACK2 ) && (m_iClip1 >=2) && !m_bNeedPump )
		{
			m_bInReload		= false;
			m_bDelayedFire2 = true;
		}
		else if (m_flNextPrimaryAttack <= gpGlobals->curtime)
		{
			// If out of ammo end reload
			if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <=0)
			{
				FinishReload();
				return;
			}
			// If clip not full reload again
			if (m_iClip1 < GetMaxClip1())
			{
				Reload();
				return;
			}
			// Clip full, stop reloading
			else
			{
				FinishReload();
				return;
			}
		}
	}
	else
	{			
		// Make shotgun shell invisible
		SetBodygroup(1,1);
	}

	if (m_bNeedPump && m_flNextPrimaryAttack <= gpGlobals->curtime)
	{
		m_bNeedPump = false;
		return;
	}
	
	// Shotgun uses same timing and ammo for secondary attack
	if ((m_bDelayedFire2 || pOwner->m_nButtons & IN_ATTACK2)&&(m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		m_bDelayedFire2 = false;

		if ( (m_iClip1 <= 1 && UsesClipsForAmmo1()))
		{
			// If only one shell is left, do a single shot instead	
			if ( m_iClip1 == 1 )
				PrimaryAttack();
			else
				StartReload();
		}
		else
		{
			// If the firing button was just pressed, reset the firing time
			if ( pOwner->m_afButtonPressed & IN_ATTACK )
				 m_flNextPrimaryAttack = gpGlobals->curtime;

			SecondaryAttack();
		}
	}
	else if ( (m_bDelayedFire1 || pOwner->m_nButtons & IN_ATTACK) && m_flNextPrimaryAttack <= gpGlobals->curtime)
	{
		m_bDelayedFire1 = false;

		if ( (m_iClip1 <= 0 && UsesClipsForAmmo1()) || ( !UsesClipsForAmmo1() && !pOwner->GetAmmoCount(m_iPrimaryAmmoType) ) )
		{
				StartReload();
		}
		else
		{
			// If the firing button was just pressed, reset the firing time
			CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
			if ( pPlayer && pPlayer->m_afButtonPressed & IN_ATTACK )
				 m_flNextPrimaryAttack = gpGlobals->curtime;

			PrimaryAttack();
		}
	}

	if ( pOwner->m_nButtons & IN_RELOAD && UsesClipsForAmmo1() && !m_bInReload ) 
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		StartReload();
	}
	else 
	{
		// no fire buttons down
		m_bFireOnEmpty = false;

		if ( !HasAnyAmmo() && m_flNextPrimaryAttack < gpGlobals->curtime ) 
		{
			// weapon isn't useable, switch.
			if ( !(GetWeaponFlags() & ITEM_FLAG_NOAUTOSWITCHEMPTY) && pOwner->SwitchToNextBestWeapon( this ) )
			{
				m_flNextPrimaryAttack = gpGlobals->curtime + 0.3;
				return;
			}
		}
		else
		{
			// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
			if ( m_iClip1 <= 0 && !(GetWeaponFlags() & ITEM_FLAG_NOAUTORELOAD) && m_flNextPrimaryAttack < gpGlobals->curtime )
			{
				if (StartReload())
				{
					// if we've successfully started to reload, we're done
					return;
				}
			}
		}

		WeaponIdle( );
		return;
	}

}

CWeaponBigshotgun::CWeaponBigshotgun( void )
{
	m_bReloadsSingly = true;

	m_bNeedPump		= false;
	m_bReloadEmpty = false;
	m_bDelayedFire1 = false;
	m_bDelayedFire2 = false;

	SetNextThink(0.1f);
	SetThink( &CWeaponBigshotgun::Think );

	m_fMinRange1		= 0.0;
	m_fMaxRange1		= 500;
	m_fMinRange2		= 0.0;
	m_fMaxRange2		= 200;
}

void CWeaponBigshotgun::ItemHolsterFrame( void )
{
}