#include <iostream>

#include "Menu.h"

int main()
{
	pm::bll::UsersManagement uman;
	pm::bll::TeamsManagement tman;

	MainMenu mn("Management", false, false);
	MenuItem* mi[2] = { new UsersMenu(&uman), new QuitMenu() };


}