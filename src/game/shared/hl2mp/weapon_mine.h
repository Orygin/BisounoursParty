// BISOUNOURS PARTY : MINE (weapon)

#ifndef	WEAPONMINE_H
#define	WEAPONMINE_H

#include "weapon_hl2mpbasehlmpcombatweapon.h"

#ifdef CLIENT_DLL
#define CWeaponMine C_WeaponMine
#endif

class CWeaponMine : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponMine, CBaseHL2MPCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CNetworkVar( bool, m_bNeedReload);
	CNetworkVar( bool, m_bAttachMine);

	float				m_flWallSwitchTime;

	void				Spawn( void );
	void				Precache( void );

	void				PrimaryAttack( void );
	void				WeaponIdle( void );
	
	void				SetPickupTouch( void );
	void				MineTouch( CBaseEntity *pOther );
	void				ItemPostFrame( void );	
	bool				Reload( void );
	void				StartAttach( void );
	void				FinishAttach( void );
	void				StartMineAttach( void );
	void				MineAttach( void );

	bool				Deploy( void );
	bool				Holster( CBaseCombatWeapon *pSwitchingTo = NULL );

#ifndef CLIENT_DLL
	void				ChooseMineColor( CNPCMine *pMine );
#endif

	CWeaponMine();

	DECLARE_ACTTABLE();
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

private:
	CWeaponMine( const CWeaponMine & );
};

#endif