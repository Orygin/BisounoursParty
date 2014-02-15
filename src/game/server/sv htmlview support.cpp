//========= Public Domain 2009, Julian 'Daedalus' Thatcher. =====================//
 //
 // Purpose: HTMLView Server Entity Triggering implementation
 //
 // Ingame Usage Commands:
 //  sv_ent_fire entity [action] [parameter]		Functions as ent_fire command
 // 
 // $Created: Thursday, 26 April 2007
 // $LastUpdated: Thursday, 5th February 2007
 // $Author:  Julian 'Daedalus' Thatcher (daedalus.raistlin@gmail.com)
 // $NoKeywords: $
 //=============================================================================//
 
 #include "cbase.h"
 #include "EventQueue.h"
 
 // memdbgon must be the last include file in a .cpp file!!!
 #include "tier0/memdbgon.h"
 
 void CC_sv_ent_fire_f (const CCommand& args)
 {
 	// fires a command from the console
 	CBasePlayer *pPlayer = ToBasePlayer( UTIL_GetCommandClient() );
 
 	if(!pPlayer)
 	{
 		// If we can't get it, use this class
 		pPlayer = NULL;
 	}
 
 
 	if ( args.ArgC() < 2 )
 	{
 		ClientPrint( pPlayer, HUD_PRINTCONSOLE, "Usage:\n   ent_fire <target> [action] [value] [delay]\n" );
 	}
 	else
 	{
 		const char *target = "", *action = "Trigger";
 		variant_t value;
 		int delay = 0;
 
 		target = STRING( AllocPooledString(args[1]) );
 		if ( args.ArgC() >= 3 )
 		{
 			action = STRING( AllocPooledString(args[2]) );
 		}
 		if ( args.ArgC() >= 4 )
 		{
 			value.SetString( AllocPooledString(args[3]) );
 		}
 		if ( args.ArgC() >= 5 )
 		{
 			delay = atoi( args[4] );
 		}
 
 		g_EventQueue.AddEvent( target, action, value, delay, pPlayer, pPlayer );
 	}
 }
 static ConCommand sv_ent_fire("sv_ent_fire", CC_sv_ent_fire_f, "Fires an entity serverside", 0);