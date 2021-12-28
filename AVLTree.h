//Fahad Waheed (20I-0651)

#pragma once
#include"List.h"
#include<queue>

typedef unsigned long long UL;

//node of AVL Tree to store Linked List
template <typename T>
struct AVLnode
{
	UL key;
	DataList<T> List;
	int height;
	AVLnode<T>* left;
	AVLnode<T>* right;

	//default constructor
	AVLnode() :key(-1), height(-1), left(NULL), right(NULL) {};

	//parameterized constructors
	AVLnode(UL key, T data) :key(key), height(0), left(NULL), right(NULL)
	{
		List.insert(data);
	};

	AVLnode(UL key, DataList<T> List) : key(key), List(List), height(0), left(NULL), right(NULL) {};

	AVLnode(UL key, DataList<T> List, int height, AVLnode<T>* left, AVLnode<T>* right)
		: key(key), List(List), height(height), left(left), right(right) {};
};

template <typename T>
class AVLTree
{
	AVLnode<T>* root;

	//function to return height of node (subtree) used to handle exceptions
	int getheight(AVLnode<T>* node)
	{
		if (node == NULL)
			return -1;
		return node->height;
	}

	//function to return maximum from two integer values
	int max(int i1, int i2)
	{
		return (i1 > i2 ? i1 : i2);
	}

	//function to get balance factor of a node
	int getbalancefactor(AVLnode<T> * node)
	{
		if (node == NULL)
			return 0;
		return getheight(node->left) - getheight(node->right);
	}

	//function to traverse Tree inorder
	void inorder(AVLnode<T> * currnode)
	{
		if (currnode != NULL)
		{
			inorder(currnode->left);
			cout << "Key: " << currnode->key << "\nData:" << endl;
			currnode->List.display();
			cout << endl << endl;
			inorder(currnode->right);
		}
	}

	//function to right rotate a node
	AVLnode<T>* singlerotationwithright(AVLnode<T> * previousroot)
	{
		AVLnode<T>* newroot = previousroot->left;//new root
		AVLnode<T>* temp = newroot->right;//preserving the right subtree of previous root's left child

		//new root's left child will remain same

		newroot->right = previousroot;//previous root will be new root's right child 
		previousroot->left = temp;//setting previous root's left which previously points to new root

		//calculating height of both previous root and new root, as height of remaining nodes remains the same
		previousroot->height = max(getheight(previousroot->left),
			getheight(previousroot->right)) + 1;

		newroot->height = max(getheight(newroot->left),
			getheight(newroot->right)) + 1;

		return newroot;
	}

	//function to left rotate a node
	AVLnode<T>* singlerotationwithleft(AVLnode<T> * previousroot)
	{
		AVLnode<T>* newroot = previousroot->right;//new root
		AVLnode<T>* temp = newroot->left;//preserving the left subtree of previous root's right child

		//new root's right child will remain same

		newroot->left = previousroot;//previous root will be new root's left child 
		previousroot->right = temp;//setting previous root's right which previously points to new root

		//calculating height of both previous root and new root, as height of remaining nodes remains the same
		previousroot->height = max(getheight(previousroot->left),
			getheight(previousroot->right)) + 1;

		newroot->height = max(getheight(newroot->left),
			getheight(newroot->right)) + 1;

		return newroot;
	}

	//function that first right rotates right child of a node and then left rotates the node
	AVLnode<T>* doublerotationrightleft(AVLnode<T> * previousroot)
	{

		previousroot->right = singlerotationwithright(previousroot->right);
		AVLnode<T>* newroot = singlerotationwithleft(previousroot);

		return newroot;
	}

	//function that first left rotates left child of a node and then right rotates the node
	AVLnode<T>* doublerotationleftright(AVLnode<T> * previousroot)
	{
		previousroot->left = singlerotationwithleft(previousroot->left);
		AVLnode<T>* newroot = singlerotationwithright(previousroot);

		return newroot;
	}

	//function to insert data in Linked List of AVL
	AVLnode<T>* AVLinsertion(UL key, T data, AVLnode<T> * currnode)
	{
		if (currnode == NULL)//if there is no linked list with key
		{
			currnode = new AVLnode<T>(key, data);
			return currnode;
		}
		else if (key < currnode->key)//recursive call and Rotation
		{
			currnode->left = AVLinsertion(key, data, currnode->left);
			if (getbalancefactor(currnode) == 2)
			{
				if (key < currnode->left->key)
					currnode = singlerotationwithright(currnode); //RR rotation
				else
					currnode = doublerotationleftright(currnode); //LR rotation 
			}
		}
		else if (key > currnode->key)//recursive call and Rotation
		{
			currnode->right = AVLinsertion(key, data, currnode->right);
			if (getbalancefactor(currnode) == -2)
			{
				if (key > currnode->right->key)
					currnode = singlerotationwithleft(currnode); //LL rotation
				else
					currnode = doublerotationrightleft(currnode); //RL rotation
			}
		}
		else if (key == currnode->key)//if linked list is already there
		{
			currnode->List.insert(data);
		}

		//updating height of nodes
		currnode->height = max(getheight(currnode->left),
			getheight(currnode->right)) + 1;

		return currnode;
	}

	//function to remove data from Linked List of AVL
	AVLnode<T>* AVLdeletion(UL key, T data, AVLnode<T> * currnode)
	{
		if (currnode == NULL)//if node is NULL (data not found)
			return NULL;

		else if (key < currnode->key)//recursive call
		{
			currnode->left = AVLdeletion(key, data, currnode->left);
		}
		else if (key > currnode->key)//recursive call
		{
			currnode->right = AVLdeletion(key, data, currnode->right);
		}
		else if (key == currnode->key)//if node found
		{
			currnode->List.remove(data);//deleting data from linked list of node if data is there

			//checking if linked list of currnode becomes empty after removing data from linked list
			if (currnode->List.isEmpty())
			{
				if (currnode->left == NULL && currnode->right == NULL)//if node is a leaf node
				{
					delete currnode;
					currnode = NULL;
				}
				else if (currnode->left == NULL)//if node has only right child
				{
					AVLnode<T>* temp = currnode;
					currnode = currnode->right;

					delete temp;
				}
				else if (currnode->right == NULL)//if node has only left child
				{
					AVLnode<T>* temp = currnode;
					currnode = currnode->left;

					delete temp;
				}
				else//if node has both left and right children
				{
					//traversing to get the minimum node from currnode
					AVLnode<T>* temp = currnode->right;
					while (temp->left)
					{
						temp = temp->left;
					}

					//copying the content of minimum node to currnode
					currnode->key = temp->key;
					currnode->List = temp->List;

					//deleting the minimum node
					currnode->right = AVLdeletion(temp->key, currnode->right);
				}
			}
		}

		//if node was leaf node
		if (currnode == NULL)
			return NULL;

		//calculating height after deletion
		currnode->height = max(getheight(currnode->left),
			getheight(currnode->right)) + 1;

		//left subtree is heavy
		if (getbalancefactor(currnode) == 2)
		{
			//checking if left child of left subtree exists or not
			if (getbalancefactor(currnode->left) >= 0)
			{
				currnode = singlerotationwithright(currnode);
			}
			else if (getbalancefactor(currnode->left) < 0)
			{
				currnode = doublerotationleftright(currnode);
			}
		}

		//right subtree is heavy
		else if (getbalancefactor(currnode) == -2)
		{
			//checking if right child of right subtree exists or not
			if (getbalancefactor(currnode->right) <= 0)
			{
				currnode = singlerotationwithleft(currnode);
			}
			else if (getbalancefactor(currnode->right) > 0)
			{
				currnode = doublerotationrightleft(currnode);
			}
		}

		return currnode;
	}

	//function to remove a node of AVL
	AVLnode<T>* AVLdeletion(UL key, AVLnode<T>* currnode)
	{
		if (currnode == NULL)//if node not found
			return NULL;

		else if (key < currnode->key)//recursive call
		{
			currnode->left = AVLdeletion(key, currnode->left);
		}
		else if (key > currnode->key)//recursive call
		{
			currnode->right = AVLdeletion(key, currnode->right);
		}
		else if (key == currnode->key)//if node found
		{
			if (currnode->left == NULL && currnode->right == NULL)//if node is a leaf node
			{
				delete currnode;
				currnode = NULL;
			}
			else if (currnode->left == NULL)//if node has only right child
			{
				AVLnode<T>* temp = currnode;
				currnode = currnode->right;

				delete temp;
			}
			else if (currnode->right == NULL)//if node has only left child
			{
				AVLnode<T>* temp = currnode;
				currnode = currnode->left;

				delete temp;
			}
			else//if node has both left and right children
			{
				//traversing to get the minimum node from currnode
				AVLnode<T>* temp = currnode->right;
				while (temp->left)
				{
					temp = temp->left;
				}

				//copying the content of minimum node to currnode
				currnode->key = temp->key;
				currnode->List = temp->List;

				//deleting the minimum node
				currnode->right = AVLdeletion(temp->key, currnode->right);
			}
		}

		//if node was leaf node
		if (currnode == NULL)
			return NULL;

		//calculating height after deletion
		currnode->height = max(getheight(currnode->left),
			getheight(currnode->right)) + 1;

		//left subtree is heavy
		if (getbalancefactor(currnode) == 2)
		{
			//checking if left child of left subtree exists or not
			if (getbalancefactor(currnode->left) >= 0)
			{
				currnode = singlerotationwithright(currnode);
			}
			else if (getbalancefactor(currnode->left) < 0)
			{
				currnode = doublerotationleftright(currnode);
			}
		}

		//right subtree is heavy
		else if (getbalancefactor(currnode) == -2)
		{
			//checking if right child of right subtree exists or not
			if (getbalancefactor(currnode->right) <= 0)
			{
				currnode = singlerotationwithleft(currnode);
			}
			else if (getbalancefactor(currnode->right) > 0)
			{
				currnode = doublerotationrightleft(currnode);
			}
		}

		return currnode;
	}

	//function to search a node in AVL
	void Search(AVLnode<T> * currnode, UL key)
	{
		if (currnode == NULL)
			return;
		else if (currnode->key == key)
		{
			cout << "Key: " << currnode->key << "\nData:" << endl;
			currnode->List.display();
			cout << endl;
		}
		else if (key < currnode->key)
			Search(currnode->left, key);
		else if (key > currnode->key)
			Search(currnode->right, key);
	}

///////////////////////////////////////////////////////////////////////////////////////////

	//simple AVL insertion function
	AVLnode<T>* AVLinsertion(UL key, DataList<T> data, AVLnode<T> * currnode)
	{
		if (currnode == NULL)
		{
			currnode = new AVLnode<T>(key, data);
			return currnode;
		}
		else if (key < currnode->key)
		{
			currnode->left = AVLinsertion(key, data, currnode->left);
			if (getbalancefactor(currnode) == 2)
			{
				if (key < currnode->left->key)
					currnode = singlerotationwithright(currnode); //RR rotation
				else
					currnode = doublerotationleftright(currnode); //LR rotation 
			}
		}
		else if (key > currnode->key)
		{
			currnode->right = AVLinsertion(key, data, currnode->right);
			if (getbalancefactor(currnode) == -2)
			{
				if (key > currnode->right->key)
					currnode = singlerotationwithleft(currnode); //LL rotation
				else
					currnode = doublerotationrightleft(currnode); //RL rotation
			}
		}

		currnode->height = max(getheight(currnode->left),
			getheight(currnode->right)) + 1;

		return currnode;
	}

	//function to insert nodes of a Tree into another Tree which is initially not empty
	void FullTreeInsertion(AVLnode<T>* currnode, AVLTree<T>& Tree)
	{
		if (currnode != NULL)
		{
			Tree.root = Tree.AVLinsertion(currnode->key, currnode->List, Tree.root);
			FullTreeInsertion(currnode->left, Tree);
			FullTreeInsertion(currnode->right, Tree);
		}
	}

	//function to split AVL Tree
	void Split(UL key, AVLnode<T> * currnode, AVLTree<T> & Tree1, AVLTree<T> & Tree2)
	{
		if (currnode == NULL)//if node was NULL or becomes NULL while traversing
		{
			return;
		}
		if (currnode->key <= key)//inserting nodes into Tree 1 if they are less than or equal to key
		{
			Tree1.root = Tree1.AVLinsertion(currnode->key, currnode->List, Tree1.root);
		}
		if (currnode->key > key)//inserting nodes into Tree 2 if they are greater than key
		{
			Tree2.root = Tree2.AVLinsertion(currnode->key, currnode->List, Tree2.root);
		}
		//recursive calls
		Split(key, currnode->left, Tree1, Tree2);
		Split(key, currnode->right, Tree1, Tree2);
	}

	//function used to copy nodes from one Tree to another according to their position
	AVLnode<T>* AVLcopynodes(AVLnode<T> * currnode)
	{
		if (currnode == NULL)
			return NULL;
		else
		{
			AVLnode<T>* left = AVLcopynodes(currnode->left);
			AVLnode<T>* right = AVLcopynodes(currnode->right);
			return new AVLnode<T>(currnode->key, currnode->List, currnode->height, left, right);
		}
	}

public:
	AVLTree() :root(NULL) {};

	//copy constructor
	AVLTree(const AVLTree<T> & obj):root(NULL)
	{
		this->root = AVLcopynodes(obj.root);
	}

	//copy assignment operator
	AVLTree<T>& operator=(const AVLTree<T> & obj)
	{
		this->clear();

		this->root = AVLcopynodes(obj.root);

		return *this;
	}

	//function to get root of AVL
	AVLnode<T>* getroot()
	{
		return root;
	}

	//function to Display whole AVL
	void DisplayInOrder()
	{
		inorder(root);
	}

	//function to insert data in Linked list of a node of AVL
	void insert(UL key, T data)
	{
		root = AVLinsertion(key, data, root);
	}

	//function to search a node of AVL
	void Search(UL key)
	{
		Search(root, key);
	}

	//function to remove data from Linked list of a node of AVL
	void remove(UL key, T data)
	{
		root = AVLdeletion(key, data, root);
	}

	//function to remove a node of AVL
	void remove(UL key)
	{
		root = AVLdeletion(key, root);
	}

///////////////////////////////////////////////////////////////////////////////////////////////

	//function to split an AVL Tree into two
	void Split(UL key, AVLTree<T> MainTree, AVLTree<T> & Tree1, AVLTree<T> & Tree2)
	{
		Tree1.clear();
		Tree2.clear();
		Split(key, MainTree.root, Tree1, Tree2);
	}

	//function to merge two AVL Tree into one
	void Merge(AVLTree<T> & MainTree, AVLTree<T> Tree1, AVLTree<T> Tree2)
	{
		MainTree.clear();

		MainTree = Tree1;

		MainTree.FullTreeInsertion(Tree2.root, MainTree);
	}

///////////////////////////////////////////////////////////////////////////////////////////////

	//function to delete all nodes of AVL Tree using queue
	void clear()//to delete all elements that are in queue
	{
		if (root != NULL)
		{
			queue<AVLnode<T>*> Q;
			AVLnode<T>* currnode = NULL;
			Q.push(root);

			while (!Q.empty())
			{
				currnode = Q.front();
				Q.pop();

				if (currnode->left != NULL)
					Q.push(currnode->left);
				if (currnode->right != NULL)
					Q.push(currnode->right);

				currnode->List.clear();
				delete currnode;
			}

			root = NULL;
		}
	}

	//destructor
	~AVLTree()
	{
		clear();
	}
};