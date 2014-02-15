// BISOUNOURS PARTY : HUD (HEALTH LOGO)

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

class CHudHealthLogo : public CHudElement, public Panel
{
    DECLARE_CLASS_SIMPLE( CHudHealthLogo, Panel );

public:

    CHudHealthLogo( const char *pElementName );
protected:

    virtual void Paint();
	int m_iLogo1;
	int m_iLogo2;
	int m_iLogo3;
	int m_iLogo4;
	int m_iLogo5;
	int m_iLogo6;
};

DECLARE_HUDELEMENT( CHudHealthLogo );

CHudHealthLogo::CHudHealthLogo( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudHealthLogo" )
{

    Panel *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );

    m_iLogo1 = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iLogo1, "hud/visage_1" , true, true);

	m_iLogo2 = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iLogo2, "hud/visage_2" , true, true);

	m_iLogo3 = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iLogo3, "hud/visage_3" , true, true);

	m_iLogo4 = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iLogo4, "hud/visage_4" , true, true);

	m_iLogo5 = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iLogo5, "hud/visage_5" , true, true);

	m_iLogo6 = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iLogo6, "hud/visage_6" , true, true);

    SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD);
}
void CHudHealthLogo::Paint()
{
	int iTexture = m_iLogo1;
	//Choix de la texture à afficher en fonction de l'état de santé
	CBasePlayer* local = C_BasePlayer::GetLocalPlayer();
	if(local){
		if(local->GetHealth() > 100)
			iTexture = m_iLogo6;
		else if(local->GetHealth() > 80)
			iTexture = m_iLogo5;
		else if(local->GetHealth() > 60)
			iTexture = m_iLogo4;
		else if(local->GetHealth() > 40)
			iTexture = m_iLogo3;
		else if(local->GetHealth() > 20)
			iTexture = m_iLogo2;
		else
			iTexture = m_iLogo1;
	}
	int width = 128;
	int tall = 128;
	surface()->DrawSetTexture( iTexture );
    surface()->DrawTexturedRect( 0, 0, width, tall );
}