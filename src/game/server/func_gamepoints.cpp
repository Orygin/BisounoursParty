#include "cbase.h"
#include "hl2mp_gamerules.h"
#include "multiplay_gamerules.h"
#include "player.h"
#include "team.h"

#include "doors.h"
#include "ndebugoverlay.h"
#include "spark.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "tier1/strtools.h"
#include "buttons.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define SF_BUTTON_DONTMOVE				1
#define SF_ROTBUTTON_NOTSOLID			1
#define	SF_BUTTON_TOGGLE				32		// button stays pushed until reactivated
#define SF_BUTTON_TOUCH_ACTIVATES		256		// Button fires when touched.
#define SF_BUTTON_DAMAGE_ACTIVATES		512		// Button fires when damaged.
#define SF_BUTTON_USE_ACTIVATES			1024	// Button fires when used.
#define SF_BUTTON_LOCKED				2048	// Whether the button is initially locked.
#define	SF_BUTTON_SPARK_IF_OFF			4096	// button sparks in OFF state
#define	SF_BUTTON_JIGGLE_ON_USE_LOCKED	8192	// whether to jiggle if someone uses us when we're locked

class CFuncGamePoints : public CBaseButton
{
public:
	DECLARE_CLASS(CFuncGamePoints, CBaseButton);
	DECLARE_DATADESC();

	void Spawn();
	void ButtonUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
private:
	int m_nPointCost;
	char *m_sUseSound;
};

LINK_ENTITY_TO_CLASS( func_gamepoints, CFuncGamePoints );
 
BEGIN_DATADESC( CFuncGamePoints )
	DEFINE_KEYFIELD( m_nPointCost, FIELD_INTEGER, "points" ),
	DEFINE_KEYFIELD( m_sUseSound, FIELD_STRING, "usesound" ),
END_DATADESC()

void CFuncGamePoints::Spawn()
{
	BaseClass::Spawn();

	if (HasSpawnFlags(SF_BUTTON_USE_ACTIVATES))
	{
		SetUse(&CFuncGamePoints::ButtonUse);
	}
	
	SetMoveType( MOVETYPE_PUSH );
	SetSolid( SOLID_BSP );
	SetModel( STRING( GetModelName() ) );

	m_toggle_state = TS_AT_BOTTOM;

	VPhysicsInitShadow( false, false );
}

void CFuncGamePoints::ButtonUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if (m_toggle_state == TS_GOING_UP || m_toggle_state == TS_GOING_DOWN )
		return;

	if (m_bLocked)
	{
		OnUseLocked( pActivator );
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer*)pActivator;
	if(!pPlayer)
		return;

	if(pPlayer->FragCount() < m_nPointCost)
	{
		OnUseLocked( pActivator );
		return;
	}

	m_hActivator = pActivator;

	if ( m_toggle_state == TS_AT_TOP)
	{
		//
		// If it's a toggle button it can return now. Otherwise, it will either
		// return on its own or will stay pressed indefinitely.
		//
		if ( HasSpawnFlags(SF_BUTTON_TOGGLE))
		{
			if ( m_sNoise != NULL_STRING )
			{
				CPASAttenuationFilter filter( this );

				EmitSound_t ep;
				ep.m_nChannel = CHAN_VOICE;
				ep.m_pSoundName = (char*)STRING(m_sNoise);
				ep.m_flVolume = 1;
				ep.m_SoundLevel = SNDLVL_NORM;

				EmitSound( filter, entindex(), ep );
			}

			pPlayer->AddPoints(-m_nPointCost, false);
	
			CPASAttenuationFilter filter( pActivator );

			EmitSound_t ep;
			ep.m_nChannel = CHAN_ITEM;
			ep.m_pSoundName = m_sUseSound;
			ep.m_flVolume = 1.0f;
			ep.m_SoundLevel = SNDLVL_NORM;
		
			CBaseEntity::EmitSound( filter, pActivator->entindex(), ep );
			m_OnPressed.FireOutput(m_hActivator, this);
			ButtonReturn();
		}
	}
	else
	{
		pPlayer->AddPoints(-m_nPointCost, false);
	
		CPASAttenuationFilter filter( pActivator );

		EmitSound_t ep;
		ep.m_nChannel = CHAN_ITEM;
		ep.m_pSoundName = m_sUseSound;
		ep.m_flVolume = 1.0f;
		ep.m_SoundLevel = SNDLVL_NORM;
		
		CBaseEntity::EmitSound( filter, pActivator->entindex(), ep );
		m_OnPressed.FireOutput(m_hActivator, this);
		ButtonActivate( );
	}
}