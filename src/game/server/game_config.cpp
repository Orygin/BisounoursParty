#include "cbase.h"
#include "hl2mp_gamerules.h"
#include "multiplay_gamerules.h"
#include "player.h"
#include "team.h"
#include "triggers.h"
#include "playerSkins.h"
#include "eventqueue.h"
#include "game_config.h"
#include "game_entities.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar mp_co_difficulty;

CGameConfig::CGameConfig(void)
{
	m_iGameType = -1;
	m_iBunnyhop = -1;
	m_iTimeLimit = -1;
	m_iFragLimit = -1;
	m_iPogostick = -1;
	m_iStartArmor = -1;
	m_iFallDamage = -1;
	m_iFlashlight = -1;
	m_iPsychicTimeLimit = -1;

	m_iDomScoreMultiplier = -1;
	m_flDomScoreTimer = -1;

	m_bDontDropWeapons = false;
	m_bOnlyGiveFlower = false;
	g_GameConfig = this;
}


ConVar sv_disable_gameconfig( "sv_disable_gameconfig", "0", FCVAR_REPLICATED | FCVAR_CHEAT );
void CGameConfig::Spawn(void)
{
	if(sv_disable_gameconfig.GetBool())
		return;

	BaseClass::Spawn();
	SetGameType();

	SetLimits();

	SetThink( &CGameConfig::RulesThink );
	SetNextThink( gpGlobals->curtime + 0.1f);

	switch(mp_co_difficulty.GetInt())
	{
	case 1:
		m_OnEasyMapSpawn.FireOutput(NULL, this);
		break;
	case 2:
		m_OnMediumMapSpawn.FireOutput(NULL, this);
		break;
	case 3:
		m_OnHardMapSpawn.FireOutput(NULL, this);
		break;
	case 4:
		m_OnBisounoursMapSpawn.FireOutput(NULL, this);
		break;
	};
}
void CGameConfig::Activate(void)
{
	SetMultiplayerVars();
	BaseClass::Activate();
}
void CGameConfig::SetGameType(void)
{
	if(m_iGameType < 0 || m_iGameType > GAME_COOP)
		return;
	Msg("Setting GameType");
	switch(m_iGameType)
	{
	case GAME_CTB:
	case GAME_DOM:
	case GAME_TDM:
	case GAME_PUSH:
		Msg(" CTB,DOM,TDM,PUSH\n");
		HL2MPRules()->SetTeamplay(true);
		break;
	case GAME_FORTS:
		Msg(" Forts\n");
		HL2MPRules()->SetTeamplay(true);
		HL2MPRules()->SetFortsBuildPhase(true);
		m_bLastBuildPhaseState = true;
		break;
	case GAME_DM:
		HL2MPRules()->SetTeamplay(cvar->FindVar( "mp_teamplay" )->GetBool());
		break;
	case GAME_COOP:
		Msg(" COOP\n");
		HL2MPRules()->SetTeamplay(false);
		break;
	case GAME_KOTH:
		Msg(" KOTH\n");
		break;
	}

	HL2MPRules()->SetGameType(m_iGameType);
	DevMsg("game_info : GameType #%d selected\n", m_iGameType);
}
void CGameConfig::RulesThink(void)
{
	SetNextThink(gpGlobals->curtime + 0.1f);

	if(HL2MPRules()->GetGameType() == GAME_FORTS)
	{
		if(HL2MPRules()->IsFortsBuildPhase() == m_bLastBuildPhaseState)
			return; // Rien à chagé : on s'casse
		else
		{
			m_bLastBuildPhaseState = !m_bLastBuildPhaseState;
			if(m_bLastBuildPhaseState)
				m_OnFortsBuildPhase.FireOutput(NULL, this);
			else
				m_OnFortsFightPhase.FireOutput(NULL, this);
		}
	}

	float flTimeLimit = (m_iTimeLimit != -1)?m_iTimeLimit:mp_timelimit.GetFloat();
	int iPsychic = (m_iPsychicTimeLimit != -1)?m_iPsychicTimeLimit:20;
	flTimeLimit *= 60;
	flTimeLimit -= iPsychic;
	if(flTimeLimit != 0 && gpGlobals->curtime >= flTimeLimit && m_iGameType != GAME_FORTS)
		m_OnTimeLimitAlmostReached.FireOutput(NULL, this);
}
void CGameConfig::SetMultiplayerVars(void)
{
	if(m_iBunnyhop >= 0 && m_iBunnyhop <= 1)
		HL2MPRules()->SetForceBunny(m_iBunnyhop);
	if(m_iPogostick >= 0 && m_iPogostick <= 1)
		HL2MPRules()->SetForcePogo(m_iPogostick);
	if(m_iFlashlight >= 0 && m_iFlashlight <= 1)
		HL2MPRules()->SetForceFlashlight(m_iFlashlight);

	if(m_iFallDamage >= 0 && m_iFallDamage <= 2)
		HL2MPRules()->SetForceFallDamage(m_iFallDamage);
	

	HL2MPRules()->SetDontDropWeapons(m_bDontDropWeapons);
	HL2MPRules()->SetOnlyGiveFlower(m_bOnlyGiveFlower);
}
void CGameConfig::SetLimits(void)
{
	if(m_iTimeLimit >= 0)
		HL2MPRules()->SetForceTimeLimit(m_iTimeLimit);
	if(m_iFragLimit >= 0)
		HL2MPRules()->SetForceFragLimit(m_iFragLimit);
	if(m_iStartArmor >= 0)
		HL2MPRules()->SetForceStartArmor(m_iStartArmor);
	if(m_iDomScoreMultiplier >= 1)
		HL2MPRules()->SetForceDomScoreMultiplier(m_iDomScoreMultiplier);
	if(m_flDomScoreTimer >= 1)
		HL2MPRules()->SetForceDomScoreTimer(m_flDomScoreTimer);
}

void CGameConfig::InputForceSkillOutput(inputdata_t &inputData)
{
	ForceSkillOutput();
}
void CGameConfig::InputAddTimeLimit(inputdata_t &inputData)
{
	HL2MPRules()->AddTimeLimitMod(inputData.value.Int());
}
void CGameConfig::InputChangeTimeLimit(inputdata_t &inputData)
{
	HL2MPRules()->SetTimeLimitMod(inputData.value.Int());
}
void CGameConfig::TeamWinning(int team)
{
	if(team == TEAM_PINK)
		m_OnPinkWinning.FireOutput(NULL, this);
	else if (team == TEAM_GREEN)
		m_OnGreenWinning.FireOutput(NULL, this);
	else
		m_OnNoneWinning.FireOutput(NULL, this);
}
void CGameConfig::InputWhichTeamWinning(inputdata_t &inputData)
{
	if(g_Teams[TEAM_PINK]->GetScore() > g_Teams[TEAM_GREEN]->GetScore())
		m_OnPinkWinning.FireOutput(NULL, this);
	else if(g_Teams[TEAM_PINK]->GetScore() < g_Teams[TEAM_GREEN]->GetScore())
		m_OnGreenWinning.FireOutput(NULL, this);
	else
		m_OnNoneWinning.FireOutput(NULL, this);
}

void CGameConfig::ForceSkillOutput(void)
{
	switch(mp_co_difficulty.GetInt())
	{
	case 1:
		m_OnSkillEasy.FireOutput(NULL, this);
		break;
	case 2:
		m_OnSkillMedium.FireOutput(NULL, this);
		break;
	case 3:
		m_OnSkillHard.FireOutput(NULL, this);
		break;
	case 4:
		m_OnSkillBisounours.FireOutput(NULL, this);
		break;
	};
}

void CGameConfig::PushNewRound()
{
	m_OnPushNewRound.FireOutput(NULL, this);
}

BEGIN_DATADESC( CGameConfig )
	DEFINE_KEYFIELD( m_iGameType, FIELD_INTEGER, "gametype" ),
	DEFINE_KEYFIELD( m_iBunnyhop, FIELD_INTEGER, "bunnyhop" ),
	DEFINE_KEYFIELD( m_iPogostick, FIELD_INTEGER, "pogostick" ),
	DEFINE_KEYFIELD( m_iTimeLimit, FIELD_INTEGER, "timelimit" ),
	DEFINE_KEYFIELD( m_iFragLimit, FIELD_INTEGER, "fraglimit" ),
	DEFINE_KEYFIELD( m_iStartArmor, FIELD_INTEGER, "startarmor" ),
	DEFINE_KEYFIELD( m_iFallDamage, FIELD_INTEGER, "falldamage" ),
	DEFINE_KEYFIELD( m_iFlashlight, FIELD_INTEGER, "flashlight" ),
	DEFINE_KEYFIELD( m_iPsychicTimeLimit, FIELD_INTEGER, "psychictimelimit" ),

	DEFINE_KEYFIELD( m_iDomScoreMultiplier, FIELD_INTEGER, "domscoremultiplier" ),
	DEFINE_KEYFIELD( m_flDomScoreTimer, FIELD_FLOAT, "domscoretimer" ),

	DEFINE_KEYFIELD( m_bDontDropWeapons, FIELD_BOOLEAN, "dontdropweapons" ),
	DEFINE_KEYFIELD( m_bOnlyGiveFlower, FIELD_BOOLEAN, "onlygiveflower" ),

	DEFINE_FIELD( m_bLastBuildPhaseState, FIELD_BOOLEAN),

	DEFINE_OUTPUT( m_OnFortsBuildPhase, "OnFortsBuildPhase"),
	DEFINE_OUTPUT( m_OnFortsFightPhase, "OnFortsFightPhase"),
	DEFINE_OUTPUT( m_OnTimeLimitAlmostReached, "OnTimeLimitAlmostReached"),

	DEFINE_OUTPUT( m_OnSkillEasy, "OnSkillEasy"),
	DEFINE_OUTPUT( m_OnSkillMedium, "OnSkillMedium"),
	DEFINE_OUTPUT( m_OnSkillHard, "OnSkillHard"),
	DEFINE_OUTPUT( m_OnSkillBisounours, "OnSkillBisounours"),

	DEFINE_OUTPUT( m_OnEasyMapSpawn, "OnEasyMapSpawn"),
	DEFINE_OUTPUT( m_OnMediumMapSpawn, "OnMediumMapSpawn"),
	DEFINE_OUTPUT( m_OnHardMapSpawn, "OnHardMapSpawn"),
	DEFINE_OUTPUT( m_OnBisounoursMapSpawn, "OnBisounoursMapSpawn"),

	DEFINE_OUTPUT( m_OnPinkWinning, "OnWinningPink"),
	DEFINE_OUTPUT( m_OnGreenWinning, "OnWinningGreen"),
	DEFINE_OUTPUT( m_OnNoneWinning, "OnWinningNone"),

	DEFINE_OUTPUT( m_OnPushNewRound, "OnPushNewRound"),

	DEFINE_INPUTFUNC( FIELD_VOID, "ForceSkillOutput", InputForceSkillOutput ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "AddTimeLimit", InputAddTimeLimit ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "ChangeTimeLimit", InputChangeTimeLimit ),
	DEFINE_INPUTFUNC( FIELD_VOID, "WhichTeamWinning", InputWhichTeamWinning ),

	DEFINE_THINKFUNC( RulesThink ),
END_DATADESC()

CGameConfig *g_GameConfig;