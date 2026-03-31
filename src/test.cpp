#include "../classes/IBuilder.h"
#include <iostream>

void displayStart() {
	using namespace std;
	cout << "start" << endl;
}
void displayQuit() {
	using namespace std;
	cout << "quit" << endl;
	exit(0);
}

void displayMainMenu() {
	using namespace std;

	IBuilder menu(true);
	vector<string> options = { "Start Program", "Quit" };
	vector<function<void()>> functions = {displayStart, displayQuit};

	menu.push_toOptions(options);
	menu.push_Functions(functions);

	menu.buildMenu();
}


int main() {
	displayMainMenu();
	return 0;
}