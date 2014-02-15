// BISOUNOURS PARTY : GRENADE (npc)

#ifndef GRENADE_BOING_H
#define GRENADE_BOING_H
#pragma once

class CBaseGrenade;
struct edict_t;

CBaseGrenade *GrenadeBoing_Create( const Vector &position, const QAngle &angles, const Vector &velocity, const AngularImpulse &angVelocity, CBaseEntity *pOwner, float timer, bool combineSpawned );
bool GrenadeBoing_WasPunted( const CBaseEntity *pEntity );

#endif
