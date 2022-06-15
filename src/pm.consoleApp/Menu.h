#pragma once

#include "../pm.dal/UsersStore.h"
#include "../pm.dal/TeamsStore.h"
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

};

class MainMenu : public MenuItem
{
public:

	MainMenu(std::string name, bool horizontal, bool execModule) : MenuItem(name, horizontal, execModule) { }

	virtual void Run() { }
	virtual void Show();

};

class SubMenu : public MenuItem
{
public:

	SubMenu(std::string name, bool horizontal, bool execModule, bool users /*pm::bll::UserManager* uMM, pm::bll::TeamManager* tMM*/);

	virtual void moveToItem(bool next);

protected:

	std::vector<pm::dal::UsersStore::USER> users;
	std::vector<pm::dal::TeamsStore::TEAM> teams;

	//pm::bll::UserManager* tM;
	//pm::bll::TeamManager* tM;

private:

	struct ITEM_INFO
	{
		unsigned short row, column;
		std::string itemName;
	};

	std::vector<ITEM_INFO> itemData;

};

class QuitMenu
{

};