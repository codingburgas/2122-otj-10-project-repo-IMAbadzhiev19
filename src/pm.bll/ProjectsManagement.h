#pragma once

#include "pch.h"

#include "../../lib/nanodbc/nanodbc.h"
#include "../pm.consoleApp/Database.h"

#include "../pm.bll/UsersManagement.h"
#include "../pm.bll/TeamsManagement.h"
#include "../pm.dal/ProjectsStore.h"

namespace pm::bll
{
	class ProjectsManagement
	{
	public:

		pm::dal::ProjectsStore m_projectStore;

		void createProject(pm::dal::ProjectsStore::PROJECT& project, size_t userId);
		void updateProject(pm::dal::ProjectsStore::PROJECT& project, size_t projectId, size_t userId);
		void removeProject(size_t projectId);

		void assignTeamToProject(size_t projectId, size_t teamId);
		void removeTeamFromProject(size_t projectId, size_t teamId);

		std::vector<pm::dal::ProjectsStore::PROJECT> loadAllProjects();
		std::vector<pm::dal::TeamsStore::TEAM> getTeamFromProject(size_t projectId);
		//std::vector<pm::dal::TasksStore::TASK> getTasksFromProject(size_t projectId);

	};
}