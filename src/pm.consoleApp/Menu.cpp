#include "Menu.h"

//MenuItem & MainMenu
void MenuItem::gotoXY(int x, int y)
{
	COORD coords;
	coords.X = x;
	coords.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
}

int MenuItem::getKeyPressed()
{
	int k = _getch();

	if (k == 224)
		k = _getch();

	return k;
}

void MenuItem::moveToItem(bool next)
{
	size_t oldSelectedItem = selectedItem;

	if (next && (selectedItem < submenu.size() - 1))
		selectedItem++;

	if (!next && (selectedItem > 0))
		selectedItem--;

	if (selectedItem != oldSelectedItem)
	{
		gotoXY(submenu[oldSelectedItem]->position.column, submenu[oldSelectedItem]->position.row);
		for (size_t i = 0; i < selectedItemMarker.length(); i++)
			std::cout << ' ';

		gotoXY(submenu[selectedItem]->position.column, submenu[selectedItem]->position.row);
		std::cout << selectedItemMarker;
	}
}

void MenuItem::addItem(MenuItem* i, unsigned short row, unsigned short column)
{
	size_t l = submenu.size();

	if ((row == 0) && (column == 0) && (l > 0))
		if (horizontal) {
			i->position.row = submenu[0]->position.row;
			i->position.column = submenu[l - 1]->position.column + submenu[l - 1]->name.length() + selectedItemMarker.length();
		}
		else { //Vertical
			i->position.row = submenu[l - 1]->position.row + 1;
			i->position.column = submenu[0]->position.column;
		}
	else {
		i->position.row = row;
		i->position.column = column;
	}

	submenu.push_back(i);
}

void MenuItem::Show()
{
	if (execModule)
		return;

	int key;

	std::string separator = (horizontal) ? " " : "\r\n";

	do
	{
		system("cls");

		for (size_t i = 0; i < submenu.size(); i++)
		{
			if (i == selectedItem)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.length(); c++)
					std::cout << ' ';

			std::cout << submenu[i]->name << separator;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72: if (!horizontal)
			moveToItem(false);
			break;

		case 75: if (horizontal)
			moveToItem(false);
			break;

		case 80: if (!horizontal)
			moveToItem(true);
			break;

		case 77: if (horizontal)
			moveToItem(true);
			break;

		case 13: system("cls");
			submenu[selectedItem]->Run();
			break;
		}

	} while (key != 27);

}

void MenuItem::setPosition(unsigned short row, unsigned short column)
{
	position.row = row;
	position.column = column;
}

void MainMenu::Show()
{
	system("cls");
	MenuItem::Show();
}

/*SubMenu*/
SubMenu::SubMenu(std::string name, bool horizontal, bool execModule, bool users, pm::bll::UsersManagement* uMM, pm::bll::TeamsManagement* tMM, pm::bll::ProjectsManagement* pMM, pm::bll::TasksManagement* taskMM ) : MenuItem(name, horizontal, execModule), uM(uMM), tM(tMM), pM(pMM), taskM(taskMM)
{
	unsigned short c = 1;

	if (users)
		itemData.push_back({ c++, 10 , "Login" });

	itemData.push_back({ c++, 10 , "Create" });
	itemData.push_back({ c++, 10 , "Update" });
	itemData.push_back({ c++, 10 , "Delete" });
	itemData.push_back({ c++, 10 , "ShowAll" });

	if (!users) {

		std::string func;

		if (name.compare("Teams") == 0)
			func = " User";
		else if (name.compare("Projects") == 0)
			func = " Team";
		else if (name.compare("Tasks") == 0)
			func = " Project";

		itemData.push_back({ c++, 10, "Add" + func });
		itemData.push_back({ c++, 10, "Remove" + func });
	}
}

void SubMenu::Show()
{
	if (execModule)
		return;

	int key;
	std::string separator = (horizontal) ? " " : "\r\n";
	do
	{
		system("cls");
		for (size_t i = 0; i < itemData.size(); i++)
		{
			gotoXY(itemData[i].column, itemData[i].row);
			if (i == selectedItem)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';
			std::cout << itemData[i].itemName << separator;
		}
		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToItem(false);
			break;
		case 75:if (horizontal)
			moveToItem(false);
			break;
		case 80:if (!horizontal)
			moveToItem(true);
			break;
		case 77:if (horizontal)
			moveToItem(true);
			break;
		case 13:system("cls");
			runItem();
			break;
		} // switch
	} while (key != 27);
}

void SubMenu::runItem()
{
	switch (selectedItem + 1)
	{
	case 1: Create();
		break;
	case 2: Update();
		break;
	case 3: Delete();
		break;
	case 4: showAll();
		break;
	}
}

void SubMenu::moveToItem(bool next)
{
	size_t oldSelectedItem = selectedItem;

	if (next && (selectedItem < itemData.size() - 1))
		selectedItem++;

	if (!next && (selectedItem > 0))
		selectedItem--;

	if (selectedItem != oldSelectedItem)
	{
		gotoXY(itemData[oldSelectedItem].column, itemData[oldSelectedItem].row);
		for (size_t i = 0; i < selectedItemMarker.length(); i++)
			std::cout << ' ';

		gotoXY(itemData[selectedItem].column, itemData[selectedItem].row);
		std::cout << selectedItemMarker;
	}
}
/*SubMenu*/

/*UsersMenu*/
UsersMenu::UsersMenu(pm::bll::UsersManagement* be) : SubMenu("Users", false, false, true, be, nullptr, nullptr, nullptr)
{
	users.clear();

	try
	{
		users = uM->getRegisteredUsers();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void UsersMenu::moveToUser(bool next)
{
	size_t oldSelectedItem = selectedUser;

	if (next && (selectedUser < users.size() - 1))
		selectedUser++;

	if (!next && (selectedUser > 0))
		selectedUser--;

	if (selectedUser != oldSelectedItem)
	{
		gotoXY(8, oldSelectedItem);
		for (size_t i = 0; i < selectedItemMarker.length(); i++)
			std::cout << ' ';

		gotoXY(8, selectedUser);
		std::cout << selectedItemMarker;
	}
}

void UsersMenu::runItem()
{
	switch (selectedItem + 1)
	{
	case 1: Login();
		break;
	case 2: Create();
		break;
	case 3: 
		if (currentUser.firstName != "" && currentUser.admin == true)
			Update();
		else
		{
			std::cout << "You must be admin!" << std::endl;
			Sleep(1000);
		}
		break;
	case 4: 
		if (currentUser.firstName != "" && currentUser.admin == true)
			Delete();
		else
		{
			std::cout << "You must be admin!" << std::endl;
			Sleep(1000);
		}
		break;
	case 5:
		if (currentUser.firstName != "" && currentUser.admin == true)
			showAll();
		else
		{
			std::cout << "You must be admin!" << std::endl;
			Sleep(1000);
		}
		break;
	}
}

void UsersMenu::Login()
{
	std::string email, password;

	try {
		std::cout << "Email: "; getline(std::cin, email);
		std::cout << "Password: "; getline(std::cin, password);

		uM->loginUser(email, password);
		currentUser = uM->m_usersStore.getUserByEmail(email);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "You have successfully logged in";
	Sleep(1000);

}

void UsersMenu::Create()
{
	pm::dal::UsersStore::USER usr;

	do
	{
		system("cls");
		try {
			std::cout << "Enter first name: "; getline(std::cin, usr.firstName);
			std::cout << "Enter last name: "; getline(std::cin, usr.lastName);
			std::cout << "Enter email: "; getline(std::cin, usr.email);
			std::cout << "Age: "; std::cin >> usr.age; std::cin.ignore();
			std::cout << "Enter password: "; getline(std::cin, usr.password);

			uM->registerUser(usr);
			users = uM->getRegisteredUsers();

			break;
		}
		catch (std::string msg) {
			std::cout << msg << std::endl;
			Sleep(1000);
		}
	} while (true);

	std::cout << "You have successfully created an account";
	Sleep(1000);
}

void UsersMenu::showAll()
{
	std::string separator = (horizontal) ? " " : "\r\n";

	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < users.size(); i++)
		{
			gotoXY(5 , i + 1);

			std::cout << users[i].id << ". " << users[i].firstName << " " << users[i].lastName << ", " << users[i].email << ", " << users[i].age << ", " << users[i].createdOn.day << "/" << users[i].createdOn.month << "/" << users[i].createdOn.year;

			std::string isAdminOut = (users[i].admin == 0) ? "| Admin: NO" : "| Admin: YES";
			std::cout << isAdminOut << separator;
		}

		key = getKeyPressed();

	} while (key != 27);

}

void UsersMenu::Delete()
{
	selectedUser = 0;
	std::string separator = (horizontal) ? " " : "\r\n";

	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < users.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedUser)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << users[i].firstName << " " << users[i].lastName << ", " << users[i].email << ", " << users[i].age << ", " << users[i].createdOn.day << "/" << users[i].createdOn.month << "/" << users[i].createdOn.year;

			std::string isAdminOut = (users[i].admin == 0) ? "| Admin: NO" : "| Admin: YES";
			std::cout << isAdminOut << separator;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToUser(false);
			break;
		case 75:if (horizontal)
			moveToUser(false);
			break;
		case 80:if (!horizontal)
			moveToUser(true);
			break;
		case 77:if (horizontal)
			moveToUser(true);
			break;
		case 13:
			uM->removeUser(users[selectedUser].id);
			users = uM->getRegisteredUsers();
			break;
		} // switch
	} while (key != 27);
}

void UsersMenu::Update()
{
	selectedUser = 0;
	std::string separator = (horizontal) ? " " : "\r\n";

	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < users.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedUser)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << users[i].firstName << " " << users[i].lastName << ", " << users[i].email << ", " << users[i].age << ", " << users[i].createdOn.day << "/" << users[i].createdOn.month << "/" << users[i].createdOn.year;

			std::string isAdminOut = (users[i].admin == 0) ? "| Admin: NO" : "| Admin: YES";
			std::cout << isAdminOut << separator;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToUser(false);
			break;
		case 75:if (horizontal)
			moveToUser(false);
			break;
		case 80:if (!horizontal)
			moveToUser(true);
			break;
		case 77:if (horizontal)
			moveToUser(true);
			break;
		case 13:
		{
			system("cls");
			pm::dal::UsersStore::USER usr;

			std::cout << "Enter first name: "; getline(std::cin, usr.firstName);
			std::cout << "Enter last name: "; getline(std::cin, usr.lastName);
			std::cout << "Enter email: "; getline(std::cin, usr.email);
			std::cout << "Age: "; std::cin >> usr.age; std::cin.ignore();
			std::cout << "Enter password: "; getline(std::cin, usr.password);

			usr.admin = 0;

			uM->updateUser(users[selectedUser].id ,usr);

			users = uM->getRegisteredUsers();
			break;
		}
		} // switch
	} while (key != 27);
}
/*UsersMenu*/

/*TeamsMenu*/
TeamsMenu::TeamsMenu(pm::bll::TeamsManagement* be) : SubMenu("Teams", false, false, true, nullptr, be, nullptr, nullptr)
{
	teams.clear();

	try
	{
		teams = tM->loadTeams();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void TeamsMenu::moveToTeam(bool next)
{
	size_t oldSelectedItem = selectedTeam;

	if (next && (selectedTeam < users.size() - 1))
		selectedTeam++;

	if (!next && (selectedTeam > 0))
		selectedTeam--;

	if (selectedTeam != oldSelectedItem)
	{
		gotoXY(8, oldSelectedItem);
		for (size_t i = 0; i < selectedItemMarker.length(); i++)
			std::cout << ' ';

		gotoXY(8, selectedTeam);
		std::cout << selectedItemMarker;
	}
}

void TeamsMenu::Create()
{
	pm::dal::TeamsStore::TEAM team;
	
	std::cout << "Enter team's name: "; std::getline(std::cin, team.title);

	tM->createTeam(team, currentUser.id);
	teams = tM->loadTeams();
}

void TeamsMenu::Update()
{
	selectedTeam = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < teams.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTeam)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << teams[i].title << " " << teams[i].createdOn.day << "/" << teams[i].createdOn.month << "/" << teams[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTeam(false);
			break;
		case 75:if (horizontal)
			moveToTeam(false);
			break;
		case 80:if (!horizontal)
			moveToTeam(true);
			break;
		case 77:if (horizontal)
			moveToTeam(true);
			break;
		case 13:
		{
			system("cls");
			
			pm::dal::TeamsStore::TEAM t;

			std::getline(std::cin, t.title);

			tM->updateTeam(teams[selectedTeam].id, t, currentUser.id);
			teams = tM->loadTeams();

			break;
		}
		} // switch
	} while (key != 27);
}

void TeamsMenu::Delete()
{
	selectedTeam = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < teams.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTeam)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << teams[i].title << " " << teams[i].createdOn.day << "/" << teams[i].createdOn.month << "/" << teams[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTeam(false);
			break;
		case 75:if (horizontal)
			moveToTeam(false);
			break;
		case 80:if (!horizontal)
			moveToTeam(true);
			break;
		case 77:if (horizontal)
			moveToTeam(true);
			break;
		case 13:
		{
			system("cls");

			tM->removeTeam(teams[selectedTeam].id);
			teams = tM->loadTeams();

			break;
		}
		} // switch
	} while (key != 27);
}

void TeamsMenu::AddUser()
{
	selectedTeam = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < teams.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTeam)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << teams[i].title << " " << teams[i].createdOn.day << "/" << teams[i].createdOn.month << "/" << teams[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTeam(false);
			break;
		case 75:if (horizontal)
			moveToTeam(false);
			break;
		case 80:if (!horizontal)
			moveToTeam(true);
			break;
		case 77:if (horizontal)
			moveToTeam(true);
			break;
		case 13:
		{
			system("cls");

			UsersMenu usersMenu;
			usersMenu.showAll();

			int choice;
			std::cout << "\n\n\nEnter the id of the user who you would like to add in the current team: "; std::cin >> choice;

			tM->addUserToTeam(teams[selectedTeam].id, choice);

			users = uM->getRegisteredUsers();
			teams = tM->loadTeams();

			break;
		}
		} // switch
	} while (key != 27);
}

void TeamsMenu::RemoveUser()
{
	selectedTeam = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < teams.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTeam)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << teams[i].title << " " << teams[i].createdOn.day << "/" << teams[i].createdOn.month << "/" << teams[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTeam(false);
			break;
		case 75:if (horizontal)
			moveToTeam(false);
			break;
		case 80:if (!horizontal)
			moveToTeam(true);
			break;
		case 77:if (horizontal)
			moveToTeam(true);
			break;
		case 13:
		{
			system("cls");

			std::vector<pm::dal::UsersStore::USER> usersTeam;

			usersTeam = tM->getUsersFromTeam(teams[selectedTeam].id);

			std::cout << std::endl;
			for (const auto& x : usersTeam)
			{
				std::cout << "          ";
				std::cout << x.id << ". " << x.firstName << " " << x.lastName << ", " << x.email << ", " << x.age << ", " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year;

				std::string isAdminOut = (x.admin == 0) ? "| Admin: NO" : "| Admin: YES";
				std::cout << isAdminOut << separator;
			}

			int choice;
			std::cout << "\n\n\nEnter the id of the user who you would like to remove from the current team: "; std::cin >> choice;

			tM->removeUserFromTeam(teams[selectedTeam].id, choice);

			users = uM->getRegisteredUsers();
			teams = tM->loadTeams();

			break;
		}
		} // switch
	} while (key != 27);
}

void TeamsMenu::showAll()
{
	selectedTeam = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	gotoXY(8, 0); std::cout << "Press enter on the selected team to view the users in it";

	do
	{
		system("cls");

		for (size_t i = 0; i < teams.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTeam)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << teams[i].title << " " << teams[i].createdOn.day << "/" << teams[i].createdOn.month << "/" << teams[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTeam(false);
			break;
		case 75:if (horizontal)
			moveToTeam(false);
			break;
		case 80:if (!horizontal)
			moveToTeam(true);
			break;
		case 77:if (horizontal)
			moveToTeam(true);
			break;
		case 13:
		{
			system("cls");

			std::vector<pm::dal::UsersStore::USER> usersTeam;

			usersTeam = tM->getUsersFromTeam(teams[selectedTeam].id);

			std::cout << std::endl;
			for (const auto& x : usersTeam)
			{
				std::cout << "          ";
				std::cout << x.id << ". " << x.firstName << " " << x.lastName << ", " << x.email << ", " << x.age << ", " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year;

				std::string isAdminOut = (x.admin == 0) ? "| Admin: NO" : "| Admin: YES";
				std::cout << isAdminOut << separator;
			}

			std::cout << "\n\nPress enter to go back";
			system("pause>nul");
			Show();
		}
		} // switch
	} while (key != 27);
}
/*TeamsMenu*/

/*ProjectsMenu*/
ProjectsMenu::ProjectsMenu(pm::bll::ProjectsManagement* be)
{
	projects.clear();

	try
	{
		projects = pM->loadAllProjects();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void ProjectsMenu::Create()
{
	pm::dal::ProjectsStore::PROJECT project;

	std::cout << "Enter project's name: "; std::getline(std::cin, project.title);
	std::cout << "Enter a description of the project: "; std::getline(std::cin, project.description);

	pM->createProject(project, currentUser.id);
	projects = pM->loadAllProjects();
}

void ProjectsMenu::Update()
{
	selectedProject = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < projects.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedProject)
				std::cout << selectedItemMarker;
			else
				for (short c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << projects[i].title << " " << projects[i].description << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToProject(false);
			break;
		case 75:if (horizontal)
			moveToProject(false);
			break;
		case 80:if (!horizontal)
			moveToProject(true);
			break;
		case 77:if (horizontal)
			moveToProject(true);
			break;
		case 13:
		{
			system("cls");

			pm::dal::ProjectsStore::PROJECT p;

			std::cout << "Enter new name or if you don't want to change it enter the same: "; std::getline(std::cin, p.title);
			std::cout << "Enter new description or if you don't want to change it enter the same: "; std::getline(std::cin, p.description);

			pM->updateProject(p, projects[selectedProject].id, currentUser.id);
			projects = pM->loadAllProjects();

			break;
		}
		} // switch
	} while (key != 27);
}

/*ProjectsMenu*/