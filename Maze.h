#pragma once
#include <assert.h>

const size_t N = 10;

//void GetMaze(int** maze, size_t N) X
//void GetMaze(int (*maze)[10], size_t N)
//void GetMaze(int maze [][N], size_t N)
void GetMaze(int* maze, size_t N)
{
	FILE* fout = fopen("MazeMap.txt", "r");
	assert(fout);

	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; )
		{
			int value = fgetc(fout);
			if (value == '1' || value == '0')
			{
				//maze[i][j] = value-'0';
				maze[i*N+j] = value-'0';
				++j;
			}
			else if (value == EOF)
			{
				cout<<"Maze Error!"<<endl;
				return;
			}
		}
	}
}

void PrintMaze(int* maze, size_t N)
{
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; ++j)
		{
			cout<<maze[i*N+j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

struct Pos
{
	int _row;
	int _col;
};

bool CheckIsAccess(int* maze, size_t n, Pos pos)
{
	if (pos._row >= 0 && pos._row < n
		&& pos._col >= 0 && pos._col < n
		&& maze[pos._row*n+pos._col] == 0)
	{
		return true;
	}

	return false;
}

bool GetMazePath(int* maze, size_t n, Pos entry,
				 stack<Pos>& path)
{
	assert(maze);
	path.push(entry);
	//maze[entry._row*n +entry._col] = 2;

	Pos cur, next;
	while (!path.empty())
	{
		cur = path.top();
		maze[cur._row*n +cur._col] = 2;

		// 找到出口
		if (cur._row == n-1)
			return true;

		// 探测上下左右四个方向
		next = cur;
		next._row -= 1;
		if (CheckIsAccess(maze, n, next))
		{
			path.push(next);
			continue;
		}

		next = cur;
		next._col += 1;
		if (CheckIsAccess(maze, n, next))
		{
			path.push(next);
			continue;
		}

		// 下
		next = cur;
		next._row += 1;
		if (CheckIsAccess(maze, n, next))
		{
			path.push(next);
			continue;
		}


		next = cur;
		next._col -= 1;
		if (CheckIsAccess(maze, n, next))
		{
			path.push(next);
			continue;
		}

		// 四个方向都不可以通，进行回溯
		path.pop();
	}

	return false;
}

bool CheckIsAccess(int* maze, size_t n, Pos cur, Pos next)
{
	if (next._row < 0 || next._row >= n
		|| next._col < 0 || next._col >= n
		|| maze[next._row*n+next._col] == 1)
	{
		return false;
	}

	if(maze[next._row*n+next._col] == 0)
		return true;

	return maze[next._row*n+next._col]+1 > maze[cur._row*n+cur._col];
}

void GetMazePathR(int* maze, size_t n, Pos entry,
				 stack<Pos>& path, stack<Pos>& shortPath)
{
	assert(maze);
	path.push(entry);
	Pos cur, next;
	cur = entry;

	// 找到出口
	if (cur._row == n-1)
	{
		PrintMaze(maze, n);
		if (shortPath.empty() || path.size() < shortPath.size())
		{
			shortPath = path;
		}
			
		path.pop();
		return;
	}

	// 探测上下左右四个方向

	// 上
	next = cur;
	next._row -= 1;
	if (CheckIsAccess(maze, n, cur, next))
	{
		maze[next._row*n+next._col] = maze[cur._row*n+cur._col]+1;
		GetMazePathR(maze, n, next, path, shortPath);
	}

	// 右
	next = cur;
	next._col += 1;
	if (CheckIsAccess(maze, n, cur, next))
	{
		if (cur._row == 6 && cur._col == 4)
		{
			int i = 0;
		}

		maze[next._row*n+next._col] = maze[cur._row*n+cur._col]+1;
		GetMazePathR(maze, n, next, path, shortPath);
	}

	// 下
	next = cur;
	next._row += 1;
	if (CheckIsAccess(maze, n, cur, next))
	{
		maze[next._row*n+next._col] = maze[cur._row*n+cur._col]+1;
		GetMazePathR(maze, n, next, path, shortPath);
	}


	// 左
	next = cur;
	next._col -= 1;
	if (CheckIsAccess(maze, n, cur, next))
	{
		maze[next._row*n+next._col] = maze[cur._row*n+cur._col]+1;
		GetMazePathR(maze, n, next, path, shortPath);
	}

	// 四个方向都不可以通
	path.pop();
}

void TestMaze()
{
	int maze[N][N];
	GetMaze((int*)maze, N);
	Pos entry = {2,0};
	stack<Pos> path;
	//GetMazePath((int*)maze, N, entry, path);
	maze[entry._row][entry._col] = 2;
	stack<Pos> shortPath;
	GetMazePathR((int*)maze, N, entry, path, shortPath);
	cout<<"是否找到迷宫的最短出口？"<<!shortPath.empty()<<endl;
	PrintMaze((int*)maze, N);
}