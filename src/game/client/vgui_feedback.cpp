
 #include "cbase.h"
 #include "vgui_feedback.h"
 using namespace vgui;
 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include <vgui_controls/TextEntry.h>
 #include "ienginevgui.h"
 #include "steam\steam_api.h"


 class CFeedPanel : public vgui::Frame
 {
 	DECLARE_CLASS_SIMPLE(CFeedPanel, vgui::Frame); 

 	CFeedPanel(vgui::VPANEL parent);
 	~CFeedPanel(){};

 protected:
 	virtual void OnTick();
 	virtual void OnCommand(const char* pcCommand);
	void Submit();
	void Close();

 private:
	 float lastThink;
	 vgui::TextEntry* m_tFeed;
	 vgui::Button *m_butSubmit;
 };

ConVar seekfeedbackshowpanel("seekfeedbackshowpanel", "0", FCVAR_CLIENTDLL, "OHAI");

CON_COMMAND(OpenFeedbackPanel,NULL)
{
	seekfeedbackshowpanel.SetValue(1);
}

CFeedPanel::CFeedPanel(vgui::VPANEL parent) : BaseClass(NULL, "FeedPanel")
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

 m_tFeed = new vgui::TextEntry(this, "FeedText");
 
 	/*if(BPCloud()->IsOutDated())
	{
		m_tFeed->SetEditable(false);
		m_tFeed->SetText("Your version of the game is outdated. Please update (no cloud avaliable)");
	}*/
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	
	LoadControlSettings("resource/UI/Feedback.res");
 
	m_tFeed->SetMultiline(true);
	
	vgui::ivgui()->AddTickSignal( GetVPanel(), 100 );
	lastThink = gpGlobals->curtime;
}
ConVar cloud_loopupdate( "bp_cloud_loopupdate", "10", 0, "Allow a thread to think about updates. Do not change if unsure");
void CFeedPanel::OnTick()
{
	BaseClass::OnTick();
	if(seekfeedbackshowpanel.GetBool())
	{
		/*if(BPCloud()->IsUnavailable())
		{
			m_tFeed->SetEditable(false);
			m_tFeed->SetText("Bisounours Party cloud is unavailable");
		}*/
		SetVisible(true);
	}
	seekfeedbackshowpanel.SetValue(0);
	
	if(cloud_loopupdate.GetBool())
	{
		if(lastThink <= gpGlobals->curtime)
		{
			//BPCloud()->UpdateNext();
			lastThink = gpGlobals->curtime + 2;
		}
	}
}
void CFeedPanel::OnCommand(const char* pcCommand)
{

	if(!Q_stricmp(pcCommand, "Closeit"))
		Close();
	if(!Q_stricmp(pcCommand, "Submit"))
		Submit();
}
void CFeedPanel::Submit()
{
	if(BPCloud()->IsUnavailable())
		return;

	DevMsg("Sending feedback\n");

	int length = m_tFeed->GetTextLength() + 1;

	char *feed = new char[length];

	m_tFeed->GetText(feed,length);

	//BPCloud()->SendFeedback(feed);
	//BPAchievements()->EarnAchievement("a001Report_feed");
	
	this->SetVisible(false);
}

void CFeedPanel::Close()
{
	SetVisible(false);
	m_tFeed->SetText("");
}

class CFeedPanelInterface : public IFeedPanel
{
private:
	CFeedPanel *FeedPanel;
public:
	CFeedPanelInterface()
	{
		FeedPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		FeedPanel = new CFeedPanel(parent);
	}
	void Destroy()
	{
		if (FeedPanel)
		{
			FeedPanel->SetParent( (vgui::Panel *)NULL);
			delete FeedPanel;
		}
	}
	 void Activate( void )
	{
		if ( feedpanel )
		{
			//feedpanel->Activate();
		}
	}
};
static CFeedPanelInterface g_FeedPanel;
IFeedPanel* feedpanel = (IFeedPanel*)&g_FeedPanel;