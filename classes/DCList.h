//using DOXYGEN comments for documentation.

/*Gavin Harban, Jon Moore, Game.h CIS 240-1 Data Structures Project*/

#pragma once
#include<string>
#include<ostream>

typedef std::string T;

class DCList {
private:
	class Node {
	public:
		T data;
		Node* next, * prev;
	};

	/**
	 * @brief			Retreive a node at an index. Used in peek() and subscript operator overloads.
	 * @param index		Specific index to retreive at.
	 * @return Node*
	 */
	Node* retrieveAt(int index) const;

	Node* first; // first element; change this when the head of the list is updated.
	int size;
public:
	DCList();
	DCList(const DCList&);
	~DCList();
	int getSize() const;
	bool isEmpty() const;
	bool isFull() const;

	//-------------------//

	/**
	 * @brief  Provides the data at a certain element in the list.
	 *
	 * @param index		The position of the data being returned.
	 * @return			Type T, the data at the specified position.
	 *
	 * @note			Uses the private helper function retreiveAt(int) to traverse and return the data at the index.
	 *
	*/
	T peek(const int) const;

	/**
	 * @brief  Replaces the data at thechosen position.
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

	/**
	 * @brief			Assignment operator overload.
	 * @param source	Constant object reference. 
	 * @return			This object reference.
	 */
	const DCList& operator=(const DCList&);

	/**
	 * @brief						Displays the whole content of the List.
	 * @param std::ostream& out		Output stream object.
	 * @return void
	 */
	void display(std::ostream&) const;

	/**
	 * @brief						Displays the data at the specified index.
	 * @param std::ostream& out		Output stream operator.
	 * @param int index				Specific index of the data.
	 */
	void display(std::ostream&, int) const;

	/**
	 * @brief							Output stream operator overload.
	 * @param std::ostream& out			Output stream object.  
	 * @param const DCList& content		Constant class reference. Used to represent outputted data.
	 * @return std::ostream& 
	 */
	friend std::ostream& operator<< (std::ostream&, const DCList&);

	/**
	 * @brief			Subscript operator overload.
	 * @param index		Specific subscript.
	 * @return T& 
	 */
	T& operator[](int index);
	/**
	 * @brief			Subscript operator overload (constant compliance).
	 * @param index		Specific subscript. 
	 * @return const T& 
	 */
	const T& operator[](int index) const;

};

