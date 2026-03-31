#include "DCList.h"

T DCList::peek(int index) const
{
	if (isEmpty()) return T();
	else
	{
		Node* ptr = first; //first index (CAN BE VIEWED AS FIRST ELEMENT IN THE LIST OR ID:0) is ptr->first
		for (int i = 0; i < index; i++)
		{
			ptr = ptr->next;
		}
		return ptr->data;
	}
}
/**

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

*/
