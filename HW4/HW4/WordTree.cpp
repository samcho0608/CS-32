#include "WordTree.h"

void WordTree::copyTree(WordNode*& thisRoot, WordNode* sourceRoot)
{
	if (sourceRoot == nullptr)
		thisRoot = sourceRoot;
	else
	{
		thisRoot = new WordNode(sourceRoot->m_data);
		thisRoot->count = sourceRoot->count;
		copyTree(thisRoot->m_left, sourceRoot->m_left);
		copyTree(thisRoot->m_right, sourceRoot->m_right);
	}
}

void WordTree::clearTree(WordNode* thisRoot)
{
	if (thisRoot == nullptr)
		return;
	else
	{
		clearTree(thisRoot->m_left);
		clearTree(thisRoot->m_right);
		delete thisRoot;
	}
}

// copy constructor
WordTree::WordTree(const WordTree& rhs)
{
	copyTree(root, rhs.root);
}

// assignment operator
const WordTree& WordTree::operator=(const WordTree& rhs)
{
	clearTree(root);
	copyTree(root, rhs.root);
	return *this;
}

// Inserts v into the WordTree
void WordTree::add(IType v)
{
	if (root == NULL)
		root = new WordNode(v);
	else
	{
		WordNode* cur = root;
		for (;;)
		{
			if (v == cur->m_data)
			{
				cur->count++;
				return;
			}
			if (v < cur->m_data)
			{
				if (cur->m_left != NULL)
					cur = cur->m_left;
				else
				{
					cur->m_left = new WordNode(v);
					return;
				}
			}
			else // v > cur->m_data
			{
				if (cur->m_right != NULL)
					cur = cur->m_right;
				else
				{
					cur->m_right = new WordNode(v);
					return;
				}
			}
		}
	}
}

void WordTree::countDistinct(int& count, WordNode* thisRoot) const
{
	if (thisRoot == nullptr)
		return;
	else
	{
		count++;
		countDistinct(count, thisRoot->m_left);
		countDistinct(count, thisRoot->m_right);
	}
}
// Returns the number of distinct words / nodes
int WordTree::distinctWords() const
{
	if (root == nullptr)
		return 0;
	else
	{
		int count = 0;
		countDistinct(count, root);
		return count;
	}
}

void WordTree::countTotal(int& count, WordNode* thisRoot) const
{
	if (thisRoot == nullptr)
		return;
	else
	{
		count += thisRoot->count;
		countTotal(count, thisRoot->m_left);
		countTotal(count, thisRoot->m_right);
	}
}

// Returns the total number of words inserted, including
// duplicate values
int WordTree::totalWords() const
{
	int total = 0;
	countTotal(total, root);
	return total;
}

void WordTree::printRec(ostream& out, WordNode* root) const
{
	if (root == nullptr)
		return;
	else
	{
		printRec(out, root->m_left);
		out << root->m_data << " " << root->count << endl;
		printRec(out, root->m_right);
		return;
	}
}

// Prints the LinkedList
ostream& operator<<(ostream &out, const WordTree& rhs)
{
	rhs.printRec(out, rhs.root);
	return out;
}

// Destroys all the dynamically allocated memory in the
// tree
WordTree::~WordTree()
{
	clearTree(root);
}