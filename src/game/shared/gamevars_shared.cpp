//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "gamevars_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef GAME_DLL
void MPForceCameraCallback( IConVar *var, const char *pOldString, float flOldValue )
{
	if ( mp_forcecamera.GetInt() < OBS_ALLOW_ALL || mp_forcecamera.GetInt() >= OBS_ALLOW_NUM_MODES )
	{
		mp_forcecamera.SetValue( OBS_ALLOW_TEAM );
	}
}
#endif 

// some shared cvars used by game rules
ConVar mp_forcecamera( 
	"mp_forcecamera", 
	"1", 
	FCVAR_REPLICATED,
	"Restricts spectator modes for dead players"
#ifdef GAME_DLL
	, MPForceCameraCallback 
#endif
	);
	
ConVar mp_allowspectators(
	"mp_allowspectators", 
	"0", 
	FCVAR_REPLICATED,
	"toggles whether the server allows spectator mode or not" );

ConVar friendlyfire(
	"mp_friendlyfire",
	"0",
	FCVAR_REPLICATED | FCVAR_NOTIFY,
	"Allows team members to injure other members of their team"
	);

ConVar mp_fadetoblack( 
	"mp_fadetoblack", 
	"0", 
	FCVAR_REPLICATED | FCVAR_NOTIFY, 
	"fade a player's screen to black when he dies" );

ConVar mp_forts_buildtime( "mp_forts_buildtime", "3", FCVAR_REPLICATED | FCVAR_NOTIFY );
ConVar mp_forts_firstbuildtime( "mp_forts_firstbuildtime", "8", FCVAR_REPLICATED | FCVAR_NOTIFY );
ConVar mp_forts_fighttime( "mp_forts_fighttime", "8", FCVAR_REPLICATED | FCVAR_NOTIFY );
ConVar mp_forts_roundlimit( "mp_forts_roundlimit", "3", FCVAR_REPLICATED | FCVAR_NOTIFY );
ConVar mp_forts_egality("mp_forts_egality", "1", FCVAR_REPLICATED | FCVAR_NOTIFY);
ConVar mp_dom_scoretime( "mp_dom_scoretime", "5", FCVAR_REPLICATED);
ConVar mp_dom_scoremultiplier("mp_dom_scoremultiplier", "2", FCVAR_REPLICATED);
ConVar mp_co_difficulty("mp_co_difficulty","1", FCVAR_REPLICATED | FCVAR_NOTIFY);
ConVar mp_scorelimit_dom("mp_scorelimit_dom", "200", FCVAR_REPLICATED | FCVAR_NOTIFY);
ConVar mp_scorelimit_tdm("mp_scorelimit_tdm", "30", FCVAR_REPLICATED | FCVAR_NOTIFY);
ConVar mp_scorelimit_forts("mp_scorelimit_forts", "50", FCVAR_REPLICATED | FCVAR_NOTIFY);
ConVar mp_scorelimit_push("mp_scorelimit_push", "10", FCVAR_REPLICATED | FCVAR_NOTIFY);