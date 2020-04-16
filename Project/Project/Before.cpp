#include "Before.h"
#include "globals.h"
#include <iostream>
using namespace std;

Before::Before(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
			m_grid[i][j] = '.';
}

bool Before::keepTrack(int r, int c)
{
	if (r < 1 || c < 1 ||r >= m_rows || c >= m_cols)
		return false;
	int row = r - 1;
	int col = c - 1;
	if (m_grid[row][col] == '.')
		m_grid[row][col] = 'A';
	else if (m_grid[row][col] == 'Z')
		return true;
	else
		m_grid[row][col]++;	// increment based on ASCII code
	return true;
}

void Before::printWhatWasBefore() const
{
	clearScreen();
	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_cols; j++)
			cout << m_grid[i][j];
		cout << endl;
	}
}