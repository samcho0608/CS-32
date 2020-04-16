#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <string>
using namespace std;

const int MAXRESULTS = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

// loadWords functions
int loop(int i, int max, istream &dictfile, string dict[])
{
	if (dictfile.eof() || i >= max)	// if end of textfile reached or MAXDICTWORDS reached
		return i; 
	getline(dictfile, dict[i]);
	return loop(i + 1, max, dictfile, dict);
}

int loadWords(istream &dictfile, string dict[])
{
	return loop(0, MAXDICTWORDS, dictfile, dict);
}

// printPermutation functions
void printPermutations(string prefix, string rest);

void permLoop(int i, int max, string prefix, string rest)
{
	if (i >= max)
		return;
	else 
	{
		char c = rest[i];
		prefix += c;
		rest.erase(i, 1);
		printPermutations(prefix, rest);

		rest.insert(i, 1, c);
		prefix.erase(prefix.length() - 1, 1);

		permLoop(i+1, max, prefix, rest);
	}
}

void printPermutations(string prefix, string rest)
{
	if (rest.empty())
		cout << prefix << endl;
	else
	{
		permLoop(0, rest.size(), prefix, rest);
	}
}


//recBlends functions
//1. find the permutation
//2. check dict
//3. add if exists in dict and is unique

// returns true if it's unique, false otherwise.
bool uniqueLoop(int i, int max, string word, string results[])
{
	if (i >= max)
		return true;
	else if (results[i] == word)
		return false;
	else
		return uniqueLoop(i + 1, max, word, results);
}

bool dictLoop(int i, int max, const string dict[], string prefix)	// loop through dict to see if any matches prefix
{
	if (i >= max)	// if end reached
		return false;
	else if (dict[i] == prefix)	// if an element of dict matches prefix
		return true;
	else
		return dictLoop(i + 1, max, dict, prefix);
}

void findPerm(string prefix, string rest, const string dict[],
	int size, string results[], int& count);

void permLoop(int i, int max, string prefix, string rest, const string dict[],
	int size, string results[], int& count)
{
	if (i >= max)
		return;
	else
	{
		// choose a character
		char c = rest[i];
		prefix += c;
		rest.erase(i, 1);
		findPerm(prefix, rest, dict, size, results, count);

		// unchoose a character
		rest.insert(i, 1, c);
		prefix.erase(prefix.length() - 1, 1);

		permLoop(i + 1, max, prefix, rest, dict, size, results, count);
	}
}

void findPerm(string prefix, string rest, const string dict[], 
	int size, string results[], int& count)
{
	if (rest.empty())
	{
		if (dictLoop(0, size, dict, prefix))
		{
			if (count < MAXRESULTS && uniqueLoop(0, count, prefix, results))
				results[count++] = prefix;
			else
				return;
		}
		else
			return;
	}
	else
		permLoop(0, rest.size(), prefix, rest, dict, size, results, count);
}

int recBlends(string word, const string dict[], int size, string
	results[])
{
	int count = 0;
	findPerm("", word, dict, size, results, count);
	return count;
}

void resultsLoop(int i, int max, const string results[])
{
	if (i >= max)
		return;
	else
	{
		cout << results[i] << endl;
		resultsLoop(i+1, max, results);
	}
}

void showResults(const string results[], int size)
{
	resultsLoop(0, size, results);
	return;
}

int main()
{
	string results[MAXRESULTS];
	string dict[MAXDICTWORDS];
	ifstream dictfile;         // file containing the list of words
	int nwords;                // number of words read from dictionary
	string word;

	dictfile.open("words.txt");
	if (!dictfile) {
		cout << "File not found!" << endl;
		return (1);
	}

	nwords = loadWords(dictfile, dict);
	cout << nwords << endl;

	printPermutations("", "art");
	cout << "Please enter a string for an anagram: ";
	cin >> word;

	int numMatches = recBlends(word, dict, nwords, results);
	cout << numMatches << endl;
	if (!numMatches)
		cout << "No matches found" << endl;
	else
		showResults(results, numMatches);
}