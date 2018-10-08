#include <string>
#include <queue>
#include <iostream>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;

	coordQueue.push(Coord(sr, sc));				// push the starting coordinate
	maze[sr][sc] = 'O';							// 'O' indicates that cell was encountered

	while (!(coordQueue.empty()))
	{
		Coord current = coordQueue.front();		// set current point as front item of queue
		coordQueue.pop();						// pop the front item of queue

		if (current.r() == er && current.c() == ec)
			return true;

		//move EAST
		if (maze[current.r()][current.c() + 1] == '.')
		{
			coordQueue.push(Coord(current.r(), current.c() + 1));
			maze[current.r()][current.c() + 1] = 'O';
		}

		//move SOUTH
		if (maze[current.r() + 1][current.c()] == '.')
		{
			coordQueue.push(Coord(current.r() + 1, current.c()));
			maze[current.r() + 1][current.c()] = 'O';
		}

		//move WEST
		if (maze[current.r()][current.c() - 1] == '.')
		{
			coordQueue.push(Coord(current.r(), current.c() - 1));
			maze[current.r()][current.c() - 1] = 'O';
		}

		//move NORTH
		if (maze[current.r() - 1][current.c()] == '.')
		{
			coordQueue.push(Coord(current.r() - 1, current.c()));
			maze[current.r() - 1][current.c()] = 'O';
		}
	}

	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X..XX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 4, 3, 1, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}