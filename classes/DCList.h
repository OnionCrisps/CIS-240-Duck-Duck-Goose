//using DOXYGEN comments for documentation.
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
	DCList();
	DCList(const DCList&);
	~DCList();
	int getSize() const;
	bool isEmpty() const;
	bool isFull() const;

	/**
	 * @brief  Provides the data at a certain element in the list.
	 *
	 * @param index		The position of the data being returned.
	 * @return			Type T, the data at the specified position.
	 *
	 * @note
	 *
	*/
	T peek(const int) const;

	/**
	 * @brief  Replaces the data at the chosen position.
	 *
	 * @param data		The new data.
	 * @param index		The position of the data being replaced.
	 * @return			True or false.
	 *
	 * @note  
	 *       
	*/
	bool replace(const T&, const int);

	/**
	* @brief		Insert data at a specific index.
	* @param data	The new data.
	* @param index	The position in the list the data is being inserted at.
	* 
	* @return		True or false.
	*/
	bool insert(const T&, const int);

	/**
	 * @brief			Remove data at a specific index.
	 * @param index		The position of the data to be removed.
	 * @return			True or false.
	 */
	bool remove(const int);

	const DCList& operator=(const DCList&);

	void display(std::ostream&) const;

	friend std::ostream& operator<< (std::ostream&, const DCList&);

private:
	Node* first; // pointer to the first element of the linked list
	int size;
};


