//Fahad Waheed (20I-0651)

#pragma once
#include<iostream>

using namespace std;

//declaration of Machinenode
struct Machinenode;

//node of Routing Table to hold references of machine
struct RTnode
{
	int index;
	static int base;
	Machinenode* machine;
	RTnode* next, * previous;

	RTnode() :index(-1), machine(NULL), next(NULL), previous(NULL) {};

	RTnode(Machinenode* machine) :index(base++), machine(machine), next(NULL), previous(NULL) {};

	//static function to set base variable to zero (used in indexing)
	static void clearbase()
	{
		base = 0;
	}
};

int RTnode::base = 0;

//Definition of class Routing Table
class RoutingTable
{
	RTnode* head, * tail;
public:
	RoutingTable() : head(NULL), tail(NULL) {};

	//to check if Routing Table is empty or not
	bool isEmpty()
	{
		return (head == NULL && tail == NULL);
	}

	//to insert a new node at the end of Routing Table (Doubly Linked List)
	void insert(Machinenode * machine)
	{
		if (isEmpty())
		{
			tail = head = new RTnode(machine);
		}
		else
		{
			tail->next = new RTnode(machine);
			tail->next->previous = tail;
			tail = tail->next;
		}
	}

	RTnode* gethead()
	{
		return head;
	}

	RTnode* gettail()
	{
		return tail;
	}

	//function to get machine of a specific index
	Machinenode* getmachine(int index)
	{
		RTnode* currnode = head;

		while (currnode != NULL && currnode->index != index)
			currnode = currnode->next;

		if (currnode == NULL)
			return NULL;
		return currnode->machine;
	}

	//function to delete all nodes of Doubly Linked List
	void clear()
	{
		RTnode* currnode = head;
		while (head != NULL)
		{
			head = head->next;
			delete currnode;
			currnode = head;
		}
		tail = head;
	}

	//destructor
	~RoutingTable()
	{
		clear();
	}
};