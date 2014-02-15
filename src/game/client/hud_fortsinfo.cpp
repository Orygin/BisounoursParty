//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "ienginevgui.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGUI.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/ImagePanel.h>
#include "hud_fortsinfo.h"
#include "steam/steam_api.h"
#include "iachievementmgr.h"
#include "fmtstr.h"
#include "vgui_avatarimage.h"
#include "util_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

#define HUD_FORT_PANEL_DURATION 2.5f

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

DECLARE_HUDELEMENT_DEPTH( CHudFortsInfo, 100 );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudFortsInfo::CHudFortsInfo( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "AchievementNotificationPanel" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_flHideTime = 0;
	m_pPanelBackground = new EditablePanel( this, "Notification_Background" );
	m_pIcon = new ImagePanel( this, "Notification_Icon" );
	m_pLabelHeading = new Label( this, "HeadingLabel", "" );
	m_pLabelTitle = new Label( this, "TitleLabel", "" );
	m_bFirstNot = true;
	m_pIcon->SetShouldScaleImage( true );

	vgui::ivgui()->AddTickSignal( GetVPanel() );
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudFortsInfo::Init()
{
	ListenForGameEvent( "forts_event" );
	ListenForGameEvent( "cp_caped" );
	ListenForGameEvent( "push_caped" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudFortsInfo::ApplySchemeSettings( IScheme *pScheme )
{
	// load control settings...
	LoadControlSettings( "resource/UI/FortsNotification.res" );
	
	BaseClass::ApplySchemeSettings( pScheme );
	SetBounds(0,0, XRES(640), YRES(480));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudFortsInfo::PerformLayout( void )
{
	BaseClass::PerformLayout();

	// Set background color of various elements.  Need to do this in code, if we do it in res file it gets slammed by the
	// scheme.  (Incl. label background: some products don't have label background colors set in their scheme and helpfully slam it to white.)
	SetBgColor( Color( 0, 0, 0, 0 ) );
	m_pLabelHeading->SetBgColor( Color( 0, 0, 0, 0 ) );
	m_pLabelTitle->SetBgColor( Color( 0, 0, 0, 0 ) );
	m_pPanelBackground->SetBgColor( Color( 62,70,55, 200 ) );
	m_pPanelBackground->SetBgColor(COLOR_PINK);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudFortsInfo::FireGameEvent( IGameEvent * event )
{
	const char *name = event->GetName();

	if ( 0 == Q_strcmp( name, "forts_event" ) )
	{
		//engine->GetPlayerForUserID(event->GetInt("userid"));
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(engine->GetPlayerForUserID(event->GetInt("userid")));
		if (pPlayer == NULL)
			return;

		if(event->GetInt("team") == CBasePlayer::GetLocalPlayer()->GetTeamNumber()) // Our team did what ?
		{ //bad thing
			if(event->GetInt("action") == 1) // We stole their flag
				AddNotification("", L"", g_pVGuiLocalize->Find("FORTS_GOTTHEIR_FLAG"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
			else if(event->GetInt("action") == 2) // We lost their flag
				AddNotification("", L"", g_pVGuiLocalize->Find("FORTS_RETURNOUR_FLAG"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
			else if(event->GetInt("action") == 0) // We scored their flag
				AddNotification("", L"", g_pVGuiLocalize->Find("FORTS_SCOREDTHEIR_FLAG"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
		}
		else
		{
			if(event->GetInt("action") == 1)
				AddNotification("", L"", g_pVGuiLocalize->Find("FORTS_LOSTOUR_FLAG"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
			else if(event->GetInt("action") == 2)
				AddNotification("", L"", g_pVGuiLocalize->Find("FORTS_RETURNTHEIR_FLAG"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
			else if(event->GetInt("action") == 0)
				AddNotification("", L"", g_pVGuiLocalize->Find("FORTS_SCOREDOUR_FLAG"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
		}
	}
	else if ( 0 == Q_strcmp( name, "cp_caped" ) )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(engine->GetPlayerForUserID(event->GetInt("userid")));
		if (pPlayer == NULL)
			return;
		AddNotification("", L"",  g_pVGuiLocalize->Find("CP_CAPED"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
	}
	else if ( 0 == Q_strcmp( name, "push_caped" ) )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(engine->GetPlayerForUserID(event->GetInt("userid")));
		if (pPlayer == NULL)
			return;
		AddNotification("", L"",  g_pVGuiLocalize->Find("PUSH_CAPED"), pPlayer,event->GetInt("userid"), event->GetInt("team"));
	}
}

//-----------------------------------------------------------------------------
// Purpose: Called on each tick
//-----------------------------------------------------------------------------
void CHudFortsInfo::OnTick( void )
{
	if ( ( m_flHideTime > 0 ) && ( m_flHideTime < gpGlobals->curtime ) )
	{
		m_flHideTime = 0;
		ShowNextNotification();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CHudFortsInfo::ShouldDraw( void )
{
	return ( ( m_flHideTime > 0 ) && ( m_flHideTime > gpGlobals->curtime ) && CHudElement::ShouldDraw() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudFortsInfo::AddNotification( const char *szIconBaseName, const wchar_t *pHeading, const wchar_t *pTitle, CBasePlayer *pPlayer, int plid, char team)
{
	// put this notification in our queue
	int iQueueItem = m_queueNotification.AddToTail();
	Notification_t &notification = m_queueNotification[iQueueItem];
	Q_strncpy( notification.szIconBaseName, szIconBaseName, ARRAYSIZE( notification.szIconBaseName ) );
	Q_wcsncpy( notification.szHeading, pHeading, sizeof( notification.szHeading ) );
	Q_wcsncpy( notification.szTitle, pTitle, sizeof( notification.szTitle ) );
	notification.pPlayer = pPlayer;
	notification.playerId = plid;
	notification.team = team;

	// if we are not currently displaying a notification, go ahead and show this one
	if ( 0 == m_flHideTime )
	{
		ShowNextNotification();
	}
}
//-----------------------------------------------------------------------------
// Purpose: Shows next notification in queue if there is one
//-----------------------------------------------------------------------------
void CHudFortsInfo::ShowNextNotification()
{
	// see if we have anything to do
	if ( 0 == m_queueNotification.Count() )
	{
		m_flHideTime = 0;
		return;
	}

	Notification_t &notification = m_queueNotification[ m_queueNotification.Head() ];

	m_flHideTime = gpGlobals->curtime + HUD_FORT_PANEL_DURATION;
	mbstowcs(notification.szHeading, notification.pPlayer->GetPlayerName(), 256);
	// set the text and icon in the dialog
	SetDialogVariable( "heading", notification.szHeading );
	SetDialogVariable( "title", notification.szTitle );

	if(notification.team == TEAM_PINK)
		m_pPanelBackground->SetBgColor(COLOR_PINK);
	else
		m_pPanelBackground->SetBgColor(COLOR_GREEN);

		//CSteamID steamIDForPlayer( notification.pPlayer->GetUserID(), 1, steamapicontext->SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );
		player_info_t pi;

		if ( engine->GetPlayerInfo( engine->GetPlayerForUserID(notification.playerId), &pi ) )
		{
			if ( pi.friendsID )
			{
				CSteamID steamIDForPlayer( pi.friendsID, 1, steamapicontext->SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );
						CAvatarImage *pImage = new CAvatarImage();
						pImage->SetAvatarSteamID( steamIDForPlayer);
						pImage->SetAvatarSize( 32, 32 );	// Deliberately non scaling
				if(m_bFirstNot){
					m_pIcon->SetWide(m_pIcon->GetWide() + 10);
					m_bFirstNot = false;
				}
				m_pIcon->SetImage( pImage );
			}
		}
		//m_pIcon->SetImage( CFmtStr( "achievements/%s.vmt", pchIconBaseName ) );

						



	// resize the panel so it always looks good

	// get fonts
	HFont hFontHeading = m_pLabelHeading->GetFont();
	HFont hFontTitle = m_pLabelTitle->GetFont();
	// determine how wide the text strings are
	int iHeadingWidth = UTIL_ComputeStringWidth( hFontHeading, notification.szHeading );
	int iTitleWidth = UTIL_ComputeStringWidth( hFontTitle, notification.szTitle );
	// use the widest string
	int iTextWidth = max( iHeadingWidth, iTitleWidth );
	// don't let it be insanely wide
	iTextWidth = min( iTextWidth, XRES( 300 ) );
	int iIconWidth = m_pIcon->GetWide();
	int iSpacing = XRES( 10 );
	int iPanelWidth = iSpacing + iIconWidth + iSpacing + iTextWidth + iSpacing;
	int iPanelX = (GetWide() / 2) - (iPanelWidth / 2);
	int iIconX = iPanelX + iSpacing;
	int iTextX = iIconX + iIconWidth + iSpacing;
	// resize all the elements
	SetXAndWide( m_pPanelBackground, iPanelX, iPanelWidth );
	SetXAndWide( m_pIcon, iIconX, iIconWidth );
	SetXAndWide( m_pLabelHeading, iTextX, iTextWidth );
	SetXAndWide( m_pLabelTitle, iTextX, iTextWidth );
	
	m_queueNotification.Remove( m_queueNotification.Head() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudFortsInfo::SetXAndWide( Panel *pPanel, int x, int wide )
{
	int xCur, yCur;
	pPanel->GetPos( xCur, yCur );
	pPanel->SetPos( x, yCur );
	pPanel->SetWide( wide );
}

CON_COMMAND( forts_notification_test, "Test the hud notification UI")
{
#if 0
	static int iCount=0;

	CHudFortsInfo *pPanel = GET_HUDELEMENT( CHudFortsInfo );
	if ( pPanel )
	{		
		pPanel->AddNotification( "HL2_KILL_ODESSAGUNSHIP", L"Achievement Progress", ( 0 == ( iCount % 2 ) ? L"Test Notification Message A (1/10)" :
			L"Test Message B" ) );
	}
#endif

	IGameEvent *event = gameeventmanager->CreateEvent( "forts_event" );
	if ( event )
	{
		event->SetInt( "userid", CBasePlayer::GetLocalPlayer()->GetUserID() );
		event->SetInt( "team", CBasePlayer::GetLocalPlayer()->GetTeamNumber() );
		event->SetInt( "action", 1 );
		gameeventmanager->FireEvent( event );
	}	


	//iCount++;
}