#include <iostream>
#include <sstream>
#include <unistd.h>
#include <winuser.h>
#include <windows.h>
using namespace std;


void musleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10000*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}


/*
char ** buildMatrix(int rows, int cols)
{
	char **myArray = new char *[rows];
	for(int i = 0; i < rows; i++)
	{
		myArray[i] = new char[cols];
	}
	return myArray;
}

void fillMatrix(int rows, int cols, char **matrix, char value)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      matrix[i][j] = value;
    }
  }
}

void printMatrix(int rows, int cols, char **matrix)
{
	cout << endl;
	for(int i = 0; i < cols + 2; i++)
	{
		cout << "-";
	}
	cout << endl;
	for(int i = 0; i < rows; i++)
	{
		cout << "|";
		for(int j = 0; j < cols; j++)
		{
			cout << matrix[i][j];
		}
		cout << "|" << endl;
	}
	for(int i = 0; i < cols + 2; i++)
	{
		cout << "-";
	}
	cout << endl;
}

void deleteMatrix(int rows, char **matrix)
{
	for(int i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	matrix = NULL;
}
*/

void printMatrix(int rows, int cols, int **matrix)
{
  ostream stream(nullptr);
  //ostream stream;
  std::stringbuf str;
  stream.rdbuf(&str);
  
	stream << endl;
	for(int i = 0; i < cols + 2; i++)
	{
		stream << "-";
	}
	stream << endl;
	for(int i = 0; i < rows; i++)
	{
		stream << "|";
		for(int j = 0; j < cols; j++)
		{
			if(matrix[i][j] == 0)
			{
				stream << ' ';
			}
			else if(matrix[i][j] == -1)
			{
				stream << '*';
			}
			else if(matrix[i][j] > 0)
			{
				stream << '@';
			}
		}
		stream << "|" << endl;
	}
	for(int i = 0; i < cols + 2; i++)
	{
		stream << "-";
	}
	stream << endl;
  std::cout << str.str();
}

int ** buildIntMatrix(int rows, int cols)
{
	int **myArray = new int *[rows];
	for(int i = 0; i < rows; i++)
	{
		myArray[i] = new int[cols];
	}
	return myArray;
}

void fillIntMatrix(int rows, int cols, int **matrix, int value)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      matrix[i][j] = value;
    }
  }
}

void printIntMatrix(int rows, int cols, int **matrix)
{
	cout << endl;
	for(int i = 0; i < cols + 2; i++)
	{
		cout << "-";
	}
	cout << endl;
	for(int i = 0; i < rows; i++)
	{
		cout << "|";
		for(int j = 0; j < cols; j++)
		{
			cout << matrix[i][j];
		}
		cout << "|" << endl;
	}
	for(int i = 0; i < cols + 2; i++)
	{
		cout << "-";
	}
	cout << endl;
}

void deleteIntMatrix(int rows, int **matrix)
{
	for(int i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	matrix = NULL;
}

typedef struct snake snake;

bool fillStep(int **decay, int rows, int cols, snake snake);

struct snake
{
	int length;
	int direction;
	int xPos;
	int yPos;
	
	snake(int rows, int cols)
	{
		xPos = rows / 2;
		yPos = cols / 2;
		direction = 0;
		length = 2;
	}
	
	void feed()
	{
		length++;
	}
	
	
	void takeInput()
	{
		if(GetAsyncKeyState(VK_UP))
		{
			direction = 0;
		}
		if(GetAsyncKeyState(VK_RIGHT))
		{
			direction = 1;
		}
		if(GetAsyncKeyState(VK_DOWN))
		{
			direction = 2;
		}
		if(GetAsyncKeyState(VK_LEFT))
		{
			direction = 3;
		}
	}
	
	
	bool step(int **decay, int rows, int cols)
	{
    takeInput();
		switch(direction)
		{
			case 0:
			{
				xPos--;
				break;
			}
			case 1:
			{
				yPos++;
				break;
			}
			case 2:
			{
				xPos++;
				break;
			}
			case 3:
			{
				yPos--;
				break;
			}
		}
		if(fillStep(decay, rows, cols, *this))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

bool fillStep(int **decay, int rows, int cols, snake snake)
{
	if(snake.xPos < 0 ||
	   snake.xPos >= rows ||
	   snake.yPos < 0 ||
	   snake.yPos >= cols ||
	   decay[snake.xPos][snake.yPos] > 0)
	{
		return false;
	}
	else
	{
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < cols; j++)
			{
				if(decay[i][j])
				{
					decay[i][j]--;
				}
			}
		}
		decay[snake.xPos][snake.yPos] += snake.length;
		return true;
	}
}