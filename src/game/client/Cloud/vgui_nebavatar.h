//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef VGUI_NEBAVATAR_H
#define VGUI_NEBAVATAR_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/Frame.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Label.h>
#include "NebuleuseAvatar.h"
#include "c_baseplayer.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CNebAvatar : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CNebAvatar, vgui::Panel);
public:
	CNebAvatar( vgui::Panel parent );

	// Call this to set the steam ID associated with the avatar
	void SetAvatar(unsigned char * Avatar);
	bool IsAvatarSet();
	// Call to Paint the image
	// Image will draw within the current panel context at the specified position
	virtual void Paint( void );
	virtual void PaintBackground(void);
	

	// Set the position of the image
	virtual void SetPos(int x, int y)
	{
		m_nX = x;
		m_nY = y;
	}

	// Gets the size of the content
	virtual void GetContentSize(int &wide, int &tall)
	{
		wide = m_nWide;
		tall = m_nTall;
	}

	// Get the size the image will actually draw in (usually defaults to the content size)
	virtual void GetSize(int &wide, int &tall)
	{
		GetContentSize( wide, tall );
	}

	// Sets the size of the image
	virtual void SetSize(int wide, int tall)	
	{ 
		m_nWide = wide; 
		m_nTall = tall; 
	}

	void SetAvatarSize(int wide, int tall)	
	{
		m_iAvatarWidth = wide;
		m_iAvatarHeight = tall;
		SetBounds(m_nX, m_nY, wide, tall);
	}

	// Set the draw color 
	virtual void SetColor(Color col)			
	{ 
		m_Color = col; 
	}
	virtual void SetBgColor(Color col)
	{
		m_BgColor = col;
	}
	
	int		GetWide( void ) { return m_nWide; }

private:
	Color m_Color;
	Color m_BgColor;
	int m_iTextureID;
	int m_nX, m_nY, m_nWide, m_nTall;
	int	 m_iAvatarWidth;
	int	 m_iAvatarHeight;
	unsigned char * m_Avatar;
	bool m_bSet;
};

#endif // VGUI_NEBAVATAR_H
