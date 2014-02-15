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
//This file contains the parsing function of nebuleuse. They are separated from the main file so you can implement you own parser easly

#include "cbase.h"
#include "Nebuleuse.h"
#include "rapidjson/document.h"
#include "achievementsdialog.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

void Nebuleuse::Parse_SessionData(char* data)
{
	DevMsg(data);
	DevMsg("\n");

	Document doc;
	if (doc.Parse<0>(data).HasParseError())
		return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
	if (!doc.IsObject())
		return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);

	/*if(NEBULEUSE_BETA)
		if(doc.HasMember("rank") && doc["rank"].IsInt())
			if(doc["rank"].GetInt() < NEBULEUSE_BETARANK)
				return ThrowError(NEBULEUSE_ERROR_NON_BETA_PLAYER);*/

	if(doc.HasMember("gameVersion") && doc["gameVersion"].IsUint())
		if(doc["gameVersion"].GetUint() > m_uiVersion )
			return ThrowError(NEBULEUSE_ERROR_OUTDATED);

	if(doc.HasMember("version_major") && doc.HasMember("version_minor") && doc.HasMember("version_patch") && doc["version_major"].IsInt() && doc["version_minor"].IsInt() && doc["version_patch"].IsInt())
		if(doc["version_major"].GetInt() > NEBULEUSE_VERSION_MAJOR || doc["version_minor"].GetInt() > NEBULEUSE_VERSION_MINOR)
			return ThrowError(NEBULEUSE_ERROR_OUTDATED);

	if(doc.HasMember("rank") && doc["rank"].IsInt()){
		m_iPlayerRank = doc["rank"].GetInt();
		if(doc["rank"].GetUint() == 0)
			return ThrowError(NEBULEUSE_ERROR_BANNED);
	}
	if(doc.HasMember("subMessage"))	{
		m_cSubMessage = new char[doc["subMessage"].GetStringLength()];
		memcpy(m_cSubMessage,doc["subMessage"].GetString(),doc["subMessage"].GetStringLength());
	}

	if(doc.HasMember("sessionid") && doc["sessionid"].IsUint())
		m_iSessionID = doc["sessionid"].GetUint();

	if(doc.HasMember("achievements"))
	{
		const Value& achievements = doc["achievements"];
		int AchNbr = 0;
		for (rapidjson::SizeType i = 0; i < achievements.Size(); i++)
	    {
			const Value& Ach = achievements[i];
			if(Ach.HasMember("name") && Ach.HasMember("progress") && Ach.HasMember("max"))
			{
				m_Achievements[AchNbr].Name = new char[Ach["name"].GetStringLength()];
				//memcpy(m_Achievements[AchNbr].Name, Ach["name"].GetString(), Ach["name"].GetStringLength());
				strcpy(m_Achievements[AchNbr].Name, Ach["name"].GetString());
				m_Achievements[AchNbr].Progress =  Ach["progress"].GetUint();
				m_Achievements[AchNbr].ProgressMax = Ach["max"].GetUint();
				m_Achievements[AchNbr].Id = AchNbr;
				AchNbr++;
			}
		}
		m_iAchievementCount = AchNbr;
	}
	if(doc.HasMember("stats"))
	{
		const Value& stats = doc["stats"];
		int StatNbr = 0;
		for (rapidjson::SizeType i = 0; i < stats.Size(); i++)
	    {
			const Value& Stat = stats[i];
			if(Stat.HasMember("Name") && Stat.HasMember("Value"))
			{
				m_PlayerStats[StatNbr].Name = new char[Stat["Name"].GetStringLength()];
				//memcpy(m_Achievements[AchNbr].Name, Ach["name"].GetString(), Ach["name"].GetStringLength());
				strcpy(m_PlayerStats[StatNbr].Name, Stat["Name"].GetString());
				m_PlayerStats[StatNbr].Value =  Stat["Value"].GetInt();
				m_PlayerStats[StatNbr].Changed = false;
				StatNbr++;
			}
		}
		m_iPlayerStatsCount = StatNbr;
	}

	m_iState = NEBULEUSE_CONNECTED;
	AchievementsPanel->Update();
}
const char * Nebuleuse::CreateStatsMsg()
{
	Document doc;
	doc.SetObject();

	Value Sts(kArrayType);
	Document::AllocatorType& allocator = doc.GetAllocator();

	doc.AddMember("map", m_sCurMap, allocator);

	for(int i=0; i < m_iPlayerStatsCount; i++) // Insert all the different stats we have
	{
		if(m_PlayerStats[i].Changed == true)
		{
			Value St(kObjectType);
			St.AddMember("name", m_PlayerStats[i].Name, allocator);
			St.AddMember("value", m_PlayerStats[i].Value, allocator);
			Sts.PushBack(St, allocator);
		}
	}
	doc.AddMember("Stats", Sts, allocator);

	Value Kls(kArrayType);
	for(unsigned int i=0; i < m_vKills.size(); i++) // Insert all the kills we have
	{
		nKillInfo *cur = m_vKills[i];

		Value Kl(kObjectType);
		Kl.AddMember("x", cur->getPos().x, allocator);
		Kl.AddMember("y", cur->getPos().y, allocator);
		Kl.AddMember("z", cur->getPos().z, allocator);
		Kl.AddMember("weapon", cur->getWeapon(), allocator);

		Kls.PushBack(Kl, allocator);
	}

	doc.AddMember("Kills", Kls, allocator);

	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	doc.Accept(writer);
	
	char* json = new char[buffer.GetSize()];
	strcpy(json, buffer.GetString()); // Get a string to send
	DevMsg(json);

	while(!m_vKills.empty())
	{
		delete m_vKills.back();
		m_vKills.pop_back(); // Clean up to avoid duplicate entries
	}

	return json;
}
void Nebuleuse::Parse_RcvErrorCode(int code)
{
	switch (code)
	{
		case NebuleuseError::NEBULEUSE_ERROR_BANNED:
			m_iState = NebuleuseState::NEBULEUSE_DISABLED;
			m_iPlayerRank = NEBULEUSE_PLAYER_RANK_BANNED;
			break;
		case NebuleuseError::NEBULEUSE_ERROR_MAINTENANCE:
			m_iState = NEBULEUSE_DISABLED;
			break;
		case NebuleuseError::NEBULEUSE_ERROR_DISCONNECTED:
			m_iState = NEBULEUSE_NOTCONNECTED;
			Talk_GetNewSession();
			break;
		default:
			break;
	}
	ThrowError((NebuleuseError)code);
}