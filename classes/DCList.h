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

	/**
	 * @brief  Provides the data at a certain element in the list.
	 *
	 * @param element	The element the data is being inserted at.
	 * @return			Type T, the data at the specified position.
	 *
	 * @note
	 *
	*/
	T peek(const int) const;

	/**
	 * @brief  Replaces the data at the chosen position.
	 *
	 * @param data		Data being inserted.
	 * @param element	The element the data is being inserted at.
	 * @return			True or false.
	 *
	 * @note  
	 *       
	*/
	bool replace(const T&, const int);
	bool insert(const T&, const int);
	bool remove(const int);
	const DCList& operator=(const DCList&);
	void display(std::ostream&) const;
	friend std::ostream& operator<< (std::ostream&, const DCList&);
	//-------------------------------------------------------------------------------------------
private:
	Node* first; // pointer to the first element of the linked list
	int size; //count
};


