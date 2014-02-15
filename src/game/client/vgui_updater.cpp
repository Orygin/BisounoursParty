 #include "cbase.h"
 #include "vgui_updater.h"
 using namespace vgui;
 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include <vgui_controls/TextEntry.h>
 #include "Cloud\Nebuleuse.h"
 #include <stdlib.h>


 class CUpdaterPanel : public vgui::Frame
 {
	DECLARE_CLASS_SIMPLE(CUpdaterPanel, vgui::Frame); 

	CUpdaterPanel(vgui::VPANEL parent);
	~CUpdaterPanel(){};

 protected:
	virtual void OnCommand(const char* pcCommand);
	void StartUpdater();
	void Close();
 };

CUpdaterPanel::CUpdaterPanel(vgui::VPANEL parent) : BaseClass(NULL, "UpdaterPanel")
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
 
	if(g_Nebuleuse->IsOutDated())
	{
		SetVisible(true);
	}

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	
	LoadControlSettings("resource/UI/Updater.res");
}
void CUpdaterPanel::OnCommand(const char* pcCommand)
{
	if(!Q_stricmp(pcCommand, "Close"))
		Close();
	if(!Q_stricmp(pcCommand, "Update"))
		StartUpdater();
}
void CUpdaterPanel::StartUpdater()
{	
	char directory[1024];
	const char *start    = "\"";
	const char *pGameDir = engine->GetGameDirectory();
	strcpy(directory, start);
	strcat(directory, pGameDir);
	strcat(directory, "\\bin\\Updater.exe\" -path ");
	strcat(directory, pGameDir);
	strcat(directory, "\\bin\\");

	_popen(directory, "r");
	engine->ClientCmd("quit");
}
void CUpdaterPanel::Close()
{
	SetVisible(false);
}

class CUpdaterPanelInterface : public IUpdaterPanel
{
private:
	CUpdaterPanel *UpdaterPanel;
public:
	CUpdaterPanelInterface()
	{
		UpdaterPanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		UpdaterPanel = new CUpdaterPanel(parent);
	}
	void Destroy()
	{
		if (UpdaterPanel)
		{
			UpdaterPanel->SetParent( (vgui::Panel *)NULL);
			delete UpdaterPanel;
		}
	}
	 void Activate( void )
	{
		if ( Updaterpanel )
		{
			UpdaterPanel->SetVisible(true);
		}
	}
};
static CUpdaterPanelInterface g_UpdaterPanel;
IUpdaterPanel* Updaterpanel = (IUpdaterPanel*)&g_UpdaterPanel;