#include"RingDHT.h"
#include<iomanip>

int askchoice()
{
	int ch;

	//cout << setw(75) << "Distributed Hash Table System\n";

	cout << "\t\t\t\t\t _____  _    _ _______ _____ \n"
		<< "\t\t\t\t\t|  __ \\| |  | |__   __/ ____|\n"
		<< "\t\t\t\t\t| |  | | |__| |  | | | (___  \n"
		<< "\t\t\t\t\t| |  | |  __  |  | |  \\___ \\ \n"
		<< "\t\t\t\t\t| |__| | |  | |  | |  ____) |\n"
		<< "\t\t\t\t\t|_____/|_|  |_|  |_| |_____/ \n";

	do
	{
		cout << "Main Menu:\n"
			<< "Enter 1 to add data.\n"
			<< "Enter 2 to delete data.\n"
			<< "Enter 3 to search data.\n"
			<< "Enter 4 to print routing table of a machine.\n"
			<< "Enter 5 to print AVL Tree of a machine.\n"
			<< "Enter 6 to add a machine.\n"
			<< "Enter 7 to delete a machine.\n"
			<< "Enter 8 to Display All machines with Data and Routing Table.\n"
			<< "Enter 0 to exit.\n"
			<< "Enter your choice: ";
		cin >> ch;
		cin.ignore();
	} while (ch < 0 || ch > 8);

	return ch;
}

void Main()
{
	int number_of_bits = 4;
	UL number_of_machines = 4;
	int choice = -1;

	do
	{
		cout << "Enter identifier bits (1-63): ";
		cin >> number_of_bits;
		cin.ignore();
	} while (number_of_bits < 1 || number_of_bits > 63);

	cout << endl;

	UL x = int(pow(2, number_of_bits));

	do
	{
		cout << "Enter number of machines: ";
		cin >> number_of_machines;
		cin.ignore();
	} while (number_of_machines < 1 || number_of_machines > x);

	cout << endl;

	do
	{
		cout << "Enter 1 if you want to manually enter ids of machines.. Otherwise enter 0..\n";
		cout << "Please enter your choice: ";
		cin >> choice;
		cin.ignore();
	} while (choice < 0 || choice > 1);

	RingDHT DHT(number_of_machines, number_of_bits, choice);

Again:
	choice = askchoice();

	switch (choice)
	{
	case 1:
	{
		DHT.DataInsertion();
		goto Again;
	}
	case 2:
	{
		DHT.DataDeletion();
		goto Again;
	}
	case 3:
	{
		DHT.DataSearch();
		goto Again;
	}
	case 4:
	{
		DHT.PrintRoutingTable();
		goto Again;
	}
	case 5:
	{
		DHT.PrintAVL();
		goto Again;
	}
	case 6:
	{
		DHT.NewMachine();
		goto Again;
	}
	case 7:
	{
		DHT.DeleteMachine();
		goto Again;
	}
	case 8:
	{
		DHT.DisplayAll();
		goto Again;
	}
	case 0:
	{
		exit(0);
	}
	}
}

int main()
{
	Main();

}