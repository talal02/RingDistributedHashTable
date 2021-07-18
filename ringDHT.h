#pragma once
#include "SHA_1.h"
#include "UtilityFunctions.h"
#include "c_LinkList.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <cstdlib>
#include <ctime>
using namespace boost::multiprecision;
using namespace std;

template <typename T>
class RingDHT
{
private:
	c_LinkList<T> NetworkList; /* Holds nodes of Machines */
	int nBits; /* No. of bits of identifier Space */
	int256_t nMachines; /* No. of Machines*/
	string* nodeIds;
public:
	/* Coonstrutor to initialize linked list with Machines Automatically / Manually */
	RingDHT(int256_t num_Machines, int num_Bits, int option) : nMachines(num_Machines), nBits(num_Bits)
	{
		T str = "192.168.1.1:"; // Machine IP Address
		int port = 1010;
		nodeIds = new string[int64_t(num_Machines)];
		for (int64_t i = 0; i < num_Machines; i++)
		{
			string IDtoassign = "";
			if (option == 0)
			{
				T newhash = testSHA1(str, to_string(port), num_Bits);
				stringstream ss;
				ss << mod(newhash, num_Bits);
				IDtoassign = ss.str();
				for (int64_t j = 0; j < i; j++)
				{
					if (nodeIds[j] == IDtoassign)
					{
						i--;
						goto jump;
					}
				}
				nodeIds[i] = IDtoassign;
			jump:
				port += 1010 * rand() % 15;
			}
			else if (option == 1)
			{
			inputback:
				cout << "Enter Machine [" << i << "] IP address : ";
				cin >> str;
				T newhash = testSHA1(str, "", num_Bits);
				stringstream ss;
				ss << mod(newhash, num_Bits);
				IDtoassign = ss.str();
				for (int64_t j = 0; j < i; j++)
				{
					if (nodeIds[j] == IDtoassign)
					{
						cout << "\n--> Machine with that ID already exists, Enter Different Address..! \n\n";
						goto inputback;
					}
				}
				cout << "\n--> Node ID assigned : " << IDtoassign << " \n\n";
				nodeIds[i] = IDtoassign;
			}
		}
		cout << endl;
		sortArr(nodeIds, num_Machines);
		for (int64_t i = 0; i < num_Machines; i++)
		{
			NetworkList.append(nodeIds[i], num_Bits);
		}
		NetworkList.generateFingerTable(nodeIds, num_Machines);
		cout << ":: Machines ::\n";
		NetworkList.display();
	}
	/* Display All Devices and their data */
	void display()
	{
		NetworkList.print();
	}
	/* Takes Key,Value Pair and machine id from the user... Hash(key) ==> node_id to insert data at specific machine*/
	void uploadData(T dataKey, T dataValue, T machine)
	{
		int256_t idx = getidx(machine);
		if (idx == -1)
		{
			cout << "Machine Not Found. Try Again :)\n";
			return;
		}
		T hashedKey = testSHA1(dataKey, "", nBits);
		stringstream ss;
		ss << mod(hashedKey, nBits);
		cout << "Key to insert : " << ss.str() << endl;
		cout << "Path : ";
		NetworkList.operations(ss.str(), handleFile(dataKey, dataValue), NetworkList.getMachine(idx), 0);
		cout << endl;
	}
	/* Takes Key and machine id from the user... Hash(key) ==> node_id to remove data from specific machine */
	void removeData(T dataKey, T machine)
	{
		int256_t idx = getidx(machine);
		if (idx == -1)
		{
			cout << "Machine Not Found. Try Again :)\n";
			return;
		}
		T hashedKey = testSHA1(dataKey, "", nBits);
		stringstream ss;
		ss << mod(hashedKey, nBits);
		cout << "Requested HashedKey: " << ss.str() << endl;
		cout << "Path : ";
		NetworkList.operations(ss.str(), "", NetworkList.getMachine(idx), 2);
		cout << endl;
	}
	/* Takes IP Address from user... Matches that ip address with previous machines... Then add machine to linked list... This also includes correct distribution of data */
	void AddMachine(T ipaddress)
	{
		if (nMachines == power(2, nBits)) {
			cout << "No More Machines can be added in the Netork..!\n";
			return;
		}
		int port = 1250 * rand() % 30;
	hashagain:
		T newhash = testSHA1(ipaddress, to_string(port), nBits);
		stringstream ss;
		ss << mod(newhash, nBits);
		string IDtoassign = ss.str();
		for (int64_t i = 0; i < nMachines; i++)
		{
			if (nodeIds[i] == IDtoassign)
			{
				port += 1250 * rand() % 15;
				goto hashagain;
			}
		}
		nMachines++;
		nodeIds = appendMachineId(nodeIds, IDtoassign);
		sortArr(nodeIds, nMachines);
		cout << "ID to assign : " << IDtoassign << endl;
		NetworkList.newMachine(IDtoassign, nodeIds);
		NetworkList.display();
	}
	string* appendMachineId(string* nodeArr, string ID)
	{
		string* newNodes = new string[int64_t(nMachines)];
		int64_t i = 0;
		for (; i < nMachines - 1; i++)
		{
			newNodes[i] = nodeArr[i];
		}
		newNodes[i] = ID;
		return newNodes;
	}
	/* Takes machine node_id from user...Then remove machine from linked list... This also includes correct distribution of data */
	void removeMachine(T machine)
	{
		if (nMachines == 1) {
			cout << "One device is Necessary in our network...!" << endl;
			return;
		}
		int256_t idx = getidx(machine);
		if (idx == -1)
		{
			cout << "Machine Not Found. Try Again :)\n";
			return;
		}
		nodeIds = machineLeaving(nodeIds, idx);
		NetworkList.remMachine(NetworkList.getMachine(idx), nodeIds);
		NetworkList.display();
	}
	T* machineLeaving(T* nodeArr, int256_t idx)
	{
		T* newNodes = new T[int64_t(--nMachines)];
		for (int64_t i = 0, j = 0; j < nMachines; i++, j++)
		{
			if (i == idx)
			{
				i++;
			}
			if (i == nMachines + 1)
			{
				break;
			}
			newNodes[j] = nodeArr[i];
		}
		return newNodes;
	}
	/* Display's AVL Tree of specific machine */
	void displayAVL(T machine)
	{
		int256_t idx = getidx(machine);
		if (idx == -1)
		{
			cout << "Machine Not Found. Try Again :)\n";
			return;
		}
		NetworkList.displayAVL(NetworkList.getMachine(idx));
	}
	/* Display's Finger Table of specific machine */
	void displayFT(T machine)
	{
		int256_t idx = getidx(machine);
		if (idx == -1)
		{
			cout << "Machine Not Found. Try Again :)\n";
			return;
		}
		NetworkList.printFingerTable(NetworkList.getMachine(idx));
	}
	int256_t getidx(T machine)
	{
		for (int64_t i = 0; i < nMachines; i++)
		{
			if (stoint(nodeIds[i]) == stoint(machine))
			{
				return i;
			}
		}
		return -1;
	}
	~RingDHT()
	{
	}
};
