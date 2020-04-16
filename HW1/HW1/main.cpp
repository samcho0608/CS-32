#include <iostream>
#include <string>
#include "LinkedList.h"
#include <cassert>
using namespace std;

int main()
{
	LinkedList ls;
	ls.insertToFront("Steve");
	ls.insertToFront("Judy");
	ls.insertToFront("Laura");
	ls.insertToFront("Eddie");
	ls.insertToFront("Hariette");
	ls.insertToFront("Carl");
	ls.reverseList();
	ls.printList();
	
	for (int k = 0; k < ls.size(); k++)
	{
		string x;
		ls.get(k, x);
		cout << x << endl;
	}
	
	LinkedList e1;
	e1.insertToFront("bell");
	e1.insertToFront("biv");
	e1.insertToFront("devoe");
	LinkedList e2;
	e2.insertToFront("Andre");
	e2.insertToFront("Big Boi");
	//e1.append(e2); // adds contents of e2 to the end of e1
	string s;
	cout << e1.size();
	e1.get(3, s);
	cout << s;
	cout << e2.size();
	e2.get(1, s);
	cout << s;
	LinkedList e3;
	e3.insertToFront("A");
	e3.insertToFront("B");
	e3.insertToFront("C");
	e3.insertToFront("D");
	LinkedList e4;
	e4.insertToFront("X");
	e4.insertToFront("Y");
	e4.insertToFront("Z");
	e3.swap(e4); // exchange contents of e1 and e2
	string sa;
	cout << e3.size();
	e3.get(0, sa);
	cout << sa;
	cout << e4.size();
	e4.get(2, sa);
	cout << sa;

	e3 = e4;
	e3.printList();
}