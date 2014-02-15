#ifndef ACHLISTENER
#define ACHLISTENER
#pragma once

#include "cbase.h"
#include "GameEventListener.h"
#include "hl2orange.spa.h"
#include "Cloud\Nebuleuse.h"

class bp_achievListener : public CGameEventListener // Will listen the earn and updates of achievements.
{
	public:
	DECLARE_CLASS_NOBASE( bp_achievListener );

	DECLARE_SIMPLE_DATADESC();

	bp_achievListener() 
	{
		m_iHealthTaken = 0;
		m_flRangeMineSuicide = 0;
		StartToListen();
	};
	void StartToListen();
	void FireGameEvent(IGameEvent *event);
	void Think();
	void SendEarnEvent(char* name);
	void SendEarnEvent(const char* name);
protected:
	float m_flRangeMineSuicide;
	float m_flRangeMineKill;
	int m_iHealthTaken;

private:
};
#endif