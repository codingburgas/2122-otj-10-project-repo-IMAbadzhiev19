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

void pm::bll::TeamsManagement::addUserToTeam(size_t teamId, size_t userId)
{
	if (!m_teamsStore.addUserToTeam(teamId, userId))
		throw "We couldn't add the user to the team. Try again later!";
}

void pm::bll::TeamsManagement::removeUserFromTeam(size_t teamId, size_t userId)
{
	if (!m_teamsStore.removeFromTeam(teamId, userId))
		throw "We couldn't remove the user from this team right now. Try again later!";
}

std::vector<pm::dal::TeamsStore::TEAM> pm::bll::TeamsManagement::loadTeams()
{
	return m_teamsStore.loadTeams();
}

std::vector<pm::dal::UsersStore::USER> pm::bll::TeamsManagement::getUsersFromTeam(size_t teamId)
{
	return m_teamsStore.getUsersFromTeam(teamId);
}