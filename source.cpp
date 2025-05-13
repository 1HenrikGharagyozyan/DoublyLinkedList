#include <iostream>
#include <list>

#include "DoublyLinkedList.h"

int main()
{
	DoublyLinkedList<int> list = { 3, 9, 9, 1, 22, 89, 99 };

	list.print();

	list.push_back(999);
	list.push_front(355);

	list.print();

	list.insert(list.begin() + 4, 9999);

	list.sort();

	list.print();

	for (const auto& elem : list)
		std::cout << elem << " ";
}