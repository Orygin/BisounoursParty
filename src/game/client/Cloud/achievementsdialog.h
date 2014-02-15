//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef ACHIEVEMENTSDIALOG_H
#define ACHIEVEMENTSDIALOG_H
#ifdef _WIN32
#pragma once
#endif

//#include "basedialog.h"
#include "vgui_controls/panel.h"
#include "vgui_controls/frame.h"
#include "vgui_controls/PanelListPanel.h"
#include "vgui_controls/Label.h"
#include "tier1/KeyValues.h"
#include "TGAImagePanel.h"
//#include "iachievementmgr.h"
#include "baseachievement.h"

class IAchievement;

#define ACHIEVED_ICON_PATH "hud/icon_check.vtf"
#define LOCK_ICON_PATH "hud/icon_locked.vtf"

// Loads an achievement's icon into a specified image panel, or turns the panel off if no achievement icon was found.
bool LoadAchievementIcon( vgui::ImagePanel* pIconPanel, IAchievement *pAchievement, const char *pszExt = NULL );

// Updates a listed achievement item's progress bar. 
void UpdateProgressBar( vgui::EditablePanel* pPanel, IAchievement *pAchievement, Color clrProgressBar );


//////////////////////////////////////////////////////////////////////////// 
// PC version
//////////////////////////////////////////////////////////////////////////
class CAchievementsDialog : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE ( CAchievementsDialog, vgui::Frame );

public:
	CAchievementsDialog( vgui::VPANEL parent );
	~CAchievementsDialog();

	virtual void ApplySchemeSettings( IScheme *pScheme );
	virtual void UpdateAchievementDialogInfo( void );
	virtual void OnCommand( const char* command );
	void UpdateAchievementsData();

	virtual void ApplySettings( KeyValues *pResourceData );
	virtual void OnSizeChanged( int newWide, int newTall );
	virtual void OnTick();
	MESSAGE_FUNC_PARAMS( OnTextChanged, "TextChanged", data );

	void CreateNewAchievementGroup( int iMinRange, int iMaxRange );

	vgui::PanelListPanel	*m_pAchievementsList;
	vgui::ImagePanel		*m_pListBG;

	vgui::ImagePanel		*m_pPercentageBarBackground;
	vgui::ImagePanel		*m_pPercentageBar;

	vgui::ComboBox			*m_pAchievementPackCombo;

	int m_nUnlocked;

	int m_iFixedWidth;

	typedef struct 
	{
		int m_iMinRange;
		int m_iMaxRange;
		int m_iNumAchievements;
		int m_iNumUnlocked;
	} achievement_group_t;

	int m_iNumAchievementGroups;

	achievement_group_t m_AchievementGroups[15];
};

//////////////////////////////////////////////////////////////////////////
// Individual item panel, displaying stats for one achievement
class CAchievementDialogItemPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CAchievementDialogItemPanel, vgui::EditablePanel );

public:
	CAchievementDialogItemPanel( vgui::PanelListPanel *parent, const char* name, int iListItemID );
	~CAchievementDialogItemPanel();

	void SetAchievementInfo ( IAchievement* pAchievement );
	void UpdateAchievementInfo();
	virtual void ApplySchemeSettings( IScheme *pScheme );
	IAchievement* GetSourceAchievement() { return m_pSourceAchievement; }

private:
	IAchievement* m_pSourceAchievement;
	int	m_iSourceAchievementIndex;

	vgui::PanelListPanel *m_pParent;

	vgui::Label *m_pAchievementNameLabel;
	vgui::Label *m_pAchievementDescLabel;
	vgui::Label *m_pPercentageText;

	vgui::ImagePanel *m_pLockedIcon;
	vgui::ImagePanel *m_pAchievementIcon;

	vgui::ImagePanel		*m_pPercentageBarBackground;
	vgui::ImagePanel		*m_pPercentageBar;

	vgui::IScheme			*m_pSchemeSettings;

	CPanelAnimationVar( Color, m_clrProgressBar, "ProgressBarColor", "124 153 63 255" );

	int m_iListItemID;
};

class IAchievementsPanel
{
public:
	virtual void		Create( vgui::VPANEL parent ) = 0;
	virtual void		Destroy( void ) = 0;
	virtual void		Activate( void ) = 0;
	virtual void		Update(void) = 0;
};
extern IAchievementsPanel *AchievementsPanel;
#endif // ACHIEVEMENTSDIALOG_H
