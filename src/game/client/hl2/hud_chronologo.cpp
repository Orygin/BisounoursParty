// BISOUNOURS PARTY : HUD (FORTS LOGO)


#include "cbase.h"
#include "hud.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include <vgui_controls/Panel.h>
#include "hl2mp_gamerules.h"
#include "tier0/memdbgon.h"

using namespace vgui;

class CHudChronoLogo : public CHudElement, public Panel
{
    DECLARE_CLASS_SIMPLE( CHudChronoLogo, Panel );

public:

    CHudChronoLogo( const char *pElementName );
	    virtual void Paint();

protected:
	int m_nLogoFort;
	int m_nLogoBattle;
	float m_flStartTime;
	bool m_bFirst;
};

DECLARE_HUDELEMENT( CHudChronoLogo );

CHudChronoLogo::CHudChronoLogo( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudChronoLogo" )
{
    Panel *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );

    m_nLogoFort = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nLogoFort, "hud/bloc_forts" , true, true);

	m_nLogoBattle = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( m_nLogoBattle, "hud/bloc_battle" , true, true);
	SetEnabled(true);
	SetPaintBackgroundEnabled(false);
	SetPaintEnabled(true);
    SetHiddenBits(HIDEHUD_PLAYERDEAD);
	SetBounds(0,0,640,480);
	m_bFirst = true;
}

void CHudChronoLogo::Paint()
{
	if(m_bFirst)
	{
		m_flStartTime = gpGlobals->curtime + 4.0f;
		m_bFirst = false;
		SetPos(XRES(640)-256-40,YRES(20));
		return;
	}
	else
	{
		if(m_flStartTime > gpGlobals->curtime)
			return;
	}
	try{
		if(HL2MPRules()->GetGameType() == GAME_FORTS )
		{
			int TextureToDraw = m_nLogoFort;
			if( HL2MPRules()->IsFortsBuildPhase() || HL2MPRules()->IsFortsFirstBuildPhase() )
				TextureToDraw = m_nLogoFort;
			else
				TextureToDraw = m_nLogoBattle;

			surface()->DrawSetTexture( TextureToDraw );
			surface()->DrawTexturedRect( 0, 0, 256, 128 );
		}
	}
	catch(...)	{	}
}
