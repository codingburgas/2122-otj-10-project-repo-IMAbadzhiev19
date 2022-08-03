#pragma once

#include "pch.h"
#include "../pm.dal/TeamsStore.h"

namespace pm::bll
{
	class TeamsManagement
	{
	public:

		pm::dal::TeamsStore m_teamsStore;

		void createTeam(pm::dal::TeamsStore::TEAM& team, size_t userId);
		void updateTeam(size_t teamId, pm::dal::TeamsStore::TEAM& team, size_t userId);
		void removeTeam(size_t teamId);

		void addUserToTeam(size_t teamId, size_t userId);
		void removeUserFromTeam(size_t teamId, size_t userId);

		std::vector<pm::dal::TeamsStore::TEAM> loadTeams();
		std::vector<pm::dal::UsersStore::USER> getUsersFromTeam(size_t teamId);
	};
}