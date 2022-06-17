#include <iostream>

#include "Menu.h"

int main()
{
	pm::bll::UsersManagement uman;

	MainMenu mn("", false, false);
	MenuItem* mi[2] = { new UsersMenu(&uman), new QuitMenu() };

	for (int i = 0; i < 2; i++)
		mn.addItem(mi[i]);

	mn.Show();

}