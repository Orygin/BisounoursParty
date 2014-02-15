// BISOUNOURS PARTY : HUD (ARMOR LOGO)

#include "hud.h"
#include "cbase.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include <vgui_controls/Panel.h>

#include "tier0/memdbgon.h"

using namespace vgui;

class CHudArmorLogo : public CHudElement, public Panel
{
    DECLARE_CLASS_SIMPLE( CHudArmorLogo, Panel );

public:

    CHudArmorLogo( const char *pElementName );
	void Init( void );
	void Reset( void );
	void VidInit( void );

protected:

    virtual void Paint();
	int m_nLogo;
};

DECLARE_HUDELEMENT( CHudArmorLogo );

CHudArmorLogo::CHudArmorLogo( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudArmorLogo" )
{
    Panel *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );

    m_nLogo = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_nLogo, "hud/health" , true, true);
	SetPaintBackgroundEnabled(false);

    SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD);
}
void CHudArmorLogo::Init()
{
	SetPos(100,0);
}
void CHudArmorLogo::Reset()
{
	SetPos(100,0);
}
void CHudArmorLogo::VidInit( void )
{
	Reset();
}
void CHudArmorLogo::Paint()
{
	int width = 128;
	int tall = 128;
	surface()->DrawSetTexture( m_nLogo );
    surface()->DrawTexturedRect( 0, 0, width, tall );
}