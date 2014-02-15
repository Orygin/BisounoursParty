//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include <vgui_controls/Controls.h>
#include <vgui_controls/Panel.h>
#include <vgui/ISurface.h>
#include "vgui_nebavatar.h"

#define AVATARBODREDWIDTH 4
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CNebAvatar::CNebAvatar( vgui::Panel parent )
{
	m_nX = 0;
	m_nY = 0;
	m_nWide = 256;
	m_nTall = 256;
	m_BgColor = Color(255,255,255,0);
	m_bSet = false;
	SetPaintBackgroundEnabled(true);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNebAvatar::SetAvatar( unsigned char * Avatar )
{
	if(!Avatar)
		return;

	m_Avatar = Avatar;

	m_iTextureID = vgui::surface()->CreateNewTextureID( true );
	vgui::surface()->DrawSetTextureRGBA( m_iTextureID, m_Avatar, m_nWide, m_nTall, false, false );
	
	m_Color = Color( 255, 255, 255, 255 );
	m_bSet = true;
}
bool CNebAvatar::IsAvatarSet()
{
	return m_bSet;
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNebAvatar::Paint( void )
{
	PaintBackground();

	vgui::surface()->DrawSetColor( m_Color );
	vgui::surface()->DrawSetTexture( m_iTextureID );
	vgui::surface()->DrawTexturedRect( m_nX , m_nY, m_nX + (m_iAvatarWidth), m_nY + (m_iAvatarHeight) );
}
void CNebAvatar::PaintBackground()
{
	surface()->DrawSetColor(m_BgColor);
	surface()->DrawFilledRect( m_nX - AVATARBODREDWIDTH , m_nY - AVATARBODREDWIDTH , m_nX + AVATARBODREDWIDTH + m_iAvatarWidth, m_nY + AVATARBODREDWIDTH + m_iAvatarHeight);
}