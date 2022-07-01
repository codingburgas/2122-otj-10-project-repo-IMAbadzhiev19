#include "Menu.h"

namespace structure
{
	pm::dal::UsersStore::USER currentUserG;
}

//MenuItem & MainMenu

void MenuItem::gotoXY(int x, int y)
{
	COORD coords;
	coords.X = x;
	coords.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
}

void MenuItem::clearScreen()
{
	COORD cursorPosition;

	cursorPosition.X = 0;
	cursorPosition.Y = 0;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

int MenuItem::random_in_range(int min, int max)
{
	thread_local std::ranlux48 rng(
		std::chrono::system_clock::now().time_since_epoch().count());

	return std::uniform_int_distribution <int>(min, max)(rng);
}

void MenuItem::writeInColor(unsigned short color, std::string output)
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handler, color);
	std::cout << output;
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
			i->position.column = static_cast<unsigned short>(submenu[l - 1]->position.column + submenu[l - 1]->name.length() + selectedItemMarker.length());
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
		clearScreen();

		for (size_t i = 0; i < submenu.size(); i++)
		{
			gotoXY(submenu[i]->position.column, submenu[i]->position.row);
			if (i == selectedItem)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.length(); c++)
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

	system("cls");

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
	
	gotoXY(40, 0); std::cout << "Have a nice day! ;)";
}

/*SubMenu*/
SubMenu::SubMenu(std::string name, bool horizontal, bool execModule, bool users, pm::bll::UsersManagement* uMM, pm::bll::TeamsManagement* tMM, pm::bll::ProjectsManagement* pMM, pm::bll::TasksManagement* taskMM ) : MenuItem(name, horizontal, execModule), uM(uMM), tM(tMM), pM(pMM), taskM(taskMM)
{
	unsigned short c = 3;

	if (users)
		itemData.push_back({ c++, 40 , "Login" });

	itemData.push_back({ c++, 40 , "Create" });
	itemData.push_back({ c++, 40 , "Update" });
	itemData.push_back({ c++, 40 , "Delete" });
	itemData.push_back({ c++, 40 , "Show All" });

	if (!users && name != "Tasks") {

		std::string func;

		if (name.compare("Teams") == 0)
			func = " User";
		else if (name.compare("Projects") == 0)
			func = " Team";

		itemData.push_back({ c++, 40, "Add" + func });
		itemData.push_back({ c++, 40, "Remove" + func });
	}
	else if (name.compare("Tasks") == 0)
	{
		itemData.push_back({ c++, 40, "Assign"});
		itemData.push_back({ c++, 40, "Remove" });
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
		clearScreen();
		for (size_t i = 0; i < itemData.size(); i++)
		{
			gotoXY(itemData[i].column, itemData[i].row);
			if (i == selectedItem)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

	system("cls");
}

void SubMenu::moveToItem(bool next)
{
	size_t OldSelectedItem = selectedItem;

	if (next && (selectedItem < itemData.size() - 1))
		selectedItem++;
	if (!next && (selectedItem > 0))
		selectedItem--;
	if (selectedItem != OldSelectedItem)
	{
		gotoXY(itemData[OldSelectedItem].column, itemData[OldSelectedItem].row);
		for (size_t j = 0; j < selectedItemMarker.length(); j++)
			std::cout << ' ';

		gotoXY(itemData[selectedItem].column, itemData[selectedItem].row);
		std::cout << selectedItemMarker;
	}
}
/*SubMenu*/

/*UsersMenu*/
UsersMenu::UsersMenu(pm::bll::UsersManagement* be) : SubMenu("Users", false, false, true, be, nullptr, nullptr, nullptr), selectedUser(0)
{
	users.clear();

	try
	{
		users = uM->getRegisteredUsers();
	}
	catch (std::string& error)
	{
		std::cout << error << std::endl;
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
		gotoXY(5, oldSelectedItem + 1);
		for (size_t i = 0; i < selectedItemMarker.length(); i++)
			std::cout << ' ';

		gotoXY(5, selectedUser + 1);
		std::cout << selectedItemMarker;
	}
}

void UsersMenu::runItem()
{
	std::string errorMsg;

	if (structure::currentUserG.firstName == "") {
		errorMsg = "You must log in first!";
	}
	else if (structure::currentUserG.admin != 1) {
		errorMsg = "You must be admin!";
	}

	switch (selectedItem + 1)
	{
	case 1: Login();
		break;
	case 2: Create();
		break;
	case 3: 
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Update();
		break;

	case 4:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Delete();
		break;

	case 5:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			showAll();
		break;
	}

	system("cls");
}

void UsersMenu::Login()
{
	std::string password;
	std::string email;

	char key;

	do
	{
		email.clear();
		password.clear();

		try
		{
			system("cls");

			std::cout << "Email: " << std::endl;
			std::cout << "Password: " << std::endl;

			gotoXY(7, 0); getline(std::cin, email);

			gotoXY(10, 1); 
			for (int i = 0; i < 301; i++)
			{
				key = getKeyPressed();
				if (key == 13)
					break;

				password += key; 
				_putch('*');
			}

			uM->loginUser(email, password);
			structure::currentUserG = uM->m_usersStore.getUserByEmail(email);

			break;
		}
		catch (std::exception& e)
		{
			std::cout << std::endl;
			std::string str = e.what();
			if (str.compare("index out of range") == 0)
				std::cout << "Wrong email: " << email << std::endl;
			else
				std::cerr << e.what() << std::endl;
			Sleep(1000);
		}
	} while (true);

	std::cout << "\nYou've successfully logged in. Enjoy ;)" << std::endl;
	Sleep(1000);

	system("cls");
}

void UsersMenu::Create()
{
	pm::dal::UsersStore::USER usr;
	int key;

	do
	{
		system("cls");

		try
		{
			std::cout << "First name: " << std::endl;
			std::cout << "Last name: " << std::endl;
			std::cout << "Email: " << std::endl;
			std::cout << "Age: " << std::endl;
			std::cout << "Enter password: " << std::endl;
			if (structure::currentUserG.admin == 1) 
				std::cout << "Admin (1 - true / 0 - false) -> " << std::endl;

			gotoXY(12, 0); getline(std::cin, usr.firstName);
			gotoXY(11, 1); getline(std::cin, usr.lastName);
			gotoXY(7, 2); getline(std::cin, usr.email);
			gotoXY(5, 3); std::cin >> usr.age; std::cin.ignore();
			gotoXY(16, 4);

			for (int i = 0; i < 301; i++)
			{
				key = getKeyPressed();
				if (key == 13)
					break;

				usr.password += key;
				_putch('*');
			}

			if (structure::currentUserG.admin == 1)
			{
				while (true)
				{
					gotoXY(34, 5); std::cin >> usr.admin;

					if (usr.admin >= 0 && usr.admin <= 1)
						break;
					else
						std::cout << "Wrong input! ;(" << std::endl;

					continue;
				}
			}

			uM->registerUser(usr);
			users = uM->getRegisteredUsers();

			break;
		}
		catch (std::string& errorMsg)
		{
			std::cout << std::endl;
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}

	} while (true);

	std::cout << "\nYou've successfully created an account!" << std::endl;
	Sleep(1000);

	system("cls");

	std::cin.ignore();
}

void UsersMenu::showAll()
{
	std::string separator = (horizontal) ? " " : "\r\n";

	int key;

	do
	{
		clearScreen();

		for (size_t i = 0; i < users.size(); i++)
		{
			gotoXY(5, i + 1);

			std::cout << users[i].id << ". " << users[i].firstName << " " << users[i].lastName << ", " << users[i].email << ", " << users[i].age << ", " << users[i].createdOn.day << "/" << users[i].createdOn.month << "/" << users[i].createdOn.year;

			std::string isAdminOut = (users[i].admin == 0) ? "| Admin: NO" : "| Admin: YES";
			std::cout << isAdminOut << separator;
		}

		key = getKeyPressed();

	} while (key != 27);

	system("cls");
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

			system("cls");

			int n1 = random_in_range(1, 99);
			int tmp = random_in_range(1, 99);

			int n2 = (tmp != n1) ? tmp : rand();
			int res;

			std::cout << n1 << " + " << n2 << " = "; std::cin >> res;

			if (res != (n1 + n2)) {
				std::cout << "Wrong answer :(" << std::endl;
				Sleep(1000);
				continue;
			}

			uM->removeUser(users[selectedUser].id);
			users = uM->getRegisteredUsers();
			break;
		} // switch
	} while (key != 27);

	system("cls");
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

			writeInColor(6, "Don't type anything if you don't want to update the field! If you don't want to update the age field, enter 0!\n");
			writeInColor(6, "IMPORTANT: Admin field is mandatory and you are required to enter a value there!");

			writeInColor(7, "");
			gotoXY(0, 3); std::cout << "First name: " << std::endl;
			gotoXY(0, 4); std::cout << "Last name: " << std::endl;
			gotoXY(0, 5); std::cout << "Email: " << std::endl;
			gotoXY(0, 6); std::cout << "Age: " << std::endl;
			gotoXY(0, 7); std::cout << "Enter password: " << std::endl;
			gotoXY(0, 8); std::cout << "Admin (1 - yes \\ 0 - no) -> ";

			pm::dal::UsersStore::USER temp = uM->m_usersStore.getUserById(users[selectedUser].id);

			gotoXY(12, 3); getline(std::cin, usr.firstName);
			gotoXY(11, 4); getline(std::cin, usr.lastName);
			gotoXY(7, 5); getline(std::cin, usr.email);
			gotoXY(5, 6); std::cin >> usr.age; std::cin.ignore();
			gotoXY(16, 7);

			for (int i = 0; i < 301; i++)
			{
				key = getKeyPressed();
				if (key == 13)
					break;

				usr.password += key;
				_putch('*');
			}

			unsigned short admin = 0;

			if (admin <= 1 && admin >= 0)
			{
				gotoXY(28, 8); std::cin >> usr.admin;
			}
			else
				continue;

			if (usr.firstName.empty())
				usr.firstName = temp.firstName;
			if (usr.lastName.empty())
				usr.lastName = temp.lastName;
			if (usr.email.empty())
				usr.email = temp.email;
			if (usr.age == 0)
				usr.age = temp.age;
			if (usr.password.empty())
				usr.password = temp.password;

			uM->updateUser(users[selectedUser].id, usr);

			users = uM->getRegisteredUsers();
			break;
		}
		} // switch
	} while (key != 27);

	system("cls");

	std::cin.ignore();
}
/*UsersMenu*/

/*TeamsMenu*/
TeamsMenu::TeamsMenu(pm::bll::TeamsManagement* be) : SubMenu("Teams", false, false, false, nullptr, be, nullptr, nullptr), selectedTeam(0)
{
	teams.clear();
	users.clear();

	try
	{
		teams = tM->loadTeams();
	}
	catch (std::string& error)
	{
		std::cout << error << std::endl;
	}
}

void TeamsMenu::moveToTeam(bool next)
{
	size_t oldSelectedItem = selectedTeam;

	if (next && (selectedTeam < teams.size() - 1))
		selectedTeam++;

	if (!next && (selectedTeam > 0))
		selectedTeam--;

	if (selectedTeam != oldSelectedItem)
	{
		gotoXY(8, oldSelectedItem + 1);
		for (size_t i = 0; i < selectedItemMarker.length(); i++)
			std::cout << ' ';

		gotoXY(8, selectedTeam + 1);
		std::cout << selectedItemMarker;
	}
}

void TeamsMenu::runItem()
{
	std::string errorMsg;

	if (structure::currentUserG.firstName == "") {
		errorMsg = "You must log in first!";
	}
	else if (structure::currentUserG.admin != 1) {
		errorMsg = "You must be admin!";
	}

	switch (selectedItem + 1)
	{
	case 1: Create();
		break;
	case 2:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Update();
		break;

	case 3:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Delete();
		break;

	case 4:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			showAll();
		break;

	case 5:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			AddUser();
		break;

	case 6:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			RemoveUser();
		break;
	}

	system("cls");
}

void TeamsMenu::Create()
{
	pm::dal::TeamsStore::TEAM team;

	std::cout << "Team's name: ";
	gotoXY(14, 0); getline(std::cin, team.title);

	tM->createTeam(team, structure::currentUserG.id);
	teams = tM->loadTeams();
	system("cls");

	std::cin.ignore();
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

			std::cout << "Team's name: ";
			gotoXY(14, 0); std::getline(std::cin, t.title);

			tM->updateTeam(teams[selectedTeam].id, t, structure::currentUserG.id);
			teams = tM->loadTeams();

			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
	std::cin.ignore();
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

			int n1 = random_in_range(1, 99);
			int tmp = random_in_range(1, 99);

			int n2 = (tmp != n1) ? tmp : rand();
			int res;

			std::cout << n1 << " + " << n2 << " = "; std::cin >> res;

			if (res != (n1 + n2)) {
				std::cout << "Wrong answer :(" << std::endl;
				Sleep(1000);
				continue;
			}

			tM->removeTeam(teams[selectedTeam].id);
			teams = tM->loadTeams();

			break;
		}
		} // switch
	} while (key != 27);
	system("cls");
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

			size_t counter = 0;
			std::vector<pm::dal::UsersStore::USER> usersTeam = tM->getUsersFromTeam(selectedTeam + 1);

			std::string query = "SELECT * FROM Users";
			nanodbc::result res = db.getResultFromSelect(query);

			while (res.next())
			{
				std::cout << "             " << res.get<int>(0) << ". " << res.get<std::string>(1) << " " << res.get<std::string>(2) << ", " << res.get<std::string>(3) << ", " << res.get<int>(4) << ", " << res.get<nanodbc::date>(6).day << "/" << res.get<nanodbc::date>(6).month << "/" << res.get<nanodbc::date>(6).year << "| Admin: ";

				std::string end = (res.get<int>(7) == 1) ? "Yes" : "No";
				
				bool flag = false;
				for (const auto& x : usersTeam)
				{
					if (res.get<int>(0) == x.id)
						flag = true;
				}

				if(flag)
					end += " | (In the team)\n";
				else
					end += " | (Not in the team)\n";

				std::cout << end;

				counter++;
			}

			int choice;
			std::cout << "\n\nEnter the id of the user who you would like to add to the current team: "; std::cin >> choice; std::cin.ignore();

			bool flag = false;

			for (const auto& x : usersTeam)
			{
				if (x.id == choice)
					flag = true;
			}

			if (!flag) {
				tM->addUserToTeam(teams[selectedTeam].id, choice);
				teams = tM->loadTeams();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to add a user who is already in the team!";
				usersTeam.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
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

			size_t counter = 0;
			std::vector<pm::dal::UsersStore::USER> usersTeam = tM->getUsersFromTeam(selectedTeam + 1);

			if (usersTeam.empty()) {
				std::cout << "There are no users in the current team";
				Sleep(1000);
				break;
			}

			for (const auto& x : usersTeam)
			{
				std::cout << "             " << x.id << ". " << x.firstName << " " << x.lastName << ", " << x.email << ", " << x.age << ", " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year << "| Admin: ";

				std::string end = (x.admin == 1) ? "Yes | (In Team)\n" : "No | (In Team)\n";
				std::cout << end;
			}

			int choice;
			std::cout << "\n\nEnter the id of the user who you would like to remove from the current team: "; std::cin >> choice; std::cin.ignore();

			bool flag = false;

			for (const auto& x : usersTeam)
			{
				if (x.id == choice)
					flag = true;
			}

			if (flag) {
				tM->removeUserFromTeam(teams[selectedTeam].id, choice);
				teams = tM->loadTeams();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to remove a user who isn't in the team!";
				usersTeam.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TeamsMenu::showAll()
{
	selectedTeam = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	gotoXY(20, 0); std::cout << "Press enter on the selected team to view the users in it";
	Sleep(2000);

	do
	{
		system("cls");

		for (size_t i = 0; i < teams.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTeam)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << teams[i].title << " " << teams[i].createdOn.day << "/" << teams[i].createdOn.month << "/" << teams[i].createdOn.year << std::endl;
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

			if (usersTeam.empty()) {
				std::cout << "There are no users in the current team";
				Sleep(1000);
				break;
			}

			std::cout << std::endl;
			for (const auto& x : usersTeam)
			{
				std::cout << "          ";
				std::cout << x.id << ". " << x.firstName << " " << x.lastName << ", " << x.email << ", " << x.age << ", " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year;

				std::string isAdminOut = (x.admin == 0) ? "| Admin: NO" : "| Admin: YES";
				std::cout << isAdminOut << separator;
			}

			if (_getch() == 27)
				break;
		}
		} // switch
	} while (key != 27);

	system("cls");
}
/*TeamsMenu*/

/*ProjectsMenu*/
ProjectsMenu::ProjectsMenu(pm::bll::ProjectsManagement* be) : SubMenu("Projects", false, false, false, nullptr, nullptr, be, nullptr), selectedProject(0)
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

void ProjectsMenu::runItem()
{
	std::string errorMsg;

	if (structure::currentUserG.firstName == "")
		errorMsg = "You must log in first!";

	switch (selectedItem + 1)
	{
	case 1: Create();
		break;
	case 2:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Update();
		break;
	case 3:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Delete();
		break;
	case 4:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			showAll();
		break;

	case 5:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			AddTeam();
		break;

	case 6:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			RemoveTeam();
		break;
	}
	system("cls");
}

void ProjectsMenu::moveToProject(bool next, std::vector<pm::dal::ProjectsStore::PROJECT> tmp)
{
	size_t OldSelectedItem = selectedProject;

	if (next && (selectedProject < tmp.size() - 1))
		selectedProject++;

	if (!next && (selectedProject > 0))
		selectedProject--;

	if (selectedProject != OldSelectedItem)
	{
		gotoXY(5, OldSelectedItem + 1);
		for (size_t j = 0; j < selectedItemMarker.length(); j++)
			std::cout << ' ';
		gotoXY(5, selectedProject + 1);
		std::cout << selectedItemMarker;
	}
}

void ProjectsMenu::Create()
{
	pm::dal::ProjectsStore::PROJECT project;

	std::cout << "Enter project's name: " << std::endl;
	std::cout << "Enter a description of the project: " << std::endl;

	gotoXY(22, 0); std::getline(std::cin, project.title);
	gotoXY(36, 1); std::getline(std::cin, project.description);

	pM->createProject(project, structure::currentUserG.id);
	projects = pM->loadAllProjects();

	system("cls");
	std::cin.ignore();
}

void ProjectsMenu::Update()
{
	selectedProject = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	std::vector<pm::dal::ProjectsStore::PROJECT> projects_user;

	do
	{
		system("cls");

		if (structure::currentUserG.admin == 1)
		{
			for (size_t i = 0; i < projects.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedProject)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << projects[i].title << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.day << std::endl;
			}
		}
		else {
			projects_user.clear();

			for (const auto& x : projects)
			{
				if (x.creatorId == structure::currentUserG.id)
					projects_user.push_back({ x.id, x.title, x.description, x.createdOn, x.creatorId });
			}

			for (size_t i = 0; i < projects_user.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedProject)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << projects_user[i].title << " " << projects_user[i].createdOn.day << "/" << projects_user[i].createdOn.month << "/" << projects_user[i].createdOn.day << std::endl;
			}

			if (projects_user.empty()) {
				system("cls");
				std::cout << "There are no projects created by you that you can update" << std::endl;
				Sleep(1500);
				break;
			}
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			if (structure::currentUserG.admin == 1)
				moveToProject(false, projects);
			else
				moveToProject(false, projects_user);
			break;
		case 75:if (horizontal)
			if (structure::currentUserG.admin == 1)
				moveToProject(false, projects);
			else
				moveToProject(false, projects_user);
			break;
		case 80:if (!horizontal)
			if (structure::currentUserG.admin == 1)
				moveToProject(true, projects);
			else
				moveToProject(true, projects_user);
			break;
		case 77:if (horizontal)
			if (structure::currentUserG.admin == 1)
				moveToProject(true, projects);
			else
				moveToProject(true, projects_user);
			break;
		case 13:
		{
			system("cls");

			pm::dal::ProjectsStore::PROJECT p;

			std::cout << "Enter project's name: " << std::endl;
			std::cout << "Enter a description of the project: " << std::endl;

			gotoXY(22, 0); std::getline(std::cin, p.title);
			gotoXY(36, 1); std::getline(std::cin, p.description);

			if(structure::currentUserG.admin == 1)
				pM->updateProject(p, projects[selectedProject].id, structure::currentUserG.id);
			else
				pM->updateProject(p, projects_user[selectedProject].id, structure::currentUserG.id);
			projects = pM->loadAllProjects();

			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
	std::cin.ignore();
}

void ProjectsMenu::Delete()
{
	selectedProject = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	std::vector<pm::dal::ProjectsStore::PROJECT> projects_user;

	do
	{
		system("cls");

		if (structure::currentUserG.admin == 1)
		{
			for (size_t i = 0; i < projects.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedProject)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << projects[i].title << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.day << std::endl;
			}
		}
		else {
			projects_user.clear();

			for (const auto& x : projects)
			{
				if (x.creatorId == structure::currentUserG.id)
					projects_user.push_back({ x.id, x.title, x.description, x.createdOn, x.creatorId });
			}


			for (size_t i = 0; i < projects_user.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedProject)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << projects_user[i].title << " " << projects_user[i].createdOn.day << "/" << projects_user[i].createdOn.month << "/" << projects_user[i].createdOn.day << std::endl;
			}

			if (projects_user.empty()) {
				system("cls");
				std::cout << "There are no projects created by you that you can delete" << std::endl;
				Sleep(1500);
				break;
			}
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
					if(structure::currentUserG.admin == 1)
						moveToProject(false, projects);
					else
						moveToProject(false, projects_user);
			break;
		case 75:if (horizontal)
					if (structure::currentUserG.admin == 1)
						moveToProject(false, projects);
					else
						moveToProject(false, projects_user);
			break;
		case 80:if (!horizontal)
					if (structure::currentUserG.admin == 1)
						moveToProject(true, projects);
					else
						moveToProject(true, projects_user);
			break;
		case 77:if (horizontal)
					if (structure::currentUserG.admin == 1)
						moveToProject(true, projects);
					else
						moveToProject(true, projects_user);
			break;
		case 13:
		{
			system("cls");

			int n1 = random_in_range(1, 99);
			int tmp = random_in_range(1, 99);

			int n2 = (tmp != n1) ? tmp : rand();
			int res;

			std::cout << n1 << " + " << n2 << " = "; std::cin >> res;

			if (res != (n1 + n2)) {
				std::cout << "Wrong answer :(" << std::endl;
				Sleep(1000);
				continue;
			}
			
			if(structure::currentUserG.admin == 1)
				pM->removeProject(projects[selectedProject].id);
			else
				pM->removeProject(projects_user[selectedProject].id);

			projects = pM->loadAllProjects();
			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void ProjectsMenu::showAll()
{
	selectedProject = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	pm::dal::TasksStore tmp_taskStore;
	

	gotoXY(8, 0); std::cout << "Press enter on the selected project to see detailed info about it";
	Sleep(1000);

	do
	{
		system("cls");

		for (size_t i = 0; i < projects.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedProject)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << projects[i].title << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.year << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToProject(false, projects);
			break;
		case 75:if (horizontal)
			moveToProject(false, projects);
			break;
		case 80:if (!horizontal)
			moveToProject(true, projects);
			break;
		case 77:if (horizontal)
			moveToProject(true, projects);
			break;
		case 13:
		{
			system("cls");

			gotoXY(35, 9);  std::cout << "Project's Description";
			gotoXY(35, 10); std::cout << "Teams In The Project";
			gotoXY(35, 11); std::cout << "View assigned tasks";

			gotoXY(32, 9); std::cout << "-> ";

			int y = 9, choice = 1, key = 0;

			while (key != 27)
			{
				key = getKeyPressed();

				if (key == 72 && y != 9)
				{
					gotoXY(32, y); std::cout << "   ";
					y--;
					gotoXY(32, y); std::cout << "-> ";
					choice--;

					continue;
				}

				if (key == 80 && y != 11)
				{
					gotoXY(32, y); std::cout << "   ";
					y++;
					gotoXY(32, y); std::cout << "-> ";
					choice++;

					continue;
				}

				if (key == 13)
				{
					switch (choice)
					{
					case 1:
						system("cls");

						std::cout << "         Description: " << projects[selectedProject].description << std::endl;

						system("pause>nul");

						break;
					case 2: {
						while (true)
						{
							system("cls");
							std::vector<pm::dal::TeamsStore::TEAM> teamsProject;

							teamsProject = pM->getTeamFromProject(projects[selectedProject].id);

							if (teamsProject.empty()) {
								std::cout << "There are no teams in the current project";
								Sleep(1000);
								break;
							}

							std::cout << std::endl;
							for (const auto& x : teamsProject)
							{
								std::cout << "          ";
								std::cout << x.id << ". " << x.title << " " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year << std::endl;
							}

							if (_getch() == 27)
								break;
						}

						break;
					}
					case 3: {
						system("cls");

						std::string query = "SELECT Tasks.Id, Tasks.Title, Tasks.[Description], Tasks.[Status], Tasks.CreatedOn, Tasks.CreatorId FROM ProjectsAndTasks, Projects, Tasks WHERE (Projects.Id = ProjectId) AND (TaskId = Tasks.Id) AND (ProjectId = " + std::to_string(selectedProject + 1) + ")";

						nanodbc::result res = db.getResultFromSelect(query);

						unsigned short i = 0;
						while (res.next())
						{
							gotoXY(5, i + 1);

							std::cout << res.get<int>(0) << ". " << res.get<std::string>(1) << " | Status: " << res.get<std::string>(3) << " | " << res.get<nanodbc::date>(4).day << "/" << res.get<nanodbc::date>(4).month << "/" << res.get<nanodbc::date>(4).year << " | Created by: ";

							nanodbc::result users_res = db.getResultFromSelect("SELECT Id, FirstName, LastName FROM Users");
							std::pair<std::string, std::string> name;

							while (users_res.next())
							{
								if (users_res.get<int>(0) == res.get<int>(5))
								{
									name.first = users_res.get<std::string>(1);
									name.second = users_res.get<std::string>(2);
								}
							}

							std::cout << name.first << " " << name.second << std::endl;

							i++;
						}

						if (i < 1) {
							std::cout << "There are no tasks assigned to the current project" << std::endl;
							Sleep(1500);
							break;
						}

						if(_getch() == 27)
							break;
					}
					}

					break;
				}
			}
			
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void ProjectsMenu::AddTeam()
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << projects[i].title << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToProject(false, projects);
			break;
		case 75:if (horizontal)
			moveToProject(false, projects);
			break;
		case 80:if (!horizontal)
			moveToProject(true, projects);
			break;
		case 77:if (horizontal)
			moveToProject(true, projects);
			break;
		case 13:
		{
			system("cls");

			size_t counter = 0;
			std::vector<pm::dal::TeamsStore::TEAM> teamsProject = pM->getTeamFromProject(selectedProject + 1);
			std::vector<int> ids;

			std::string query = "SELECT * FROM Teams";
			nanodbc::result res = db.getResultFromSelect(query);

			while (res.next())
			{
				std::cout << "             " << res.get<int>(0) << ". " << res.get<std::string>(1) << " " << res.get<nanodbc::date>(2).day << "/" << res.get<nanodbc::date>(2).month << " " << res.get<nanodbc::date>(2).year;

				bool flag = false;
				for (const auto& x : teamsProject)
				{
					if (res.get<int>(0) == x.id)
						flag = true;
				}

				std::string end;

				if (flag)
					end += " | (In the project)\n";
				else
					end += " | (Not in the project)\n";

				std::cout << end;

				counter++;
				ids.push_back(res.get<int>(0));
			}

			if (counter == 0)
			{
				std::cout << "There are no created teams" << std::endl;
				Sleep(1000);
				
				continue;
			}

			size_t choice;
			std::cout << "\n\nEnter the id of the team who you would like to add to the current project: "; std::cin >> choice; std::cin.ignore();

			bool inTeam = false, invalid = true;

			for (const auto& x : ids)
			{
				if (x == choice)
					invalid = false;
			}

			if (invalid)
			{
				system("cls");
				std::cout << "Invalid id" << std::endl;
				Sleep(1000);
				continue;
			}

			for (const auto& x : teamsProject)
			{
				if (x.id == choice)
					inTeam = true;
			}

			if (!inTeam) {
				pM->assignTeamToProject(projects[selectedProject].id, choice);
				projects = pM->loadAllProjects();
				break;
			}
			else {
				system("cls");
				gotoXY(17, 0); std::cout << "You are trying to add a team who is already in the project";
				teamsProject.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void ProjectsMenu::RemoveTeam()
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << projects[i].title << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToProject(false, projects);
			break;
		case 75:if (horizontal)
			moveToProject(false, projects);
			break;
		case 80:if (!horizontal)
			moveToProject(true, projects);
			break;
		case 77:if (horizontal)
			moveToProject(true, projects);
			break;
		case 13:
		{
			system("cls");

			size_t counter = 0;
			std::vector<pm::dal::TeamsStore::TEAM> teamsProject = pM->getTeamFromProject(selectedProject + 1);

			if (teamsProject.empty()) {
				std::cout << "There are no teams in the current project";
				Sleep(1000);
				break;
			}

			for (const auto& x : teamsProject)
			{
				std::cout << "             " << x.id << ". " << x.title << " " << x.createdOn.day << "/" << x.createdOn.month << " " << x.createdOn.year << " | (In Team)" << std::endl;
			}

			int choice;
			std::cout << "\n\nEnter the id of the team which you would like to remove from the current project: "; std::cin >> choice; std::cin.ignore();

			bool flag = false;

			for (const auto& x : teamsProject)
			{
				if (x.id == choice)
					flag = true;
			}

			if (flag) {
				pM->removeTeamFromProject(projects[selectedProject].id, choice);
				projects = pM->loadAllProjects();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to remove a team which isn't in the project!";
				teamsProject.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
}
/*ProjectsMenu*/

/*TasksMenu*/
TasksMenu::TasksMenu(pm::bll::TasksManagement* be) : SubMenu("Tasks", false, false, false, nullptr, nullptr, nullptr, be), selectedTask(0)
{
	tasks.clear();

	try
	{
		tasks = taskM->loadAllTasks();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void TasksMenu::runItem()
{
	std::string errorMsg;

	if (structure::currentUserG.firstName == "")
		errorMsg = "You must log in first!";

	switch (selectedItem + 1)
	{
	case 1: Create();
		break;
	case 2:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Update();
		break;
	case 3:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			Delete();
		break;
	case 4:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			showAll();
		break;

	case 5:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			AssignMenu();

		break;

	case 6:
		if (!errorMsg.empty()) {
			std::cout << errorMsg << std::endl;
			Sleep(1000);
		}
		else
			RemoveMenu();

		break;
	}
	system("cls");
}

void TasksMenu::moveToTask(bool next, std::vector<pm::dal::TasksStore::TASK> tmp)
{
	size_t OldSelectedItem = selectedTask;

	if (next && (selectedTask < tmp.size() - 1))
		selectedTask++;

	if (!next && (selectedTask > 0))
		selectedTask--;

	if (selectedTask != OldSelectedItem)
	{
		gotoXY(5, OldSelectedItem + 1);
		for (size_t j = 0; j < selectedItemMarker.length(); j++)
			std::cout << ' ';
		gotoXY(5, selectedTask + 1);
		std::cout << selectedItemMarker;
	}
}

void TasksMenu::Create()
{
	pm::dal::TasksStore::TASK task;

	std::cout << "Enter tasks's name: " << std::endl;
	std::cout << "Enter a description of the project: " << std::endl;

	gotoXY(20, 0); std::getline(std::cin, task.title);
	gotoXY(36, 1); std::getline(std::cin, task.description);

	task.status = "Pending";

	taskM->createTask(task, structure::currentUserG.id);
	tasks = taskM->loadAllTasks();

	system("cls");
	std::cin.ignore();
}

void TasksMenu::Update()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	std::vector<pm::dal::TasksStore::TASK> tasks_user;

	do
	{
		system("cls");

		if (structure::currentUserG.admin == 1)
		{
			for (size_t i = 0; i < tasks.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedTask)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.day << std::endl;
			}
		}
		else {
			tasks_user.clear();

			for (const auto& x : tasks)
			{
				if (x.creatorId == structure::currentUserG.id)
					tasks.push_back({ x.id, x.title, x.description, x.status, x.createdOn, x.creatorId });
			}

			for (size_t i = 0; i < tasks_user.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedTask)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << tasks_user[i].title << " | Status: " << tasks_user[i].status << " | " << tasks_user[i].createdOn.day << "/" << tasks_user[i].createdOn.month << "/" << tasks_user[i].createdOn.day << std::endl;
			}

			if (tasks_user.empty()) {
				system("cls");
				std::cout << "There are no tasks created by you that you can update" << std::endl;
				Sleep(1500);
				break;
			}
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(false, tasks);
			else
				moveToTask(false, tasks_user);
			break;
		case 75:if (horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(false, tasks);
			else
				moveToTask(false, tasks_user); 
			break;
		case 80:if (!horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(true, tasks);
			else
				moveToTask(true, tasks_user);
			break;
		case 77:if (horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(true, tasks);
			else
				moveToTask(true, tasks_user); 
			break;
		case 13:
		{
			system("cls");
			while (true)
			{
				system("cls");
				pm::dal::TasksStore::TASK t;

				std::cout << "Enter task's name: " << std::endl;
				std::cout << "Enter a description of the task: " << std::endl;
				std::cout << "Enter status of the task (Pending/InProgress/Completed): " << std::endl;

				gotoXY(20, 0); std::getline(std::cin, t.title);
				gotoXY(34, 1); std::getline(std::cin, t.description);
				gotoXY(57, 2);

				std::string status_temp; std::getline(std::cin, status_temp);

				if (status_temp.compare("Pending") == 0 || status_temp.compare("InProgress") == 0 || status_temp.compare("Completed") == 0)
					t.status = status_temp;
				else {
					std::cout << "\n The status you've entered doesn't meet the requirements" << std::endl;
					Sleep(1500);
					continue;
				}


				if (structure::currentUserG.admin == 1)
					taskM->updateTask(t, tasks[selectedTask].id, structure::currentUserG.id);
				else
					taskM->updateTask(t, tasks_user[selectedTask].id, structure::currentUserG.id);

				tasks = taskM->loadAllTasks();
				break;
			}
			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
	std::cin.ignore();
}

void TasksMenu::Delete()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	std::vector<pm::dal::TasksStore::TASK> tasks_user;

	do
	{
		system("cls");

		if (structure::currentUserG.admin == 1)
		{
			for (size_t i = 0; i < tasks.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedTask)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.day << std::endl;
			}
		}
		else {
			tasks_user.clear();

			for (const auto& x : tasks)
			{
				if (x.creatorId == structure::currentUserG.id)
					tasks.push_back({ x.id, x.title, x.description, x.status, x.createdOn, x.creatorId });
			}

			for (size_t i = 0; i < tasks_user.size(); i++)
			{
				gotoXY(5, i + 1);
				if (i == selectedTask)
					std::cout << selectedItemMarker;
				else
					for (size_t c = 0; c < selectedItemMarker.size(); c++)
						std::cout << ' ';

				std::cout << tasks_user[i].title << " | Status: " << tasks_user[i].status << " | " << tasks_user[i].createdOn.day << "/" << tasks_user[i].createdOn.month << "/" << tasks_user[i].createdOn.day << std::endl;
			}

			if (tasks_user.empty()) {
				system("cls");
				std::cout << "There are no tasks created by you that you can delete" << std::endl;
				Sleep(1500);
				break;
			}
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(false, tasks);
			else
				moveToTask(false, tasks_user);
			break;
		case 75:if (horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(false, tasks);
			else
				moveToTask(false, tasks_user);
			break;
		case 80:if (!horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(true, tasks);
			else
				moveToTask(true, tasks_user);
			break;
		case 77:if (horizontal)
			if (structure::currentUserG.admin == 1)
				moveToTask(true, tasks);
			else
				moveToTask(true, tasks_user);
			break;
		case 13:
		{
			system("cls");
			while (true)
			{
				if (structure::currentUserG.admin == 1)
					taskM->deleteTask(tasks[selectedTask].id);
				else
					taskM->deleteTask(tasks_user[selectedTask].id);

				tasks = taskM->loadAllTasks();
				break;
			}
			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TasksMenu::showAll()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	pm::dal::TasksStore tmp_taskStore;


	gotoXY(8, 0); std::cout << "Press enter on the selected task to see detailed info about it";
	Sleep(1000);

	do
	{
		system("cls");

		for (size_t i = 0; i < tasks.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTask)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.year << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTask(false, tasks);
			break;
		case 75:if (horizontal)
			moveToTask(false, tasks);
			break;
		case 80:if (!horizontal)
			moveToTask(true, tasks);
			break;
		case 77:if (horizontal)
			moveToTask(true, tasks);
			break;
		case 13:
		{
			system("cls");

			gotoXY(35, 9);  std::cout << "Task's Description";
			gotoXY(35, 10); std::cout << "Projects to which the task has been asssigned";
			gotoXY(35, 11); std::cout << "Users to whom the tasks has been assigned";

			gotoXY(32, 9); std::cout << "-> ";

			int y = 9, choice = 1, key;

			while (true)
			{
				key = getKeyPressed();

				if (key == 72 && y != 9)
				{
					gotoXY(32, y); std::cout << "   ";
					y--;
					gotoXY(32, y); std::cout << "-> ";
					choice--;

					continue;
				}

				if (key == 80 && y != 11)
				{
					gotoXY(32, y); std::cout << "   ";
					y++;
					gotoXY(32, y); std::cout << "-> ";
					choice++;

					continue;
				}

				if (key == 13)
				{
					switch (choice)
					{
					case 1:
						system("cls");

						std::cout << "         Description: " << tasks[selectedTask].description << std::endl;

						system("pause>nul");

						break;
					case 2: {
						while (true)
						{
							system("cls");
							std::vector<pm::dal::ProjectsStore::PROJECT> projectsTask;

							projectsTask = taskM->getProjectsFromTask(tasks[selectedTask].id);

							if (projectsTask.empty()) {
								std::cout << "There are no projects to which the current task has been assigned";
								Sleep(1000);
								break;
							}

							std::cout << std::endl;
							for (const auto& x : projectsTask)
							{
								std::cout << "          ";
								std::cout << x.id << ". " << x.title << " | " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year << std::endl;
							}

							if (_getch() == 27)
								break;
						}

						break;
					}
					case 3: {
						system("cls");

						std::vector<pm::dal::UsersStore::USER> task_users;
						task_users = taskM->getUsersFromTask(tasks[selectedTask].id);

						std::cout << std::endl;
						
						unsigned short i = 0;

						for (const auto& x : task_users)
						{
							std::cout << "          ";
							std::cout << x.id << ". " << x.firstName << " " << x.lastName << " | " << x.email << " " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year << std::endl;

							i++;
						}

						if (i < 1) {
							std::cout << "The task has no assigned users to it" << std::endl;
							Sleep(1500);
							break;
						}

						if (_getch() == 27)
							break;
					}
					}

					break;
				}
			}

		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TasksMenu::AddProject()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < tasks.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTask)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTask(false, tasks);
			break;
		case 75:if (horizontal)
			moveToTask(false, tasks);
			break;
		case 80:if (!horizontal)
			moveToTask(true, tasks);
			break;
		case 77:if (horizontal)
			moveToTask(true, tasks);
			break;
		case 13:
		{
			system("cls");

			size_t counter = 0;
			std::vector<pm::dal::ProjectsStore::PROJECT> projectsTask = taskM->getProjectsFromTask(selectedTask + 1);

			std::string query = "SELECT * FROM Projects";
			nanodbc::result res = db.getResultFromSelect(query);
			std::vector<int> ids;

			while (res.next())
			{
				std::cout << "             " << res.get<int>(0) << ". " << res.get<std::string>(1) << " " << res.get<nanodbc::date>(3).day << "/" << res.get<nanodbc::date>(3).month << " " << res.get<nanodbc::date>(3).year;

				bool flag = false;
				for (const auto& x : projectsTask)
				{
					if (res.get<int>(0) == x.id)
						flag = true;
				}

				std::string end;

				if (flag)
					end += " | (Assigned to the current task)\n";
				else
					end += " | (Not assigned to the current task)\n";

				std::cout << end;

				counter++;
				ids.push_back(res.get<int>(0));
			}



			if (counter == 0)
			{
				std::cout << "There are no created projects" << std::endl;
				Sleep(1000);

				continue;
			}

			size_t choice;
			std::cout << "\n\nEnter the id of the project which you would like to add to the current task: "; std::cin >> choice; std::cin.ignore();

			bool inTask = false, invalid = true;

			for (const auto& x : ids)
			{
				if (x == choice)
					invalid = false;
			}

			if (invalid)
			{
				system("cls");
				std::cout << "Invalid id" << std::endl;
				Sleep(1000);
				continue;
			}

			for (const auto& x : projectsTask)
			{
				if (x.id == choice)
					inTask = true;
			}

			if (!inTask) {
				taskM->assignProjectToTask(choice, tasks[selectedTask].id);
				tasks = taskM->loadAllTasks();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to add a project who is already in the task!";
				projectsTask.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TasksMenu::RemoveProject()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < tasks.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTask)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTask(false, tasks);
			break;
		case 75:if (horizontal)
			moveToTask(false, tasks);
			break;
		case 80:if (!horizontal)
			moveToTask(true, tasks);
			break;
		case 77:if (horizontal)
			moveToTask(true, tasks);
			break;
		case 13:
		{
			system("cls");

			size_t counter = 0;
			std::vector<pm::dal::ProjectsStore::PROJECT> projectsTask = taskM->getProjectsFromTask(selectedTask + 1);

			if (projectsTask.empty()) {
				std::cout << "The task isn't assigned to any project";
				Sleep(1000);
				break;
			}

			for (const auto& x : projectsTask)
			{
				std::cout << "             " << x.id << ". " << x.title << " | " << x.createdOn.day << "/" << x.createdOn.month << " " << x.createdOn.year << " | (Assigned to the current task)" << std::endl;
			}

			int choice;
			std::cout << "\n\nEnter the id of the project which you would like to remove from the current task: "; std::cin >> choice; std::cin.ignore();

			bool flag = false;

			for (const auto& x : projectsTask)
			{
				if (x.id == choice)
					flag = true;
			}

			if (flag) {
				taskM->removeProjectFromTask(choice, tasks[selectedTask].id);
				tasks = taskM->loadAllTasks();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to remove a project which isn't assigned to the current task!";
				projectsTask.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TasksMenu::AssignUser()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < tasks.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTask)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTask(false, tasks);
			break;
		case 75:if (horizontal)
			moveToTask(false, tasks);
			break;
		case 80:if (!horizontal)
			moveToTask(true, tasks);
			break;
		case 77:if (horizontal)
			moveToTask(true, tasks);
			break;
		case 13:
		{
			system("cls");

			size_t counter = 0;
			std::vector<pm::dal::UsersStore::USER> usersTask = taskM->getUsersFromTask(selectedTask + 1);

			std::string query = "SELECT * FROM Users";
			nanodbc::result res = db.getResultFromSelect(query);
			std::vector<int> ids;

			while (res.next())
			{
				std::cout << "             " << res.get<int>(0) << ". " << res.get<std::string>(1) << " " << res.get<std::string>(2) << " | " << res.get<std::string>(3) << " | " << res.get<nanodbc::date>(6).day << "/" << res.get<nanodbc::date>(6).month << "/" << res.get<nanodbc::date>(6).year;

				bool flag = false;
				for (const auto& x : usersTask)
				{
					if (res.get<int>(0) == x.id)
						flag = true;
				}

				std::string end;

				if (flag)
					end += " | (Assigned to the current task)\n";
				else
					end += " | (Not assigned to the current task)\n";

				std::cout << end;

				counter++;
				ids.push_back(res.get<int>(0));
			}



			if (counter == 0)
			{
				std::cout << "There are no created users" << std::endl;
				Sleep(1000);

				continue;
			}

			size_t choice;
			std::cout << "\n\nEnter the id of the user who you would like to add to the current task: "; std::cin >> choice; std::cin.ignore();

			bool inTask = false, invalid = true;

			for (const auto& x : ids)
			{
				if (x == choice)
					invalid = false;
			}

			if (invalid)
			{
				system("cls");
				std::cout << "Invalid id" << std::endl;
				Sleep(1000);
				continue;
			}

			for (const auto& x : usersTask)
			{
				if (x.id == choice)
					inTask = true;
			}

			if (!inTask) {
				taskM->assignTaskToUser(tasks[selectedTask].id, choice);
				tasks = taskM->loadAllTasks();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to add a user who is already in the task!";
				usersTask.clear();
				Sleep(1000);
			}
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TasksMenu::RemoveUser()
{
	selectedTask = 0;
	std::string separator = (horizontal) ? " " : "\r\n";
	int key;

	do
	{
		system("cls");

		for (size_t i = 0; i < tasks.size(); i++)
		{
			gotoXY(5, i + 1);
			if (i == selectedTask)
				std::cout << selectedItemMarker;
			else
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << tasks[i].title << " | Status: " << tasks[i].status << " | " << tasks[i].createdOn.day << "/" << tasks[i].createdOn.month << "/" << tasks[i].createdOn.day << std::endl;
		}

		key = getKeyPressed();

		switch (key)
		{
		case 72:if (!horizontal)
			moveToTask(false, tasks);
			break;
		case 75:if (horizontal)
			moveToTask(false, tasks);
			break;
		case 80:if (!horizontal)
			moveToTask(true, tasks);
			break;
		case 77:if (horizontal)
			moveToTask(true, tasks);
			break;
		case 13:
		{
			system("cls");

			size_t counter = 0;
			std::vector<pm::dal::UsersStore::USER> usersTask = taskM->getUsersFromTask(selectedTask + 1);

			for (const auto& x : usersTask)
			{
				std::cout << "             " << x.id << ". " << x.firstName << " " << x.lastName << " | " << x.email << " | " << x.createdOn.day << "/" << x.createdOn.month << "/" << x.createdOn.year << " (Assigned to the current task) " << std::endl;

				counter++;
			}


			if (counter == 0)
			{
				std::cout << "There are no created users" << std::endl;
				Sleep(1000);

				continue;
			}

			size_t choice;
			std::cout << "\n\nEnter the id of the user who you would like to add to the current task: "; std::cin >> choice; std::cin.ignore();

			bool flag = false;
			for (const auto& x : usersTask)
			{
				if (choice == x.id)
					flag = true;
			}

			if (!flag)
			{
				system("cls");
				std::cout << "Invalid id";
				Sleep(1000);
				usersTask.clear();
				continue;
			}

			taskM->removeTaskFromUser(tasks[selectedTask].id, choice);
			tasks = taskM->loadAllTasks();
			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
}

void TasksMenu::AssignMenu()
{
	system("cls");

	gotoXY(40, 9);  std::cout << "Assign project";
	gotoXY(40, 10); std::cout << "Assign user";

	gotoXY(37, 9); std::cout << "-> ";

	int y = 9, choice = 1, key = 0;

	while (key != 27)
	{
		key = getKeyPressed();

		if (key == 72 && y != 9)
		{
			gotoXY(37, y); std::cout << "   ";
			y--;
			gotoXY(37, y); std::cout << "-> ";
			choice--;

			continue;
		}

		if (key == 80 && y != 10)
		{
			gotoXY(37, y); std::cout << "   ";
			y++;
			gotoXY(37, y); std::cout << "-> ";
			choice++;

			continue;
		}

		if (key == 13)
		{
			switch (choice)
			{
			case 1:
				AddProject();
				break;
			case 2: 
				AssignUser();
				break;
			}

			break;
		}
	}
}

void TasksMenu::RemoveMenu()
{
	system("cls");

	gotoXY(40, 9);  std::cout << "Remove project";
	gotoXY(40, 10); std::cout << "Unassign user";

	gotoXY(37, 9); std::cout << "-> ";

	int y = 9, choice = 1, key = 0;

	while (key != 27)
	{
		key = getKeyPressed();

		if (key == 72 && y != 9)
		{
			gotoXY(37, y); std::cout << "   ";
			y--;
			gotoXY(37, y); std::cout << "-> ";
			choice--;

			continue;
		}

		if (key == 80 && y != 10)
		{
			gotoXY(37, y); std::cout << "   ";
			y++;
			gotoXY(37, y); std::cout << "-> ";
			choice++;

			continue;
		}

		if (key == 13)
		{
			switch (choice)
			{
			case 1:
				RemoveProject();
				break;
			case 2:
				RemoveUser();
				break;
			}

			break;
		}
	}

}
/*TasksMenu*/