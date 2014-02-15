
 #include "cbase.h"
 using namespace vgui;
 #include <vgui/IVGui.h>
 #include <vgui_controls/Frame.h>
 #include <vgui_controls/TextEntry.h>
 #include "ienginevgui.h"
 #include "curl/curl.h"
 #include "steam\steam_api.h"
 #include "vgui_bpachievements.h"
 #include "vgui/ILocalize.h"

 #include "Cloud\bp_cloud.h"
 #include "Cloud\bp_achievements.h"

 
 class CBPAchievPanel : public vgui::Frame
 {
 	DECLARE_CLASS_SIMPLE(CBPAchievPanel, vgui::Frame); 

 	CBPAchievPanel(vgui::VPANEL parent);
 	~CBPAchievPanel(){};
 public:
	// void	AddAchievement(char *name, char *desc, char *progress);
	 void	AddStruct(Achievement_struct structure[64], int max);
	 void	RefreshList();

	 	
	char* GetFullName(char *name);
	char* GetFullDesc(char *name);

	char* GetLocalizedName(char *name);
	char* GetLocalizedDesc(char *name);

 protected:
 	virtual void OnTick();
 	virtual void OnCommand(const char* pcCommand);
	void Close();
	//void Refresh();
	void ApplySchemeSettings( IScheme *pScheme );

 private:
	 bool m_bIsInit;
	 KeyValues *m_kv;
	 vgui::ComboBox *m_Combo;
	 vgui::ListPanel *m_pList;
	 Achievement_struct m_pAchievements[64];
	 int m_iMaxAchiev;
 };

 
ConVar seekachievshowpanel("seekachievshowpanel", "0", FCVAR_CLIENTDLL, "OHAI");

CON_COMMAND(OpenAchievPanel,NULL)
{
	seekachievshowpanel.SetValue(1);
}
void CBPAchievPanel::ApplySchemeSettings( IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );

		m_pList->SetFont( pScheme->GetFont( "DefaultVerySmall" ) );
		m_pList->SetColumnSortable(0, false);
		m_pList->SetColumnSortable(1, false);
		m_pList->SetColumnSortable(2, false);
	}
CBPAchievPanel::CBPAchievPanel(vgui::VPANEL parent) : BaseClass(NULL, "AchievPanel")
{
	SetParent( parent );

	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
	SetEnabled(false);
	SetProportional( true );
	SetTitleBarVisible( true );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( true );
	SetSizeable( false );
	SetMoveable( true );
	SetVisible( false );

	m_kv = new KeyValues( "LI" );

	m_pList = new vgui::ListPanel(this, "AchievList");
	m_pList->AddColumnHeader(0, "Unlocked","#Achiev_unlock",20,ListPanel::COLUMN_RESIZEWITHWINDOW);
	m_pList->AddColumnHeader(1, "#Achiev_name","#Achiev_name",200,ListPanel::COLUMN_RESIZEWITHWINDOW);
	m_pList->AddColumnHeader(2, "#Achiev_desc","#Achiev_desc",400,ListPanel::COLUMN_RESIZEWITHWINDOW);

	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	
	LoadControlSettings("resource/UI/Achievements.res");
	
	vgui::ivgui()->AddTickSignal( GetVPanel(), 100 );

	m_bIsInit = false;
}
/*void CBPAchievPanel::AddAchievement(char *name, char *desc, char *progress)
{
	/*m_kv->deleteThis();
	m_kv = new KeyValues( "LI" );*//*

	m_kv->SetString( "Unlocked", progress );
	m_kv->SetString( "#Achiev_name", name );
	m_kv->SetString( "#Achiev_desc", desc );
	
	m_pList->AddItem( m_kv, 0, false, false );
}*/
void CBPAchievPanel::AddStruct(Achievement_struct structure[64], int max)
{
	return;
	//m_pAchievements = structure;
	m_iMaxAchiev = max;
	m_pList->RemoveAll();
	char *progress = new char [3];
	for(int i = 0; i < max;i++)
	{
		m_kv = new KeyValues( "LI" );
		sprintf(progress, "%i", structure[i].Progress);
		m_kv->SetString( "Unlocked", progress );
		m_kv->SetString( "#Achiev_name", GetLocalizedName(structure[i].Name) );
		m_kv->SetString( "#Achiev_desc", GetLocalizedDesc(structure[i].Name) );
	
		m_pList->AddItem( m_kv, 0, false, false );
		m_pAchievements[i] = structure[i];
	}
}
char* CBPAchievPanel::GetFullName(char* name)
{

	if(!name)
		return "";
	char* prefix = "#ACH_NAME_";
		char FullName[256];

	strcpy(FullName, prefix);
	delete prefix;
	return strcat(FullName, name);
}
char* CBPAchievPanel::GetFullDesc(char* name)
{
	if(!name)
		return "";
	char* prefix = "#ACH_DESC_";
	char *FullName = new char[strlen(name) + 10];

	strcpy(FullName, prefix);
	delete prefix;
	return strcat(FullName, name);
}
char* CBPAchievPanel::GetLocalizedName(char *name)
{
	if(!name)
		return "";
	const wchar_t *localized = g_pVGuiLocalize->Find(BPAchievements()->GetFullName(name));
	if(!localized)
		return "Problem with localization";
	char *ToPrint = new char[wcslen(localized)];
	size_t i = 0;
	size_t b = wcslen(localized) + 1;
	wcstombs_s(&i, ToPrint, b, localized, b);

	return ToPrint;
}
char* CBPAchievPanel::GetLocalizedDesc(char *name)
{
	if(!name)
		return "";
	const wchar_t *localized = g_pVGuiLocalize->Find(BPAchievements()->GetFullDesc(name));
	if(!localized)
		return "Problem with localization";
	char *ToPrint = new char[wcslen(localized)];
	size_t i = 0;
	size_t b = wcslen(localized) + 1;
	wcstombs_s(&i, ToPrint, b, localized, b);

	return ToPrint;
}

/*void CBPAchievPanel::RefreshList()
{
		m_pList->RemoveAll();
	char *progress = new char [3];
	for(int i = 0; i < m_iMaxAchiev;i++)
	{
		sprintf(progress, "%i", m_pAchievements[i].Progress);
		m_kv->SetString( "Unlocked", progress );
		m_kv->SetString( "#Achiev_name", m_pAchievements[i].loc_name );
		m_kv->SetString( "#Achiev_desc", m_pAchievements[i].loc_desc );
	
		m_pList->AddItem( m_kv, 0, false, false );
	}
}*/
void CBPAchievPanel::OnTick()
{
	return;
	BaseClass::OnTick();
	if(seekachievshowpanel.GetBool())
	{
		/*if(BPCloud()->IsUnavailable())
		{
			KeyValues *kv = new KeyValues( "LI" );
			kv->SetString( "#Achiev_name", "No cloud" );
			m_pList->AddItem( kv, 0, false, false );
			kv->deleteThis();
		}*/
		
		if(!m_bIsInit)
			m_bIsInit = true;

		SetVisible(true);
	}
	seekachievshowpanel.SetValue(0);
}
void CBPAchievPanel::OnCommand(const char* pcCommand)
{
	return;
	if(!Q_stricmp(pcCommand, "Closeit"))
		Close();
}
void CBPAchievPanel::Close()
{
	SetVisible(false);
}

class CBPAchievPanelInterface : public IAchievPanel
{
private:
	CBPAchievPanel *APanel;
public:
	CBPAchievPanelInterface()
	{
		APanel = NULL;
	}
	void Create(vgui::VPANEL parent)
	{
		APanel = new CBPAchievPanel(parent);
	}
	void Destroy()
	{
		if (APanel)
		{
			APanel->SetParent( (vgui::Panel *)NULL);
			delete APanel;
		}
	}
	void Activate( void )
	{
		if ( achievpanel )
		{
			//feedpanel->Activate();
		}
	}
	 void AddStruct(Achievement_struct structure[64], int max)
	 {
		 APanel->AddStruct(structure, max);
	 }
};
static CBPAchievPanelInterface g_achievPanel;
IAchievPanel* achievpanel = (IAchievPanel*)&g_achievPanel;