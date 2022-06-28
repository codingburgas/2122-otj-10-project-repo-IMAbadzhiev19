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

	if (!users) {

		std::string func;

		if (name.compare("Teams") == 0)
			func = " User";
		else if (name.compare("Projects") == 0)
			func = " Team";
		else if (name.compare("Tasks") == 0)
			func = " Project";

		itemData.push_back({ c++, 40, "Add" + func });
		itemData.push_back({ c++, 40, "Remove" + func });
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
			if (structure::currentUserG.admin == 1) std::cout << "Admin (1 - true / 0 - false) -> " << std::endl;

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

			std::cout << "First name: " << std::endl;
			std::cout << "Last name: " << std::endl;
			std::cout << "Email: " << std::endl;
			std::cout << "Age: " << std::endl;
			std::cout << "Enter password: " << std::endl;

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

			int admin = 0;

			if (structure::currentUserG.admin == 1) {
				std::cout << "\nAdmin (1 - yes \\ 0 - no) -> "; 
				std::cin >> admin;
			}

			usr.admin = admin;

			uM->updateUser(users[selectedUser].id, usr);

			users = uM->getRegisteredUsers();
			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
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

void ProjectsMenu::moveToProject(bool next)
{
	size_t OldSelectedItem = selectedProject;

	if (next && (selectedProject < projects.size() - 1))
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
				for (size_t c = 0; c < selectedItemMarker.size(); c++)
					std::cout << ' ';

			std::cout << projects[i].title << " " << projects[i].createdOn.day << "/" << projects[i].createdOn.month << "/" << projects[i].createdOn.day << std::endl;
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

			std::cout << "Enter project's name: " << std::endl;
			std::cout << "Enter a description of the project: " << std::endl;

			gotoXY(22, 0); std::getline(std::cin, p.title);
			gotoXY(36, 1); std::getline(std::cin, p.description);

			pM->updateProject(p, projects[selectedProject].id, structure::currentUserG.id);
			projects = pM->loadAllProjects();

			break;
		}
		} // switch
	} while (key != 27);

	system("cls");
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

			gotoXY(35, 9);  std::cout << "Project's Description";
			gotoXY(35, 10); std::cout << "Teams In The Project";

			gotoXY(32, 9); std::cout << "-> ";

			int y = 9, choice = 1, key;

			while (true)
			{
				key = getKeyPressed();

				if (key == 72 && y != 9)
				{
					gotoXY(32, 10); std::cout << "   ";
					choice--;
					gotoXY(32, 9); std::cout << "-> ";
					y--;

					continue;
				}

				if (key == 80 && y != 10)
				{
					gotoXY(32, 9); std::cout << "   ";
					choice++;
					gotoXY(32, 10); std::cout << "-> ";
					y++;

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
					case 2:
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

			size_t counter = 0;
			std::vector<pm::dal::TeamsStore::TEAM> teamsProject = pM->getTeamFromProject(selectedProject + 1);

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
			}

			if (counter == 0)
			{
				std::cout << "There are no created teams" << std::endl;
				Sleep(1000);
				
				continue;
			}

			size_t choice;
			std::cout << "\n\nEnter the id of the team who you would like to add to the current project: "; std::cin >> choice; std::cin.ignore();

			if (choice < 1 || choice > counter) {
				system("cls");
				gotoXY(45, 0); std::cout << "Invalid id" << std::endl;
				Sleep(1000);

				continue;
			}

			bool flag = false;

			for (const auto& x : teamsProject)
			{
				if (x.id == choice)
					flag = true;
			}

			if (!flag) {
				pM->assignTeamToProject(projects[selectedProject].id, choice);
				projects = pM->loadAllProjects();
				break;
			}
			else {
				system("cls");
				gotoXY(25, 0); std::cout << "You are trying to add a team who is already in the project!";
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
/*TasksMenu*/