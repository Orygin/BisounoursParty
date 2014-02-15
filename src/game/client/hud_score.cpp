// BISOUNOURS PARTY : HUD (DOM LOGO)

#include "hud.h"
#include "cbase.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include <vgui_controls/Panel.h>
#include "hl2mp_gamerules.h"
#include "c_playerresource.h"
#include "tier0/memdbgon.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGUI.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/ImagePanel.h>
#include "c_team.h"
#include "fmtstr.h"

using namespace vgui;

class CHudScore : public CHudElement, public EditablePanel
{
    DECLARE_CLASS_SIMPLE( CHudScore, EditablePanel );

public:

    CHudScore( const char *pElementName );
	virtual void	ApplySchemeSettings( IScheme *scheme );
	virtual void	OnTick( void );
	virtual void	FireGameEvent( IGameEvent * event );
	virtual void	Init();
	virtual void	LevelInit();
protected:
	ImagePanel *m_pCP1;
	ImagePanel *m_pCP2;
	ImagePanel *m_pCP3;

	Label *m_pPinkScore;
	Label *m_pGreenScore;
	Label *m_pInter;

	float m_flStartTime;
	bool m_bFirst;
	int m_nLogo;
	int m_iLayout;
};

DECLARE_HUDELEMENT( CHudScore );

CHudScore::CHudScore( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudScore" )
{
    Panel *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );
	
	SetPaintBackgroundEnabled(false);
	m_bFirst = true;

	m_pCP1 = new ImagePanel( this, "CP1" );
	m_pCP2 = new ImagePanel( this, "CP2" );
	m_pCP3 = new ImagePanel( this, "CP3" );

	m_pPinkScore = new Label( this, "PinkScore", "" );
	m_pGreenScore = new Label( this, "GreenScore", "" );
	m_pInter = new Label( this, "Inter", "" );

	vgui::ivgui()->AddTickSignal( GetVPanel() );
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
}
void CHudScore::Init()
{
		ListenForGameEvent( "cp_caped" );
}
void CHudScore::LevelInit()
{
	m_pCP1->SetDrawColor(COLOR_WHITE);
	m_pCP2->SetDrawColor(COLOR_WHITE);
	m_pCP3->SetDrawColor(COLOR_WHITE);
}
void CHudScore::ApplySchemeSettings( IScheme *pScheme )
{
	LoadControlSettings( "resource/UI/Score.res" );
	
	BaseClass::ApplySchemeSettings( pScheme );
	SetBounds(0,0, XRES(640), YRES(480));
	
	m_pPinkScore->SetFgColor(COLOR_PINK);
	m_pGreenScore->SetFgColor(COLOR_GREEN);
	
	int width = m_pCP1->GetWide();
	int tall = m_pCP1->GetTall();
		
	m_pCP1->SetWide(width);
	m_pCP1->SetTall(tall);
	m_pCP2->SetWide(width);
	m_pCP2->SetTall(tall);
	m_pCP3->SetWide(width);
	m_pCP3->SetTall(tall);

	m_pCP1->SetPos(XRES(300),YRES(438));
	m_pCP2->SetPos(XRES(340),YRES(438));
	m_pCP3->SetPos(XRES(320),YRES(410));

	m_pCP1->SetImage(scheme()->GetImage("hud/hud_domCP", false));
	m_pCP2->SetImage(scheme()->GetImage("hud/hud_domCP", false));
	m_pCP3->SetImage(scheme()->GetImage("hud/hud_domCP", false));

	m_pCP1->SetAlpha(0);
	m_pCP2->SetAlpha(0);
	m_pCP3->SetAlpha(0);
	SetDialogVariable( "Inter", "" );
	SetDialogVariable( "PinkText", "");
	SetDialogVariable( "GreenText", "");
	m_iLayout = -1;
}
void CHudScore::OnTick()
{
 	if(m_bFirst)
	{
		m_flStartTime = gpGlobals->curtime + 4.0f;
		m_bFirst = false;
		return;
	}
	else
	{
		if(m_flStartTime > gpGlobals->curtime)
			return;
	}
	if(C_BasePlayer::GetLocalPlayer() == NULL)
		return;
	if(HL2MPRules()->GetGameType() == GAME_DOM)
	{
		C_Team *t_Pink = GetGlobalTeam( TEAM_PINK );
		C_Team *t_Green = GetGlobalTeam( TEAM_GREEN );

		SetDialogVariable( "PinkText", t_Pink->Get_Score() );
		SetDialogVariable( "GreenText", t_Green->Get_Score() );

		if(m_iLayout != GAME_DOM)
		{
			m_iLayout = GAME_DOM;
			m_pCP1->SetAlpha(128);
			m_pCP2->SetAlpha(128);
			m_pCP3->SetAlpha(128);
			SetDialogVariable( "Inter", "/" );
			m_pPinkScore->SetFgColor(COLOR_PINK);
			m_pGreenScore->SetFgColor(COLOR_GREEN);
		}
		if(GetAlpha() == 0) SetAlpha(255);
	}
	else if(HL2MPRules()->GetGameType() == GAME_FORTS || HL2MPRules()->GetGameType() == GAME_PUSH )
	{
		C_Team *t_Pink = GetGlobalTeam( TEAM_PINK );
		C_Team *t_Green = GetGlobalTeam( TEAM_GREEN );
		if(GetAlpha() == 0) SetAlpha(255);
		SetDialogVariable( "PinkText", t_Pink->Get_Score() );
		SetDialogVariable( "GreenText", t_Green->Get_Score() );
		if(m_iLayout != GAME_FORTS)
		{
			m_iLayout = GAME_FORTS;
			m_pCP1->SetAlpha(0);
			m_pCP2->SetAlpha(0);
			m_pCP3->SetAlpha(0);
			SetDialogVariable( "Inter", "/" );
			m_pPinkScore->SetFgColor(COLOR_PINK);
			m_pGreenScore->SetFgColor(COLOR_GREEN);
		}
	}
	else if(HL2MPRules()->GetGameType() == GAME_COOP)
	{
		if(GetAlpha() == 0) SetAlpha(255);
		SetDialogVariable( "GreenText",  g_PR->GetPlayerScore(CBasePlayer::GetLocalPlayer()->entindex()) );
		if(m_iLayout != GAME_COOP)
		{
			m_iLayout = GAME_COOP;
			m_pCP1->SetAlpha(0);
			m_pCP2->SetAlpha(0);
			m_pCP3->SetAlpha(0);
			SetDialogVariable( "Inter", "" );
			SetDialogVariable( "PinkText", "" );
			m_pGreenScore->SetFgColor(COLOR_WHITE);
		}
	}
	else if(HL2MPRules()->GetGameType() == GAME_DM)
	{
		if(GetAlpha() == 0) SetAlpha(255);
		if(CBasePlayer::GetLocalPlayer()->GetTeamNumber() == TEAM_SPECTATOR)
			SetDialogVariable( "GreenText",  "-" );
		else
			SetDialogVariable( "GreenText",  g_PR->GetPlayerScore(CBasePlayer::GetLocalPlayer()->entindex()) );

		SetDialogVariable( "PinkText",  g_PR->GetHighestScore() );
		if(m_iLayout != GAME_DM)
		{
			m_iLayout = GAME_DM;
			m_pCP1->SetAlpha(0);
			m_pCP2->SetAlpha(0);
			m_pCP3->SetAlpha(0);
			SetDialogVariable( "Inter", "/" );
			m_pPinkScore->SetFgColor(COLOR_WHITE);
			m_pGreenScore->SetFgColor(COLOR_WHITE);
		}
	}
	else if(HL2MPRules()->GetGameType() == GAME_TDM)
	{
		if(GetAlpha() == 0) SetAlpha(255);
		SetDialogVariable( "GreenText",  g_PR->GetTeamScore(TEAM_GREEN) );
		SetDialogVariable( "PinkText",  g_PR->GetTeamScore(TEAM_PINK) );
		if(m_iLayout != GAME_TDM)
		{
			m_iLayout = GAME_TDM;
			m_pCP1->SetAlpha(0);
			m_pCP2->SetAlpha(0);
			m_pCP3->SetAlpha(0);
			SetDialogVariable( "Inter", "/" );
			m_pPinkScore->SetFgColor(COLOR_PINK);
			m_pGreenScore->SetFgColor(COLOR_GREEN);
		}
	}
	else
		if(GetAlpha() != 0) SetAlpha(0);
}
void CHudScore::FireGameEvent( IGameEvent * event )
{
	const char *name = event->GetName();
	if ( 0 == Q_strcmp( name, "cp_caped" ) )
	{
		if (event->GetInt("id") == 0)
		{
			if(event->GetInt("team") == TEAM_PINK)
				m_pCP1->SetDrawColor(COLOR_PINK);
			else if(event->GetInt("team") == TEAM_GREEN)
				m_pCP1->SetDrawColor(COLOR_GREEN);
		}
		if (event->GetInt("id") == 1)
		{
			if(event->GetInt("team") == TEAM_PINK)
				m_pCP2->SetDrawColor(COLOR_PINK);
			else if(event->GetInt("team") == TEAM_GREEN)
				m_pCP2->SetDrawColor(COLOR_GREEN);
		}
		if (event->GetInt("id") == 2)
		{
			if(event->GetInt("team") == TEAM_PINK)
				m_pCP3->SetDrawColor(COLOR_PINK);
			else if(event->GetInt("team") == TEAM_GREEN)
				m_pCP3->SetDrawColor(COLOR_GREEN);
		}
		if(event->GetInt("thirdexist") == 0)
			m_pCP3->SetAlpha(0);
		else
			m_pCP3->SetAlpha(128);
	}
}