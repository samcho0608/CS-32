#include "LinkedList.h"
#include <iostream>
using namespace std;

// default constructor
LinkedList::LinkedList() : head(nullptr), tail(nullptr) {}

// copy constructor
LinkedList::LinkedList(const LinkedList& rhs)
{
	if (rhs.head == nullptr) head = tail = nullptr;
	else
	{
		/*Node* p = rhs.head;
		while (p != nullptr)
		{
			Node* temp = new Node;
			temp->previous = tail;
			if (tail != nullptr)
				tail->next = temp;
			temp->value = p->value;
			tail = temp;
			if (p->previous == nullptr)
				head = temp;
			p = p->next;
		}*/
		if (rhs.head == nullptr) head = nullptr;
		else
		{
			Node* p = rhs.tail;
			while (p != nullptr)
			{
				Node* q = new Node;
				q->value = p->value;
				q->next = head;
				q->previous = nullptr;
				head = q;
				p = p->previous;
			}
		}
	}
}

// Destroys all the dynamically allocated memory
// in the list.
LinkedList::~LinkedList()
{
/*	Node *p, *n;
	p = head;
	while (p != nullptr)
	{
		n = p->next;
		delete p;
		p = n;
	}*/

	Node *p;
	while (head != nullptr)
	{
		p = head;
		head = head->next;
		delete p;
	}
}

// assignment operator
/*const LinkedList& LinkedList::operator=(const LinkedList& rhs)
{
	Node *p;
	p = head;
	while (p != nullptr)
	{
		delete p->previous;
		p = p->next;
	}
	delete tail;

	Node* h = rhs.tail;

	while (h != nullptr)
	{
		Node *t;
		t = new Node;
		t->value = h->value;
		t->previous = nullptr;
		t->next = head;
		head = t;
		if (t->next == nullptr)
			tail = t;
		h = h->previous;
	}
	return *this;
}*/
const LinkedList& LinkedList::operator=(const LinkedList& rhs)
{
	if (this == &rhs) return *this;
	LinkedList temp = rhs;
	swap(temp);
	return *this;
}

// Inserts val at the front of the list
void LinkedList::insertToFront(const ItemType &val)
{
	Node *p = new Node;
	p->value = val;
	p->previous = nullptr;
	p->next = head;
	if (head != nullptr)
		head->previous = p;
	head = p;
	if (p->next == nullptr)
		tail = p;
	
}

// Prints the LinkedList
void LinkedList::printList() const
{
	Node* p;
	p = head;
	while (p != nullptr)
	{
		cout << p->value << endl;
		p = p->next;
	}
}

// Sets item to the value at position i in this
// LinkedList and return true, returns false if 
// there is no element i
bool LinkedList::get(int i, ItemType& item) const
{
	int count = 0;
	Node *p;
	p = head;
	while (p != nullptr)
	{
		if (count == i)
		{
			item = p->value;
			return true;
		}
		count++;
		p = p->next;
	}
	return false;
}

// Reverses the LinkedList
void LinkedList::reverseList()
{
	if (head == nullptr || head->next == nullptr)
		return;
	Node* p;
	p = head;
	while (p != nullptr)
	{
		Node* temp = p->next;
		p->next = p->previous;
		p->previous = temp;
		p = temp;
	}
	head = tail;
	tail = p;
}

// Prints the LinkedList in reverse order
void LinkedList::printReverse() const
{
	Node* p = tail;
	while (p != nullptr)
	{
		cout << p->value << endl;
		p = p->previous;
	}
}

// Appends the values of other onto the end of this
// LinkedList.
void LinkedList::append(const LinkedList &other)
{
	LinkedList a = other;
	Node* p = a.head;
	tail->next = p;
	p->previous = tail;
	tail = a.tail;
}

// Exchange the contents of this LinkedList with the other
// one.
void LinkedList::swap(LinkedList &other)
{
	Node* p = head;
	head = other.head;
	other.head = p;
	p = tail;
	tail = other.tail;
	other.tail = p;
}

// Returns the number of items in the Linked List.
int LinkedList::size() const
{
	int count = 0;
	Node* p = head;
	while (p != nullptr)
	{
		count++;
		p = p->next;
	}
	return count;
}
