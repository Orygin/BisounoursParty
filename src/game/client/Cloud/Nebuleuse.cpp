/*
	Copyright (C) 2012  Louis 'Orygin' Geuten

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or any later version.
	

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
*/
#include "cbase.h"
#include "Nebuleuse.h"
#include <string.h>


Nebuleuse::Nebuleuse(uint64 PlayerID, const char* addr, unsigned int version, const char* key)
{
	m_uiPlayerID = PlayerID;
	m_uiVersion = version;
	m_cKey = key;
	SetState(NEBULEUSE_NOTCONNECTED);
	LastError = NEBULEUSE_ERROR_NONE;
	m_cSubMessage = "";
	m_sCurMap = NULL;
	m_cHostName = addr;
	m_iSessionID = 0;
	m_iAchievementCount = 0;
	m_iPlayerRank = 0;
	m_iPlayerStatsCount = 0;
	g_Nebuleuse = this;
}
Nebuleuse::~Nebuleuse()
{
	if(m_cHostName)
		delete[] m_cHostName;
	if(m_cKey)
		delete[] m_cKey;
}

//void Nebuleuse::SetPlayerStats(PlayerStat stats) { m_PlayerStats = stats; }
//PlayerStat Nebuleuse::GetPlayerStats() { return m_PlayerStats; }
int Nebuleuse::GetPlayerRank() { return m_iPlayerRank; }

bool Nebuleuse::IsBanned() { return (m_iPlayerRank == NEBULEUSE_PLAYER_RANK_BANNED || LastError == NEBULEUSE_ERROR_NON_BETA_PLAYER); }
bool Nebuleuse::IsUnavailable() 
{ 
	return (LastError != NEBULEUSE_ERROR_NONE) || (GetState() == NEBULEUSE_NOTCONNECTED); 
}
bool Nebuleuse::IsOutDated() { return (LastError == NEBULEUSE_ERROR_OUTDATED); }

void Nebuleuse::SetState(int state){ m_iState = state; }
int Nebuleuse::GetState(){ return m_iState; }

void InitErrorCb(NebuleuseError e)
{
	switch (e){
	case 1:
		break;
	case NebuleuseError::NEBULEUSE_ERROR_OUTDATED:
		g_Nebuleuse->SetOutDated();
		break;
	default:
		break;
	}
}
void Nebuleuse::Init()
{
	if(m_iState != NEBULEUSE_NOTCONNECTED)
		return;
	SetErrorCallBack(&InitErrorCb);
	Talk_GetNewSession();
	Talk_GetAvatar();
}

void Nebuleuse::SetLogCallBack(void (*Callback)(char *))
{
	if(Callback)
		m_NebuleuseLog_Callback = Callback;
}
void Nebuleuse::SetErrorCallBack(void (*Callback)(NebuleuseError))
{
	if(Callback)
		m_NebuleuseError_Callback = Callback;
}
void Nebuleuse::SetAchievementCallBack(void (*Callback)(int))
{
	if(Callback)
		m_AchievementEarned_CallBack = Callback;
}
void Nebuleuse::ThrowError(NebuleuseError e)
{
	LastError = e;
	Log("Error Thrown");
	if(m_NebuleuseError_Callback)
		m_NebuleuseError_Callback(e);
}
void Nebuleuse::Log(char* msg)
{
	if(m_NebuleuseLog_Callback)
		m_NebuleuseLog_Callback(msg);
}
void Nebuleuse::SetCurrentMap(const char *map)
{
	if(!map)
		return;

	delete[] m_sCurMap;
	m_sCurMap = NULL;

	m_sCurMap = new char[strlen(map)];
	strcpy(m_sCurMap, map);
}
Nebuleuse *g_Nebuleuse;