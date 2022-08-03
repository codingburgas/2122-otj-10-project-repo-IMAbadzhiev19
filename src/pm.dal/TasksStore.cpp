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

std::vector<pm::dal::TasksStore::TASK> pm::dal::TasksStore::loadAllTasks()
{
	std::vector<TASK> tasks;
	std::string query = "SELECT * FROM Tasks";

	nanodbc::result res = db.getResultFromSelect(query);

	while (res.next())
	{
		tasks.push_back({ res.get<int>(0), res.get<std::string>(1), res.get<std::string>(2), res.get<std::string>(3), res.get<nanodbc::date>(4), res.get<int>(5) });
	}

	return tasks;
}

std::vector<pm::dal::ProjectsStore::PROJECT> pm::dal::TasksStore::getProjectsFromTask(size_t taskId)
{
	std::vector<pm::dal::ProjectsStore::PROJECT> projects;

	std::string query = "SELECT Projects.Id, Projects.Title, Projects.Description, Projects.CreatedOn, Projects.CreatorId "
		"FROM Projects, Tasks, ProjectsAndTasks WHERE (Projects.Id = ProjectId) AND (Tasks.Id = TaskId) AND (TaskId = " + std::to_string(taskId) + ")";

	nanodbc::result res = db.getResultFromSelect(query);

	while (res.next())
	{
		projects.push_back({ res.get<int>(0), res.get<std::string>(1), res.get<std::string>(2), res.get<nanodbc::date>(3), res.get<int>(4) });
	}

	return projects;
}

std::vector<pm::dal::UsersStore::USER> pm::dal::TasksStore::getUsersFromTask(size_t taskId)
{
	std::vector<pm::dal::UsersStore::USER> users;

	std::string query = "SELECT Users.Id, Users.FirstName, Users.LastName, Users.Email, Users.Age, Users.Password, Users.CreatedOn, Users.Admin "
		"FROM Users, Tasks, TasksAndUsers WHERE (Users.Id = UserId) AND (Tasks.Id = TaskId) AND (TaskId = " + std::to_string(taskId) + ")";

	nanodbc::result res = db.getResultFromSelect(query);

	while (res.next())
	{
		users.push_back({ res.get<int>(0), res.get<std::string>(1), res.get<std::string>(2), res.get<std::string>(3), res.get<short>(4), res.get<std::string>(5), res.get<nanodbc::date>(6), res.get<short>(7) });
	}

	return users;
}