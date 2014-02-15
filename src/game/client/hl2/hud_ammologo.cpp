// BISOUNOURS PARTY : HUD (AMMO LOGO)


#include "cbase.h"
#include "hud.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include <vgui_controls/Panel.h>

#include "tier0/memdbgon.h"

using namespace vgui;

class CHudAmmoLogo : public CHudElement, public Panel
{
    DECLARE_CLASS_SIMPLE( CHudAmmoLogo, Panel );

public:

    CHudAmmoLogo( const char *pElementName );

protected:

    virtual void Paint();
	int m_nLogo;
	bool firstpaint;
};

DECLARE_HUDELEMENT( CHudAmmoLogo );

CHudAmmoLogo::CHudAmmoLogo( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudAmmoLogo" )
{
    Panel *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );

    m_nLogo = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_nLogo, "hud/cloud_ammo" , true, true);
	SetPaintEnabled(true);
	SetPaintBackgroundEnabled(false);
	this->SetAlpha(100);
    SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD);
	
	firstpaint = true;
}

void CHudAmmoLogo::Paint()
{
	if(firstpaint)
	{
		firstpaint = false;
		SetPos(0,YRES(480)-240);
	}
	int width = 256;
	int tall =  256;
	
	surface()->DrawSetTexture( m_nLogo );
    surface()->DrawTexturedRect( 0, 0, width, tall );
}