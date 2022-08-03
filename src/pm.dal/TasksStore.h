#pragma once

#include "pch.h"

#include "../../lib/nanodbc/nanodbc.h"
#include "../pm.consoleApp/Database.h"

#include "../pm.dal/ProjectsStore.h"
#include "../pm.dal/TeamsStore.h"
#include "../pm.dal/UsersStore.h"

namespace pm::dal
{
	class TasksStore
	{
	public:

		struct TASK
		{
			int id;
			std::string title;
			std::string description;
			std::string status;
			nanodbc::date createdOn;
			int creatorId;
		};

		void createTask(TASK& task, size_t userId);
		void updateTask(TASK& task, size_t taskId, size_t userId);
		void deleteTask(size_t taskId);

		void assignProjectToTask(size_t projectId, size_t taskId);
		void removeProjectFromTask(size_t projectId, size_t taskId);

		void assignTaskToUser(size_t taskId, size_t userId);
		void removeTaskFromUser(size_t taskId, size_t userId);

		std::vector<TASK> loadAllTasks();
		std::vector<pm::dal::ProjectsStore::PROJECT> getProjectsFromTask(size_t taskId);
		std::vector<pm::dal::UsersStore::USER> getUsersFromTask(size_t taskId);

	private:
		DatabaseManagement db;
	};
}