//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef HUD_FORTINFO_PANEL_H
#define HUD_FORTINFO_PANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include "hudelement.h"

using namespace vgui;
enum
{
	TEAM_PINK = 2,
	TEAM_GREEN
};
class CHudFortsInfo : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudFortsInfo, EditablePanel );

public:
	CHudFortsInfo( const char *pElementName );

	virtual void	Init();
	virtual void	ApplySchemeSettings( IScheme *scheme );
	virtual bool	ShouldDraw( void );
	virtual void	PerformLayout( void );
	virtual void	LevelInit( void ) { m_flHideTime = 0; }
	virtual void	FireGameEvent( IGameEvent * event );
	virtual void	OnTick( void );

	void AddNotification( const char *szIconBaseName, const wchar_t *pHeading, const wchar_t *pTitle, CBasePlayer *pPlayer , int plid, char team);

private:
	void ShowNextNotification();
	void SetXAndWide( Panel *pPanel, int x, int wide );

	float m_flHideTime;

	Label *m_pLabelHeading;
	Label *m_pLabelTitle;
	EditablePanel *m_pPanelBackground;
	ImagePanel *m_pIcon;
	bool m_bFirstNot;
	struct Notification_t
	{
		char szIconBaseName[255];
		wchar_t szHeading[255];
		wchar_t szTitle[255];
		CBasePlayer *pPlayer;
		int playerId;
		char team;
	};

	CUtlLinkedList<Notification_t> m_queueNotification;
};

#endif