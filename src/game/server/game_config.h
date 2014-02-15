
#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include "maprules.h"

class CGameConfig : public CRulePointEntity
{
public:
	DECLARE_CLASS( CGameConfig, CRulePointEntity );
	DECLARE_DATADESC();

	CGameConfig(void);
	void Spawn(void);
	void Activate(void);
	void SetGameType(void);
	void SetMultiplayerVars(void);
	void SetLimits(void);

	void RulesThink(void);

	void InputForceSkillOutput(inputdata_t &inputData);
	void InputChangeTimeLimit(inputdata_t &inputData);
	void InputAddTimeLimit(inputdata_t &inputData);
	void InputWhichTeamWinning(inputdata_t &inputData);
	void ForceSkillOutput(void);

	void PushNewRound();
	void TeamWinning(int team); //-1 is no team

private:
	int m_iGameType;
	int m_iBunnyhop;
	int m_iTimeLimit;
	int m_iFragLimit;
	int m_iPogostick;
	int m_iStartArmor;
	int m_iFallDamage;
	int m_iFlashlight;
	int m_iPsychicTimeLimit; // TimeLimit - n : m_OnTimeLimitAlmostReached

	int		m_iDomScoreMultiplier;
	float	m_flDomScoreTimer;

	bool	m_bLastBuildPhaseState;
	bool	m_bDontDropWeapons;
	bool	m_bOnlyGiveFlower;

	COutputEvent m_OnFortsBuildPhase;
	COutputEvent m_OnFortsFightPhase;
	COutputEvent m_OnTimeLimitAlmostReached;

	COutputEvent m_OnSkillEasy;
	COutputEvent m_OnSkillMedium;
	COutputEvent m_OnSkillHard;
	COutputEvent m_OnSkillBisounours;

	COutputEvent m_OnEasyMapSpawn;
	COutputEvent m_OnMediumMapSpawn;
	COutputEvent m_OnHardMapSpawn;
	COutputEvent m_OnBisounoursMapSpawn;

	COutputEvent m_OnPinkWinning;
	COutputEvent m_OnGreenWinning;
	COutputEvent m_OnNoneWinning;

	COutputEvent m_OnPushNewRound;
};

LINK_ENTITY_TO_CLASS( game_config, CGameConfig );

extern CGameConfig *g_GameConfig;

#endif // #ifndef GAME_CONFIG_H