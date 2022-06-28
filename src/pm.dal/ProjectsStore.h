#pragma once

#include "pch.h"

#include "../../lib/nanodbc/nanodbc.h"
#include "../pm.consoleApp/Database.h"

#include "../pm.dal/TeamsStore.h"
#include "../pm.dal/UsersStore.h"

namespace pm::dal
{
	class ProjectsStore
	{
	public:

		struct PROJECT
		{
			int id;
			std::string title;
			std::string description;
			nanodbc::date createdOn;
			int creatorId;
		};

		void createProject(PROJECT& project, size_t userId);
		void updateProject(PROJECT& project, size_t projectId, size_t userId);
		void removeProject(size_t projectId);

		void assignTeamToProject(size_t projectId, size_t teamId);
		void removeTeamFromProject(size_t projectId, size_t teamId);

		std::vector<PROJECT> loadAllProjects();
		std::vector<pm::dal::TeamsStore::TEAM> getTeamFromProject(size_t projectId);
		//std::vector<pm::dal::TasksStore::TASK> getTasksFromProject(size_t projectId);

	private:
		DatabaseManagement db;

	};
}