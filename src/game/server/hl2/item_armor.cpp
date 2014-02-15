//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements health kits and wall mounted health chargers.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "gamerules.h"
#include "player.h"
#include "items.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"
#include "autovocals.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define ARMOR_MODEL "models/items/armure.mdl"

class CArmor : public CItem
{
public:
	DECLARE_CLASS( CArmor, CItem );

	void Spawn( void );
	void Precache( void );
	bool MyTouch( CBasePlayer *pPlayer);
	bool SetArmor(CBasePlayer *pPlayer);
};

class CArmorMedium : public CArmor
{
public:
	DECLARE_CLASS( CArmorMedium, CArmor );
};

class CArmorLarge : public CArmor
{
public:
	DECLARE_CLASS( CArmorLarge, CArmor );
};

LINK_ENTITY_TO_CLASS( item_armor_small, CArmor );
PRECACHE_REGISTER(item_armor_small);
LINK_ENTITY_TO_CLASS( item_armor_medium, CArmorMedium );
PRECACHE_REGISTER(item_armor_medium);
LINK_ENTITY_TO_CLASS( item_armor_large, CArmorLarge );
PRECACHE_REGISTER(item_armor_large);

void CArmor::Spawn( void )
{
	Precache();
	SetModel( ARMOR_MODEL );

	if(Q_strcmp(this->GetClassname(), "item_armor_small") == 0)
		m_nSkin = 1;
	else if(Q_strcmp(this->GetClassname(), "item_armor_medium") == 0)
		m_nSkin = 2;
	else
		m_nSkin = 3;

	BaseClass::Spawn();
}

void CArmor::Precache( void )
{
	PrecacheModel(ARMOR_MODEL);
	if(Q_strcmp(this->GetClassname(), "item_armor_small") == 0)
		PrecacheScriptSound( "Armor_Small.Touch" );
	else if(Q_strcmp(this->GetClassname(), "item_armor_medium") == 0)
		PrecacheScriptSound( "Armor_Medium.Touch" );
	else
		PrecacheScriptSound( "Armor_Large.Touch" );
}

bool CArmor::MyTouch( CBasePlayer *pPlayer )
{
	if ( SetArmor(pPlayer) )
	{
		CSingleUserRecipientFilter user( pPlayer );
		user.MakeReliable();

		UserMessageBegin( user, "ItemPickup" );
			WRITE_STRING( GetClassname() );
		MessageEnd();

		if(Q_strcmp(this->GetClassname(), "item_armor_small") == 0)
		{
			CPASAttenuationFilter filter( pPlayer, "Armor_Small.Touch" );
			EmitSound( filter, pPlayer->entindex(), "Armor_Small.Touch" );
		}
		else if(Q_strcmp(this->GetClassname(), "item_armor_medium") == 0)
		{
			CPASAttenuationFilter filter( pPlayer, "Armor_Medium.Touch" );
			EmitSound( filter, pPlayer->entindex(), "Armor_Medium.Touch" );
		}
		else
		{
			CPASAttenuationFilter filter( pPlayer, "Armor_Large.Touch" );
			EmitSound( filter, pPlayer->entindex(), "Armor_Large.Touch" );

		}

		if ( g_pGameRules->ItemShouldRespawn( this ) )
		{
			Respawn();
		}
		else
		{
			UTIL_Remove(this);	
		}

		return true;
	}

	return false;
}

bool CArmor::SetArmor(CBasePlayer *pPlayer)
{
	int newArmorValue = 0;
	if(Q_strcmp(this->GetClassname(), "item_armor_small") == 0)
		newArmorValue = clamp(pPlayer->ArmorValue()+50, 0, 100);
	else if(Q_strcmp(this->GetClassname(), "item_armor_medium") == 0)
		newArmorValue = clamp(pPlayer->ArmorValue()+100, 0, 150);
	else
		newArmorValue = clamp(pPlayer->ArmorValue()+150, 0, 200);

	if(pPlayer->ArmorValue() < newArmorValue)
	{
		pPlayer->SetArmorValue(newArmorValue);
		if(newArmorValue >= 150)
			pPlayer->PlayAutovocal(ARMORPICKUP);
		return true;
	}
	else
		return false;
}