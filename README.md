# CIS 240 – Data Structure Project
## Due: April 19, 2026 at 11:59 p.m.
--- 
## Project Description
In this project, you will simulate the children’s game Duck Duck Goose using a doubly circular linked list.  
Duck Duck Goose is a classic circle game where one player (the "it" or "fox") walks around tapping others on the head, saying "duck" until choosing a "goose." The goose chases the tapper around the circle, trying to tag him/her before he/she can sit in the empty spot.  If the tapper gets to the empty spot before getting tagged,  the goose becomes the “it” in the next round.  If the tapper gets tagged, he/she goes into the middle, the cooking pot. The goose then becomes the next “it” and another round begins. The game repeats until only one player remains. 

## Simulation
Before the game starts, your program should allow the user to enter the names of players (minimum of 5) via keyboard or files.  The players then are inserted into the doubly circular linked list. 
At the beginning of each round, determine which player will be the “it” and the “it” is not in the circle.  Then move around the circle while displaying “duck, duck, duck,..., goose!”. You will need to decide on how the goose may be selected. For example, select the goose randomly or by user’s choice or any other way.  (You will need to provide the rationale behind it.)  Simulate the chase between the tapper and the goose. (Again, you will need to provide rationale for the design choice).  Note that they can move either in clockwise or anticlockwise direction.  If the tapper escapes by reaching the place where the goose was, the tapper takes that position and the goose becomes the new “it”.  If the tapper gets tagged (meaning the goose catches up to the tapper), the tapper is removed from the circle and placed into the cooking pot and the goose becomes the new “it”.  After each round, display the players in the cooking pot.  The game continues until only one player remains and that player is declared as the winner.  Also, allow the user to end the game in between rounds. 

## Note: 
You are NOT allowed to use arrays, vectors, or STL containers for storage of the circle.
You can earn extra points by adding additional features. 

### Class Definition
Create a file named `DCList.h` with the following class definition:
``` cpp
#include<string>
typedef string T;
class List {
	private:
	class Node {
	public:
	T data;
	Node * next, * prev;
	};
	public:
	List();
	List(const List&);
	~List();
	int getSize() const;
	bool isEmpty() const;
	bool isFull() const;
	T peek(const int) const;
	bool replace(const T&, const int); // replacing the data in a specified node with new data.
	bool insert(const T&, const int);
	bool remove(const int);
	const List& operator=(const List&);
	void display(ostream&) const;
	friend ostream& operator<< (ostream&, const List&);
	private:
	Node * first; // pointer to the first element of the linked list
};
```
### Hint: Start with the pointer-based linked list implementation that we developed in class and modify the class so that it holds a doubly circular linked list.
`LinkedList.h`:
``` cpp
#pragma once
typedef int type;

class LinkedList
{
private:
	class Node 
	{
	public:
		type data;
		Node* next;

		Node(type d) :data(d), next(0)
		{}
	};

	// data attributes
	Node* first;
	int count;

public:
	LinkedList();
	~LinkedList();
	LinkedList(const LinkedList& source);
	LinkedList& operator=(const LinkedList& source);

	bool insert(type d, int index);
	bool remove(int index);
	bool isEmpty() const;
	bool isFull() const;
	type peek(int index) const;
	int getCount() const;
};
```
`LinkedList.cpp`
``` cpp
#include "LinkedList.h"

LinkedList::LinkedList()
{
	first = nullptr;
	count = 0;
}

LinkedList::~LinkedList()
{
	while (!isEmpty())
	{
		remove(0);
	}
}

LinkedList::LinkedList(const LinkedList& source)
{
	if (!source.isEmpty())
	{
		first = new Node(source.first->data);

		Node* srcPtr = source.first->next;
		Node* ptr = first;

		while (srcPtr != nullptr)
		{
			ptr->next = new Node(srcPtr->data);
			srcPtr = srcPtr->next;
			ptr = ptr->next;
		}
		count = source.count;
	}
}

LinkedList& LinkedList::operator=(const LinkedList& source)
{
	if (this != &source)
	{
		// removing all nodes 
		while (!isEmpty())
		{
			remove(0);
		}

		if (!source.isEmpty())
		{
			first = new Node(source.first->data);
			Node* srcPtr = source.first->next;
			Node* ptr = first;

			while (srcPtr != nullptr)
			{
				ptr->next = new Node(srcPtr->data);
				srcPtr = srcPtr->next;
				ptr = ptr->next;
			}
			count = source.count;
		}
	}
	return *this;
}
bool LinkedList::insert(type d, int index)
{
	// check if the list is full and index is valid
	if (isFull() || index < 0 || index > count) return false;
	
	Node* ptr = new Node(d);
	if (index == 0) // when inserting data as the first node
	{
		ptr->next = first; // the old first node becomes the second node
		first = ptr;  // the new node becomes the first node
	}
	else // inserting elsewhere in the list
	{
		// getting to the predecessor node
		Node* predecessor = first;
		for (int i = 0; i < index - 1; i++)
		{
			predecessor = predecessor->next;
		}

		ptr->next = predecessor->next; // the node after the predecessor node should now come after the new node
		predecessor->next = ptr; // the new node now comes after the predecessor node.
	}
	count++;
	return true;
}

bool LinkedList::remove(int index)
{
	// check if the list is empty and index is valid
	if (isEmpty() || index < 0 || index >= count) return false;

	Node* target = first;
	if (index == 0) // when removing the first node
	{
		first = first->next; // the second node now becomes the new first node.
	}
	else // when removing node from elsewhere in the list
	{
		Node* predecessor = nullptr;

		// getting the pointers to the nodes.
		for (int i = 0; i < index; i++)
		{
			predecessor = target;
			target = target->next;
		}
		predecessor->next = target->next; //the node right after the target node now comes after the predecessor node
	}
	delete target; // deallocate the node
	count--; 
	return true;
}


bool LinkedList::isEmpty() const
{
	return first ==nullptr;
}

bool LinkedList::isFull() const
{
	Node* temp = new Node(type());
	if (temp == nullptr) return true;
	else 
	{
		delete temp;
		return false;
	}
}

type LinkedList::peek(int index) const
{
	if (isEmpty() || index < 0 || index >= count)
		return type();
	else
	{
		Node* ptr = first;
		for (int i = 0; i < index; i++)
		{
			ptr = ptr->next;
		}
		return ptr->data;
	}
}

int LinkedList::getCount() const
{
	return count;
}
```
Note: The codes for simulation should be in separate files from the linked list.


What to submit: 
* Make sure that your names are in all files submitted.
* Source files (.h and .cpp files).
* Sample run/output.
* Short report (1–2 pages) including:
	* Design explanation
	* Goose selection method and chase simulation (with rationale)
	* Challenges faced
	* Additional features