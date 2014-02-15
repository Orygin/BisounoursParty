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

AchievementData Nebuleuse::GetAchievementData(char *name)
{
	for(int i=0; i < m_iAchievementCount; i++)
	{
		if(strncmp(m_Achievements[i].Name,name,64) == 0)
			return m_Achievements[i];
	}
	AchievementData e;
	e.Name = "Error";
	e.Progress = 0;
	e.ProgressMax = 0;
	return e;
}
AchievementData Nebuleuse::GetAchievementData(const char *name)
{
	for(int i=0; i < m_iAchievementCount; i++)
	{
		if(strncmp(m_Achievements[i].Name,name,64) == 0)
			return m_Achievements[i];
	}
	AchievementData e;
	e.Name = "Error";
	e.Progress = 0;
	e.ProgressMax = 0;
	return e;
}
AchievementData Nebuleuse::GetAchievementData(int index)
{
	assert(index<=m_iAchievementCount);
	return m_Achievements[index];
}
void Nebuleuse::SetAchievementData(AchievementData data)
{
	for(int i=0; i<m_iAchievementCount;i++)
	{
		if(strcmp(m_Achievements[i].Name, data.Name) == 0)
		{
			m_Achievements[i] = data;
			Talk_SendAchievementProgress(i);
		}
	}
}
void Nebuleuse::UpdateAchievementProgress(char *Name, int Progress)
{
	if(IsOutDated() || IsUnavailable())
		return;
	for(int i=0; i < m_iAchievementCount; i++)
	{
		if(strncmp(m_Achievements[i].Name,Name,64) == 0)
		{			
			if(m_Achievements[i].IsCompleted())
				return; //Already have this one
			m_Achievements[i].Progress += Progress;

			if(m_Achievements[i].IsCompleted()) // Tell the game we have an achievement earned
				if(m_AchievementEarned_CallBack)
					m_AchievementEarned_CallBack(i);

			Talk_SendAchievementProgress(i);
		}
	}
}
void Nebuleuse::UpdateAchievementProgress(const char *Name, int Progress)
{
	if(IsOutDated() || IsUnavailable())
		return;
	for(int i=0; i < m_iAchievementCount; i++)
	{
		if(strncmp(m_Achievements[i].Name,Name,64) == 0)
		{			
			if(m_Achievements[i].IsCompleted())
				return; //Already have this one
			m_Achievements[i].Progress += Progress;
			
			if(m_Achievements[i].IsCompleted()) // Tell the game we have an achievement earned
				if(m_AchievementEarned_CallBack)
					m_AchievementEarned_CallBack(i);

			Talk_SendAchievementProgress(i);
		}
	}
}
void Nebuleuse::UpdateAchievementProgress(int index, int Progress)
{	
	if(IsOutDated() || IsUnavailable())
		return;
	if(m_Achievements[index].IsCompleted())
		return; //Already have this one
	m_Achievements[index].Progress += Progress;

	if(m_Achievements[index].IsCompleted()) // Tell the game we have an achievement earned
		if(m_AchievementEarned_CallBack)
			m_AchievementEarned_CallBack(index);


	Talk_SendAchievementProgress(index);
}
void Nebuleuse::EarnAchievement(char *Name)
{
	if(IsOutDated() || IsUnavailable())
		return;
	for(int i=0; i < m_iAchievementCount; i++)
	{
		if(strncmp(m_Achievements[i].Name,Name,64) == 0)
		{			
			if(m_Achievements[i].IsCompleted())
				return; //Already have this one
			m_Achievements[i].Complete();

			if(m_AchievementEarned_CallBack)
				m_AchievementEarned_CallBack(i);

			Talk_SendAchievementProgress(i);
		}
	}
}
void Nebuleuse::EarnAchievement(const char *Name)
{
	if(IsOutDated() || IsUnavailable())
		return;
	for(int i=0; i < m_iAchievementCount; i++)
	{
		if(strncmp(m_Achievements[i].Name,Name,64) == 0)
		{			
			if(m_Achievements[i].IsCompleted())
				return; //Already have this one
			m_Achievements[i].Complete();

			if(m_AchievementEarned_CallBack)
				m_AchievementEarned_CallBack(i);

			Talk_SendAchievementProgress(i);
		}
	}
}
void Nebuleuse::EarnAchievement(int index)
{	
	if(IsOutDated() || IsUnavailable())
		return;
	if(m_Achievements[index].IsCompleted())
		return; //Already have this one
	m_Achievements[index].Complete();
	
	if(m_AchievementEarned_CallBack)
		m_AchievementEarned_CallBack(index);

	Talk_SendAchievementProgress(index);
}