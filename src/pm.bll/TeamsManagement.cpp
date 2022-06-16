#include "pch.h"

#include "TeamsManagement.h"
#include "../../lib/nanodbc/nanodbc.h"

void pm::bll::TeamsManagement::createTeam(pm::dal::TeamsStore::TEAM& team, size_t userId)
{
	if (!m_teamsStore.createTeam(team, userId))
		throw "There was a problem with the team creation";
}

void pm::bll::TeamsManagement::updateTeam(size_t teamId, pm::dal::TeamsStore::TEAM& team, size_t userId)
{
	if (!m_teamsStore.updateTeam(teamId, team, userId))
		throw "There was a problem with the team update";
}

void pm::bll::TeamsManagement::removeTeam(size_t teamId)
{
	if (!m_teamsStore.removeTeam(teamId))
		throw "There was a problem with removing the team";
}