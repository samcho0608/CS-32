#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Returns the product of two non-negative integers, m and n,
// using only repeated addition.
int prod(unsigned int m, unsigned int n)
{
	if (n == 0)
		return 0;
	return m + prod(m, n - 1);
}
// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9
// inclusive.
//
// Pseudocode Example:
// numberOfDigits(18838, 8) => 3
// numberOfDigits(55555, 3) => 0
// numberOfDigits(0, 0) => 0 or 1 (either if fine)
//
int numberOfDigits(int num, int digit)
{
	if (num == 0)
		return 0;
	else
	{
		if (num % 10 != digit)
			return 0 + numberOfDigits(num / 10, digit);
		else
			return 1 + numberOfDigits(num / 10, digit);
	}
}
// Returns a string where the same characters next each other in
// string n are separated by "22"
//
// Pseudocode Example:
// doubleDouble("goodbye") => "go22odbye"
// doubleDouble("yyuu") => "y22yu22u"
// doubleDouble("aaaa") => "a22a22a22a"
//
string doubleDouble(string n)
{
	if (n.size() == 1)
		return n;
	else
	{
		if (n[0] == n[1])
			return n.substr(0, 1) + "22" + doubleDouble(n.substr(1));
		else
			return n.substr(0, 1) + doubleDouble(n.substr(1));
	}
}

// str contains a single pair of curly brackets, return a new
// string made of only the curly brackets and whatever those
// curly brackets contain
//
// Pseudocode Example:
// curlyFries("abc{ghj}789") => "{ghj}"
// curlyFries("{x}7") => "{x}"
// curlyFries("4agh{y}") => "{y}"
// curlyFries("4agh{}") => "{}"
//
string curlyFries(string str)
{
	if (str[0] == '{'&& str[str.size() - 1] == '}')
		return str;
	else
	{
		if (str[str.size() - 1] != '}')
			return curlyFries(str.substr(0, str.size() - 1));
		else if (str[0] != '{')
			return curlyFries(str.substr(1));
	}
	return "";
}
// Return true if the total of any combination of elements in
// the array a equals the value of the target.
//
// Pseudocode Example:
// addEmUp([2, 4, 8], 3, 10) => true
// addEmUp([2, 4, 8], 3, 6) => true
// addEmUp([2, 4, 8], 3, 11) => false
// addEmUp([2, 4, 8], 3, 0) => true
// addEmUp([], 0, 0) => true  
//
bool addEmUp(const int a[], int size, int target) 
{
	if (target == 0)
		return true;
	else if (size == 0)
		return false;
	else
		return (addEmUp(a + 1, size - 1, target - *a) || addEmUp(a + 1, size - 1, target));
	// either element is included or not
}
bool canWeFinish(string maze[], int nRows, int nCols,
	int sr, int sc, int er, int ec)
{
	maze[sr][sc] = 'V';
	if (sr == er && sc == ec)
		return true;
	if (maze[sr + 1][sc] == '.')
		if(canWeFinish(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	if (maze[sr][sc-1] == '.')
		if(canWeFinish(maze, nRows, nCols, sr, sc - 1, er, ec))
			return true;
	if (maze[sr][sc+1] == '.')
		if(canWeFinish(maze, nRows, nCols, sr, sc + 1, er, ec))
			return true;
	if (maze[sr - 1][sc] == '.')
		if(canWeFinish(maze, nRows, nCols, sr - 1, sc, er, ec ))
			return true;
	return false;
}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

int main()
{
	assert(prod(0, 0) == 0);
	assert(prod(4, 5) == 20);

	assert(numberOfDigits(18838, 8) == 3);
	assert(numberOfDigits(55555, 3) == 0);
	assert(numberOfDigits(0, 0) == 0 || numberOfDigits(0, 0) == 1);
	assert(numberOfDigits(10000, 0) == 4); // 4

	assert(doubleDouble("goodbye") == "go22odbye");
	assert(doubleDouble("yyuu") == "y22yu22u");
	assert(doubleDouble("aaaa") == "a22a22a22a");

	assert(curlyFries("abc{ghj}789") == "{ghj}");
	assert(curlyFries("{x}7") == "{x}");
	assert(curlyFries("4agh{y}") == "{y}");
	assert(curlyFries("4agh{}") == "{}");

	int a[4] = { 2,4,3,6 };
	assert(!addEmUp(a, 4, 20));

	int b[3] = {2, 4, 8};
	assert(addEmUp(b, 3, 6));
	assert(!addEmUp(b, 3, 11));
	assert(addEmUp(b, 3, 0));

	int c[1];
	assert(addEmUp(c, 3, 0));

	int d[4] = {6,4,8,7};
	assert(addEmUp(d,4,17));


	cout << "All tests except maze passed!" << endl;

	string maze[10] = {
 "XXXXXXXXXX",
 "X.......@X",
 "XX@X@@.XXX",
 "X..X.X...X",
 "X..X...@.X",
 "X....XXX.X",
 "X@X....XXX",
 "X..XX.XX.X",
 "X...X....X",
 "XXXXXXXXXX"
	};

	if (canWeFinish(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << maze[i] << endl;
	}
}