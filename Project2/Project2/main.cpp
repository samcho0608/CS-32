#include "BballRoster.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	BballRoster mwp;
	assert(mwp.signPlayer("Adam", "Wright", 41));
	assert(mwp.signPlayer("Jason", "Falitz", 37));
	assert(mwp.howManyPlayers() == 2);
	string first, last;
	int ac;
	assert(mwp.choosePlayer(0, first, last, ac) && ac == 37);
	assert(mwp.choosePlayer(1, first, last, ac) && (first ==
		"Adam"
		&& ac == 41));

	BballRoster clippers, result;

	clippers.signPlayer("Brook", "Lopez", 11);
	clippers.signPlayer("Robin", "Lopez", 8);
	clippers.signPlayer("Greek", "Freak", 34);
	string f, l;
	SomeType v;
	assert(clippers.choosePlayer(2, f, l, v) && f == "Robin" && l == "Lopez");
	checkRoster("*", "*", clippers, result);
	assert(result.howManyPlayers() == 3);
	checkRoster("*", "Lopez", clippers, result);
	assert(result.howManyPlayers() == 2);
	checkRoster("Greek", "*", clippers, result);
	assert(result.howManyPlayers() == 1);

	BballRoster a, b, ab;
	a.signPlayer("Kyrie", "Irving", 11);
	a.signPlayer("Kevin", "Durant", 7);
	a.signPlayer("DeAndre", "Jordan", 6);
	b.signPlayer("Kevin", "Durant", 7);
	b.signPlayer("Spencer", "Dinwiddie", 8);
	joinRosters(a, b, ab);
	assert(ab.howManyPlayers()==4);
	b.resignPlayer("Kevin", "Durant", 35);
	joinRosters(a, b, ab);
	assert(ab.howManyPlayers() == 3);

	return;
}
int main()
{
	test();
	cout << "Passed all tests" << endl;
	return 0;
}