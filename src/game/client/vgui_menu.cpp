/*
Bisounours Party Menu.
Original file by Louis Orygin Geuten.

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must
	not claim that you wrote the original software. If you use this
	software in a product, an acknowledgment in the product documentation
	would be appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#include "cbase.h"
#include "vgui_menu.h"
#include "vgui_controls/Frame.h"
#include <vgui/ISurface.h>
#include <vgui\ISystem.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Label.h>

#include "vgui_controls/Button.h"
#include "vgui_controls/ImagePanel.h"
#include "steam\steam_api.h"
#include "vgui_avatarimage.h"
#include "vgui_bitmapbutton.h"
#include "MGUI_BorderlessButton.h"
#include "Cloud/Nebuleuse.h"
#include "NebuleuseAvatar.h"
#include "Cloud\vgui_nebavatar.h"
#include "vgui_updater.h"

using namespace vgui;
 
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BUTTONDISTANCE 15
#define SUBUPDATERATE 60

//-----------------------------------------------------------------------------
// Purpose: Displays the logo panel
//-----------------------------------------------------------------------------
class CMainMenuButtons : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMainMenuButtons, vgui::Frame);
	public:
	CMainMenuButtons( vgui::VPANEL parent );
	~CMainMenuButtons();

	virtual void OnScreenSizeChanged(int i, int e)	{ InitPos(); }
	virtual void OnCommand(const char *command);
	virtual void Paint();
	virtual void InitPos();

	virtual void PaintBackground()
	{
		SetBgColor(Color(0,0,0,0));
		SetPaintBackgroundType( 0 );
		BaseClass::PaintBackground();
	}
	bool InGame()
	{
		C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
 
		if(pPlayer && IsVisible())
		{ // We do not want to show the ingame menu when we are not really in a game.
			return !(!strcmp(engine->GetLevelName(), "maps/background02.bsp") || !strcmp(engine->GetLevelName(), "maps/background01.bsp") || !strcmp(engine->GetLevelName(), "maps/background03.bsp"));
		} else {
			return false;
		}
	}
		virtual void ApplySchemeSettings( vgui::IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );
		m_pButtonLeave->SetFont(pScheme->GetFont("MenuEntry"));
		m_pButtonOptions->SetFont(pScheme->GetFont("MenuEntry"));
		m_pButtonAchievements->SetFont(pScheme->GetFont("MenuEntry"));
		m_pButtonPlay->SetFont(pScheme->GetFont("MenuEntry"));
		m_pButtonResume->SetFont(pScheme->GetFont("MenuEntry"));
		m_pButtonChangeTeam->SetFont(pScheme->GetFont("MenuEntry"));
		ComputeHeadings(m_pButtonLeave);
		ComputeHeadings(m_pButtonOptions);
		ComputeHeadings(m_pButtonAchievements);
		ComputeHeadings(m_pButtonPlay);
		ComputeHeadings(m_pButtonChangeTeam);
		ComputeHeadings(m_pButtonResume);
	}
	void ComputeHeadings(MButtonBorderless *button);
private:
	MButtonBorderless *m_pButtonPlay;
	MButtonBorderless *m_pButtonResume;
	vgui::Button *m_pButtonMute;
	MButtonBorderless *m_pButtonOptions;
	MButtonBorderless *m_pButtonAchievements;
	MButtonBorderless *m_pButtonLeave;
	MButtonBorderless *m_pButtonChangeTeam;
	vgui::ImagePanel *m_pImgCadre2;
	bool InGameLayout;
};
CMainMenuButtons::CMainMenuButtons( vgui::VPANEL parent ) : BaseClass( NULL, "CMainMenuButtons" )
{
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme"));
	SetParent( parent );
	SetTitleBarVisible( false );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( false );
	SetSizeable( false );
	SetMoveable( false );
	SetProportional( true );
	SetVisible( true );
	SetMouseInputEnabled(true);
	SetKeyBoardInputEnabled(true);
	InGameLayout = false;

	m_pImgCadre2 = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Cadre2"));

	m_pButtonLeave = new MButtonBorderless(this);
	m_pButtonLeave->SetCommand("Leave");
	m_pButtonLeave->SetText("#bp_Quit");
	m_pButtonLeave->SetCursor(dc_hand);
	m_pButtonLeave->SetMouseInputEnabled(true);

	m_pButtonOptions = new MButtonBorderless(this);
	m_pButtonOptions->SetCommand("Options");
	m_pButtonOptions->SetText("#bp_Options");
	m_pButtonOptions->SetCursor(dc_hand);
	m_pButtonOptions->SetMouseInputEnabled(true);
	
	m_pButtonAchievements = new MButtonBorderless(this);
	m_pButtonAchievements->SetCommand("Achievements");
	m_pButtonAchievements->SetText("#bp_Achievements");
	m_pButtonAchievements->SetCursor(dc_hand);
	m_pButtonAchievements->SetMouseInputEnabled(true);

	m_pButtonPlay= new MButtonBorderless(this);
	m_pButtonPlay->SetCommand("Play");
	m_pButtonPlay->SetText("#Bp_Play");
	m_pButtonPlay->SetCursor(dc_hand);
	m_pButtonPlay->SetMouseInputEnabled(true);

	m_pButtonResume= new MButtonBorderless(this);
	m_pButtonResume->SetCommand("Resume");
	m_pButtonResume->SetText("#bp_ResumeGame");
	m_pButtonResume->SetPaintEnabled(false);
	m_pButtonResume->SetEnabled(false);
	m_pButtonResume->SetMouseInputEnabled(true);

	m_pButtonChangeTeam= new MButtonBorderless(this);
	m_pButtonChangeTeam->SetCommand("Changeteam");
	m_pButtonChangeTeam->SetText("#Bp_team_join_swap");
	m_pButtonChangeTeam->SetPaintEnabled(false);
	m_pButtonChangeTeam->SetEnabled(false);
	m_pButtonChangeTeam->SetMouseInputEnabled(true);

	InitPos();
}
CMainMenuButtons::~CMainMenuButtons()
{
}
void CMainMenuButtons::ComputeHeadings(MButtonBorderless *button)
{
	HFont hFontHeading = button->GetFont();
	wchar_t buff[128];
	button->GetText(buff,128);
	int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, buff );

	if(iHeadingWidth > 290){
		button->SetFont(vgui::scheme()->GetIScheme(vgui::scheme()->GetScheme("ClientScheme"))->GetFont("MenuEntrySmall"));
		hFontHeading = button->GetFont();
		int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, buff );
	}
	button->SetTextInset((button->GetWide()-iHeadingWidth)/2,0);
}
void CMainMenuButtons::InitPos()
{
	m_pImgCadre2->SetImage("cadre2");
	m_pImgCadre2->SetSize(XRES(500),YRES(480));
	m_pImgCadre2->SetZPos(-3);

	SetBounds(161,209,295,YRES(480));

	m_pButtonLeave->SetSize(291,40);
	m_pButtonLeave->SetPos(0,YRES(480)-300);
	
	m_pButtonOptions->SetSize(291,40);
	m_pButtonOptions->SetPos(0,YRES(480)-340-BUTTONDISTANCE);

	m_pButtonAchievements->SetSize(291,40);
	m_pButtonAchievements->SetPos(0,YRES(480)-380-BUTTONDISTANCE*2);

	m_pButtonPlay->SetSize(291,40);
	m_pButtonPlay->SetPos(0,36);
	m_pButtonPlay->MoveToFront();

	m_pButtonResume->SetSize(291,40);
	m_pButtonResume->SetPos(0,76+BUTTONDISTANCE);

	m_pButtonChangeTeam->SetSize(291,40);
	m_pButtonChangeTeam->SetPos(0,116+2*BUTTONDISTANCE);
}
void CMainMenuButtons::OnCommand(const char *command)
{
	if(!Q_stricmp(command, "Leave"))
	{
		if(InGame())
			engine->ClientCmd("disconnect");
		else
		{
			engine->ClientCmd("gamemenucommand quit");
		}
	}
	else if(!Q_stricmp(command, "Options"))
		engine->ClientCmd("gamemenucommand openoptionsdialog");
	else if(!Q_stricmp(command, "Resume"))
		engine->ClientCmd("gamemenucommand ResumeGame");
	else if(!Q_stricmp(command, "Changeteam"))
		engine->ClientCmd("swapteam");
	else if(!Q_stricmp(command, "Play"))
		engine->ClientCmd("OpenPlaybackPanel");
	else if(!Q_stricmp(command, "Achievements"))
		engine->ClientCmd("OpenAchievementsPanel");
}
void CMainMenuButtons::Paint()
{
	m_pImgCadre2->SetPos(-111,0);
	if(InGame() && !InGameLayout)
	{
		InGameLayout = true;
		m_pButtonLeave->SetText("#bp_Disconnect");
		m_pButtonResume->SetPaintEnabled(true);
		m_pButtonResume->SetEnabled(true);
		m_pButtonResume->SetCursor(dc_hand);
		m_pButtonChangeTeam->SetPaintEnabled(true);
		m_pButtonChangeTeam->SetEnabled(true);
		m_pButtonChangeTeam->SetCursor(dc_hand);
		ComputeHeadings(m_pButtonLeave);
		ComputeHeadings(m_pButtonChangeTeam);
		ComputeHeadings(m_pButtonResume);
	}
	else if(!InGame() && InGameLayout)
	{
		InGameLayout = false;
		m_pButtonLeave->SetText("#bp_Quit");
		m_pButtonResume->SetPaintEnabled(false);
		m_pButtonResume->SetEnabled(false);
		m_pButtonResume->SetCursor(dc_blank);
		m_pButtonChangeTeam->SetPaintEnabled(false);
		m_pButtonChangeTeam->SetEnabled(false);
		m_pButtonChangeTeam->SetCursor(dc_blank);
		ComputeHeadings(m_pButtonLeave);
	}
}
class CMainMenu : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE(CMainMenu, vgui::Frame);
 
public:
	CMainMenu( vgui::VPANEL parent );
	~CMainMenu();

	virtual void OnCommand(const char *command);
	virtual void Paint();
	virtual void InitPos();
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );
		m_pUserNameText->SetFont(pScheme->GetFont("MenuUserName"));
		m_pUserSubText->SetFont(pScheme->GetFont("MenuUserSub"));
	}
	// The panel background image should be square, not rounded.
	virtual void PaintBackground()
	{
		SetBgColor(Color(0,0,0,0));
		SetPaintBackgroundType( 0 );
		BaseClass::PaintBackground();
	}
	virtual void PerformLayout()
	{
		// re-position
		SetPos(vgui::scheme()->GetProportionalScaledValue(defaultX), vgui::scheme()->GetProportionalScaledValue(defaultY));
 
		BaseClass::PerformLayout();
	}
	void PerformDefaultLayout()
	{
		this->CanChainKeysToParent();
	}

	virtual void OnThink()
	{
	}
	void SetSubText(char*msg)
	{
		m_pUserSubText->SetText(msg);
		HFont hFontHeading = m_pUserSubText->GetFont();
		int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, msg );
		m_pUserSubText->SetWide(iHeadingWidth);
		m_pUserSubText->SetPos(XRES(640)-138-iHeadingWidth,138);
	}
	void SetSubText(wchar_t*msg)
	{
		m_pUserSubText->SetText(msg);
		HFont hFontHeading = m_pUserSubText->GetFont();
		int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, msg );
		m_pUserSubText->SetWide(iHeadingWidth);
		m_pUserSubText->SetPos(XRES(640)-138-iHeadingWidth,138);
	}
	void UpdateSubText();

	bool InGame()
	{
		C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
 
		if(pPlayer && IsVisible())
		{
			return true;
		} else {
			return false;
		}
	}

	virtual void OnScreenSizeChanged(int i, int e);

private:
	vgui::ImagePanel *m_pImgLogo;
	vgui::ImagePanel *m_pImgCadre1;
	vgui::ImagePanel *m_pImgCadre2;

	vgui::Label *m_pUserNameText;
	vgui::Label *m_pUserSubText;
	CNebAvatar *m_pAvatar;
	CAvatarImage *m_pDefaultAvatar;

	float m_flNextSubUpdate;

	bool m_bFirstInterestingPaint;
	int defaultX;
	int defaultY;
	int m_nLogo;
	int m_nAvatar;
};
void MotdCallback(char *msg)
{
	SMenu->SetSubText(msg);
}
CMainMenu::CMainMenu( vgui::VPANEL parent ) : BaseClass( NULL, "CMainMenu" )
{
	SetParent( parent );
	SetTitleBarVisible( false );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetCloseButtonVisible( false );
	SetSizeable( false );
	SetMoveable( false );
	SetProportional( true );
	SetVisible( true );
	SetKeyBoardInputEnabled( false );
	SetMouseInputEnabled( false );
	//ActivateBuildMode();
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/ClientScheme.res", "ClientScheme"));
	SetPaintBackgroundEnabled(false);

	SetZPos(-1);
		// These coords are relative to a 640x480 screen
		// Good to test in a 1024x768 resolution.
	defaultX = 0; // x-coord for our position
	defaultY = 0; // y-coord for our position

	m_pImgLogo = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Logo"));
	m_pImgCadre1 = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "Cadre1"));

	m_pUserNameText = new vgui::Label(this, "TextEntry", steamapicontext->SteamFriends()->GetPersonaName());	
	m_pUserSubText = new vgui::Label(this, "TextEntry", GAMENAME);

	m_pDefaultAvatar = new CAvatarImage();

	m_pAvatar = new CNebAvatar(this);

	InitPos();
	m_nAvatar = surface()->CreateNewTextureID(true);
	m_bFirstInterestingPaint = true;

	PerformDefaultLayout();

	m_flNextSubUpdate = 60;
}
void CMainMenu::InitPos()
{
	SetSize(XRES(640),YRES(480));

	this->SetPaintEnabled(true);

	m_pImgLogo->SetImage("logo");
	m_pImgCadre1->SetImage("cadre1");

	m_pImgLogo->SetPos(0,20);		m_pImgLogo->SetSize(795, 297);
	m_pImgCadre1->SetPos(XRES(640)-2049,103);	m_pImgCadre1->SetSize(2048,YRES(500));

	m_pImgCadre1->SetZPos(-2);
	m_pImgLogo->SetZPos(-1);

	m_pUserNameText->SetPos(250,250);
	m_pUserNameText->SetSize(300,100);
	
	m_pUserSubText->SetPos(250,250);
	m_pUserSubText->SetSize(300,100);
	m_pUserSubText->SetFgColor(Color(215,251,254,255));

	m_pAvatar->SetPos(XRES(640)-120,117);
	m_pAvatar->SetAvatarSize( 100, 100 );
	m_pAvatar->SetBgColor(Color(128,255,128,255));

	m_pDefaultAvatar->SetPos(XRES(640)-142,116);
	m_pDefaultAvatar->SetSize(100, 100);
	m_pDefaultAvatar->SetAvatarSize(100,100);
	m_pDefaultAvatar->SetColor(Color( 255,255,255,255));
}
//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMainMenu::~CMainMenu()
{
}
void CMainMenu::OnScreenSizeChanged(int i, int e)
{
	InitPos();
	UpdateSubText();

	HFont hFontHeading = m_pUserNameText->GetFont();
	int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, steamapicontext->SteamFriends()->GetPersonaName() );
	if(iHeadingWidth != 0)
	{
		m_bFirstInterestingPaint= false;
		m_pUserNameText->SetPos(XRES(640)-138-iHeadingWidth,98);
	}
}
void CMainMenu::UpdateSubText()
{
	if(g_Nebuleuse)
	{
		if(g_Nebuleuse->LastError == NEBULEUSE_ERROR_PARSEFAILED){
			SetSubText(g_pVGuiLocalize->Find( "#BP_Sub_CantConnect" ));
			m_pAvatar->SetBgColor(Color(255, 0, 0, 255));
			m_flNextSubUpdate = gpGlobals->curtime + 5; // Nebuleuse may be trying to reconnect, so update more frequently
			return;
		}
		else if(g_Nebuleuse->LastError == NEBULEUSE_ERROR_OUTDATED){
			SetSubText(g_pVGuiLocalize->Find( "#BP_Sub_Outdated" ));
			m_pAvatar->SetBgColor(Color(255, 162, 0, 255));
			m_flNextSubUpdate = gpGlobals->curtime + 60;
			Updaterpanel->Activate();
			return;
		}
		else if(g_Nebuleuse->LastError == NEBULEUSE_ERROR_BANNED){
			SetSubText(g_pVGuiLocalize->Find( "#BP_Sub_Banned" ));
			m_pAvatar->SetBgColor(Color(255, 0, 0, 255));
			m_flNextSubUpdate = gpGlobals->curtime + 60;
			return;
		}
	}
	
	// Calculate the number of friends playing 
		uint64 nGameID;
		uint32 unGameIP;
		uint16 usGamePort;
		uint16 usQueryPort;
		ISteamFriends *friends = steamapicontext->SteamFriends();

		uint64 OurGameId = 11900392278788669658; // Game Id assigned by steam for BP

		int activefriends = 0;

		int fcount = friends->GetFriendCount(k_EFriendFlagImmediate);
		for(int i = 0; i < fcount; i++)
		{
			CSteamID sid = friends->GetFriendByIndex(i, k_EFriendFlagImmediate); // steam id
			friends->GetFriendGamePlayed(sid, &nGameID, &unGameIP, &usGamePort, &usQueryPort);

			if(nGameID == OurGameId)
			{
				activefriends++;
			}
		}
		wchar_t wszLocalized[256]=L"";
		wchar_t wszPlayers[16]=L"";
		swprintf(wszPlayers, L"%i", activefriends);
		if(activefriends == 1)
			g_pVGuiLocalize->ConstructString( wszLocalized, sizeof( wszLocalized ), g_pVGuiLocalize->Find( "#BP_Sub_Friend" ), 1, wszPlayers );
		else
			g_pVGuiLocalize->ConstructString( wszLocalized, sizeof( wszLocalized ), g_pVGuiLocalize->Find( "#BP_Sub_Friends" ), 1, wszPlayers );
		SetSubText(wszLocalized);

	m_flNextSubUpdate = gpGlobals->curtime + 60;
}
void CMainMenu::OnCommand(const char *command)
{
	BaseClass::OnCommand(command);
}

void CMainMenu::Paint()
{
	if(m_bFirstInterestingPaint)
	{	
		m_pDefaultAvatar->SetAvatarSteamID(steamapicontext->SteamUser()->GetSteamID());
		
		UpdateSubText();

		if(g_NebAvatar && g_NebAvatar->GetAvatarRGB())
			m_pAvatar->SetAvatar(g_NebAvatar->GetAvatarRGB());
		
		HFont hFontHeading = m_pUserNameText->GetFont();
		int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, steamapicontext->SteamFriends()->GetPersonaName() );
		if(iHeadingWidth != 0)
		{
			m_bFirstInterestingPaint= false;
			m_pUserNameText->SetPos(XRES(640)-138-iHeadingWidth,98);
		}
	}

	if(m_flNextSubUpdate < gpGlobals->curtime)
		UpdateSubText();

	m_pAvatar->Paint();
	
	if(!m_pAvatar->IsAvatarSet())
		m_pDefaultAvatar->Paint();

	m_pUserSubText->SetFgColor(Color(215,251,254,255));
}

class CSMenu : public ISMenu
{
private:
	CMainMenu *MainMenu;
	CMainMenuButtons *MainButtons;
	vgui::VPANEL m_hParent;
 
public:
	CSMenu( void )
	{
		MainMenu = NULL;
		MainButtons = NULL;
	}
 
	void Create( vgui::VPANEL parent )
	{
		// Create immediately
		MainMenu = new CMainMenu(parent);
		MainButtons = new CMainMenuButtons(parent);
	}
 
	void Destroy( void )
	{
		if ( MainMenu )
		{
			MainMenu->SetParent( (vgui::Panel *)NULL );
			MainButtons->SetParent( (vgui::Panel *)NULL );
			delete MainMenu;
			delete MainButtons;
		}
	}
	void SetSubText(char*msg)
	{
		if ( MainMenu )
		{
			MainMenu->SetSubText(msg);
		}
	}
 
};
 
static CSMenu g_SMenu;
ISMenu *SMenu = ( ISMenu * )&g_SMenu;