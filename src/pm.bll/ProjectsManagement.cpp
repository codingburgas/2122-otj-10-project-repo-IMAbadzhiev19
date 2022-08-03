#include "pch.h"

#include "ProjectsManagement.h"

void pm::bll::ProjectsManagement::createProject(pm::dal::ProjectsStore::PROJECT& project, size_t userId)
{
	m_projectStore.createProject(project, userId);
}

void pm::bll::ProjectsManagement::updateProject(pm::dal::ProjectsStore::PROJECT& project, size_t projectId, size_t userId)
{
	m_projectStore.updateProject(project, projectId, userId);
}

void pm::bll::ProjectsManagement::removeProject(size_t projectId)
{
	m_projectStore.removeProject(projectId);
}

void pm::bll::ProjectsManagement::assignTeamToProject(size_t projectId, size_t teamId)
{
	m_projectStore.assignTeamToProject(projectId, teamId);
}

void pm::bll::ProjectsManagement::removeTeamFromProject(size_t projectId, size_t teamId)
{
	m_projectStore.removeTeamFromProject(projectId, teamId);
}

std::vector<pm::dal::ProjectsStore::PROJECT> pm::bll::ProjectsManagement::loadAllProjects()
{
	return m_projectStore.loadAllProjects();
}

std::vector<pm::dal::TeamsStore::TEAM> pm::bll::ProjectsManagement::getTeamFromProject(size_t projectId)
{
	return m_projectStore.getTeamFromProject(projectId);
}

//std::vector<pm::dal::TasksStore::TASK> pm::bll::ProjectsManagement::getTasksFromProject(size_t projectId)
//{
//	return m_projectStore.getTasksFromProject(projectId);
//}