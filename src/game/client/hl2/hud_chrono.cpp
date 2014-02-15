// BISOUNOURS PARTY : HUD (CHRONO)

#include "cbase.h"
#include "hud.h"

#include "iclientmode.h"
#include <vgui/IVGUI.h>
#include "hud_macros.h"
#include "hudelement.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Label.h>
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"

#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

class CHudChrono : public CHudElement, public Panel
{
	DECLARE_CLASS_SIMPLE( CHudChrono, Panel );

public:
	CHudChrono( const char *pElementName );
	virtual void OnThink();
	virtual void ApplySchemeSettings( IScheme *scheme );
	void Reset( void );
	void VidInit( void );

private:
	float m_flStartTime;
	bool m_bFirst;
	Label *m_pDisplay;
};	

DECLARE_HUDELEMENT( CHudChrono );

CHudChrono::CHudChrono( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudChrono" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
	//vgui::ivgui()->AddTickSignal( GetVPanel(), 1000 );

	SetPaintBackgroundEnabled(false);
	SetHiddenBits(HIDEHUD_PLAYERDEAD);

	m_bFirst = true;
	m_flStartTime= 0;
	m_pDisplay = new Label(this, "Display", "");
}

void CHudChrono::ApplySchemeSettings( IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );
	m_pDisplay->SetFont(pScheme->GetFont("HudChrono"));
	m_pDisplay->SetSize(220,100);
	SetPos(XRES(640)-200,10);
	SetZPos(1);
}
void CHudChrono::Reset()
{
	m_pDisplay->SetSize(220,100);
	SetPos(XRES(640)-200,10);
	SetZPos(1);
}
void CHudChrono::VidInit( void )
{
	Reset();
}


void CHudChrono::OnThink()
{
	if(m_bFirst)
	{
		m_flStartTime = gpGlobals->curtime + 4.0f;
		m_bFirst = false;
		return;
	}
	else
	{
		if(m_flStartTime > gpGlobals->curtime)
			return;
	}
	if(HL2MPRules()->GetGameType() != GAME_FORTS)
	{
		int seconds = 0;

		if(HL2MPRules()->IsHumiliation())
			seconds = HL2MPRules()->GetHumiliationEndTime() - gpGlobals->curtime;
		else
			seconds = HL2MPRules()->GetMapRemainingTime();

		if(seconds <= 0)
		{
			m_pDisplay->SetText("");
			return;
		}
		int minutes = floor((float)(seconds / 60));
		if(seconds < 0)
			seconds = 0;
		seconds = seconds - (minutes * 60);
					
		char DisplayText[128];

		if(seconds < 10)
			sprintf_s(DisplayText, 128, "%i : 0%i", minutes, seconds);
		else
			sprintf_s(DisplayText, 128, "%i : %i", minutes, seconds);

		m_pDisplay->SetText(DisplayText);
	}
	else
	{
		m_pDisplay->SetText("");
	}
}