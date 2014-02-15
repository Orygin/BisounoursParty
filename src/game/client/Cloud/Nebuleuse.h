/*
	Copyright (C) 2012  Louis 'Orygin' Geuten
	All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	Neither the name of the owner nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#pragma once
#include "cbase.h"
#include <vector>
#include <string>
#ifndef NEBULEUSE_HEADER
#define NEBULEUSE_HEADER

#define NEBULEUSE_GAMEKEY "FdMowqlMVaduFNZYd7Ri"

#define NEBULEUSE_VERSION_MAJOR 1 //Every patch version of Nebuleuse should work under the same Minor Version
#define NEBULEUSE_VERSION_MINOR 0 //	so if you break retro compatibility, change the Minor version
#define NEBULEUSE_VERSION_PATCH 2
#define NEBULEUSE_MAX_ACHIEVEMENTS 512
#define NEBULEUSE_MAX_STATS 512

enum NebuleuseState
{
	NEBULEUSE_NOTCONNECTED	= 0,
	NEBULEUSE_DISABLED,
	NEBULEUSE_CONNECTED,
	NEBULEUSE_WORKING
};
enum NebuleuseError
{
	NEBULEUSE_ERROR_NONE = 0,
	NEBULEUSE_ERROR, //Unspecified error
	NEBULEUSE_ERROR_BANNED,
	NEBULEUSE_ERROR_MAINTENANCE,//The service is on maintenance or offline
	NEBULEUSE_ERROR_DISCONNECTED,//The session timed out or never existed
	NEBULEUSE_ERROR_OUTDATED,//The server version is too different from the Nebuleuse version.
	NEBULEUSE_ERROR_BADKEY, //The key provided do not correspond with the key the server has for this game
	NEBULEUSE_ERROR_PARSEFAILED,
	NEBULEUSE_ERROR_NON_BETA_PLAYER
};
enum NebuleusePlayerRank
{
	NEBULEUSE_PLAYER_RANK_BANNED = 0,
	NEBULEUSE_PLAYER_RANK_NORMAL,
	NEBULEUSE_PLAYER_RANK_VIP,
	NEBULEUSE_PLAYER_RANK_DEV //This shall always be the last rank
};
struct AchievementData
{
	char *Name;
	unsigned int Progress;
	unsigned int ProgressMax;
	unsigned int Id;
	bool IsCompleted() { return Progress >= ProgressMax ? true : false; }
	void Complete() { Progress = ProgressMax; }
};
struct PlayerStat
{
	char *Name;
	int Value;
	bool Changed;
};
struct nKillInfo
{
public:
	nKillInfo(const char *weapon, Vector pos) : weapon(weapon), pos(pos)
	{
	}
	~nKillInfo()
	{
		delete weapon;
	}
	 
	const char *getWeapon() const
	{
		return weapon;
	}

	Vector getPos() const
	{
		return pos;
	}
 
private:
	const char* weapon;
	const Vector pos;
};
class Nebuleuse
{
public:
	///Create the Nebuleuse.
	Nebuleuse(uint64 PlayerID, const char* addr, unsigned int version, const char* key);

	~Nebuleuse();

		///Start Nebuleuse
	void Init();

		///Return the current state of Nebuleuse
	int GetState();
	void SetState(int state);

	bool IsBanned();
	bool IsUnavailable();
	bool IsOutDated();

	uint64 GetPlayerID() { return m_uiPlayerID; };
	const char * getHost() { return m_cHostName; };
	const char * GetKey() { return m_cKey; };
	unsigned int GetSessionID() { return m_iSessionID; };

	void SetCurrentMap(const char * map);


	void SetOutDated() { LastError = NEBULEUSE_ERROR_OUTDATED; };

		///Sends a notification to the server. You will need to add your own handler in the server
	void SendNotification(char *Name, char *Value);

		///Returns the player rank (from NebuleusePlayerRank)
	int GetPlayerRank();

		///Get the player stats
	PlayerStat GetPlayerStats(char *Name);
	PlayerStat GetPlayerStats(const char *Name);
		///Set the player stats
	void SetPlayerStats(PlayerStat stat);
		///Send Stats data
	void SendPlayerStats();
		///Add a Kill to the list
	void AddKill(nKillInfo *Pos);

		///Get the specified achievemnt data
	AchievementData GetAchievementData(char *Name);
	AchievementData GetAchievementData(const char *Name);
	AchievementData GetAchievementData(int index);
		///Set the specified achievement data
	void SetAchievementData(AchievementData data);
		///Update the Progress of this achievement (and send it)
	void UpdateAchievementProgress(char *Name, int Progress);
	void UpdateAchievementProgress(const char *Name, int Progress);
	void UpdateAchievementProgress(int index, int Progress);
		///Earn the achievement (and send it)
	void EarnAchievement(char *Name);
	void EarnAchievement(const char *Name);
	void EarnAchievement(int index);
		///Sends the achievements that have been modified
	void SendAchievements();
		///Achievement earn CallBack
	void SetAchievementCallBack(void (*Callback)(int));
		///Return the number of achievements
	int GetAchievementCount() { return m_iAchievementCount; }

	char* GetSubMessage() { return m_cSubMessage; }

	void SetLogCallBack(void (*Callback)(char *));

	void CustomTalk(char* Url, void (*Callback)(char*));
	void (*m_CustomTalk_Callback)(char *);

	//Parser
	void Parse_SessionData(char* data);

	void Parse_RcvErrorCode(int code);
private:
	void SetErrorCallBack(void (*Callback)(NebuleuseError));
	void (*m_NebuleuseError_Callback)(NebuleuseError);
	
	void (*m_NebuleuseLog_Callback)(char *);

	void (*m_AchievementEarned_CallBack)(int);

	void ThrowError(NebuleuseError e);
	void Log(char* msg);

	const char * CreateStatsMsg();

	//Talker
	void Talk_GetStatData();
	void Talk_GetNewSession();
	void Talk_SendAchievementProgress(int id);
	void Talk_SendStatsUpdate(const char* stats);
	void Talk_GetAvatar();
	size_t RcvConnectInfos( void *ptr, size_t size, size_t nmemb, void *userdata);
	//size_t RcvSessionInfos( void *ptr, size_t size, size_t nmemb, void *userdata);
public:
		int LastError;
		char *m_ConnectAns;
private:
	PlayerStat m_PlayerStats[NEBULEUSE_MAX_STATS];
	int m_iPlayerStatsCount;

	AchievementData m_Achievements[NEBULEUSE_MAX_ACHIEVEMENTS];
	int m_iAchievementCount;

	const char *m_cHostName;
	const char *m_cKey;
	uint m_uiVersion;
	uint64 m_uiPlayerID;

	char *m_cSubMessage;

	int m_iPlayerRank;
	int m_iState;
	unsigned int m_iSessionID;

	std::vector<nKillInfo*> m_vKills;
	char* m_sCurMap;
};
extern Nebuleuse *g_Nebuleuse;
#endif