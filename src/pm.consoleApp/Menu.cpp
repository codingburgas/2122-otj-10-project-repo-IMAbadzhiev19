#include "Menu.h"

#include<iostream>
#include<windows.h>
#include<conio.h>

void MenuItem::gotoXY(int x, int y)
{
	COORD coords;
	coords.X = x;
	coords.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
}

int MenuItem::getKeyPressed()
{
	int key = _getch();

	if (key == 224)
		key = _getch();

	return key;
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
		gotoXY(submenu[oldSelectedItem]->position.column, submenu[oldSelectedItem]->position.column);

		for (size_t i = 0; i < selectedItemMarker.size(); i++)
			std::cout << ' ';

		gotoXY(submenu[selectedItem]->position.column, submenu[selectedItem]->position.row);
		std::cout << selectedItemMarker;
	}

}

void MenuItem::addItem(MenuItem* i, unsigned short row, unsigned short column)
{
	size_t length = submenu.size();

	if((row == 0) && (column == 0) && (length > 0))
		if (horizontal) {
			i->position.row = submenu[0]->position.row;
			i->position.column = submenu[length - 1]->position.column + submenu[length - 1]->name.size() + selectedItemMarker.size();
		}
		else {
			i->position.row = submenu[length - 1]->position.row + 1;
			i->position.column = submenu[0]->position.column;
		}
	else {
		i->position.row = row;
		i->position.column = column;
	}

	submenu.push_back(i);
}