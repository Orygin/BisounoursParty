#include "cbase.h"
#include "triggers.h"
#include "trigger_voidfall.h"
#include "game_entities.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CTriggerVoidfall::Activate(void)
{
	BaseClass::Activate();

	SetTouch( &CTriggerVoidfall::FallTouch );
}

void CTriggerVoidfall::FallTouch(CBaseEntity *pEntity)
{
	CPushBall *pBall = dynamic_cast<CPushBall*>(pEntity);
	if(pBall)
	{
		if(pBall->IsActive())
		{
			pBall->ResetBall();
			return;	
		}
	}

	CTakeDamageInfo info( this, this, abs(pEntity->GetHealth()*2), DMG_FALL );
	info.SetDamagePosition( pEntity->GetAbsOrigin() );
	info.SetDamageForce(Vector(0, 0, 0));
	pEntity->TakeDamage( info );

	CBasePlayer *pPlayer = ToBasePlayer(pEntity);
	if(pPlayer == NULL)
		return;

	pPlayer->EmitSound("Player.VoidFall");

#if 0 // Le fadetoblack rend pas bien il paraît
	color32 black = {0, 0, 0, 255};
	UTIL_ScreenFade( pPlayer, black, 1.5, 9999, FFADE_OUT | FFADE_STAYOUT );
#endif
	

	m_OnFall.FireOutput(pPlayer, pPlayer);
}

BEGIN_DATADESC( CTriggerVoidfall )
	DEFINE_OUTPUT( m_OnFall, "OnFall" ),
END_DATADESC()
