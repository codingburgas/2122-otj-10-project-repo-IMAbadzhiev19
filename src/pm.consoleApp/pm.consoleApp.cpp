#include <iostream>
#include <vector>

#include "Menu.h"
#include "../pm.dal/TasksStore.h"

int main()
{
	pm::bll::UsersManagement uman;
	pm::bll::TeamsManagement teamman;
	pm::bll::ProjectsManagement pman;
	pm::bll::TasksManagement taskman;

	MainMenu mn("", false, false);
	MenuItem* mi[5] = { new UsersMenu(&uman), new TeamsMenu(&teamman), new ProjectsMenu(&pman), new TasksMenu(&taskman), new QuitMenu() };

	for (int i = 0; i < 5; i++)
		mn.addItem(mi[i]);

	mn.Show();
}