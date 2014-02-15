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

#ifndef GAME_CLOUD_H
#define GAME_CLOUD_H

#ifdef _WIN32
#pragma once
#endif


#define BP_BUILD 5063
#define BP_KEY "D9A0B3151F7130570A23AC2241E56D8A490530048C7DA9ADABF63BCFCC9F6A4A"
#define GAMENAME "Bisounours Party 6"
// Modify this key to random char to make sure no other game use your cloud 
// /!\ Keep this key the same here and on the server or you won't be able to use the cloud

	struct Empty_Struct_t	{	}; // Structs aren't precise enough.
									// Get rid of it by having temporary pointers in BPCloud

///-----------------------------------------------------------------------------------
/// This class is used by the BPCloud : Check of bans, register feedbacks, etc.
///-----------------------------------------------------------------------------------
struct Update_struct
	{
		char *Name;
		int Progress;
	};

enum Cloud_states
{
	CLOUD_DISABLED		= 0x0001,
	CLOUD_BANNED		= 0x0002,
	CLOUD_MAINTENANCE	= 0x0004,
	CLOUD_OUTDATED		= 0x0008,
	CLOUD_DISCONNECTED	= 0x0010,
	CLOUD_CONNECTED		= 0x0020,
	CLOUD_WORKING		= 0x0040,
	CLOUD_WRONGKEY		= 0x0080,
	CLOUD_UPACHIEV		= 0x0100
};

class bp_cloud
{
public:
	bp_cloud();

	void SendFeedback(char *feed);
	void SendNewmap(const char *map);
	void SendEarnAchievement(char *name);
	void SendUpdateAchievement(char *name, int amount);
	void SendStats(char * xml);

	void CleanUpStruct();
	void CleanUpStats();

	void SetStatus(int status) { m_iStatus = status; };

	bool IsBanned() { return (m_iStatus & CLOUD_BANNED) ? true : false; };
	bool IsUnavailable() { return ((m_Status & CLOUD_BANNED || m_Status & CLOUD_MAINTENANCE || m_Status & CLOUD_DISABLED || m_Status & CLOUD_DISCONNECTED || m_Status & CLOUD_OUTDATED )) ? true : false; };
	bool IsOutDated() { return (m_iStatus & CLOUD_OUTDATED) ? true : false; };

	bool m_bInUpdate;

	unsigned int m_Status;

	char *m_cSteamID;
	char *m_cVersion;
	char *m_cTempAchiev;

	const char *m_cTempMap;
	char *m_cTempFeed;
	int	  m_cTempAmount;
	char *m_cTempStats;

	void UpdateNext();

private:
	int m_iStatus;
	bool m_bIsUpThink;
	void LoadAchievements(); // Private, we don't want a refresh on runtime
	CUtlLinkedList<Update_struct> m_pcAchievementList;
};

extern bp_cloud *g_pCloud;

inline bp_cloud* BPCloud() // Getting the cloud anywhere
{
	if(g_pCloud == NULL)
		return new bp_cloud();
	else
		return g_pCloud;
}

#endif