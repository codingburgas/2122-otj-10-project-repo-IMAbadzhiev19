#include "Menu.h"

#include<iostream>
#include<windows.h>
#include<conio.h>

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

void MainMenu::Show()
{
	system("cls");
	MenuItem::Show();
}

/*SubMenu*/
SubMenu::SubMenu(std::string name, bool horizontal, bool execModule, bool users, pm::bll::UsersManagement* uMM, pm::bll::TeamsManagement* tMM ) : MenuItem(name, horizontal, execModule), uM(uMM), tM(tMM)
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
	case 5: Add();
		break;
	case 6: Remove();
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
UsersMenu::UsersMenu(pm::bll::UsersManagement* be) : SubMenu("Users", false, false, true, be, nullptr)
{
	users = uM->getRegisteredUsers();

	for (size_t i = 0; i < users.size(); i++)
		users_data.push_back({ (unsigned short)(++i), 10, users[i] });
}

void UsersMenu::runItem()
{
	switch (selectedItem + 1)
	{
	case 1: Login();
		break;
	case 2: Create();
		break;
	case 3: Update();
		break;
	case 4: Delete();
		break;
	case 5: showAll();
		break;
	case 6: Remove();
		break;
	}
}

void UsersMenu::Login()
{
	std::string email, password;

	try {
		getline(std::cin, email);
		getline(std::cin, password);

		uM->loginUser(email, password);
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

	try {
		std::cout << "Enter first name: "; getline(std::cin, usr.firstName);
		std::cout << "Enter last name: "; getline(std::cin, usr.lastName);
		std::cout << "Enter email: "; getline(std::cin, usr.email);
		std::cin >> usr.age; std::cin.ignore();
		std::cout << "Enter : "; getline(std::cin, usr.password);

		uM->registerUser(usr);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "You have successfully created an account";
	Sleep(1000);
}

/*UsersMenu*/