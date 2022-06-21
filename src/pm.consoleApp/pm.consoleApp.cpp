#include <iostream>
#include <vector>

#include "Menu.h"
#include "../pm.dal/TasksStore.h"

int main()
{
	pm::dal::TasksStore::TASK task;
	pm::dal::TasksStore ts;

	task.title = "Task 4";
	task.description = "No desc";
	task.status = "Done";
	
	ts.assignProjectToTask(1, 1);

	/*pm::bll::UsersManagement uman;

	MainMenu mn("", false, false);
	MenuItem* mi[2] = { new UsersMenu(&uman), new QuitMenu() };

	for (int i = 0; i < 2; i++)
		mn.addItem(mi[i]);

	mn.Show();*/
}