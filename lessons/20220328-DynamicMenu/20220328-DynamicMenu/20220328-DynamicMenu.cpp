#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include "20220328-DynamicMenu.h"

using namespace std;

typedef void (*MenuHandler)(void*); //pointer to a function

struct MenuItem 
{
	char key;
	string text;
	MenuHandler handler;
};

void menuOption1(void* params)
{
	cout << "MenuOption1 handler: " << (char*)params << endl;
}

void menuOption2(void* params)
{
	cout << "MenuOption2 handler" << endl;
}

void menuOption3(void* params)
{
	cout << "MenuOption3 handler" << endl;
}

void displayMenu(std::vector<MenuItem>& menuItems)
{
	for (auto item : menuItems)
	{
		cout << item.key << ". " << item.text << endl;
	}

	cout << "Choose an option: ";
}

void HandleMenu(std::vector<MenuItem>& menuItems)
{
	char choice;
	char name[] = { "Ivan" };

	while (true)
	{
		displayMenu(menuItems);

		cin >> choice;

		auto it = find_if(menuItems.begin(), menuItems.end(),
			[&choice](const MenuItem& item) { return item.key == choice; });

		if (it != menuItems.end())
		{
			auto handler = (*it).handler;
			if (handler != NULL)
				handler(name);
			else
				break;
		}
		else {
			cout << "Invalid option!" << endl;
		}
	};
}

int main()
{
	vector<MenuItem> menuItems =
	{
		{ '1', "Start", menuOption1}, //we don't need &menuOption1 because it is done automatically
		{ '2', "Help", menuOption2},
		{ '9', "Exit", NULL},
	};

	HandleMenu(menuItems);
}