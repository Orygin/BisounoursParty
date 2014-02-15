// BISOUNOURS PARTY : SPECTATOR HUD

#include "cbase.h"
#include <cdll_client_int.h>
#include <globalvars_base.h>
#include <cdll_util.h>
#include <KeyValues.h>

#include "spectatorgui.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IPanel.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/MenuItem.h>
#include <vgui_controls/TextImage.h>

#include <stdio.h> // _snprintf define

#include <game/client/iviewport.h>
#include "commandmenu.h"
#include "hltvcamera.h"

#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Menu.h>
#include "IGameUIFuncs.h" // for key bindings
#include <imapoverview.h>
#include <shareddefs.h>
#include <igameresources.h>
#include "hl2mp_gamerules.h"
#include "c_playerresource.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define UPDATE_DELAY 1.0f

extern IGameUIFuncs *gameuifuncs; // for key binding details

CSpectatorGUI *g_pSpectatorGUI = NULL;

using namespace vgui;

ConVar cl_spec_mode(
	"cl_spec_mode",
	"1",
	FCVAR_USERINFO | FCVAR_SERVER_CAN_EXECUTE,
	"spectator mode" );

CSpectatorGUI::CSpectatorGUI(IViewPort *pViewPort) : EditablePanel( NULL, PANEL_SPECGUI )
{
	m_pViewPort = pViewPort;

	g_pSpectatorGUI = this;
	
	// initialize dialog
	SetVisible(false);
	SetProportional(true);

	// load the new scheme early!
	SetScheme("ClientScheme");

	SetMouseInputEnabled( false );
	SetKeyBoardInputEnabled( false );

	m_pTopBar = new Panel( this, "topbar" );

	m_pPlayerLabel = new Label(this, "playerlabel", "");
	m_pPlayerLabel->SetVisible(true);

	m_pScoreLabel1 = new Label(this, "scorelabel1", "");
	m_pScoreLabel2 = new Label(this, "scorelabel2", "");
	m_pScoreLabel3 = new Label(this, "scorelabel3", "");

	m_pScoreValueLabel1 = new Label(this, "scorevaluelabel1", "");
	m_pScoreValueLabel2 = new Label(this, "scorevaluelabel2", "");
	m_pScoreValueLabel3 = new Label(this, "scorevaluelabel3", "");

	SetPaintBorderEnabled(false);
	SetPaintBackgroundEnabled(false);

	InvalidateLayout();
}

CSpectatorGUI::~CSpectatorGUI()
{
	g_pSpectatorGUI = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Sets the colour of the top bar
//-----------------------------------------------------------------------------
void CSpectatorGUI::ApplySchemeSettings(IScheme *pScheme)
{
	LoadControlSettings("Resource/UI/Spectator.res");
	m_pTopBar->SetVisible(true);

	BaseClass::ApplySchemeSettings(pScheme);

	SetBgColor(Color(0, 0, 0, 0)); // make the background transparent
	m_pTopBar->SetBgColor(BAR_COLOR);
	SetPaintBorderEnabled(false);

	SetBorder(NULL);
}

//-----------------------------------------------------------------------------
// Purpose: makes the GUI fill the screen
//-----------------------------------------------------------------------------
void CSpectatorGUI::PerformLayout()
{
	int w, h;
	GetHudSize(w, h);
	SetBounds(0, 0, w, h);
}

void CSpectatorGUI::SetLabelText(const char *textEntryName, const char *text)
{
	Label *entry = dynamic_cast<Label *>(FindChildByName(textEntryName));
	if (entry)
		entry->SetText(text);
}

void CSpectatorGUI::SetLabelText(const char *textEntryName, wchar_t *text)
{
	Label *entry = dynamic_cast<Label *>(FindChildByName(textEntryName));
	if (entry)
		entry->SetText(text);
}

void CSpectatorGUI::MoveLabelToFront(const char *textEntryName)
{
	Label *entry = dynamic_cast<Label *>(FindChildByName(textEntryName));
	if (entry)
		entry->MoveToFront();
}

void CSpectatorGUI::ShowPanel(bool bShow)
{
	SetVisible(bShow);
}

void CSpectatorGUI::Update()
{
	int wide, tall;
	int bx, by, bwide, btall;

	// full top bar
	GetHudSize(wide, tall);
	m_pTopBar->GetBounds( bx, by, bwide, btall );
	m_pTopBar->SetSize( wide , btall ); // change width, keep height
	m_pTopBar->SetPos( 0, 0 );

	// Affichage du chrono de respawn
	UpdateTimer();

	// Affichage du nom du joueur observé, ainsi que de sa vie et de son armure (commun à tous les modes de jeu)
	UpdatePlayerInfo();	

	// Affichage du nom de la map
	UpdateMapName();

	// Affichage des scores des trois meilleurs joueurs OU des scores de chaque équipe, selon le mode de jeu
	if(HL2MPRules()->GetGameType() == GAME_DM || HL2MPRules()->GetGameType() == GAME_COOP)
		UpdateBestPlayersScores();
	else
		UpdateTeamsScores();
}

void CSpectatorGUI::UpdatePlayerInfo()
{
	int playernum = GetSpectatorTarget();
	if (playernum > 0 && playernum <= gpGlobals->maxClients && GameResources())
	{
		Color c = GameResources()->GetTeamColor(GameResources()->GetTeam(playernum)); // Player's team color
		m_pPlayerLabel->SetFgColor(c);
		
		wchar_t playerText[80], playerName[64], health[4], armor[4];
		memset(playerName, 0x0, sizeof(playerName));

		g_pVGuiLocalize->ConvertANSIToUnicode(UTIL_SafeName(g_PR->GetPlayerName(playernum)), playerName, sizeof(playerName));

		int iHealth = GameResources()->GetHealth(playernum);
		int iArmor = GameResources()->GetArmor(playernum);

		if ( GameResources()->IsAlive(playernum) )
		{
			_snwprintf(health, sizeof(health), L"%i", iHealth);
			_snwprintf(armor, sizeof(armor), L"%i", iArmor);
			g_pVGuiLocalize->ConstructString( playerText, sizeof(playerText), g_pVGuiLocalize->Find("#Spec_PlayerItem_Health"), 3, playerName, health, armor);
		}
		else
			g_pVGuiLocalize->ConstructString(playerText, sizeof(playerText), g_pVGuiLocalize->Find("#Spec_PlayerItem"), 1, playerName);

		m_pPlayerLabel->SetText(playerText);
	}
	else
		m_pPlayerLabel->SetText(L"");
}

void CSpectatorGUI::UpdateTimer()
{
	int timer = 87;

	wchar_t szText[63];
	_snwprintf(szText, sizeof(szText), L"%d:%02d\n", (timer / 60), (timer % 60));

	SetLabelText("timerlabel", szText);
}

void CSpectatorGUI::UpdateMapName()
{
	char tempstr[64];
	Q_FileBase(engine->GetLevelName(), tempstr, sizeof(tempstr));

	wchar_t wMapName[64];
	g_pVGuiLocalize->ConvertANSIToUnicode(tempstr, wMapName, sizeof(wMapName));

	wchar_t szMapName[64];
	g_pVGuiLocalize->ConstructString( szMapName, sizeof(szMapName), g_pVGuiLocalize->Find("#Spec_Map"), 1, wMapName);

	SetLabelText("mapname", szMapName);
}

void CSpectatorGUI::UpdateBestPlayersScores()
{
	int iPlayer1 = -1, iPlayer2 = -1, iPlayer3 = -1;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);
		if (!pPlayer)
			continue;

		if((g_PR->GetPlayerScore(i) >= g_PR->GetPlayerScore(iPlayer1)) || iPlayer1 == -1){
			iPlayer3 = iPlayer2;
			iPlayer2 = iPlayer1;
			iPlayer1 = i;
		}
		else if((g_PR->GetPlayerScore(i) >= g_PR->GetPlayerScore(iPlayer2)) || iPlayer2 == -1){
			iPlayer3 = iPlayer2;
			iPlayer2 = i;
		}
		else if((g_PR->GetPlayerScore(i) >= g_PR->GetPlayerScore(iPlayer3)) || iPlayer3 == -1){
			iPlayer3 = i;
		}			
	}

	char score[5];

	if(iPlayer1 >= 0){
		m_pScoreLabel1->SetVisible(true);
		m_pScoreValueLabel1->SetVisible(true);

		m_pScoreLabel1->SetText(g_PR->GetPlayerName(iPlayer1));
		sprintf(score, "%d", g_PR->GetPlayerScore(iPlayer1));
		m_pScoreValueLabel1->SetText(score);
	}
	else{
		m_pScoreLabel1->SetVisible(false);
		m_pScoreValueLabel1->SetVisible(false);

		m_pScoreLabel1->SetText("");
		m_pScoreValueLabel1->SetText("");		
	}

	if(iPlayer2 >= 0){
		m_pScoreLabel2->SetVisible(true);
		m_pScoreValueLabel1->SetVisible(true);

		m_pScoreLabel2->SetText(g_PR->GetPlayerName(iPlayer2));
		sprintf(score, "%d", g_PR->GetPlayerScore(iPlayer2));
		m_pScoreValueLabel2->SetText(score);
	}
	else{
		m_pScoreLabel2->SetVisible(false);
		m_pScoreValueLabel2->SetVisible(false);

		m_pScoreLabel2->SetText("");
		m_pScoreValueLabel2->SetText("");
	}

	if(iPlayer3 >= 0){
		m_pScoreLabel3->SetVisible(true);
		m_pScoreValueLabel3->SetVisible(true);

		m_pScoreLabel3->SetText(g_PR->GetPlayerName(iPlayer3));
		sprintf(score, "%d", g_PR->GetPlayerScore(iPlayer3));
		m_pScoreValueLabel3->SetText(score);
	}
	else{
		m_pScoreLabel3->SetVisible(false);
		m_pScoreValueLabel3->SetVisible(false);

		m_pScoreLabel3->SetText("");
		m_pScoreValueLabel3->SetText("");
	}
}

void CSpectatorGUI::UpdateTeamsScores()
{
	// On affiche seulement les deux premiers emplacements de score
	m_pScoreLabel1->SetVisible(true);
	m_pScoreValueLabel1->SetVisible(true);
	m_pScoreLabel2->SetVisible(true);
	m_pScoreValueLabel2->SetVisible(true);
	m_pScoreLabel3->SetVisible(false);
	m_pScoreValueLabel3->SetVisible(false);

	char score[5];

	m_pScoreLabel1->SetFgColor(GameResources()->GetTeamColor(GameResources()->GetTeam(TEAM_PINK)));
	m_pScoreLabel1->SetText("#TEAM_PINK");
	sprintf(score, "%d", g_PR->GetTeamScore(TEAM_PINK));
	m_pScoreValueLabel1->SetText(score);

	m_pScoreLabel2->SetFgColor(GameResources()->GetTeamColor(GameResources()->GetTeam(TEAM_GREEN)));
	m_pScoreLabel2->SetText("#TEAM_GREEN");
	sprintf(score, "%d", g_PR->GetTeamScore(TEAM_GREEN));
	m_pScoreValueLabel2->SetText(score);
}