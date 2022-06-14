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