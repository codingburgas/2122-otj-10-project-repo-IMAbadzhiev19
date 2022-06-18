#include <iostream>
#include <vector>

#include "Menu.h"

int main()
{
	pm::bll::UsersManagement uman;

	MainMenu mn("", false, false);
	MenuItem* mi[2] = { new UsersMenu(&uman), new QuitMenu() };

	for (int i = 0; i < 2; i++)
		mn.addItem(mi[i]);

	mn.Show();

	/*nanodbc::connection conn = db.getConn();
	
	nanodbc::result res = db.getResultFromSelect("SELECT * FROM [Users]");
	std::vector<pm::dal::UsersStore::USER> users;*/

	/*while (res.next())
	{
		users.emplace_back(res.get<int>(0), res.get<std::string>(1), res.get<std::string>(2), res.get<std::string>(3),
			res.get<int>(4), res.get<std::string>(5), res.get<nanodbc::date>(6), res.get<short>(7));
	}*/

	//users = us.getRegisteredUsers();

	//for (size_t i = 0; i < users.size(); i++)
	//	std::cout << users[i].id << std::endl;
}