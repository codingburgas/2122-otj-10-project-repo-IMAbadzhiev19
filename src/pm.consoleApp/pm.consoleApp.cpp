#include <iostream>
#include <vector>

#include "Menu.h"
#include "../pm.dal/TasksStore.h"

int main()
{
	pm::bll::UsersManagement uman;
	pm::bll::TeamsManagement tman;

	MainMenu mn("", false, false);
	MenuItem* mi[3] = { new UsersMenu(&uman), new TeamsMenu(&tman), new QuitMenu() };

	for (int i = 0; i < 3; i++)
		mn.addItem(mi[i]);

	mn.Show();
}