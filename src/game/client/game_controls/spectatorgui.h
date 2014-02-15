// BISOUNOURS PARTY : SPECTATOR GUI

#ifndef SPECTATORGUI_H
#define SPECTATORGUI_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/IScheme.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/ComboBox.h>
#include <igameevents.h>
#include "GameEventListener.h"
#include <game/client/iviewport.h>

class KeyValues;

namespace vgui
{
	class TextEntry;
	class Panel;
}

#define BAR_COLOR Color(0, 0, 0, 196)

class IBaseFileSystem;

class CSpectatorGUI : public vgui::EditablePanel, public IViewPortPanel
{
	DECLARE_CLASS_SIMPLE( CSpectatorGUI, vgui::EditablePanel );

public:

	CSpectatorGUI( IViewPort *pViewPort );
	virtual ~CSpectatorGUI();

	virtual const char *GetName( void ) { return PANEL_SPECGUI; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset() {};
	virtual void Update();
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return false; }
	virtual void ShowPanel( bool bShow );
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent(vgui::VPANEL parent) { BaseClass::SetParent(parent); }
	
protected:

	void SetLabelText(const char *textEntryName, const char *text);
	void SetLabelText(const char *textEntryName, wchar_t *text);
	void MoveLabelToFront(const char *textEntryName);

	void UpdatePlayerInfo();
	void UpdateTimer();
	void UpdateMapName();
	void UpdateBestPlayersScores();
	void UpdateTeamsScores();

protected:	

	enum {INSET_OFFSET = 2}; 

	// vgui overrides
	virtual void PerformLayout();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

	vgui::Panel *m_pTopBar;

	vgui::Label *m_pPlayerLabel;
	vgui::Label *m_pTimerLabel;
	vgui::Label *m_pScoreLabel1;
	vgui::Label *m_pScoreLabel2;
	vgui::Label *m_pScoreLabel3;
	vgui::Label *m_pScoreValueLabel1;
	vgui::Label *m_pScoreValueLabel2;
	vgui::Label *m_pScoreValueLabel3;

	IViewPort *m_pViewPort;

public:

		virtual int GetTopBarHeight() { return m_pTopBar->GetTall(); };
};

extern CSpectatorGUI * g_pSpectatorGUI;

#endif // SPECTATORGUI_H
