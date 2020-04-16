#include "BballRoster.h"
#include <iostream>
using namespace std;

BballRoster::BballRoster() : head(nullptr), tail(nullptr) {}

BballRoster::BballRoster(const BballRoster& other)
{
	if (other.head == nullptr) head = tail = nullptr;
	else
	{
		Node* p = other.tail;
		while (p != nullptr)
		{
			Node* q = new Node;
			q->firstName = p->firstName;
			q->lastName = p->lastName;
			q->value = p->value;
			q->next = head;
			q->previous = nullptr;
			head = q;
			if (p == other.tail)
				tail = q;
			p = p->previous;
		}
	}
}

const BballRoster& BballRoster::operator=(const BballRoster& other)
{
	if (head == nullptr);
	else if (head->next == nullptr)
		delete head;
	else
	{
		Node* p;
		while (head != nullptr)
		{
			p = head;
			head = head->next;
			delete p;
		}
	}
	if (this == &other) return *this;
	BballRoster temp;
	Node* p = other.head;
	while (p != nullptr)
	{
		temp.signPlayer(p->firstName, p->lastName, p->value);
		p = p->next;
	}
	swapRoster(temp);
	return *this;
}

BballRoster::~BballRoster()
{
	if (head == nullptr);
	else if (head->next == nullptr)
		delete head;
	else
	{
		Node* p;
		while (head != nullptr)
		{
			p = head;
			head = head->next;
			delete p;
		}
	}
}

bool BballRoster::rosterEmpty() const
{
	if (head == nullptr)
		return true;
	return false;
}

int BballRoster::howManyPlayers() const
{
	if (head == nullptr)
		return 0;
	int count = 0;
	Node* p = head;
	while (p != nullptr)
	{
		count++;
		p = p->next;
	}
	return count;
}

bool BballRoster::signPlayer(const string& firstName, const string& lastName, const SomeType& value)	// check this again
{
	if (rosterEmpty())
	{
		Node* p = new Node;
		p->firstName = firstName;
		p->lastName = lastName;
		p->value = value;
		p->previous = nullptr;
		p->next = nullptr;
		head = tail = p;
		return true;
	}
	else if (playerOnRoster(firstName, lastName))	// if player with same name found
		return false;
	else
	{
		Node* p = head;
		Node* newPlayer = new Node;
		newPlayer->firstName = firstName;
		newPlayer->lastName = lastName;
		newPlayer->value = value;

		while (p != nullptr)
		{
			if (lastName < p->lastName)	//when a lastname with lower alphabetical order found
			{
				newPlayer->previous = p->previous;
				newPlayer->next = p;
				if (p != head)
					p->previous->next = newPlayer;
				p->previous = newPlayer;
				if (p == head)
					head = newPlayer;
				return true;
			}

			else if (lastName == p->lastName) // if lastname is the same as parameter
			{
				Node* q = p;
				while (q->lastName == lastName)
				{
					if (firstName < q->firstName) //when a firstname with lower alphabetical order found
					{
						newPlayer->previous = q->previous;
						newPlayer->next = q;
						if (q != head)
							q->previous->next = newPlayer;
						q->previous = newPlayer;
						if (q == head)
							head = newPlayer;
						return true;
					}
					if (q->next == nullptr)
					{
						newPlayer->previous = q;
						newPlayer->next = nullptr;
						q->next = newPlayer;
						tail = newPlayer;
						return true;
					}
					q = q->next;
				}
				q = q->previous;
				newPlayer->previous = q;
				newPlayer->next = q->next;
				if (q != tail)
					q->next = newPlayer;
				q->next->previous = newPlayer;
				if (q == tail)
					tail = newPlayer;
				return true;
			}
		
			p = p->next;
		}

		//if newPlayer is the very last one
		newPlayer->previous = tail;
		newPlayer->next = nullptr;
		tail->next = newPlayer;
		tail = newPlayer;
		return true;
	}
}

bool BballRoster::resignPlayer(const std::string& firstName, const std::string&
	lastName, const SomeType& value)
{
	if (!playerOnRoster(firstName, lastName))
		return false;
	else
	{
		Node* p = head;
		while (p != nullptr)
		{
			if (p->firstName == firstName && p->lastName == lastName)
			{
				p->value = value;
				return true;
			}
			p = p->next;
		}
		return true;	// to avoid warning
	}
}

bool BballRoster::signOrResign(const std::string& firstName, const std::string&
	lastName, const SomeType& value)
{
	if (playerOnRoster(firstName, lastName))
		return resignPlayer(firstName, lastName, value);
	else
		return signPlayer(firstName, lastName, value);
}

bool BballRoster::renouncePlayer(const std::string& firstName, const
	std::string& lastName)
{
	if (!playerOnRoster(firstName, lastName))
		return false;
	else
	{
		Node* p = head;
		while (p != nullptr)
		{
			if (p->firstName == firstName && p->lastName == lastName)
			{
				p->previous->next = p->next;
				if(p != tail)
					p->next->previous = p->previous;
				delete p;
				return true;
			}
			p = p->next;
		}
		return true;	// this is to avoid the warning
	}
}

bool BballRoster::playerOnRoster(const std::string& firstName, const
	std::string& lastName) const
{
	if (rosterEmpty())
		return false;
	else
	{
		Node* p = head;
		while (p != nullptr)
		{
			if (p->firstName == firstName && p->lastName == lastName)
				return true;
			p = p->next;
		}
		return false;
	}	
}

bool BballRoster::lookupPlayer(const std::string& firstName, const std::string&
	lastName, SomeType& value) const
{
	if (!playerOnRoster(firstName, lastName))
		return false;
	else
	{
		Node* p = head;
		while (p != nullptr)
		{
			if (p->firstName == firstName && p->lastName == lastName)
			{
				value = p->value;
				return true;
			}
			p = p->next;
		}
		return true;
	}
}

bool BballRoster::choosePlayer(int i, std::string& firstName, std::string&
	lastName, SomeType& value) const
{
	if (i >= 0 || i < howManyPlayers())
	{
		Node* p = head;
		int count = 0;
		while (p != nullptr)
		{
			if (count == i)
			{
				firstName = p->firstName;
				lastName = p->lastName;
				value = p->value;
				return true;
			}
			count++;
			p = p->next;
		}
		return false;
	}
	else
		return false;
}

void BballRoster::swapRoster(BballRoster& other) 
{
	Node* p = head;
	head = other.head;
	other.head = p;
	p = tail;
	tail = other.tail;
	other.tail = p;
}

bool joinRosters(const BballRoster & bbOne,
	const BballRoster & bbTwo,
	BballRoster & bbJoined)
{
	if (bbOne.rosterEmpty())
	{
		bbJoined = bbTwo;
		return true;
	}
	else if (bbTwo.rosterEmpty())
	{
		bbJoined = bbOne;
		return true;
	}
	else // neither rosters empty
	{
		string f, l;
		SomeType v;
		BballRoster small;
		bool returnVal = true;

		// comparing size of two rosters and assign to bbJoined and small accordingly
		if (bbTwo.howManyPlayers() >= bbOne.howManyPlayers())
		{
			bbJoined = bbTwo;
			small = bbOne;
		}
		else
		{
			bbJoined = bbOne;
			small = bbTwo;
		}

		// sign each player in the smaller roster to the bbJoined
		for (int i = 0; i < small.howManyPlayers(); i++)
		{
			small.choosePlayer(i, f, l, v);
			if (bbJoined.playerOnRoster(f, l))
			{
				SomeType vRepeated;
				if (bbJoined.lookupPlayer(f, l, vRepeated) && v != vRepeated)
				{
					bbJoined.renouncePlayer(f, l);
					returnVal = false;
				}
			}
			else
				bbJoined.signPlayer(f, l, v);
		}
		return returnVal;

	}



}

void checkRoster(const std::string& fsearch,
	const std::string& lsearch,
	const BballRoster& bbOne,
	BballRoster& bbResult)
{
	BballRoster temp;
	if (fsearch == "*" && lsearch == "*")
	{
		bbResult = bbOne;
		return;
	}
	string fname, lname;
	SomeType v;
	for (int i = 0; i < bbOne.howManyPlayers(); i++)
	{
		bbOne.choosePlayer(i, fname, lname, v);	// check every player in bbOne
		if(fname == fsearch || lname == lsearch)	//if either lastname or firstname is the same as input
		{
			if (fsearch == "*")
				lname = lsearch;
			else // case with both "*" was taken care of at the top
				fname = fsearch;
			if (bbOne.playerOnRoster(fname, lname))
				temp.signPlayer(fname, lname, v);
		}
		if (lname != "*" && lname > lsearch)
			break;
	}
	bbResult = temp;
}