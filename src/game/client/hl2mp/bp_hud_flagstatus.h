//========= Copyright © 1996-2007, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef _HUD_FLAGSTATUS_H
#define _HUD_FLAGSTATUS_H
#ifdef _WIN32
#pragma once
#endif

#include "gameeventlistener.h"
#include <vgui_controls/EditablePanel.h>

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose:  Draws the rotated arrow panels
//-----------------------------------------------------------------------------
class CArrowPanel : public EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CArrowPanel, EditablePanel );

	CArrowPanel( vgui::Panel *parent, const char *name );
	virtual void Paint();
	virtual bool IsVisible( void );
	void SetEntity( EHANDLE hEntity ){ m_hEntity = hEntity; }
	float GetAngleRotation( void );

private:

	EHANDLE				m_hEntity;	

	CMaterialReference	m_RedMaterial;
	CMaterialReference	m_BlueMaterial;
	CMaterialReference	m_NeutralMaterial;

	CMaterialReference	m_RedMaterialNoArrow;
	CMaterialReference	m_BlueMaterialNoArrow;
};

//-----------------------------------------------------------------------------
// Purpose:  
//-----------------------------------------------------------------------------
class CFlagStatus : public vgui::EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CFlagStatus, vgui::EditablePanel );

	CFlagStatus( vgui::Panel *parent, const char *name );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual bool IsVisible( void );
	void UpdateStatus( void );

	void SetEntity( EHANDLE hEntity )
	{ 
		Msg("New flag entity\n");
		m_hEntity = hEntity;

		if ( m_pArrow )
		{
			m_pArrow->SetEntity( hEntity );
		}
	}
	void OnThink();
private:

	EHANDLE			m_hEntity;

	CArrowPanel	*m_pArrow;
};

#endif	// _HUD_FLAGSTATUS_H
