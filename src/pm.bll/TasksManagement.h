#pragma once

#include "pch.h"

#include "../../lib/nanodbc/nanodbc.h"
#include "../pm.consoleApp/Database.h"

#include "../pm.dal/TasksStore.h"
#include "../pm.bll/ProjectsManagement.h"
#include "../pm.bll/TeamsManagement.h"
#include "../pm.bll/UsersManagement.h"

namespace pm::bll
{
	class TasksManagement
	{
	public:

		pm::dal::TasksStore m_tasksStore;

		void createTask(pm::dal::TasksStore::TASK& task, size_t userId);
		void updateTask(pm::dal::TasksStore::TASK& task, size_t taskId, size_t userId);
		void deleteTask(size_t taskId);

		void assignProjectToTask(size_t projectId, size_t taskId);
		void removeProjectFromTask(size_t projectId, size_t taskId);

		void assignTaskToUser(size_t taskId, size_t userId);
		void removeTaskFromUser(size_t taskId, size_t userId);

		std::vector<pm::dal::TasksStore::TASK> loadAllTasks();
		std::vector<pm::dal::ProjectsStore::PROJECT> getProjectsFromTask(size_t taskId);
		std::vector<pm::dal::UsersStore::USER> getUsersFromTask(size_t taskId);

	};
}