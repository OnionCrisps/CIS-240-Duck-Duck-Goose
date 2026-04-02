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

bool DCList::replace(const T& data, const int index)
{

	if (index >= 0 && index < size) 
	{
		if (index <= (size / 2))
		{
			Node* ptr = first;
			for (int i = 0; i < index; i++)
			{
				ptr = ptr->next;
			}
			ptr->data = data;
			return true;
		}
		else {
			Node* ptr = first->prev; // last element
			for (int i = size - 1; i > index; i--) {
				ptr = ptr->prev;
			}
			ptr->data = data;
			return true;
		}
	}
	return false;
}

bool DCList::insert(const T& data, const int index)
{
	if (isFull() || index < 0 || index > size) return false;


	Node* ptr = new Node(data);
	Node* predecessor;

	if (first==nullptr)
	{
		ptr->next = ptr->prev = ptr;
		first = ptr;

		size++;
		return true;
	}

	if (index == 0)
	{
		Node* tail = first->prev;

		ptr->next = first;
		ptr->prev = tail;

		first->prev = ptr;
		tail->next = ptr;
		first = ptr;

	} 
	else if (index < (size / 2))
	{
		predecessor = first;

		for (int i = 0; i < index - 1; i++)
		{
				predecessor = predecessor->next;
		}
		ptr->next = predecessor->next;
		predecessor->next = ptr;

		ptr->prev = predecessor;
		ptr->next->prev = ptr;

	} else {
		predecessor = first->prev;
		for (int i = size - 1; i >= index; i--)
		{
			predecessor = predecessor->prev;
		}
		ptr->next = predecessor->next;
		predecessor->next = ptr;

		ptr->prev = predecessor;
		ptr->next->prev = ptr;
	}


	size++;
	return true;
}

bool DCList::remove(const int index)
{
	if (isEmpty() || index < 0 || index >= size) return false;
	
	Node* target = (index < (size/2)) ? first : first->prev;
	Node* predecessor = nullptr;

	if (index == 0)
	{
		first = first->next;
	}
	else if (index >= (size / 2))
	{
		for (int i = size - 1; i > index; i--)
		{
			target = target->prev;
		}

		target->prev->next = target->next;
		target->next->prev = target->prev;
	}
	else {
		// getting the pointers to the nodes.
		for (int i = 0; i < index; i++)
		{
			predecessor = target;
			target = target->next;
		}
		predecessor->next = target->next;
	}

	delete target;
	size--;
	return true;

}
/**
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
		remove(3)
		A->B->C->D->E
		0  1  2  3  4
		i = 0, i < 3
		i = 0:
		pred = target = first = 0;
		target = 0->next = 1;
		i++;
		i = 1;
		pre = target = 1;
		target = 1->next = 2;
		i++;
		i = 2;
		pre = target = 2;
		target = 2->next = 3;
		i++;
		i = 3 = break;

		target = 3, pre = 2


			predecessor = target;
			target = target->next;
		}
		pre->next is 3, target->next is 4
		so 3 = 4

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
