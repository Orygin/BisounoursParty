// BISOUNOURS PARTY : HUD (WEAPON SELECTION)

#include "cbase.h"
#include "weapon_selection.h"
#include "iclientmode.h"
#include "history_resource.h"
#include "input.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/Panel.h>

#include "vgui/ILocalize.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define SELECTION_TIMEOUT_THRESHOLD		0.75f
#define SELECTION_FADEOUT_TIME			2.0f

#define FASTSWITCH_DISPLAY_TIMEOUT		1.5f
#define FASTSWITCH_FADEOUT_TIME			1.5f
using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: hl2 weapon selection hud element
//-----------------------------------------------------------------------------
class CHudWeaponSelection : public Panel, public CBaseHudWeaponSelection
{
	DECLARE_CLASS_SIMPLE( CHudWeaponSelection, Panel );

public:
	CHudWeaponSelection(const char *pElementName );

	virtual bool ShouldDraw();
	virtual void OnWeaponPickup( C_BaseCombatWeapon *pWeapon );

	virtual void CycleToNextWeapon( void );
	virtual void CycleToPrevWeapon( void );

	virtual C_BaseCombatWeapon *GetWeaponInSlot( int iSlot, int iSlotPos );
	virtual void SelectWeaponSlot( int iSlot );

	virtual C_BaseCombatWeapon	*GetSelectedWeapon( void ){ return m_hSelectedWeapon; }

	virtual void OpenSelection( void );
	virtual void HideSelection( void );

	virtual void LevelInit();

protected:
	virtual void OnThink();
	virtual void Paint();
	virtual void ApplySchemeSettings(IScheme *pScheme);
	virtual void PaintBackground();
	virtual bool IsWeaponSelectable();
	virtual void SetWeaponSelected();
	virtual int		GetTextureForWeapon(const char* weapon);
	int m_iBoxTexture;
	
	//BP armes
	int m_iTexUzi;
	int m_iTexRevolver;
	int m_iTexBazooka;
	int m_iTexBat;
	int m_iTexShotgun;
	int m_iTexCarebearStare;
	int m_iTexFlower;
	int m_iTexFlowerLauncher;
	int m_iTexGrenade;
	int m_iTexMine;
	int m_iTexTranq;


private:
	C_BaseCombatWeapon *FindNextWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition);
	C_BaseCombatWeapon *FindPrevWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition);

	void DrawEmptyBox(C_BaseCombatWeapon *pWeapon, int x, int y, int wide, int tall, int number = -1);
	void DrawWeaponBox(C_BaseCombatWeapon *pWeapon, int x, int y, int wide, int tall, int number = - 1);
	void DrawNumber(int number, int x, int y);

	void ActivateFastswitchWeaponDisplay( C_BaseCombatWeapon *pWeapon );

	int GetLastPosInSlot( int iSlot ) const;
    
	void FastWeaponSwitch( int iWeaponSlot );
	void PlusTypeFastWeaponSwitch( int iWeaponSlot );

	virtual	void SetSelectedWeapon( C_BaseCombatWeapon *pWeapon ){ m_hSelectedWeapon = pWeapon; }
	virtual	void SetSelectedSlot( int slot ){ m_iSelectedSlot = slot; }
	void SetSelectedSlideDir( int dir ){ m_iSelectedSlideDir = dir; }

	CPanelAnimationVar(HFont, m_hNumberFont, "NumberFont", "HudSelectionNumbers");
	CPanelAnimationVar(HFont, m_hTextFont, "TextFont", "HudSelectionText");
	CPanelAnimationVar(Color, m_NumberColor, "NumberColor", "SelectionNumberFg");
	CPanelAnimationVar(Color, m_TextColor, "TextColor", "SelectionTextFg");

	CPanelAnimationVarAliasType(float, m_flSelectionNumberXPos, "SelectionNumberXPos", "4", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flSelectionNumberYPos, "SelectionNumberYPos", "4", "proportional_float");
	
	CPanelAnimationVar(int, m_iBoxWidth, "BoxWidth", "128");
	CPanelAnimationVar(int, m_iBoxHeight, "BoxHeight", "128");
	CPanelAnimationVar(int, m_iBoxHOffset, "BoxHOffset", "20");
	CPanelAnimationVar(int, m_iBoxVOffset, "BoxVOffset", "10");

	CPanelAnimationVar(float, m_flBlur, "Blur", "0" );
	CPanelAnimationVar(float, m_flAlphaOverride, "Alpha", "255" );
	CPanelAnimationVar(float, m_flSelectionAlphaOverride, "SelectionAlpha", "255");

	CPanelAnimationVar(float, m_flHorizWeaponSelectOffsetPoint, "WeaponBoxOffset", "0");

	bool m_bFadingOut;

	// fastswitch weapon display
	struct WeaponBox_t
	{
		int m_iSlot;
		int m_iSlotPos;
   	};

	CUtlVector<WeaponBox_t>	m_WeaponBoxes;
	int	m_iSelectedWeaponBox;
	int	m_iSelectedSlideDir;
	int	m_iSelectedBoxPosition;
	int	m_iSelectedSlot;
	C_BaseCombatWeapon*	m_pLastWeapon;	
};

DECLARE_HUDELEMENT( CHudWeaponSelection );

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudWeaponSelection::CHudWeaponSelection( const char *pElementName ) : CBaseHudWeaponSelection(pElementName), BaseClass(NULL, "HudWeaponSelection")
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetPaintBackgroundType( 1 );
	SetPaintBackgroundEnabled(true);

	m_bFadingOut = false;

	m_iBoxTexture = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iBoxTexture, "hud/cadre" , true, true);
	
	m_iTexUzi = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexUzi, "hud/weapons/uzi" , true, true);
	
	m_iTexFlower = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexFlower, "hud/weapons/flower" , true, true);
	
	m_iTexRevolver = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexRevolver, "hud/weapons/revolver" , true, true);

	m_iTexBazooka = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexBazooka, "hud/weapons/bazooka" , true, true);
	
	m_iTexBat = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexBat, "hud/weapons/bat" , true, true);
	
	m_iTexShotgun = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexShotgun, "hud/weapons/bigshotgun" , true, true);
	
	m_iTexCarebearStare = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexCarebearStare, "hud/weapons/carebearstare" , true, true);
	
	m_iTexFlowerLauncher = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexFlowerLauncher, "hud/weapons/flowerlauncher" , true, true);
	
	m_iTexGrenade = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexGrenade, "hud/weapons/grenade" , true, true);
	
	m_iTexMine = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexMine, "hud/weapons/mine" , true, true);
	
	m_iTexTranq = surface()->CreateNewTextureID();
    surface()->DrawSetTextureFile( m_iTexTranq, "hud/weapons/sniper" , true, true);
}
int CHudWeaponSelection::GetTextureForWeapon( const char* weapon)
{
	if(!strcmp(weapon, "weapon_uzi"))
		return m_iTexUzi;
	else if (!strcmp(weapon, "weapon_revolver"))
		return m_iTexRevolver;
	else if (!strcmp(weapon, "weapon_bazooka"))
		return m_iTexBazooka;
	else if (!strcmp(weapon, "weapon_bat"))
		return m_iTexBat;
	else if (!strcmp(weapon, "weapon_bigshotgun"))
		return m_iTexShotgun;	
	else if (!strcmp(weapon, "weapon_carebearstare"))
		return m_iTexCarebearStare;
	else if (!strcmp(weapon, "weapon_flower"))
		return m_iTexFlower;
	else if (!strcmp(weapon, "weapon_flowerlauncher"))
		return m_iTexFlowerLauncher;	
	else if (!strcmp(weapon, "weapon_grenade"))
		return m_iTexGrenade;
	else if (!strcmp(weapon, "weapon_mine"))
		return m_iTexMine;
	else if (!strcmp(weapon, "weapon_tranquilizer"))
		return m_iTexTranq;
	else
		return m_iBoxTexture;
}
bool CHudWeaponSelection::IsWeaponSelectable()
{ 
	if (IsInSelectionMode())
		return true;

	return false;
}

void CHudWeaponSelection::SetWeaponSelected()
{
	CBaseHudWeaponSelection::SetWeaponSelected();

	switch( hud_fastswitch.GetInt() )
	{
	case HUDTYPE_FASTSWITCH:
		ActivateFastswitchWeaponDisplay( GetSelectedWeapon() );
		break;

	default:

		break;
	}
}
	
//-----------------------------------------------------------------------------
// Purpose: sets up display for showing weapon pickup
//-----------------------------------------------------------------------------
void CHudWeaponSelection::OnWeaponPickup( C_BaseCombatWeapon *pWeapon )
{
	CHudHistoryResource *pHudHR = GET_HUDELEMENT( CHudHistoryResource );
	if ( pHudHR )
		pHudHR->AddToHistory( pWeapon );
}

//-----------------------------------------------------------------------------
// Purpose: updates animation status
//-----------------------------------------------------------------------------
void CHudWeaponSelection::OnThink( void )
{
	float flSelectionTimeout = SELECTION_TIMEOUT_THRESHOLD;
	float flSelectionFadeoutTime = SELECTION_FADEOUT_TIME;

	if (hud_fastswitch.GetBool())
	{
		flSelectionTimeout = FASTSWITCH_DISPLAY_TIMEOUT;
		flSelectionFadeoutTime = FASTSWITCH_FADEOUT_TIME;
	}

	// Time out after awhile of inactivity
	if ( gpGlobals->curtime - m_flSelectionTime > flSelectionTimeout )
	{
		if (!m_bFadingOut)
		{
			// start fading out
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "FadeOutWeaponSelectionMenu" );
			m_bFadingOut = true;
		}
		else if (gpGlobals->curtime - m_flSelectionTime > flSelectionTimeout + flSelectionFadeoutTime)
			// finished fade, close
			HideSelection();
	}
	else if (m_bFadingOut)
	{
		// stop us fading out, show the animation again
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "OpenWeaponSelectionMenu" );
		m_bFadingOut = false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the panel should draw
//-----------------------------------------------------------------------------
bool CHudWeaponSelection::ShouldDraw()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
	{
		if ( IsInSelectionMode() )
			HideSelection();

		return false;
	}

	bool bret = CBaseHudWeaponSelection::ShouldDraw();
	if (!bret)
		return false;

	// draw weapon selection a little longer if in fastswitch so we can see what we've selected
	if ( hud_fastswitch.GetBool() && gpGlobals->curtime - m_flSelectionTime < FASTSWITCH_DISPLAY_TIMEOUT + FASTSWITCH_FADEOUT_TIME )
		return true;

	return ( m_bSelectionVisible ) ? true : false;
}

void CHudWeaponSelection::LevelInit()
{
	CHudElement::LevelInit();

	m_iSelectedWeaponBox = -1;
	m_iSelectedSlideDir = 0;
	m_pLastWeapon = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: starts animating the center of the draw point to the newly selected weapon
//-----------------------------------------------------------------------------
void CHudWeaponSelection::ActivateFastswitchWeaponDisplay( C_BaseCombatWeapon *pSelectedWeapon )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	// make sure all our configuration data is read
	MakeReadyForUse();

	m_WeaponBoxes.RemoveAll();
	m_iSelectedWeaponBox = 0;

	// find out where our selected weapon is in the full list
	int cWeapons = 0;
	int iLastSelectedWeaponBox = -1;
	for ( int i = 0; i < MAX_WEAPON_SLOTS; i++ )
	{
		for (int slotpos = 0; slotpos < MAX_WEAPON_POSITIONS; slotpos++)
		{
			C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( i, slotpos );
			if ( !pWeapon )
				continue;

			WeaponBox_t box = { i, slotpos };
			m_WeaponBoxes.AddToTail( box );

			if ( pWeapon == pSelectedWeapon )
				m_iSelectedWeaponBox = cWeapons;

			if ( pWeapon == m_pLastWeapon )
				iLastSelectedWeaponBox = cWeapons;

			cWeapons++;
		}
	}

	if ( iLastSelectedWeaponBox == -1 )
		// unexpected failure, no last weapon to scroll from, default to snap behavior
		m_pLastWeapon = NULL;

	// calculate where we would have to start drawing for this weapon to slide into center
	float flStart, flStop, flTime;
	if ( !m_pLastWeapon || m_iSelectedSlideDir == 0 || m_flHorizWeaponSelectOffsetPoint != 0 )
	{
		// no previous weapon or weapon selected directly or selection during slide, snap to exact position
		m_pLastWeapon = pSelectedWeapon;
		flStart = flStop = flTime = 0;
	}
	else
	{
		// offset display for a scroll
		// causing selected weapon to slide into position
		// scroll direction based on user's "previous" or "next" selection
		int numIcons = 0;
		int start    = iLastSelectedWeaponBox;
		for (int i=0; i<cWeapons; i++)
		{
			// count icons in direction of slide to destination
			if ( start == m_iSelectedWeaponBox )
				break;

			if ( m_iSelectedSlideDir < 0 )
				start--;
     		else
				start++;

			// handle wraparound in either direction
			start = ( start + cWeapons ) % cWeapons;
			numIcons++;
		}

		flStart = numIcons * (m_iBoxWidth + m_iBoxHOffset);
		if ( m_iSelectedSlideDir < 0 )
			flStart *= -1;
		flStop = 0;

		// shorten duration for scrolling when desired weapon is farther away
		// otherwise a large skip in the same duration causes the scroll to fly too fast
		flTime = numIcons * 0.20f;
		if ( numIcons > 1 )
			flTime *= 0.5f;
	}
	m_flHorizWeaponSelectOffsetPoint = flStart;
	g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "WeaponBoxOffset", flStop, 0, flTime, AnimationController::INTERPOLATOR_LINEAR );
}

//-------------------------------------------------------------------------
// Purpose: draws the selection area
//-------------------------------------------------------------------------
void CHudWeaponSelection::Paint()
{
	/*if (!ShouldDraw())
		return;*/
		SetPaintBackgroundType( 1 );
	SetPaintBackgroundEnabled(true);
	
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;

	// find and display our current selection
	C_BaseCombatWeapon *pSelectedWeapon = NULL;
	switch ( hud_fastswitch.GetInt() )
	{
	case HUDTYPE_FASTSWITCH:
		pSelectedWeapon = pPlayer->GetActiveWeapon();
		break;

	default:
		pSelectedWeapon = GetSelectedWeapon();
		break;
	}

	if (!pSelectedWeapon)
		return;

	int height = (MAX_WEAPON_SLOTS - 1) * (m_iBoxHeight + m_iBoxVOffset);
	int xpos = 0;
	int ypos = YRES(480) - height;

	for (int i = 0; i < MAX_WEAPON_SLOTS; i++)
	{
		//BP Calcul de la position de départ de la ligne courante
		if(pSelectedWeapon->GetSlot() == i)
			xpos = XRES(640) - (GetLastPosInSlot(i) * m_iBoxHOffset + m_iBoxWidth - 2 * m_iBoxHOffset);
		else
			xpos = XRES(640) - ((GetLastPosInSlot(i) + 1) * m_iBoxHOffset);

		for (int slotpos = GetLastPosInSlot(i); slotpos >= 0; slotpos--){
			C_BaseCombatWeapon *pWeapon = GetWeaponInSlot(i, slotpos);
			if (!pWeapon || pWeapon != pSelectedWeapon){
				if(slotpos == 0 && pSelectedWeapon->GetSlot() != i)
					DrawEmptyBox(pSelectedWeapon, xpos, ypos, m_iBoxWidth, m_iBoxHeight, i + 1);
				else
					DrawEmptyBox(pSelectedWeapon, xpos, ypos, m_iBoxWidth, m_iBoxHeight);

				xpos += m_iBoxHOffset;
			}
			else{
				DrawWeaponBox(pSelectedWeapon, xpos, ypos, m_iBoxWidth, m_iBoxHeight, i + 1);
				
				//BP décalage plus important pour laisser de la place à la boîte sélectionnée
				xpos += m_iBoxWidth - 2 * m_iBoxHOffset;
			}
		}
		
		ypos += m_iBoxHeight + m_iBoxVOffset; //BP On passe à la ligne du dessous
	}	
}
void CHudWeaponSelection::PaintBackground()
{
	SetBgColor(Color(0,0,0,1));
	return BaseClass::PaintBackground();
}
//-----------------------------------------------------------------------------
// Purpose: draws an empty weapon selection box
//-----------------------------------------------------------------------------
void CHudWeaponSelection::DrawEmptyBox(C_BaseCombatWeapon *pWeapon,int x, int y, int wide, int tall, int number)
{
	surface()->DrawSetTexture(m_iBoxTexture);
    surface()->DrawTexturedRect(x, y, x + wide, y + tall);

	DrawNumber(number, x + m_flSelectionNumberXPos, y + m_flSelectionNumberYPos);

}

//-----------------------------------------------------------------------------
// Purpose: draws a single weapon selection box
//-----------------------------------------------------------------------------
void CHudWeaponSelection::DrawWeaponBox(C_BaseCombatWeapon *pWeapon, int x, int y, int boxWide, int boxTall, int number)
{

	surface()->DrawSetTexture(GetTextureForWeapon(pWeapon->GetName()));
    surface()->DrawTexturedRect(x, y, x + boxWide, y + boxTall);
	
	DrawNumber(number, x + m_flSelectionNumberXPos, y + m_flSelectionNumberYPos);

	Color col = m_TextColor;

	//ICON
	/*col[3] *= alpha / 255.0f;
	if (pWeapon->GetSpriteActive())
	{
		// find the center of the box to draw in
		int iconWidth = pWeapon->GetSpriteActive()->Width();
		int iconHeight = pWeapon->GetSpriteActive()->Height();

		int x_offs = (boxWide - iconWidth) / 2;
		int y_offs = hud_fastswitch.GetInt() != 0 ? (boxTall / 1.5f - iconHeight) / 2 : (boxTall - iconHeight) / 2;

		if (!pWeapon->CanBeSelected())
			col = Color(255, 0, 0, col[3]);
		else
		{
			col[3] = alpha;
			pWeapon->GetSpriteInactive()->DrawSelf(x + x_offs, y + y_offs, col);
		}
	}*/

	//DrawNumber(number, x + m_flSelectionNumberXPos, y + m_flSelectionNumberYPos);
}

void CHudWeaponSelection::DrawNumber(int number, int x, int y)
{
	if (number >= 0)
	{	
		wchar_t wch = '0' + number;

		surface()->DrawSetTextFont(m_hNumberFont);
		surface()->DrawSetTextColor(m_NumberColor);
		surface()->DrawSetTextPos(x + m_flSelectionNumberXPos, y + m_flSelectionNumberYPos);
		surface()->DrawUnicodeChar(wch);
	}
}

//-----------------------------------------------------------------------------
// Purpose: hud scheme settings
//-----------------------------------------------------------------------------
void CHudWeaponSelection::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	SetPaintBackgroundEnabled(false);

	int screenWide, screenTall;
	int x, y;
	GetPos(x, y);
	GetHudSize(screenWide, screenTall);

	SetBounds( x, y, screenWide - x, screenTall - y );
}

//-----------------------------------------------------------------------------
// Purpose: Opens weapon selection control
//-----------------------------------------------------------------------------
void CHudWeaponSelection::OpenSelection( void )
{
	Assert(!IsInSelectionMode());

	CBaseHudWeaponSelection::OpenSelection();
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("OpenWeaponSelectionMenu");
	m_iSelectedBoxPosition = 0;
	m_iSelectedSlot = -1;
}

//-----------------------------------------------------------------------------
// Purpose: Closes weapon selection control immediately
//-----------------------------------------------------------------------------
void CHudWeaponSelection::HideSelection( void )
{
	CBaseHudWeaponSelection::HideSelection();
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("CloseWeaponSelectionMenu");
	m_bFadingOut = false;
}

//-----------------------------------------------------------------------------
// Purpose: Returns the next available weapon item in the weapon selection
//-----------------------------------------------------------------------------
C_BaseCombatWeapon *CHudWeaponSelection::FindNextWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return NULL;

	C_BaseCombatWeapon *pNextWeapon = NULL;

	// search all the weapons looking for the closest next
	int iLowestNextSlot = MAX_WEAPON_SLOTS;
	int iLowestNextPosition = MAX_WEAPON_POSITIONS;
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = pPlayer->GetWeapon(i);
		if ( !pWeapon )
			continue;

		if ( CanBeSelectedInHUD( pWeapon ) )
		{
			int weaponSlot = pWeapon->GetSlot(), weaponPosition = pWeapon->GetPosition();

			// see if this weapon is further ahead in the selection list
			if ( weaponSlot > iCurrentSlot || (weaponSlot == iCurrentSlot && weaponPosition > iCurrentPosition) )
			{
				// see if this weapon is closer than the current lowest
				if ( weaponSlot < iLowestNextSlot || (weaponSlot == iLowestNextSlot && weaponPosition < iLowestNextPosition) )
				{
					iLowestNextSlot = weaponSlot;
					iLowestNextPosition = weaponPosition;
					pNextWeapon = pWeapon;
				}
			}
		}
	}

	return pNextWeapon;
}

//-----------------------------------------------------------------------------
// Purpose: Returns the prior available weapon item in the weapon selection
//-----------------------------------------------------------------------------
C_BaseCombatWeapon *CHudWeaponSelection::FindPrevWeaponInWeaponSelection(int iCurrentSlot, int iCurrentPosition)
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return NULL;

	C_BaseCombatWeapon *pPrevWeapon = NULL;

	// search all the weapons looking for the closest next
	int iLowestPrevSlot = -1;
	int iLowestPrevPosition = -1;
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = pPlayer->GetWeapon(i);
		if ( !pWeapon )
			continue;

		if ( CanBeSelectedInHUD( pWeapon ) )
		{
			int weaponSlot = pWeapon->GetSlot(), weaponPosition = pWeapon->GetPosition();

			// see if this weapon is further ahead in the selection list
			if ( weaponSlot < iCurrentSlot || (weaponSlot == iCurrentSlot && weaponPosition < iCurrentPosition) )
			{
				// see if this weapon is closer than the current lowest
				if ( weaponSlot > iLowestPrevSlot || (weaponSlot == iLowestPrevSlot && weaponPosition > iLowestPrevPosition) )
				{
					iLowestPrevSlot = weaponSlot;
					iLowestPrevPosition = weaponPosition;
					pPrevWeapon = pWeapon;
				}
			}
		}
	}

	return pPrevWeapon;
}

//-----------------------------------------------------------------------------
// Purpose: Moves the selection to the next item in the menu
//-----------------------------------------------------------------------------
void CHudWeaponSelection::CycleToNextWeapon( void )
{
	// Get the local player.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = pPlayer->GetActiveWeapon();

	C_BaseCombatWeapon *pNextWeapon = NULL;
	if ( IsInSelectionMode() )
	{
		// find the next selection spot
		C_BaseCombatWeapon *pWeapon = GetSelectedWeapon();
		if ( !pWeapon )
			return;

		pNextWeapon = FindNextWeaponInWeaponSelection( pWeapon->GetSlot(), pWeapon->GetPosition() );
	}
	else
	{
		// open selection at the current place
		pNextWeapon = pPlayer->GetActiveWeapon();
		if ( pNextWeapon )
			pNextWeapon = FindNextWeaponInWeaponSelection( pNextWeapon->GetSlot(), pNextWeapon->GetPosition() );
	}

	if ( !pNextWeapon )
		// wrap around back to start
		pNextWeapon = FindNextWeaponInWeaponSelection(-1, -1);

	if ( pNextWeapon )
	{
		SetSelectedWeapon( pNextWeapon );
		SetSelectedSlideDir( 1 );

		if ( !IsInSelectionMode() )
			OpenSelection();

		// Play the "cycle to next weapon" sound
		pPlayer->EmitSound( "Player.WeaponSelectionMoveSlot" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Moves the selection to the previous item in the menu
//-----------------------------------------------------------------------------
void CHudWeaponSelection::CycleToPrevWeapon( void )
{
	// Get the local player.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = pPlayer->GetActiveWeapon();

	C_BaseCombatWeapon *pNextWeapon = NULL;
	if ( IsInSelectionMode() )
	{
		// find the next selection spot
		C_BaseCombatWeapon *pWeapon = GetSelectedWeapon();
		if ( !pWeapon )
			return;

		pNextWeapon = FindPrevWeaponInWeaponSelection( pWeapon->GetSlot(), pWeapon->GetPosition() );
	}
	else
	{
		// open selection at the current place
		pNextWeapon = pPlayer->GetActiveWeapon();
		if ( pNextWeapon )
			pNextWeapon = FindPrevWeaponInWeaponSelection( pNextWeapon->GetSlot(), pNextWeapon->GetPosition() );
	}

	if ( !pNextWeapon )
		// wrap around back to end of weapon list
		pNextWeapon = FindPrevWeaponInWeaponSelection(MAX_WEAPON_SLOTS, MAX_WEAPON_POSITIONS);

	if ( pNextWeapon )
	{
		SetSelectedWeapon( pNextWeapon );
		SetSelectedSlideDir( -1 );

		if ( !IsInSelectionMode() )
			OpenSelection();

		// Play the "cycle to next weapon" sound
		pPlayer->EmitSound( "Player.WeaponSelectionMoveSlot" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns the # of the last weapon in the specified slot
//-----------------------------------------------------------------------------
int CHudWeaponSelection::GetLastPosInSlot( int iSlot ) const
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	int iMaxSlotPos;

	if ( !player )
		return -1;

	iMaxSlotPos = -1;
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = player->GetWeapon(i);
		
		if ( pWeapon == NULL )
			continue;

		if ( pWeapon->GetSlot() == iSlot && pWeapon->GetPosition() > iMaxSlotPos )
			iMaxSlotPos = pWeapon->GetPosition();
	}

	return iMaxSlotPos;
}

//-----------------------------------------------------------------------------
// Purpose: returns the weapon in the specified slot
//-----------------------------------------------------------------------------
C_BaseCombatWeapon *CHudWeaponSelection::GetWeaponInSlot( int iSlot, int iSlotPos )
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if ( !player )
		return NULL;

	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		C_BaseCombatWeapon *pWeapon = player->GetWeapon(i);
		if ( pWeapon == NULL )
			continue;

		if ( pWeapon->GetSlot() == iSlot && pWeapon->GetPosition() == iSlotPos )
			return pWeapon;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Opens the next weapon in the slot
//-----------------------------------------------------------------------------
void CHudWeaponSelection::FastWeaponSwitch( int iWeaponSlot )
{
	// get the slot the player's weapon is in
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = NULL;

	// see where we should start selection
	int iPosition = -1;
	C_BaseCombatWeapon *pActiveWeapon = pPlayer->GetActiveWeapon();
	if ( pActiveWeapon && pActiveWeapon->GetSlot() == iWeaponSlot )
		// start after this weapon
		iPosition = pActiveWeapon->GetPosition();

	C_BaseCombatWeapon *pNextWeapon = NULL;

	// search for the weapon after the current one
	pNextWeapon = FindNextWeaponInWeaponSelection(iWeaponSlot, iPosition);
	// make sure it's in the same bucket
	if ( !pNextWeapon || pNextWeapon->GetSlot() != iWeaponSlot )
		// just look for any weapon in this slot
		pNextWeapon = FindNextWeaponInWeaponSelection(iWeaponSlot, -1);

	// see if we found a weapon that's different from the current and in the selected slot
	if ( pNextWeapon && pNextWeapon != pActiveWeapon && pNextWeapon->GetSlot() == iWeaponSlot )
		// select the new weapon
		::input->MakeWeaponSelection( pNextWeapon );
	else if ( pNextWeapon != pActiveWeapon )
		pPlayer->EmitSound( "Player.DenyWeaponSelection" );
}

//-----------------------------------------------------------------------------
// Purpose: Opens the next weapon in the slot
//-----------------------------------------------------------------------------
void CHudWeaponSelection::PlusTypeFastWeaponSwitch( int iWeaponSlot )
{
	// get the slot the player's weapon is in
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	m_pLastWeapon = NULL;
	int newSlot = m_iSelectedSlot;

	// Changing slot number does not necessarily mean we need to change the slot - the player could be
	// scrolling through the same slot but in the opposite direction. Slot pairs are 0,2 and 1,3 - so
	// compare the 0 bits to see if we're within a pair. Otherwise, reset the box to the zero position.
	if ( -1 == m_iSelectedSlot || ( ( m_iSelectedSlot ^ iWeaponSlot ) & 1 ) )
	{
		// Changing vertical/horizontal direction. Reset the selected box position to zero.
		m_iSelectedBoxPosition = 0;
		m_iSelectedSlot = iWeaponSlot;
	}
	else
	{
		// Still in the same horizontal/vertical direction. Determine which way we're moving in the slot.
		int increment = 1;
		if ( m_iSelectedSlot != iWeaponSlot )
		{
			// Decrementing within the slot. If we're at the zero position in this slot, 
			// jump to the zero position of the opposite slot. This also counts as our increment.
			increment = -1;
			if ( 0 == m_iSelectedBoxPosition )
			{
				newSlot = ( m_iSelectedSlot + 2 ) % 4;
				increment = 0;
			}
		}

		// Find out of the box position is at the end of the slot
		int lastSlotPos = -1;
		for ( int slotPos = 0; slotPos < MAX_WEAPON_POSITIONS; ++slotPos )
		{
			C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( newSlot, slotPos );
			if ( pWeapon )
				lastSlotPos = slotPos;
		}

		// Increment/Decrement the selected box position
		if ( m_iSelectedBoxPosition + increment <= lastSlotPos )
		{
			m_iSelectedBoxPosition += increment;
			m_iSelectedSlot = newSlot;
		}
		else
		{
			pPlayer->EmitSound( "Player.DenyWeaponSelection" );

			return;
		}
	}

	// Select the weapon in this position
	bool bWeaponSelected = false;
	C_BaseCombatWeapon *pActiveWeapon = pPlayer->GetActiveWeapon();
	C_BaseCombatWeapon *pWeapon = GetWeaponInSlot( m_iSelectedSlot, m_iSelectedBoxPosition );
	if ( pWeapon )
	{
		if ( pWeapon != pActiveWeapon )
		{
			// Select the new weapon
			::input->MakeWeaponSelection( pWeapon );
			SetSelectedWeapon( pWeapon );
			bWeaponSelected = true;
		}
	}

	if ( !bWeaponSelected )
		// Still need to set this to make hud display appear
		SetSelectedWeapon( pPlayer->GetActiveWeapon() );
}

//-----------------------------------------------------------------------------
// Purpose: Moves selection to the specified slot
//-----------------------------------------------------------------------------
void CHudWeaponSelection::SelectWeaponSlot( int iSlot )
{
	// iSlot is one higher than it should be, since it's the number key, not the 0-based index into the weapons
	--iSlot;

	// Get the local player.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	// Don't try and read past our possible number of slots
	if ( iSlot > MAX_WEAPON_SLOTS )
		return;
	
	// Make sure the player's allowed to switch weapons
	if ( pPlayer->IsAllowedToSwitchWeapons() == false )
		return;

	switch( hud_fastswitch.GetInt() )
	{
	case HUDTYPE_FASTSWITCH:
		{
			FastWeaponSwitch( iSlot );
			return;
		}

	case HUDTYPE_BUCKETS:
		{
			int slotPos = 0;
			C_BaseCombatWeapon *pActiveWeapon = GetSelectedWeapon();

			// start later in the list
			if ( IsInSelectionMode() && pActiveWeapon && pActiveWeapon->GetSlot() == iSlot )
				slotPos = pActiveWeapon->GetPosition() + 1;

			// find the weapon in this slot
			pActiveWeapon = GetNextActivePos( iSlot, slotPos );
			if ( !pActiveWeapon )
				pActiveWeapon = GetNextActivePos( iSlot, 0 );
			
			if ( pActiveWeapon != NULL )
			{
				if ( !IsInSelectionMode() )
					// open the weapon selection
					OpenSelection();

				// Mark the change
				SetSelectedWeapon( pActiveWeapon );
				SetSelectedSlideDir( 0 );
			}
		}

	default:
		{
			// do nothing
		}
		break;
	}

	pPlayer->EmitSound( "Player.WeaponSelectionMoveSlot" );
}