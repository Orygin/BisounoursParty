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
//This file contains the talking functions of Nebuleuse. They are separated from the main file so you can implement your own talker easly

#include "cbase.h"
#include "Nebuleuse.h"
#include "curl\curl.h"
#include "Filesystem.h"
#include "NebuleuseAvatar.h"

#define NEBCONNECTTIMEOUT 25

static size_t RcvAvatar( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FileHandle_t Avatar = (FileHandle_t)userdata;

	if(!Avatar)
		return size*nmemb;

	size_t write;
	size *= nmemb;
	write = filesystem->Write(ptr, size, Avatar);

	return size; 
}
void Nebuleuse::Talk_GetAvatar()
{
	FileHandle_t Avatar;
	Avatar = filesystem->Open( "avatar.jpg", "wb", "MOD");

	CURL *curl;
	CURLcode res;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

	char PlayerId[18];
	sprintf_s(PlayerId, 18, "%I64d", m_uiPlayerID);

	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "SteamId",
			   CURLFORM_COPYCONTENTS, PlayerId,
			   CURLFORM_END);
 
	curl = curl_easy_init();
  
	headerlist = curl_slist_append(headerlist, buf);

	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

		char Link[128];
		sprintf_s(Link,128, "%s/GetSteamAvatar.php", m_cHostName);

		curl_easy_setopt(curl, CURLOPT_URL, Link);

		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RcvAvatar);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) Avatar);
	
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
  
		curl_formfree(formpost);

		curl_slist_free_all (headerlist);
	}
	filesystem->Close(Avatar);

	(new NebuleuseAvatar())->DecompressAvatar();
}

size_t Nebuleuse::RcvConnectInfos( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	//Ideal:
	//Parse_ConnectData(ptr);
	m_ConnectAns = (char*)ptr;
	return size*nmemb;
}
static size_t RcvErrorCheck( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	int erCode = atoi((char*)ptr);
	if(erCode != NebuleuseError::NEBULEUSE_ERROR_NONE)
		g_Nebuleuse->Parse_RcvErrorCode(erCode);
	Msg((char*)ptr);
	return size*nmemb;
}
static size_t RcvCustom( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	if(g_Nebuleuse->m_CustomTalk_Callback)
		g_Nebuleuse->m_CustomTalk_Callback((char*)ptr);
	return size*nmemb;
}
void Nebuleuse::Talk_GetStatData()
{
	if(GetState() != NEBULEUSE_WORKING)
		SetState(NEBULEUSE_WORKING);

	CURL *curl;
	CURLcode res;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "playerID",
			   CURLFORM_COPYCONTENTS, m_uiPlayerID,
			   CURLFORM_END);

	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "version",
			   CURLFORM_COPYCONTENTS, m_uiVersion,
				CURLFORM_END);

	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, m_cKey,
			   CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	char Link[128];
	sprintf_s(Link,128, "%s/connect.html", m_cHostName);
	curl_easy_setopt(curl, CURLOPT_URL, Link);

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Nebuleuse::RcvConnectInfos);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, NEBCONNECTTIMEOUT);
	
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
  
	curl_formfree(formpost);

	curl_slist_free_all (headerlist);
  }
}
unsigned AchProgressThread(void *params)
{
	Nebuleuse *neb = g_Nebuleuse;

	AchievementData* ach = (AchievementData*)params;

	CURL *curl;
	CURLcode res;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

	char PlayerId[18];
	sprintf_s(PlayerId, 18, "%I64d", neb->GetPlayerID());
	char AchievementId[8];
	sprintf_s(AchievementId, 8, "%i", ach->Id);
	char AchievementProgress[8];
	sprintf_s(AchievementProgress, 8, "%i", ach->Progress);
	char SessionId[16];
	sprintf_s(SessionId, 16, "%i", neb->GetSessionID());

   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "PlayerID",
			   CURLFORM_COPYCONTENTS, PlayerId,
			   CURLFORM_END);
   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "AchievementId",
			   CURLFORM_COPYCONTENTS, AchievementId,
			   CURLFORM_END);
   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "AchievementProgress",
			   CURLFORM_COPYCONTENTS, AchievementProgress,
			   CURLFORM_END);
   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "SessionId",
			   CURLFORM_COPYCONTENTS, SessionId,
			   CURLFORM_END);

	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, neb->GetKey(),
			   CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	char Link[128];
	sprintf_s(Link,128, "%s/AchievementUpdate.php", neb->getHost());
	curl_easy_setopt(curl, CURLOPT_URL, Link);

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RcvErrorCheck);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, NEBCONNECTTIMEOUT);
	
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
  
	curl_formfree(formpost);

	curl_slist_free_all (headerlist);
  }

  return 0;
}
void Nebuleuse::Talk_SendAchievementProgress(int id)
{
	if(GetState() != NEBULEUSE_CONNECTED)
		return;
	AchievementData* ach = &m_Achievements[id];

	CreateSimpleThread( AchProgressThread, ach );
}
size_t RcvSessionInfos( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	g_Nebuleuse->Parse_SessionData((char*)ptr);
	return size*nmemb;
}
void Nebuleuse::Talk_GetNewSession()
{
	CURL *curl;
	CURLcode res;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

	char PlayerId[18];
	sprintf_s(PlayerId, 18, "%I64d", m_uiPlayerID);

   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "PlayerID",
			   CURLFORM_COPYCONTENTS, PlayerId,
			   CURLFORM_END);

	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, m_cKey,
			   CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	char Link[128];
	sprintf_s(Link,128, "%s/", m_cHostName);
	curl_easy_setopt(curl, CURLOPT_URL, Link);

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RcvSessionInfos);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, NEBCONNECTTIMEOUT);
	
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
  
	curl_formfree(formpost);

	curl_slist_free_all (headerlist);
  }
}
void Nebuleuse::CustomTalk(char* url, void (*Callback)(char*))
{
	CURL *curl;
	CURLcode res;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	if(Callback)
		m_CustomTalk_Callback = Callback;
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	char Link[128];
	sprintf_s(Link,128, "%s/%s", m_cHostName,url);
	curl_easy_setopt(curl, CURLOPT_URL, Link);

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, RcvCustom);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, NEBCONNECTTIMEOUT);
	
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
  
	curl_formfree(formpost);

	curl_slist_free_all (headerlist);
	//delete curl;
  }
}
void Nebuleuse::Talk_SendStatsUpdate(const char * Stats)
{
	CURL *curl;
	CURLcode res;
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

	char PlayerId[18];
	sprintf_s(PlayerId, 18, "%I64d", m_uiPlayerID);
	char SessionId[16];
	sprintf_s(SessionId, 16, "%i", GetSessionID());

   curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "PlayerID",
			   CURLFORM_COPYCONTENTS, PlayerId,
			   CURLFORM_END);

	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, m_cKey,
			   CURLFORM_END);
	  
	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "SessionId",
			   CURLFORM_COPYCONTENTS, SessionId,
			   CURLFORM_END);
	
	curl_formadd(&formpost,
			   &lastptr,
			   CURLFORM_COPYNAME, "Stats",
			   CURLFORM_COPYCONTENTS, Stats,
			   CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	char Link[128];
	sprintf_s(Link,128, "%s/StatsUpdate.php", m_cHostName);
	curl_easy_setopt(curl, CURLOPT_URL, Link);

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RcvErrorCheck);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, NEBCONNECTTIMEOUT);
	
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
  
	curl_formfree(formpost);

	curl_slist_free_all (headerlist);
  }
  delete Stats;
}