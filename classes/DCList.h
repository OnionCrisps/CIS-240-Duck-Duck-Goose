#pragma once
#include<string>
#include<iostream>

typedef std::string T;

class DCList {
private:
	class Node {
	public:
		T data;
		Node* next, * prev;
	};
public:
	//JON:
	DCList();
	DCList(const DCList&);
	~DCList();
	int getSize() const;
	bool isEmpty() const;
	bool isFull() const;
	//------------------------------------------------------------------------------------------
	//GAVIN:
	T peek(const int) const;
	bool replace(const T&, const int); // replacing the data in a specified node with new data.
	bool insert(const T&, const int);
	bool remove(const int);
	const DCList& operator=(const DCList&);
	void display(std::ostream&) const;
	friend std::ostream& operator<< (std::ostream&, const DCList&);
	//-------------------------------------------------------------------------------------------
private:
	Node* first; // pointer to the first element of the linked list
	int count;
};


