#include "History.h"
#include "globals.h"
#include <iostream>

using namespace std;

char numberToLetter(int n);

History::History(int nRows, int nCols)
	: m_nRows(nRows), m_nCols(nCols), historyGrid{0}
{

}

bool History::record(int r, int c)
{
	if (r > m_nRows || c > m_nCols || r < 1 || c < 1)
		return false;

	historyGrid[r - 1][c - 1]++;

	return true;
}

void History::display() const
{
	clearScreen();
	for (int r = 0; r < m_nRows; r++)
	{
		for (int c = 0; c < m_nCols; c++)
			if (historyGrid[r][c] > 26)
			{
				cout << 'Z';
			}
			else
			{
				cout << numberToLetter(historyGrid[r][c]);
			}
		cout << endl;
	}
	cout << endl;
}

char numberToLetter(int n)
{
	char string[28] = ".ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char letter = string[n];

	return letter;
}