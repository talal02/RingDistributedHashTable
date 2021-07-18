#include "ringDHT.h"
using namespace std;

int main()
{
	srand(time(NULL));
	int Bits = 0, option = 0;
	int256_t Machines = 0;
	cout << "\n\n____________________PROGRAM FOR RING DISTRIBUTED HASH TABLE_______________________" << endl
		<< endl;
	do {
		cout << "Enter Identifier Space Size [Bits] Min(1) - Max(160) : ";
		cin >> Bits;
	} while (Bits < 0 || Bits > 160);
	cout << endl;
	do {
		cout << "Enter Number of Machines within Range (1 - " << power(2, Bits) << ") : ";
		cin >> Machines;
	} while (Machines < 1 || Machines > (power(2, Bits)));
	cout << endl;
	do {
		cout << "Enter \'0\' to automatically assign ID's or \'1\' to assign ID's manually : ";
		cin >> option;
	} while (option < 0 || option > 1);
	cout << "\n\n_________________________________________________________________________"
		<< endl
		<< endl;
	cout << endl
		<< endl
		<< "=============================================================================" << endl
		<< endl;
	RingDHT<string> R_DHT(Machines, Bits, option);
	cout << endl
		<< endl
		<< "=============================================================================" << endl
		<< endl;
	string key = "", value = "", machine = "";
	do
	{
		cout << "=============================================================================";
		cout << "\n\n\t\t\t MENU FOR DISTRIBUTED HASH TABLE \n " << endl
			<< "=============================================================================" << endl
			<< "1.  Upload Data to Network" << endl
			<< "2.  Remove Data from Network" << endl
			<< "3.  Print Finger Table" << endl
			<< "4.  Print AVL Tree" << endl
			<< "5.  Add New Machine to Network" << endl
			<< "6.  Remove Machine from Network" << endl
			<< "0.  Exit" << endl
			<< endl
			<< endl;
		cout << "Enter Option : ";
		cin >> option;
		cout << "=============================================================================" << endl;
		if (option == 1)
		{
			string mID;
			cout << "_____________________________________________________________________________" << endl
				<< endl;
			cout << endl
				<< "Enter Machine ID" << endl;
			cout << "Insert Data in Machine : ";
			cin >> machine;
			cout << "Enter Key : ";
			cin.ignore();
			getline(cin, key);
			cout << "Enter Value : ";
			getline(cin, value);
			cout << endl;
			R_DHT.uploadData(key, value, machine);
		}
		else if (option == 2)
		{
			cout << "_____________________________________________________________________________" << endl
				<< endl;
			cout << endl
				<< "Enter Machine ID" << endl;
			cout << "Remove Data in Machine : ";
			cin >> machine;
			cout << "Enter Key : ";
			cin.ignore();
			getline(cin, key);
			cout << endl;
			R_DHT.removeData(key, machine);
		}
		else if (option == 3)
		{
			cout << "_____________________________________________________________________________" << endl
				<< endl;
			cout << endl
				<< " Print Finger Table of Machine : ";
			cin >> machine;
			cout << endl;
			R_DHT.displayFT(machine);
		}
		else if (option == 4)
		{
			cout << "_____________________________________________________________________________" << endl
				<< endl;
			cout << endl
				<< " Print AVL Tree of Machine : ";
			cin >> machine;
			cout << endl;
			R_DHT.displayAVL(machine);
		}
		else if (option == 5)
		{
			cout << "_____________________________________________________________________________" << endl
				<< endl;
			cout << endl
				<< "Enter IP Address Of Machine : ";
			cin >> machine;
			cout << endl;
			R_DHT.AddMachine(machine);
		}
		else if (option == 6)
		{
			cout << "_____________________________________________________________________________" << endl
				<< endl;
			cout << endl
				<< "Enter Node ID Of Machine : ";
			cin >> machine;
			cout << endl;
			R_DHT.removeMachine(machine);
		}
		else
			continue;

	} while (option != 0);
	cout << "=============================================================================" << endl
		<< "Program Terminating..." << endl
		<< "=============================================================================" << endl;
}
