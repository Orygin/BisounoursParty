// BISOUNOURS PARTY : HUD (HEALTH)

#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"
#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui/ILocalize.h>

#include "hudelement.h"
#include "hud_numericdisplay.h"

#include "ConVar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

#define INIT_HEALTH -1

class CHudHealth : public CHudElement, public CHudNumericDisplay
{
	DECLARE_CLASS_SIMPLE( CHudHealth, CHudNumericDisplay );

public:
	CHudHealth( const char *pElementName );
	virtual void Init( void );
	virtual void VidInit( void );
	virtual void Reset( void );
	virtual void OnThink();
	void MsgFunc_Damage( bf_read &msg );

private:
	int	m_iHealth;
};	

DECLARE_HUDELEMENT( CHudHealth );
DECLARE_HUD_MESSAGE( CHudHealth, Damage );

CHudHealth::CHudHealth( const char *pElementName ) : CHudElement( pElementName ), CHudNumericDisplay(NULL, "HudHealth")
{
	SetPaintBackgroundEnabled(false);

	SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD);
}

void CHudHealth::Init()
{
	HOOK_HUD_MESSAGE( CHudHealth, Damage );
	SetZPos(1);
	Reset();
	SetPos(122,100);
}

void CHudHealth::Reset()
{
	m_iHealth = INIT_HEALTH;
	SetZPos(1);
	SetDisplayValue(m_iHealth);
	SetPos(122,100);
}

void CHudHealth::VidInit()
{
	Reset();
}

void CHudHealth::OnThink()
{
	int newHealth = 0;

	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
	if ( local )
		// Never below zero
		newHealth = max( local->GetHealth(), 0 );

	// Only update the fade if we've changed health
	if ( newHealth == m_iHealth )
		return;

	m_iHealth = newHealth;
	if ( m_iHealth >= 20 )
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedAbove20");
	else if ( m_iHealth > 0 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedBelow20");
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthLow");
	}

	SetDisplayValue(m_iHealth);
}

void CHudHealth::MsgFunc_Damage( bf_read &msg )
{
	int damageTaken = msg.ReadByte();

	// Actually took damage?
	if ( damageTaken > 0 )
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthDamageTaken");
}