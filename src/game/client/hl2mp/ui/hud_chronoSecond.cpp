// BISOUNOURS PARTY : HUD (CHRONO SECONDS)

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
#include <vgui_controls/Label.h>

#include "hudelement.h"
#include "hud_numericdisplay.h"

#include "ConVar.h"
#include "hl2mp_gamerules.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

class CHudChronoSeconds : public CHudElement, public CHudNumericDisplay
{
	DECLARE_CLASS_SIMPLE( CHudChronoSeconds, CHudNumericDisplay );

public:
	CHudChronoSeconds( const char *pElementName );
	virtual void Init( void );
	virtual void VidInit( void );
	virtual void Reset( void );
	virtual void OnThink();

private:
	float	m_iTime;
	float m_flStartTime;
	bool m_bFirst;
};	

DECLARE_HUDELEMENT( CHudChronoSeconds );

CHudChronoSeconds::CHudChronoSeconds( const char *pElementName ) : CHudElement( pElementName ), CHudNumericDisplay(NULL, "HudChronoSeconds")
{
	SetPaintBackgroundEnabled(false);
	//this->SetEnabled(false);
	SetHiddenBits(HIDEHUD_PLAYERDEAD);
	SetBounds(0,0,640,480);
	m_bFirst = true;
	m_flStartTime= 0;
}

void CHudChronoSeconds::Init()
{
	Reset();
}

void CHudChronoSeconds::Reset()
{
	m_iTime = 0;
}

void CHudChronoSeconds::VidInit()
{
	Reset();
}

void CHudChronoSeconds::OnThink()
{
		if(m_bFirst)
	{
		m_flStartTime = gpGlobals->curtime + 4.0f;
		m_bFirst = false;
		SetPos(XRES(640)-180,YRES(7));
		return;
	}
	else
	{
		if(m_flStartTime > gpGlobals->curtime)
			return;
	}
	try{
		if(HL2MPRules()->GetGameType() == GAME_FORTS)
		{
		
			m_iTime = HL2MPRules()->GetPhaseTimeLeft();
			if(!_finite(m_iTime))
				m_iTime = 0;
			//Assert(0);
			SetDisplayValue(m_iTime);
			SetPaintEnabled(false);
		}
		else
		{
			m_iTime = HL2MPRules()->GetMapRemainingTime();
			int m_iMinute = floor(m_iTime / 60) * 60;
			m_iTime -= m_iMinute;
			if(m_iTime < 0)
				m_iTime =0;
			SetDisplayValue(m_iTime);
			SetPaintEnabled(true);
		}
	}

	catch(...)
	{
		return;
	}
}