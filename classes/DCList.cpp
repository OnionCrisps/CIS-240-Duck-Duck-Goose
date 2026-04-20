/*Gavin Harban, Jon Moore, Game.h CIS 240-1 Data Structures Project*/

#include "DCList.h"

DCList::DCList()
{
    first = nullptr;
    size = 0;
}

DCList::DCList(const DCList& source) {
    first = nullptr;
    size = 0;
    if (source.isEmpty()) return;
    for (int i = 0; i < source.size; i++) {
        insert(source.peek(i), size); //add to the end
    }
}

DCList::~DCList()
{//scalar deleting destructor unsigned int (warning?) in disassembly view
    while (!isEmpty()) {
        remove(0);
    }
}

int DCList::getSize() const
{
    return size;
}

bool DCList::isEmpty() const
{
    return(size == 0);
}

bool DCList::isFull() const
{
    {
        Node* temp = new Node(T());
        if (temp == nullptr) return true;
        else
        {
            delete temp;
            return false;
        }
    }

}
T DCList::peek(int index) const
{
	if (isEmpty()) return T();
	else
	{
		return retrieveAt(index)->data;
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

    // find the actual target
    if (index == 0) {
        target = first;
    } else if (index >= (size / 2)) {
        for (int i = size - 1; i > index; --i)
            target = target->prev;
    } else {
        Node* predecessor = first;
        for (int i = 0; i < index - 1; ++i)
            predecessor = predecessor->next;
        target = predecessor->next;
    }

    if (size == 1) {
        // deleting the only node
        delete target;
        first = nullptr;
        size = 0;
        return true;
    }

    // unlink target
    target->prev->next = target->next;
    target->next->prev = target->prev;

    if (target == first) {
        first = target->next; // update head if needed
    }

    delete target;
    --size;
    return true;
}
const DCList& DCList::operator=(const DCList& source) {
    if (this == &source) {return *this;}

    // clear current
    while (!isEmpty()) remove(0);

    if (source.isEmpty()){return *this;}

    // copy first
    first = new Node{source.first->data, nullptr, nullptr};
    Node* srcPtr = source.first->next;
    Node* dst = first;

    // create remaining nodes until we circle back to source.first
    while (srcPtr != source.first) {
        Node* newNode = new Node{srcPtr->data, nullptr, dst};
        dst->next = newNode;
        dst = newNode;
        srcPtr = srcPtr->next;
    }

    // close the circle
    dst->next = first;
    first->prev = dst;

    size = source.size;
    return *this;
}

void DCList::display(std::ostream& out) const
{
	Node* ptr = first;
	out << "LIST CONTENT: "<<std::endl;
	for (int i = 0; i < size; i++)
	{
		out 
			<< "Element " << i+1 << ": " 
			<< ptr->data << " at address: " 
			<< ptr << " | Prev: (" << ptr->prev->data <<" @ "<< ptr->prev <<") | Next: ("
			<< ptr->next->data <<" @ " << ptr->next <<")"<< std::endl;
		ptr = ptr->next;
	}
	out << "First Element: "<< first->data <<" @ " << first << std::endl;
	out << "Last Element: " << first->prev->data << " @ " << first->prev << std::endl;
}
void DCList::display(std::ostream& out, int index) const
{
	out << peek(index) << std::endl;
}

T& DCList::operator[](int index)
{
	return retrieveAt(index)->data;
}
const T& DCList::operator[](int index) const
{
	return retrieveAt(index)->data;
}
std::ostream& operator<<(std::ostream& out, const DCList& content)
{
	content.display(out);
	return out;
}

DCList::Node* DCList::retrieveAt(int index) const
{
	Node* ptr = first; //first index is ptr->first
	for (int i = 0; i < index; i++)
	{
		ptr = ptr->next;
	}
	return ptr;
}



