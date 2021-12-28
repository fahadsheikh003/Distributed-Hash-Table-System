//Fahad Waheed (20I-0651)

#pragma once
#include<iostream>
#include<string>
#include"AVLTree.h"
#include"RoutingTable.h"

using namespace std;

//to calculate hash of a value
UL hashfunction(UL key, UL identifier_space)
{
	return key % identifier_space;
}

//definition of Machinenode (Actual Machines)
struct Machinenode
{
	UL id;
	UL hashedid;
	AVLTree<string> AVL;
	RoutingTable RT;
	Machinenode* next;

	Machinenode() : id(-1), hashedid(-1), next(NULL) {};
	Machinenode(UL id, UL hid) : id(id), hashedid(hid), next(NULL) {};
};

//Circular Linked List of Machines (Universe)
class RingDHT
{
	Machinenode* head;
	UL number_of_machines;
	UL identifier_space;
	int identifier_bits;

public:
	RingDHT() : head(NULL), number_of_machines(0), identifier_bits(0), identifier_space(0) {};

	//parameterized constructor to set up DHT
	RingDHT(UL number_of_machines, int identifier_bits, int choice = 0) :
		head(NULL), number_of_machines(number_of_machines), identifier_bits(identifier_bits),
		identifier_space(UL(pow(2, identifier_bits)))
	{
		if (choice == 0)//assigning ids automatically 
		{
			UL x = identifier_space / number_of_machines;
			UL y = 0;
			for (UL i = 0; i < number_of_machines; i++)
			{
				y += x;
				insert(y, hashfunction(y, identifier_space));
			}
		}
		else//assigning ids manually 
		{
			UL x, hashedx;
			for (UL i = 0; i < number_of_machines; i++)
			{
				do
				{
					cout << "Please Enter id of Machine" << i + 1 << ": ";
					cin >> x;
					cin.ignore();
					hashedx = hashfunction(x, identifier_space);
				} while (x < 0 || !uniquemachine(hashedx));

				insert(x, hashedx);
			}
		}

		setroutingtable();
		DisplayAll();
	}

/////////////////////////////////////////////////////////////////////////////////////////////

	//function to display routing table of a machine
	void DisplayRoutingTable(Machinenode * node)
	{
		RTnode* rtnode = node->RT.gethead();
		int count = 1;
		while (rtnode != NULL)
		{
			cout << "Machine " << count++ << ":\n" << "ID: " << rtnode->machine->id << endl;
			cout << "Hashed ID: " << rtnode->machine->hashedid << endl;
			rtnode = rtnode->next;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////

	//function to clear routing table of all machine
	void clearroutingtable()
	{
		Machinenode* currnode = head;
		do
		{
			currnode->RT.clear();
			currnode = currnode->next;
		} while (currnode != head);
		RTnode::clearbase();
	}

	//function to set up routing table of all the machines according to the formula given in description
	void setroutingtable()
	{
		Machinenode* currnode = head;
		do
		{
			RTnode::clearbase();
			for (int i = 1; i <= identifier_bits; i++)
			{
				Machinenode* succ = successor(hashfunction(currnode->hashedid + UL(pow(2, i - 1)), identifier_space));

				currnode->RT.insert(succ);
			}
			currnode = currnode->next;
		} while (currnode != head);
	}

	//function to check if a machine with similar hashedid exists or not
	bool uniquemachine(UL hashedid)
	{
		if (head)
		{
			Machinenode* currnode = head;

			do
			{
				if (currnode->hashedid == hashedid)
					return false;
				currnode = currnode->next;
			} while (currnode != head);
		}
		return true;
	}

	//function to find successor machine against a hash
	Machinenode* successor(UL hash)
	{
		//finding machines with minimum and maximum hashedids
		Machinenode* currnode = head->next;
		Machinenode* minnode = head;
		Machinenode* maxnode = head;

		do
		{
			if (minnode->hashedid > currnode->hashedid)
				minnode = currnode;
			if (maxnode->hashedid < currnode->hashedid)
				maxnode = currnode;
			currnode = currnode->next;
		} while (currnode != head);

		//checking if hash is less than or equal to hashed id of minimum machine or greater than hashed id of maximum machine
		if (hash <= minnode->hashedid || hash > maxnode->hashedid)
			return minnode;

		//as hash will map to the first machine whose hashed id is greater than or equal to hash
		currnode = head;
		while (currnode->hashedid < hash)
		{
			currnode = currnode->next;
		}

		return currnode;
	}

	//inserting machines in the circular linked list (in ascending order based on their hashed id)
	void insert(UL id, UL hid)
	{
		if (head == NULL)//if list is NULL
		{
			head = new Machinenode(id, hid);
			head->next = head;
		}
		else
		{
			Machinenode* newnode = new Machinenode(id, hid);
			if (hid < head->hashedid)//if hashed id of new machine is less than hashed id of head machine
			{
				//finding last machine to set up pointers as it is a circular linked list
				Machinenode* currnode = head;
				do
				{
					currnode = currnode->next;
				} while (currnode->next != head);

				//setting up pointers
				newnode->next = head;
				currnode->next = newnode;
				head = newnode;
			}
			else//if hashed id of new machine is greater than all the machines or in the middle
			{
				Machinenode* currnode = head;

				while (currnode->next != head && currnode->next->hashedid < hid)
				{
					currnode = currnode->next;
				}

				newnode->next = currnode->next;
				currnode->next = newnode;
			}
		}
	}

	//Displaying All machine (including their Routing Table and AVL)
	void DisplayAll()
	{
		if (head)
		{
			UL count = 1;
			Machinenode* node = head;
			do
			{
				cout << "Machine " << count++ <<":\nID: " << node->id << endl;
				cout << "Hashed ID: " << node->hashedid << endl <<endl;
				cout << "Routing Table:\n";
				DisplayRoutingTable(node);
				if (node->AVL.getroot() != NULL)
				{
					cout << "\nAVL:\n";
					node->AVL.DisplayInOrder();
				}

				cout << endl << endl;

				node = node->next;
			} while (node != head);
		}
	}

	//function to display only id and hashed id of machines
	void ListMachines()
	{
		Machinenode* currnode = head;
		UL count = 1;
		do
		{
			cout << count++ << ". " << "Machine " << currnode->id << "(" << currnode->hashedid << ")" << endl;
			currnode = currnode->next;
		} while (currnode != head);
	}

	//function to search machine with the specified hashed id
	Machinenode* SearchMachine(UL hid)
	{
		Machinenode* currnode = head;

		do
		{
			if (currnode->hashedid == hid)
				return currnode;

			currnode = currnode->next;
		} while (currnode != head);

		return NULL;
	}

	//function to check if hash maps to the head machine or not
	bool checkspecialcase(UL hashedkey)
	{
		return (successor(hashedkey) == head);
	}

	//function that tells us about Routing
	Machinenode* Routing(UL hashedkey, Machinenode * machine, bool specialcase)
	{
		cout << "-->" << machine->id << "(" << machine->hashedid << ")";//printing id of current machine

		if (machine->hashedid == hashedkey)//if hash is equal to hashed of current machine
		{
			cout << endl;
			return machine;
		}

		if (machine->hashedid < hashedkey)//if hash is greater than hashed id of current machine
		{
			//finding first machine in routing table of current machine
			Machinenode* currmachine = machine->RT.getmachine(0);

			/*
			checking if hash is less than or equal to hashed id of first machine in 
			routing table of current machine or if hash maps to head and first machine in 
			routing table of current machine is also head.
			*/

			if ((hashedkey <= currmachine->hashedid) ||
				(specialcase && currmachine == head))
			{
				cout << "-->" << currmachine->id << "(" << currmachine->hashedid << ")" << endl;
				return currmachine;
			}
			else
			{
				int j = 0;
				Machinenode* nextmachine = machine->RT.getmachine(j + 1);

				/*
				finding the machine to which hash maps and calling this recursively for its previous machine
				if machine not found then calling the function for second last entry of routing table
				*/

				while (j < identifier_bits - 1 && nextmachine &&
					hashedkey > nextmachine->hashedid)
				{
					j++;
					currmachine = nextmachine;
					if (j + 1 < identifier_bits)
						nextmachine = machine->RT.getmachine(j + 1);

					if (specialcase && nextmachine == head)
						break;
				}
				return Routing(hashedkey, currmachine, specialcase);
			}
		}

		if (machine->hashedid > hashedkey)//if hash is less than hashed id of current machine
		{
			if (successor(hashedkey) == machine)//checking if hash maps to current machine
			{
				cout << endl;
				return machine;
			}
			else
			{
				int j = 0;
				Machinenode* currmachine = machine->RT.getmachine(j);
				Machinenode* nextmachine = machine->RT.getmachine(j + 1);

				/*
				finding the machine to which hash maps and calling this recursively for its previous machine
				if machine not found then calling the function for second last entry of routing table
				*/

				while (j < identifier_bits - 1 && nextmachine &&
					hashedkey < nextmachine->hashedid)
				{
					j++;
					currmachine = nextmachine;
					if (j + 1 < identifier_bits)
						nextmachine = machine->RT.getmachine(j + 1);

					if (specialcase && nextmachine == head)
						break;
				}
				return Routing(hashedkey, currmachine, specialcase);
			}
		}

	}

	//function to insert data
	void DataInsertion(UL hashedkey, string data, Machinenode * machine)
	{
		cout << "Path:\n";
		Machinenode* mainmachine = Routing(hashedkey, machine, checkspecialcase(hashedkey));
		mainmachine->AVL.insert(hashedkey, data);

		cout << "Updated AVL Tree of machine " << mainmachine->id << "(" << mainmachine->hashedid << ")" << ":\n";
		mainmachine->AVL.DisplayInOrder();
	}

	//function to take input for the data being inserted
	void DataInsertion()
	{
		UL key, machineid;
		string data;
		Machinenode* machine;

		do
		{
			cout << "Enter key: ";
			cin >> key;
			cin.ignore();
		} while (key < 0);

		cout << "Enter Data that you want to insert against key " << key << " : ";
		getline(cin, data);

		do
		{
			ListMachines();
			cout << "Enter id of machine that you want to use: ";
			cin >> machineid;
			cin.ignore();
			machine = SearchMachine(hashfunction(machineid, identifier_space));
		} while (machine == NULL);

		DataInsertion(hashfunction(key, identifier_space), data, machine);
	}

	//function to remove data
	void DataDeletion(UL hashedkey, string data, Machinenode* machine)
	{
		cout << "Path:\n";
		Machinenode* mainmachine = Routing(hashedkey, machine, checkspecialcase(hashedkey));
		mainmachine->AVL.remove(hashedkey, data);

		cout << "Updated AVL Tree of machine " << mainmachine->id << ":\n";
		mainmachine->AVL.DisplayInOrder();
	}

	//function to remove a node from AVL of a machine
	void DataDeletion(UL hashedkey, Machinenode* machine)
	{
		cout << "Path:\n";
		Machinenode* mainmachine = Routing(hashedkey, machine, checkspecialcase(hashedkey));
		mainmachine->AVL.remove(hashedkey);

		cout << "Updated AVL Tree of machine " << mainmachine->id << ":\n";
		mainmachine->AVL.DisplayInOrder();
	}

	//function to take input for the data being deleted
	void DataDeletion()
	{
		UL key, machineid;
		Machinenode* machine;

		do
		{
			cout << "Enter key: ";
			cin >> key;
			cin.ignore();
		} while (key < 0);

		do
		{
			ListMachines();
			cout << "Enter id of machine that you want to use: ";
			cin >> machineid;
			cin.ignore();
			machine = SearchMachine(hashfunction(machineid, identifier_space));
		} while (machine == NULL);

		int choice = -1;

		do
		{
			cout << "NOTE: Each node of AVL Tree contains a Linked List which contains data.\n";
			cout << "Enter 1 if u want to delete whole node of AVL Tree.\n"
				<< "Enter 2 if u want to delete some specific data.\n"
				<< "Enter your choice: ";
			cin >> choice;
			cin.ignore();
		} while (choice < 1 || choice > 2);

		switch (choice)
		{
		case 1:
		{
			DataDeletion(hashfunction(key, identifier_space), machine);

			break;
		}
		case 2:
		{
			string data;

			cout << "Enter Data that you want to remove against key " << key << " : ";
			getline(cin, data);

			DataDeletion(hashfunction(key, identifier_space), data, machine);
		}
		}
	}

	//function to Search data
	void DataSearch(UL hashedkey, Machinenode * machine)
	{
		cout << "Path:\n";
		Machinenode* mainmachine = Routing(hashedkey, machine, checkspecialcase(hashedkey));
		mainmachine->AVL.Search(hashedkey);
	}

	//function to take input for the data being searched
	void DataSearch()
	{
		UL key, machineid;
		Machinenode* machine;

		do
		{
			cout << "Enter key: ";
			cin >> key;
			cin.ignore();
		} while (key < 0);

		do
		{
			ListMachines();
			cout << "Enter id of machine that you want to use: ";
			cin >> machineid;
			cin.ignore();
			machine = SearchMachine(hashfunction(machineid, identifier_space));
		} while (machine == NULL);

		DataSearch(hashfunction(key, identifier_space), machine);
	}

	//function to take input for the machine to print its Routing Table
	void PrintRoutingTable()
	{
		UL machineid;
		Machinenode* machine;

		do
		{
			ListMachines();
			cout << "Enter id of machine whose Routing Table you want to see: ";
			cin >> machineid;
			cin.ignore();
			machine = SearchMachine(hashfunction(machineid, identifier_space));
		} while (machine == NULL);

		DisplayRoutingTable(machine);
	}

	//function to take input for the machine to print its AVL
	void PrintAVL()
	{
		UL machineid;
		Machinenode* machine;

		do
		{
			ListMachines();
			cout << "Enter id of machine whose AVL Tree you want to see: ";
			cin >> machineid;
			cin.ignore();
			machine = SearchMachine(hashfunction(machineid, identifier_space));
		} while (machine == NULL);

		machine->AVL.DisplayInOrder();
	}

	//function to check if Circular List is empty or not
	bool isEmpty()
	{
		return head == NULL;
	}

	//function to remove a machine from the Circular Linked List
	void remove(Machinenode * node)
	{
		if (head == NULL)//if List is empty
			return;

		//if node is first & only node
		else if (head == head->next && head == node)
		{
			delete head;
			head = NULL;
			return;
		}

		//if node is first node
		else if (head == node)
		{
			Machinenode* currnode = head->next;
			while (currnode->next != head)
				currnode = currnode->next;
			currnode->next = head->next;
			delete head;
			head = currnode->next;
			return;
		}

		//if node is in middle or at the end
		Machinenode* currnode = head->next;
		while (currnode != head && currnode->next != node)
			currnode = currnode->next;

		if (currnode->next == node)
		{
			currnode->next = currnode->next->next;
			delete node;
			node = NULL;
		}
	}

	//function to create a new machine
	void NewMachine()
	{
		UL machineid = 0, hashedid;

		do
		{
			cout << "Please Enter id of Machine: ";
			cin >> machineid;
			cin.ignore();
			hashedid = hashfunction(machineid, identifier_space);
		} while (machineid < 0 || !uniquemachine(hashedid));

		insert(machineid, hashedid);//creating machine
		Machinenode * currmachine = SearchMachine(hashedid);
		number_of_machines++;

		//finding next machine
		Machinenode * succmachine = successor(hashfunction(hashedid + 1, identifier_space));

		AVLTree<string> tempAVL = succmachine->AVL;

		//splitting AVL Trees
		tempAVL.Split(hashedid, tempAVL, currmachine->AVL, succmachine->AVL);

		//updating routing table
		clearroutingtable();
		setroutingtable();
	}

	//function to remove a machine
	void DeleteMachine()
	{
		if (number_of_machines == 1)
		{
			cout << "Sorry! there is only one machine.\n";
			return;
		}

		UL machineid;
		Machinenode* machine;
		do
		{
			ListMachines();
			cout << "Enter id of machine that you want to delete: ";
			cin >> machineid;
			cin.ignore();
			//checking if machine even exists or not
			machine = SearchMachine(hashfunction(machineid, identifier_space));
		} while (machine == NULL);

		//finding next machine
		Machinenode * succmachine = successor(
			hashfunction(machine->hashedid + 1, identifier_space));

		AVLTree<string> tempAVL = succmachine->AVL;

		//merging AVL with the next machine of the machine that is being deleted
		succmachine->AVL.Merge(succmachine->AVL, machine->AVL, tempAVL);

		//removing machine
		machine->AVL.clear();
		remove(machine);
		number_of_machines--;

		//updating routing table
		clearroutingtable();
		setroutingtable();
	}

	//function to delete all nodes of singly circular linked list
	void clear()
	{
		if (!isEmpty())
		{
			Machinenode* currmachine = head;
			Machinenode* next = NULL;
			do
			{
				next = currmachine->next;
				delete currmachine;
				currmachine = next;
			} while (currmachine != head);

			head = NULL;
		}
	}

	//destructor
	~RingDHT()
	{
		clear();
	}
};