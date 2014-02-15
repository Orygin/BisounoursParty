#include "cbase.h"
#include "vgui_PrivacyNotif.h"
using namespace vgui;
#include <vgui/IVGui.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/TextEntry.h>
#include "Cloud\Nebuleuse.h"
#include "Cloud\achievementsdialog.h"
#include <stdlib.h>

class CPrivacyPanel : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CPrivacyPanel, vgui::Frame); 

	CPrivacyPanel(vgui::VPANEL parent);
	~CPrivacyPanel(){};

protected:
	virtual void OnCommand(const char* pcCommand);
	void ApplySchemeSettings(vgui::IScheme* pScheme);
};

static ConVar bp_Privacy("bp_show_privacy", "1", FCVAR_ARCHIVE);

CPrivacyPanel::CPrivacyPanel(vgui::VPANEL parent) : BaseClass(NULL, "PrivacyPanel")
{
	SetParent( parent );
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
 
	SetProportional( true );
	SetTitleBarVisible( true );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( true );
	SetSizeable( false );
	SetMoveable( true );
	SetVisible( true ); //need to be true for ApplySchemeSettings to be called
 
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	
	LoadControlSettings("resource/UI/Privacy.res");
}
void CPrivacyPanel::OnCommand(const char* pcCommand)
{
	if(!Q_stricmp(pcCommand, "Enable")){
		g_Nebuleuse->Init();
		g_pCVar->FindVar( "bp_nebuleuse_enabled" )->SetValue(1);
		bp_Privacy.SetValue(0);
		SetVisible(false);
	}
	if(!Q_stricmp(pcCommand, "Disable")){
		g_Nebuleuse->SetState(NEBULEUSE_NOTCONNECTED);
		g_pCVar->FindVar( "bp_nebuleuse_enabled" )->SetValue(0);
		bp_Privacy.SetValue(0);
		SetVisible(false);
	}
}
void CPrivacyPanel::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	if(bp_Privacy.GetBool())
		SetVisible(true);
	else
		SetVisible( false );
}
class CPrivacyPanelInterface : public IPrivacyPanel
{
private:
	CPrivacyPanel *PrivacyPanel;
public:
	CPrivacyPanelInterface()
	{
		PrivacyPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		PrivacyPanel = new CPrivacyPanel(parent);
	}
	void Destroy()
	{
		if (PrivacyPanel)
		{
			PrivacyPanel->SetParent( (vgui::Panel *)NULL);
			delete PrivacyPanel;
		}
	}
	 void Activate( void )
	{
		if ( Privacypanel )
		{
			PrivacyPanel->SetVisible(true);
		}
	}
};
static CPrivacyPanelInterface g_PrivacyPanel;
IPrivacyPanel* Privacypanel = (IPrivacyPanel*)&g_PrivacyPanel;