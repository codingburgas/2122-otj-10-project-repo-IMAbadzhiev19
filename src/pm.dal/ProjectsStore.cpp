#include "pch.h"

#include "../pm.dal/ProjectsStore.h"

void pm::dal::ProjectsStore::createProject(PROJECT& project, size_t userId)
{
	std::string query = "INSERT INTO Projects ([Title], [Description], [CreatedOn], [CreatorId]) "
		"VALUES ('" + project.title + "', '" + project.description + "', GETDATE(), " + std::to_string(userId) + ")";

	db.executeQuery(query);
}

void pm::dal::ProjectsStore::updateProject(PROJECT& project, size_t projectId, size_t userId)
{
	std::string query = "UPDATE Projects "
		"SET Title = '" + project.title + "', Description = '" + project.description + "' WHERE Id = " + std::to_string(projectId);

	db.executeQuery(query);
}

void pm::dal::ProjectsStore::removeProject(size_t projectId)
{
	std::string query = "DELETE FROM Projects WHERE Id = " + std::to_string(projectId);
	db.executeQuery(query);
}

void pm::dal::ProjectsStore::assignTeamToProject(size_t projectId, size_t teamId)
{
	std::string query = "INSERT INTO [TeamsAndProjects] (TeamId, ProjectId) VALUES (" + std::to_string(teamId) + ", " + std::to_string(projectId) + ")";
	db.executeQuery(query);
}

void pm::dal::ProjectsStore::removeTeamFromProject(size_t projectId, size_t teamId)
{
	std::string query = "DELETE FROM [TeamsAndProjects] WHERE ProjectId = " + std::to_string(projectId) + " AND TeamId = " + std::to_string(teamId);
	db.executeQuery(query);
}

std::vector<pm::dal::ProjectsStore::PROJECT> pm::dal::ProjectsStore::loadAllProjects()
{
	std::vector<PROJECT> projects;

	std::string query = "SELECT * FROM Projects";
	nanodbc::result res = db.getResultFromSelect(query);

	while (res.next())
	{
		projects.push_back({ res.get<int>(0), res.get<std::string>(1), res.get<std::string>(2), res.get<nanodbc::date>(3), res.get<int>(4) });
	}

	return projects;
}

std::vector<pm::dal::TeamsStore::TEAM> pm::dal::ProjectsStore::getTeamFromProject(size_t projectId)
{
	std::vector<pm::dal::TeamsStore::TEAM> teams;

	std::string query = "SELECT Teams.Id, Teams.Title, Teams.CreatedOn, Teams.CreatorId FROM TeamsAndProjects, Teams, Projects "
		"WHERE (TeamId = Teams.Id) AND (ProjectId = Projects.Id) AND (ProjectId = " + std::to_string(projectId) + ")";

	nanodbc::result res = db.getResultFromSelect(query);

	while (res.next())
	{
		teams.push_back({ res.get<int>(0), res.get<std::string>(1), res.get<nanodbc::date>(2), res.get<int>(3) });
	}

	return teams;
}

//std::vector<pm::dal::TasksStore::TASK> pm::dal::ProjectsStore::getTasksFromProject(size_t projectId)
//{
//	std::vector<pm::dal::TasksStore::TASK> tasks;
//
//	std::string query = "SELECT Tasks.Id, Tasks.Title, Tasks.Description, Tasks.Status, Tasks.CreatedOn, Tasks.CreatorId FROM ProjectsAndTasks, Projects, Tasks WHERE (Projects.Id = ProjectId) AND (TaskId = Tasks.Id) AND (ProjectId = " + std::to_string(projectId) + ")";
//
//	nanodbc::result res = db.getResultFromSelect(query);
//
//	while (res.next())
//	{
//		tasks.push_back({ res.get<int>(0), res.get<std::string>(1), res.get<std::string>(2), res.get<std::string>(3), res.get<nanodbc::date>(4), res.get<int>(3) });
//	}
//
//	return tasks;
//}