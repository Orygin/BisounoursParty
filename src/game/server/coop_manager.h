#ifndef COOP_MANAGER_H
#define COOP_MANAGER_H
#pragma once

#include "maprules.h"

class CCoopManager : public CRulePointEntity
{
public:
	DECLARE_CLASS( CCoopManager, CRulePointEntity );
	DECLARE_DATADESC();
	CCoopManager();
	void Spawn();

	void WaveThink();

	void InputSpawnWave(inputdata_t &inputData);
	void InputStop(inputdata_t &inputData);
	void InputStart(inputdata_t &inputData);

private:
	int m_iMaxWaves;
	int m_iTimeBtWaves;
	int m_iBaseZombies;
	int m_iSpawnDelta; //Ecah new wave will be lastwave + ((lastwave+delta)/(delta/10))

	COutputEvent m_OnWaveSpawn;
	COutputEvent m_OnFirstWave;
	COutputEvent m_OnLastWave;
	COutputEvent m_OnWaveCleared;
};
LINK_ENTITY_TO_CLASS( coop_manager, CCoopManager );
#endif