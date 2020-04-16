#ifndef WORDTREE_H
#define WORDTREE_H

#include <iostream>
#include <string>
using namespace std;

typedef string IType;

struct WordNode {
	IType m_data;
	WordNode *m_left;
	WordNode *m_right;
	int count = 1;
	WordNode() : m_data(""), m_left(nullptr), m_right(nullptr) {}
	WordNode(IType v) : m_data(v), m_left(nullptr), m_right(nullptr) {}
	// You may add additional data members and member functions
	// in WordNode
};

class WordTree;

ostream& operator<<(ostream& stream, const WordTree& tree);

class WordTree {
private:
	WordNode *root;
	void copyTree(WordNode*& thisRoot, WordNode* sourceRoot);
	void clearTree(WordNode* thisRoot);
	void countDistinct(int& count, WordNode* thisRoot) const;
	void printRec(ostream& out, WordNode* root) const;
	void countTotal(int& count, WordNode* thisRoot) const;
public:
	// default constructor
	WordTree() : root(nullptr) { };

	// copy constructor
	WordTree(const WordTree& rhs);

	// assignment operator
	const WordTree& operator=(const WordTree& rhs);
	// Inserts v into the WordTree
	void add(IType v);

	// Returns the number of distinct words / nodes
	int distinctWords() const;
	// Returns the total number of words inserted, including
	// duplicate values
	int totalWords() const;
	
	// Prints the LinkedList
	friend ostream& operator<<(ostream &out, const WordTree&
		rhs);
	// Destroys all the dynamically allocated memory in the
	// tree
	~WordTree();
};

#endif