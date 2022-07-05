#include "Classes.h"
#include "Functions.h"

int navigate;

void Menu() {
	cout << "(1) " << endl
		<< "(2) " << endl
		<< "(3) " << endl
		<< "(4) " << endl
		<< "(5) " << endl
		<< "\nEnter your selection: ";
	cin >> navigate;
}

int main(){
	
	Account sender;
	sender.genAccount(sender);

	Account receiver;
	receiver.genAccount(receiver);

	return 0;
}