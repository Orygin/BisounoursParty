
 #include "cbase.h"
 #include "vgui_PlayPanel.h"
 using namespace vgui;
 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include <vgui_controls/TextEntry.h>
 #include "ienginevgui.h"
 #include "steam\steam_api.h"

 #include "Cloud\bp_cloud.h"
 #include "Cloud\bp_achievements.h"

 class CPlayPanel : public vgui::Frame
 {
	DECLARE_CLASS_SIMPLE(CPlayPanel, vgui::Frame); 

	CPlayPanel(vgui::VPANEL parent);
	~CPlayPanel(){};

 protected:
	virtual void OnTick();
	virtual void OnCommand(const char* pcCommand);
	void Close();

 private:
	 float lastThink;
 };

ConVar seekplayshowpanel("seekplayshowpanel", "0", FCVAR_CLIENTDLL, "OHAI");

CON_COMMAND(OpenPlaybackPanel,NULL)
{
	seekplayshowpanel.SetValue(1);
}

CPlayPanel::CPlayPanel(vgui::VPANEL parent) : BaseClass(NULL, "PlayPanel")
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
	SetVisible( false );
 
	/*if(BPCloud()->IsOutDated())
	{
		m_tPlay->SetEditable(false);
		m_tPlay->SetText("Your version of the game is outdated. Please update (no cloud avaliable)");
	}*/
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	
	LoadControlSettings("resource/UI/PlayPanel.res");
	
	vgui::ivgui()->AddTickSignal( GetVPanel(), 100 );
	lastThink = gpGlobals->curtime;
}
void CPlayPanel::OnTick()
{
	BaseClass::OnTick();
	if(seekplayshowpanel.GetBool())
	{
		/*if(BPCloud()->IsUnavailable())
		{
			m_tPlay->SetEditable(false);
			m_tPlay->SetText("Bisounours Party cloud is unavailable");
		}*/
		SetVisible(true);

		ipanel()->MoveToFront(GetVPanel());
	}
	seekplayshowpanel.SetValue(0);
}
void CPlayPanel::OnCommand(const char* pcCommand)
{
	if(!Q_stricmp(pcCommand, "Close"))
		Close();
	else if(!Q_stricmp(pcCommand, "ServerBrowser"))
		engine->ClientCmd("gamemenucommand openserverbrowser");
	else if(!Q_stricmp(pcCommand, "Host"))
		engine->ClientCmd("gamemenucommand opencreatemultiplayergamedialog");
	Close();
}

void CPlayPanel::Close()
{
	SetVisible(false);
}

class CPlayPanelInterface : public IPlayPanel
{
private:
	CPlayPanel *PlayPanel;
public:
	CPlayPanelInterface()
	{
		PlayPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		PlayPanel = new CPlayPanel(parent);
	}
	void Destroy()
	{
		if (PlayPanel)
		{
			PlayPanel->SetParent( (vgui::Panel *)NULL);
			delete PlayPanel;
		}
	}
	 void Activate( void )
	{
		if ( playpanel )
		{
			//playpanel->Activate();
		}
	}
};
static CPlayPanelInterface g_PlayPanel;
IPlayPanel* playpanel = (IPlayPanel*)&g_PlayPanel;