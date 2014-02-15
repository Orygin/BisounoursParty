/*
Bisounours Party Cloud.
Original file by Louis Orygin Geuten.

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must
	not claim that you wrote the original software. If you use this
	software in a product, an acknowledgment in the product documentation
	would be appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

///-----------------------------------------------------------------------------------
/// This class is used by the BPCloud : Check of bans, register feedbacks, etc.
///-----------------------------------------------------------------------------------

#include "cbase.h"
 #include "curl/curl.h"
 #include "steam\steam_api.h"
 #include "bp_cloud.h"
 #include "tier0/icommandline.h"
 #include "bp_achievements.h"
// #include "Nebula.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

bp_cloud*	g_pCloud = NULL;

static size_t rcvDataStatus( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	char *sreturn = (char*)ptr;
	Msg(sreturn);
	switch (sreturn[0])
	{
	case 'B':
		Warning("You are banned from Bisounours Party Cloud.\n");
		BPCloud()->m_Status = CLOUD_BANNED;
		break;
	case 'M':
		Msg("Bisounours Party Cloud is in Maintenance.\n");
		BPCloud()->m_Status = CLOUD_MAINTENANCE;
		break;
	case 'D':
		Msg("Bisounours Party Cloud is Disabled.\n");
		BPCloud()->m_Status = CLOUD_DISABLED;
		break;
	case 'O':
		Msg("Bisounours party is outdated, please update asap.\n");
		BPCloud()->m_Status = CLOUD_OUTDATED;
		break;
	case 'W':
		Msg("Incorrect private key. this may be due to outdated client");
		BPCloud()->m_Status = CLOUD_WRONGKEY;
		break;
	default:
		Msg("Bisounours party cloud is ready.\n");
		BPCloud()->m_Status = CLOUD_CONNECTED;
		break;
	}
	return size * nmemb;
}
unsigned Thread_Startup( void *params)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cSteamID,
               CURLFORM_END);

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "version",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cVersion,
               CURLFORM_END); 

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/status.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataStatus);
	
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  return 0;
}
bp_cloud::bp_cloud()
{
	if(g_pCloud != NULL)
		return;

	m_iStatus = CLOUD_DISCONNECTED;
	m_Status = CLOUD_DISCONNECTED;
	m_bInUpdate = false;
	m_bIsUpThink = false;

  	g_pCloud = this;

	m_cSteamID = new char [17];
	m_cVersion = new char [8];

	sprintf(m_cVersion, "%i", BP_BUILD);
	sprintf(m_cSteamID, "%I64d", steamapicontext->SteamUser()->GetSteamID().ConvertToUint64() );

  if ( CommandLine()->FindParm( "-nocloud" ) )
  {
		m_iStatus = CLOUD_DISABLED;
		Msg("Bisounours Party Cloud is in disabled from -nocloud\n");
		return;
  }
  
/*  CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, m_cSteamID,
               CURLFORM_END);

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "version",
			   CURLFORM_COPYCONTENTS, m_cVersion,
               CURLFORM_END);

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/templates/img/en.png");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataStatus);
	
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  LoadAchievements();*/
  /*Nebula *Neb = new Nebula(1,5065,"test");
  Neb->Connect("http://127.0.0.1", false);*/

}
static size_t rcvDataAchievements( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	BPAchievements()->StoreAchievements((char*)ptr);
	return size * nmemb;
}
unsigned Thread_LoadAchievements(void *params)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);
	char *id = BPCloud()->m_cSteamID;
   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, id,
               CURLFORM_END);
   
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/getachievements.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataAchievements);
	
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  return 0;
}
void bp_cloud::LoadAchievements()
{
	return;
	if(IsUnavailable())
		return;

	Empty_Struct_t vars;
	CreateSimpleThread( Thread_LoadAchievements, &vars );
}
static size_t rcvDataFeedback( void *ptr, size_t size, size_t nmemb, void *userdata)
{
	Msg((char*)ptr); // up to 989 characters each time
	return size * nmemb;
}
unsigned Thread_SendFeedback(void *params)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "feedback",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cTempFeed,
               CURLFORM_END);

   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cSteamID,
               CURLFORM_END);

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "version",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cVersion,
               CURLFORM_END); 
	
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/Feedback.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataFeedback);
	
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  BPCloud()->CleanUpStruct();
  return 0;
}
void bp_cloud::SendFeedback(char *feed)
{
	return;
	if(IsUnavailable())
		return;

	Empty_Struct_t vars;

	m_cTempFeed = feed;

	CreateSimpleThread( Thread_SendFeedback, &vars );
}
unsigned Thread_SendMap(void *params)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "map",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cTempMap,
               CURLFORM_END);

   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cSteamID,
               CURLFORM_END);

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "version",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cVersion,
               CURLFORM_END); 
	
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);
 
 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/Newmap.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  BPCloud()->CleanUpStruct();
  return 0;
}
void bp_cloud::SendNewmap(const char *map)
{
	return;
	if(IsUnavailable())
		return;

	if(!strcmp(map, "background02") || !strcmp(map, "Trailer_02") || !strcmp(map, "background01") || !strcmp(map, "background03") )
		return; //No need to spam useless stats. Add here your own startup maps
		//TODO : make it use gameinfo.txt's hidden maps ?

	Empty_Struct_t vars;
	m_cTempMap = map;

	CreateSimpleThread( Thread_SendMap, &vars );
}
/*unsigned Thread_SendAchievement(void *params)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);


  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "achievement",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cTempAchiev,
               CURLFORM_END);

   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cSteamID,
               CURLFORM_END);
   
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);

 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/EarnAchievement.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataFeedback);
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  BPCloud()->m_bInEarn = false;
  BPCloud()->CleanUpStruct();
	return 0;
}*/
/// Depreciated, use SendUpdateAchievement instead
/*void bp_cloud::SendEarnAchievement(char *name)
{
	if(!IsUnavailable())
		return;
	if(m_bInEarn)
		return;
	m_bInEarn = true;
	Empty_Struct_t vars;
	m_cTempAchiev = name;

	CreateSimpleThread( Thread_SendAchievement, &vars );
}*/
unsigned Thread_SendUpdateAchievement(void *params)
{
	BPCloud()->m_Status |= CLOUD_UPACHIEV;
	
	DevMsg("Achievement update thread started working\n");
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

	char *update = new char [8];

	sprintf(update, "%i", BPCloud()->m_cTempAmount);

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "achievement",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cTempAchiev,
               CURLFORM_END);

    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "amount",
			   CURLFORM_COPYCONTENTS, update,
               CURLFORM_END);

   curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "steamid",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cSteamID,
               CURLFORM_END);
   
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);

 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/UpdateAchievement.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataFeedback);
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  DevMsg("Achievement update thread ended his work\n");
  BPCloud()->CleanUpStruct();
  BPCloud()->m_Status &= ~CLOUD_UPACHIEV;
	return 0;
}
void bp_cloud::SendUpdateAchievement(char *name, int amount)
{
	return;
	for(int i =0; i < m_pcAchievementList.Count(); i++)
	{ // This check if this achievement already exist in the update list
		Update_struct &check = m_pcAchievementList[i];
		if(!strcmp(name, check.Name))
		{ // if it does, just add the current one in the existing one.
			check.Progress = amount;
			return;
		}
	}

	int iQueueItem = m_pcAchievementList.AddToTail();
	Update_struct &New = m_pcAchievementList[iQueueItem];
	New.Name = new char[32];

	Q_strncpy(New.Name, name, 32);
	New.Progress = amount;
}
unsigned Thread_SendStats(void *params)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect: 100-continue";

	curl_global_init(CURL_GLOBAL_ALL);

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "xml",
			   CURLFORM_COPYCONTENTS, BPCloud()->m_cTempStats,
               CURLFORM_END);
  
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "key",
			   CURLFORM_COPYCONTENTS, BP_KEY,
               CURLFORM_END);

 curl = curl_easy_init();
  
  headerlist = curl_slist_append(headerlist, buf);
  
  if(curl) 
  {
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.bisounoursparty.com/Cloud/update_stats.php");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rcvDataFeedback);
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  
    curl_formfree(formpost);

    curl_slist_free_all (headerlist);
  }
  BPCloud()->CleanUpStats();
	return 0;
}
void bp_cloud::SendStats(char *xml)
{
	return;
	m_cTempStats = xml;

	Empty_Struct_t vars;
	CreateSimpleThread( Thread_SendStats, &vars );
}
void bp_cloud::CleanUpStats()
{
	delete[] m_cTempStats;
}
void bp_cloud::UpdateNext()
{
	return;
	if( 0 == m_pcAchievementList.Count() || m_Status & CLOUD_UPACHIEV || IsUnavailable() )
		return;

	Update_struct &New =  m_pcAchievementList[m_pcAchievementList.Head()];

	Empty_Struct_t vars;
	m_cTempAchiev = new char[32];
	memcpy(m_cTempAchiev, New.Name, 32);
	m_cTempAmount = New.Progress;

	CreateSimpleThread( Thread_SendUpdateAchievement, &vars );

	m_pcAchievementList.Remove( m_pcAchievementList.Head());
}
void bp_cloud::CleanUpStruct()
{ // FIXME : Was causing bugs
	/*if(m_cTempAchiev != NULL)
		delete[] m_cTempAchiev;
	if(m_cTempMap != NULL)
		delete[] m_cTempMap;
	if(m_cTempFeed != NULL)
		delete[] m_cTempFeed;*/
}