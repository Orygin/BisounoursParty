// BISOUNOURS PARTY : HUD (AMMO)

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "hud_numericdisplay.h"
#include "ihudlcd.h"
#include "iclientmode.h"

#include <vgui_controls/AnimationController.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define AMMO_INIT -1

using namespace vgui;

class CHudAmmo : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudAmmo, CHudNumericDisplay );

public:
	CHudAmmo( const char *pElementName );
	void Init( void );
	void Reset();
	void SetAmmo(int ammo, bool playAnimation);
	void SetAmmo2(int ammo2, bool playAnimation);

protected:
	virtual void OnThink();
	void UpdatePlayerAmmo( C_BasePlayer *player );
	
private:
	CHandle< C_BaseCombatWeapon > m_hCurrentActiveWeapon;
	int	m_iAmmo;
	int	m_iAmmo2;
	bool firstpaint;
};

DECLARE_HUDELEMENT( CHudAmmo );

CHudAmmo::CHudAmmo( const char *pElementName ) : BaseClass(NULL, "HudAmmo"), CHudElement( pElementName )
{
	SetPaintBackgroundEnabled(false);

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_WEAPONSELECTION );
		firstpaint = true;
}
void CHudAmmo::Init( void )
{
	m_iAmmo	= AMMO_INIT;
	m_iAmmo2 = AMMO_INIT;
	SetZPos(1);
}

void CHudAmmo::Reset()
{
	m_hCurrentActiveWeapon = NULL;
	m_iAmmo = 0;
	m_iAmmo2 = 0;

	UpdatePlayerAmmo(C_BasePlayer::GetLocalPlayer());
}

void CHudAmmo::UpdatePlayerAmmo( C_BasePlayer *player )
{
	C_BaseCombatWeapon *wpn = GetActiveWeapon();
	if ( !wpn || !player )
		return;

	// get the ammo in our clip
	int ammo1 = wpn->Clip1(), ammo2;
	if (ammo1 < 0)
	{
		// we don't use clip ammo, just use the total ammo count
		ammo1 = player->GetAmmoCount(wpn->GetPrimaryAmmoType());
		ammo2 = 0;
	}
	else
		// we use clip ammo, so the second ammo is the total ammo
		ammo2 = player->GetAmmoCount(wpn->GetPrimaryAmmoType());

	if (wpn == m_hCurrentActiveWeapon)
	{
		// same weapon, just update counts
		SetAmmo(ammo1, true);
		SetAmmo2(ammo2, true);
	}
	else
	{
		// diferent weapon, change without triggering
		SetAmmo(ammo1, false);
		SetAmmo2(ammo2, false);

		//BP vérifie quelles valeurs doivent être affichées
		if(wpn->UsesPrimaryAmmo()){
			SetShouldDisplayValue(true);
			SetShouldDisplaySecondaryValue(true);
		}
		else{
			SetShouldDisplayValue(false);
			SetShouldDisplaySecondaryValue(false);
		}
		if (!strcmp(wpn->GetName(), "weapon_grenade") || !strcmp(wpn->GetName(), "weapon_bazooka") )
			SetShouldDisplaySecondaryValue(false);

		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("WeaponChanged");

		m_hCurrentActiveWeapon = wpn;
	}
}

void CHudAmmo::OnThink()
{
		if(firstpaint)
	{
		firstpaint = false;
		SetPos(80,YRES(480)-125);
	}
	
	UpdatePlayerAmmo(C_BasePlayer::GetLocalPlayer());
}

void CHudAmmo::SetAmmo(int ammo, bool playAnimation)
{
	if (ammo != m_iAmmo)
	{
		if (ammo == 0)
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoEmpty");
		else if (ammo < m_iAmmo)
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoDecreased");
		else if (ammo < -2)
			return;
		else
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoIncreased");

		m_iAmmo = ammo;
	}
	else if (ammo < -2)
			return;
	
	SetDisplayValue(ammo);
}

void CHudAmmo::SetAmmo2(int ammo2, bool playAnimation)
{
	if (ammo2 != m_iAmmo2)
	{
		if (ammo2 == 0)
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("Ammo2Empty");
		else if (ammo2 < m_iAmmo2)
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("Ammo2Decreased");
		else if (ammo2 < 0)
			return;
		else
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("Ammo2Increased");

		m_iAmmo2 = ammo2;
	}

	SetSecondaryValue(ammo2);
}

class CHudSecondaryAmmo : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE( CHudSecondaryAmmo, CHudNumericDisplay );

public:

	CHudSecondaryAmmo( const char *pElementName );
	void SetAmmo( int ammo );
	void Reset();

protected:

	void OnThink();
	void UpdateAmmoState();
	
private:

	CHandle< C_BaseCombatWeapon > m_hCurrentActiveWeapon;
	int	m_iAmmo;
		bool firstpaint;
};

DECLARE_HUDELEMENT( CHudSecondaryAmmo );

CHudSecondaryAmmo::CHudSecondaryAmmo( const char *pElementName ) : BaseClass( NULL, "HudAmmoSecondary" ), CHudElement( pElementName )
{
	m_iAmmo = AMMO_INIT;

	SetPaintBackgroundEnabled(false);
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_WEAPONSELECTION | HIDEHUD_PLAYERDEAD );
	this->SetZPos(1);
	firstpaint = true;
}

void CHudSecondaryAmmo::SetAmmo( int ammo )
{
	if (ammo != m_iAmmo)
	{
		if (ammo == 0)
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoSecondaryEmpty");
		else if (ammo < m_iAmmo)
			// ammo has decreased
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoSecondaryDecreased");
		else
			// ammunition has increased
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("AmmoSecondaryIncreased");

		m_iAmmo = ammo;
	}
	
	SetDisplayValue( ammo );
}

void CHudSecondaryAmmo::Reset()
{
	m_iAmmo = 0;
	m_hCurrentActiveWeapon = NULL;

	UpdateAmmoState();
}


void CHudSecondaryAmmo::OnThink()
{
	C_BaseCombatWeapon *wpn = GetActiveWeapon();
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if (!wpn || !player )
	{
		m_hCurrentActiveWeapon = NULL;

		return;
	}
	if(firstpaint)
	{
		firstpaint = false;
		SetPos(80,YRES(480)-125);
	}

	UpdateAmmoState();
}

void CHudSecondaryAmmo::UpdateAmmoState()
{
	C_BaseCombatWeapon *wpn = GetActiveWeapon();
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if (player && wpn && wpn->UsesSecondaryAmmo())
		SetAmmo(player->GetAmmoCount(wpn->GetSecondaryAmmoType()));

	if ( m_hCurrentActiveWeapon != wpn )
	{
		//BP vérifie si la valeur a besoin d'être affichée
		if(wpn->UsesSecondaryAmmo())
			SetShouldDisplayValue(true);
		else
			SetShouldDisplayValue(false);

		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("WeaponChanged");

		m_hCurrentActiveWeapon = wpn;
	}
}