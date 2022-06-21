#include "pch.h"

#include "TasksStore.h"

void pm::dal::TasksStore::createTask(TASK& task, size_t userId)
{
	std::string query = "INSERT INTO [Tasks] (Title, Description, Status, CreatedOn, CreatorId) "
		"VALUES ('" + task.title + "', '" + task.description + "', '" + task.status + "', GETDATE(), " + std::to_string(userId) + ")";

	db.executeQuery(query);
}

void pm::dal::TasksStore::updateTask(TASK& task, size_t taskId, size_t userId)
{
	std::string query = "UPDATE [Tasks] SET Title = '" + task.title + "', Description = '" + task.description + "', Status = '" + task.status + "' "
		"WHERE Id = " + std::to_string(taskId);

	db.executeQuery(query);
}

void pm::dal::TasksStore::deleteTask(size_t taskId)
{
	std::string query = "DELETE FROM [Tasks] WHERE Id = " + std::to_string(taskId);
	db.executeQuery(query);
}

void pm::dal::TasksStore::assignProjectToTask(size_t projectId, size_t taskId)
{
	std::string query = "INSERT INTO [ProjectsAndTasks] (ProjectId, TaskId) VALUES (" + std::to_string(projectId) + ", " + std::to_string(taskId) + ")";
	db.executeQuery(query);
}

void pm::dal::TasksStore::removeProjectFromTask(size_t projectId, size_t taskId)
{
	std::string query = "DELETE FROM [ProjectsAndTasks] WHERE ProjectId = " + std::to_string(projectId) + " AND TaskId = " + std::to_string(taskId);
	db.executeQuery(query);
}

void pm::dal::TasksStore::assignTaskToUser(size_t taskId, size_t userId)
{
	std::string query = "INSERT INTO [TasksAndUsers] (UserId, TaskId) VALUES (" + std::to_string(userId) + ", " + std::to_string(taskId) + ")";
	db.executeQuery(query);
}

void pm::dal::TasksStore::removeTaskFromUser(size_t taskId, size_t userId)
{
	std::string query = "DELETE FROM [TasksAndUsers] WHERE UserId = " + std::to_string(userId) + " AND TaskId = " + std::to_string(taskId);
	db.executeQuery(query);
}