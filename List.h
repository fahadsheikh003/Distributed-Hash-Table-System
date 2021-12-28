//Fahad Waheed (20I-0651)

#pragma once
#include<iostream>

using namespace std;

//node of a simple linked list to store data
template <typename T>
struct DataNode
{
	T data;
	DataNode<T>* next;

	DataNode() : data(T()), next(NULL) {};
	DataNode(T data) : data(data), next(NULL) {};
};

//linked list to store bunch of data together
template <typename T>
class DataList
{
	DataNode<T>* head;

public:
	DataList() : head(NULL) {};
	
	//copy constructor
	DataList(const DataList<T>& obj) : head(NULL)
	{
		DataNode<T>* currnode = obj.head;

		while (currnode != NULL)
		{
			this->insert(currnode->data);
			currnode = currnode->next;
		}
	}

	//copy assignment operator
	DataList<T>& operator=(const DataList<T>& obj)
	{
		this->clear();

		DataNode<T>* currnode = obj.head;

		while (currnode != NULL)
		{
			this->insert(currnode->data);
			currnode = currnode->next;
		}

		return *this;
	}
	
	//function to check if list is empty or not
	bool isEmpty()
	{
		return head == NULL;
	}

	//function to check if data is already in list or not
	bool uniquedata(T data)
	{
		if (!isEmpty())
		{
			DataNode<T>* currnode = head;

			while (currnode != NULL)
			{
				if (currnode->data == data)
					return false;
				currnode = currnode->next;
			}
		}
		return true;
	}

	//function to insert data at the end of linked list (append)
	void insert(T data)
	{
		if (isEmpty())
		{
			head = new DataNode<T>(data);
		}
		else
		{
			if (!uniquedata(data))
			{
				cout << data << " exists already.\n";
				return;
			}
			DataNode<T>* newnode = new DataNode<T>(data);
			DataNode<T>* currnode = head;
			while (currnode->next != NULL)
			{
				currnode = currnode->next;
			}
			currnode->next = newnode;
		}
	}

	//function to display the data in linked list
	void display()
	{
		DataNode<T>* currnode = head;
		while (currnode != NULL)
		{
			cout << currnode->data << endl;
			currnode = currnode->next;
		}
	}

	//function to remove an element from linked list
	bool remove(T data)
	{
		if (isEmpty())
			return false;

		DataNode<T>* currnode = head;
		DataNode<T>* prevnode = NULL;

		while (currnode != NULL && currnode->data != data)
		{
			prevnode = currnode;
			currnode = currnode->next;
		}

		if (currnode == NULL)//if not found
			return false;

		else if (prevnode == NULL)//if data is at head
		{
			head = currnode->next;
			delete currnode;
			return true;
		}

		else//if data is in middle or at the end
		{
			prevnode->next = currnode->next;
			delete currnode;
			return true;
		}
	}

	//function to delete all nodes in linked list
	void clear()
	{
		DataNode<T>* currnode = head;

		while (head != NULL)
		{
			head = head->next;
			delete currnode;
			currnode = head;
		}
	}

	//destructor of linked list
	~DataList()
	{
		clear();
	}
};