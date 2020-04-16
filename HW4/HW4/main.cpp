#include "WordTree.h"
using namespace std;

int main()
{
	WordTree test;
	test.add("abe"); // checks add when root is null
	test.add("brad"); // checks add when root isn't null
	WordTree copy;
	copy = test;	// checks assignment c'tor
	cout << test;	// checks <<
	cout << copy;

	WordTree w;

	w.add("Harry");
	w.add("Niall");
	w.add("Niall");	// input of repetitive item
	w.add("Liam");
	w.add("Louis");
	w.add("Harry");
	w.add("Niall");
	w.add("Zayn");

	cout << w; // checks the order or the leaves in the tree

	test = w; // checks copy c'tor

	cout << test;
	cout << w.distinctWords() << endl; // checks distinctWords
	cout << w.totalWords() << endl;	// checks totalWords()
	WordTree zero;
	cout << zero.distinctWords() << endl;	// checks the above two functions
	cout << zero.totalWords() << endl;		// when there is no element
}