#pragma once

#include<iostream>
#include<windows.h>
#include<conio.h>
#include <chrono>
#include <random>

#include "../pm.bll/UsersManagement.h"
#include "../pm.bll/TeamsManagement.h"
#include "../pm.bll/ProjectsManagement.h"
#include "../pm.bll/TasksManagement.h"

#include "Database.h"

class MenuItem
{
public:

	MenuItem() { }
	MenuItem(std::string n, bool h, bool eM) : name(n), horizontal(h), execModule(eM), position(POINT{ 0, 0 }), selectedItem(0), selectedItemMarker("  ->") { }

	struct POINT 
	{
		unsigned short row, column;
	};

	void addItem(MenuItem* item, unsigned short row = 0, unsigned short column = 0);

	virtual void Run() = 0;
	virtual void Show();

	void setPosition(unsigned short row, unsigned short column);

protected:

	void gotoXY(int x, int y);
	int getKeyPressed();
	virtual void moveToItem(bool next);
	void clearScreen();
	int random_in_range(int minimum, int maximum);

	std::string name;
	bool horizontal;
	bool execModule;

	POINT position;
	size_t selectedItem;
	std::string selectedItemMarker;
	
	std::vector<MenuItem*> submenu;
	DatabaseManagement db;

};

class MainMenu : public MenuItem
{
public:

	MainMenu(std::string name, bool horizontal, bool execModule) : MenuItem(name, horizontal, execModule) { }

	virtual void Run() { }
	virtual void Show();

};

class QuitMenu : public MenuItem
{
public:
	QuitMenu() : MenuItem("Quit", true, true) { }

	virtual void Run() { 
		gotoXY(45, 0); 
		std::cout << "Successfully exited" << std::endl; 
		exit(0); 
	}
};

class SubMenu : public MenuItem
{
public:

	SubMenu() { }

	SubMenu(std::string name, bool horizontal, bool execModule, bool users, pm::bll::UsersManagement* uMM, pm::bll::TeamsManagement* tMM, pm::bll::ProjectsManagement* pMM, pm::bll::TasksManagement* taskMM);

	virtual void moveToItem(bool next);
	virtual void Run() { Show(); }
	virtual void runItem();
	virtual void Show();
	virtual void Create() = 0;
	virtual void Delete() = 0;
	virtual void Update() = 0;
	virtual void showAll() = 0;

protected:

	std::vector<pm::dal::UsersStore::USER> users;
	std::vector<pm::dal::TeamsStore::TEAM> teams;
	std::vector<pm::dal::ProjectsStore::PROJECT> projects;
	std::vector<pm::dal::TasksStore::TASK> tasks;

	pm::bll::UsersManagement* uM;
	pm::bll::TeamsManagement* tM;
	pm::bll::ProjectsManagement* pM;
	pm::bll::TasksManagement* taskM;

	DatabaseManagement db_menu;

private:

	struct ITEM_INFO
	{
		unsigned short row, column;
		std::string itemName;
	};

	std::vector<ITEM_INFO> itemData;

};

class UsersMenu : public SubMenu
{
public:

	UsersMenu() : selectedUser(0) { }
	UsersMenu(pm::bll::UsersManagement* be);

	virtual void runItem();
	void moveToUser(bool next);

	void Login();
	void Create();
	void Delete();
	void Update();
	void showAll();

private:
	size_t selectedUser;
};

class TeamsMenu : public SubMenu
{
public:

	TeamsMenu() : selectedTeam(0) { }
	TeamsMenu(pm::bll::TeamsManagement* be);

	void moveToTeam(bool next);
	virtual void runItem();

	virtual void Create();
	virtual void Delete();
	virtual void Update();
	virtual void showAll();

	void AddUser();
	void RemoveUser();

private:
	size_t selectedTeam;
};

class ProjectsMenu : public SubMenu
{
public:
	ProjectsMenu() : selectedProject(0) { }
	ProjectsMenu(pm::bll::ProjectsManagement* be);

	void moveToProject(bool next);
	void runItem();

	virtual void Create();
	virtual void Delete();
	virtual void Update();
	virtual void showAll();

	void AddTeam();
	void RemoveTeam();

private:
	size_t selectedProject;
};

class TasksMenu : public SubMenu
{
public:
	TasksMenu() { }
	TasksMenu(pm::bll::TasksManagement* be);

	void moveToTask(bool next) { }

	virtual void Create() { }
	virtual void Delete() { }
	virtual void Update() { }
	virtual void showAll() { }

	void AddProject() { }
	void RemoveProject() { }

	void AssignUser() { }
	void RemoveUser() { }


private:
	size_t selectedTask;
};