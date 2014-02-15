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

PlayerStat Nebuleuse::GetPlayerStats(char *name)
{
	for(int i=0; i < m_iPlayerStatsCount; i++)
	{
		if(strncmp(m_PlayerStats[i].Name,name,64) == 0)
			return m_PlayerStats[i];
	}
	PlayerStat e;
	e.Name = "Error";
	e.Value = 0;
	return e;
}
PlayerStat Nebuleuse::GetPlayerStats(const char *name)
{
	for(int i=0; i < m_iPlayerStatsCount; i++)
	{
		if(strncmp(m_PlayerStats[i].Name, name, 64) == 0)
			return m_PlayerStats[i];
	}
	PlayerStat e;
	e.Name = "Error";
	e.Value = 0;
	return e;
}
void Nebuleuse::SetPlayerStats(PlayerStat stat)
{
	for(int i=0; i < m_iPlayerStatsCount; i++)
	{
		if(strncmp(m_PlayerStats[i].Name, stat.Name,64) == 0)
		{
			m_PlayerStats[i] = stat;
			m_PlayerStats[i].Changed = true;
			return;
		}
	}
}
void Nebuleuse::SendPlayerStats()
{
	if(IsOutDated() || IsUnavailable())
		return;

	Talk_SendStatsUpdate(CreateStatsMsg());
}
void Nebuleuse::AddKill(nKillInfo *info)
{
	m_vKills.push_back(info);
}