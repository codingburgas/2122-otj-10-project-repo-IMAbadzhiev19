#include "pch.h"

#include "TasksManagement.h"

void pm::bll::TasksManagement::createTask(pm::dal::TasksStore::TASK& task, size_t userId)
{
	m_tasksStore.createTask(task, userId);
}

void pm::bll::TasksManagement::updateTask(pm::dal::TasksStore::TASK& task, size_t taskId, size_t userId)
{
	m_tasksStore.updateTask(task, taskId, userId);
}

void pm::bll::TasksManagement::deleteTask(size_t taskId)
{
	m_tasksStore.deleteTask(taskId);
}

void pm::bll::TasksManagement::assignProjectToTask(size_t projectId, size_t taskId)
{
	m_tasksStore.assignProjectToTask(projectId, taskId);
}

void pm::bll::TasksManagement::removeProjectFromTask(size_t projectId, size_t taskId)
{
	m_tasksStore.removeProjectFromTask(projectId, taskId);
}

void pm::bll::TasksManagement::assignTaskToUser(size_t taskId, size_t userId)
{
	m_tasksStore.assignTaskToUser(taskId, userId);
}

void pm::bll::TasksManagement::removeTaskFromUser(size_t taskId, size_t userId)
{
	m_tasksStore.removeTaskFromUser(taskId, userId);
}