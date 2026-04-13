#include "../classes/IBuilder.h"
#include <conio.h>
#include <iostream>

using namespace std;

void IBuilder::clearScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferInfo);

	DWORD cellCount = bufferInfo.dwSize.X * bufferInfo.dwSize.Y; // total cells
	DWORD written;
	COORD origin = { 0, 0 };

	FillConsoleOutputCharacter(hConsole, ' ', cellCount, origin, &written);

	FillConsoleOutputAttribute(hConsole, bufferInfo.wAttributes, cellCount, origin, &written);

	SetConsoleCursorPosition(hConsole, origin);
}

int IBuilder::_get_len_FromOptions() const { return options.size(); }


bool IBuilder::isResized()
{
	int dW = getConsoleWidth(); //changed width
	int dH = getConsoleHeight(); //changed height

	return dW != _prevWidth || dH != _prevHeight;
}

void IBuilder::updateSize()
{
	_prevHeight = getConsoleHeight();
	_prevWidth = getConsoleWidth();
}

//layout tools
int IBuilder::centerX(int width) const
{
	return (getConsoleWidth() - width) / 2;
}

int IBuilder::centerY(int height) const
{
	return (getConsoleHeight() - height) / 2;
}

float IBuilder::relativeX(float perc) const
{
	return static_cast<int>(getConsoleWidth() * perc);
}

float IBuilder::relativeY(float perc) const
{
	return static_cast<int>(getConsoleHeight() * perc);
}

void IBuilder::displayMenu(const vector<string>& menuData,
	int selection,
	int w,
	int h)
{

	int pos_X = w, pos_Y = h; //centered X, and Y

	if (isCentered) {
		pos_X = (w / 2);
		pos_Y = (h / 2) - 1;
	}
	for (int i = 0; i < menuData.size(); i++)
	{
		//if the centering is enabled, then the menu will print centered
		isCentered ? setCursorPosition(pos_X - (menuData[i].length() + 2) / 2,
			pos_Y + i) : setCursorPosition(0, i);
		if (i == selection) {
			s_TxtColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // red bg, white text
			cout << "[" << menuData[i] << "]";
			s_TxtColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset color
		}
		else {
			s_TxtColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << " " << menuData[i] << " ";
		}

	}
}

void IBuilder::setCursorPosition(int x, int y)
{
	COORD pos = { (SHORT)&x, (SHORT)&y }; // cast to shorts, *see wincontypes.h 

	//sCCP takes  HANDLE and COORD arguments
	SetConsoleCursorPosition(hConsole, pos);

}

void IBuilder::setCentering(bool c)
{
	isCentered = c;
}

void IBuilder::s_TxtColor(WORD attributes)
{
	SetConsoleTextAttribute(hConsole, attributes);

}

int IBuilder::getConsoleWidth() const
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
	return (bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1);
}

int IBuilder::getConsoleHeight() const
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
	return (bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
}

void IBuilder::pushToOptions(const vector<string>& items)
{
	//allows the user to choose what the menus actually say
	options = items;

	return;
}


void IBuilder::pushToFunctions(function<void()> sourceFunc)
{
	functions.push_back(sourceFunc);
}

void IBuilder::pushToFunctions(const vector<function<void()>>& sourceFunc)
{
	functions = sourceFunc;
}


void IBuilder::buildMenu()
{
	int selected = 0;
	int maxSize = _get_len_FromOptions();

	_prevWidth = -1, _prevHeight = -1;

	int lastSelection = -1;
	int width, height;
	while (true) {
		width = getConsoleWidth();
		height = getConsoleHeight();

		if (_kbhit()) {
			int key = _getch();
			if (key == 224) {
				key = _getch();

				if (key == UP_ARROW) {
					selected = (selected <= 0) ? maxSize - 1 : selected - 1;
				}
				else if (key == DOWN_ARROW) {
					selected = (selected + 1) % maxSize;
				}
			}
			else if (key == ENTER_KEY) {
				clearScreen();
				functions[selected]();
				system("pause");

				lastSelection = -1; // force redraw
			}
		}

		if (selected != lastSelection || width != _prevWidth || height != _prevHeight) {
			clearScreen();
			displayMenu(options, selected, width, height);
			lastSelection = selected;
			_prevWidth = width;
			_prevHeight = height;
		}

		Sleep(50);
	}
}

void IBuilder::drawBox(int x, int y, int width, int height)
{

}

IBuilder::IBuilder()
	: isCentered(true), hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
}

IBuilder::IBuilder(bool center)
	: isCentered(center), hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
}

IBuilder::IBuilder(std::vector<std::string> data)
	: options(data), isCentered(true), hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
}