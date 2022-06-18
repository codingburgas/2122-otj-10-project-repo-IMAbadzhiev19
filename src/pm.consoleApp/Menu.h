#pragma once

#include "../pm.bll/UsersManagement.h"
#include "../pm.bll/TeamsManagement.h"
#include "Database.h"

class MenuItem
{
public:

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
	virtual void Run() { exit(0); }
};

class SubMenu : public MenuItem
{
public:

	SubMenu(std::string name, bool horizontal, bool execModule, bool users, pm::bll::UsersManagement* uMM, pm::bll::TeamsManagement* tMM);

	virtual void moveToItem(bool next);
	virtual void Run() { Show(); }
	virtual void runItem();
	virtual void Show();
	virtual void Create() = 0;
	virtual void Delete() = 0;
	virtual void Update() = 0;
	virtual void showAll() = 0;
	virtual void Add() = 0;
	virtual void Remove() = 0;

protected:

	std::vector<pm::dal::UsersStore::USER> users;
	std::vector<pm::dal::TeamsStore::TEAM> teams;

	pm::bll::UsersManagement* uM;
	pm::bll::TeamsManagement* tM;

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

	UsersMenu(pm::bll::UsersManagement* be);

	virtual void runItem();
	void moveToUser(bool next);

	void Login();
	void Create();
	void Delete();
	void Update();
	void showAll();
	void Add() { }
	void Remove() { }

private:
	size_t selectedUser = 0;
	pm::dal::UsersStore::USER currentUser;
};