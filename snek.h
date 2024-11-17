#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <windows.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
using namespace std;

const int GROWTH_RATE = 5;

void usleep(__int64 usec) 
{
	HANDLE timer; 
	LARGE_INTEGER ft; 

	ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL); 
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
	WaitForSingleObject(timer, INFINITE); 
	CloseHandle(timer); 
}

typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

void SetWindow(int Width, int Height) 
{ 
	_COORD coord; 
	coord.X = Width; 
	coord.Y = Height; 

	_SMALL_RECT Rect; 
	Rect.Top = 0; 
	Rect.Left = 0; 
	Rect.Bottom = Height - 1; 
	Rect.Right = Width - 1; 

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(Handle, true, &Rect);            // Set Window Size 
}

int loadScore()
{
	string str;
	ifstream file;
  file.open("snek.hs");
  file >> str;
  file.close();
	if(file)
		return stoi(str);
	else
		return 0;
}

void saveScore(int score)
{
	ofstream file;
  file.open("snek.hs", fstream::out);
	if(file)
		file << to_string(score);
  file.close();
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void printScreen(int x, int y, int width, string str)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position;
	Position.X = x;
	Position.Y = y;
	
	SetConsoleCursorPosition(hOut, Position);
	
	if(str.size() + x < width)
		str.insert(str.end(), width - str.size() - x, ' ');
	cout << str;
}

string toString(int rows, int cols, int **matrix)
{
	string output = "";
  output += 201;
	for(int i = 0; i < cols; i++)
	{
		output += 205;
	}
	output += 187;
	output += "\n";
	for(int i = 0; i < rows; i++)
	{
		output += 186;
		for(int j = 0; j < cols; j++)
		{
			if(matrix[i][j] == 0)
			{
				output += " ";
			}
			else if(matrix[i][j] == -1)
			{
				output += "\x1B[91m";
        output += 219;
        output += "\033[0m";
			}
			else if(matrix[i][j] > 0)
			{
				output += "\x1B[32m";
				output += 219;
        output += "\033[0m";
			}
		}
		output += 186;
		output += "\n";
	}
  output += 200;
	for(int i = 0; i < cols; i++)
	{
		output += 205;
	}
  output += 188;
	return output;
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

string toStringIntMatrix(int rows, int cols, int **matrix)
{
	string output = "";
  output += 201;
	for(int i = 0; i < cols; i++)
	{
		output += 205;
	}
	output += 187;
	output += "\n";
	for(int i = 0; i < rows; i++)
	{
		output += 186;
		for(int j = 0; j < cols; j++)
		{
			if(matrix[i][j] == 0)
			{
				output += " ";
			}
			else if(matrix[i][j] == -1)
			{
				output += "\x1B[91m";
        output += 219;
        output += "\033[0m";
			}
			else if(matrix[i][j] > 0)
			{
				output += "\x1B[32m";
				output += matrix[i][j] + '0';
        output += "\033[0m";
			}
		}
		output += 186;
		output += "\n";
	}
  output += 200;
	for(int i = 0; i < cols; i++)
	{
		output += 205;
	}
  output += 188;
	return output;
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

int fillStep(int **decay, int rows, int cols, snake& snake);

struct snake
{
	int length;
	enum direction{up, right, down, left};
	direction dir;
	int xPos;
	int yPos;
	
	snake(int rows, int cols)
	{
		xPos = rows / 2;
		yPos = cols / 2;
		dir = up;
		length = 4;
	}
	
	void feed()
	{
		length += GROWTH_RATE;
	}
	
	int takeInput()
	{
    DWORD numEvents;
    INPUT_RECORD inputBuffer[1];
    DWORD cNumRead;
    
    //Flush all non-button presses from the buffer
    while(PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputBuffer, 1, &cNumRead))
    {
      if(cNumRead && !inputBuffer->Event.KeyEvent.bKeyDown)
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputBuffer, 1, &cNumRead);
      else break;
    }
    
    GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numEvents);
    if(numEvents)
    {
      //Take ONE event from the buffer and perform it
      ReadConsoleInput( 
        GetStdHandle(STD_INPUT_HANDLE), // input buffer handle
        inputBuffer,                    // buffer to read into
        1,                              // size of read buffer
        &cNumRead);                     // number of records read  
      switch(inputBuffer->Event.KeyEvent.wVirtualKeyCode)
      {
        case VK_ESCAPE:
          return 0;
          break;
        case VK_UP:
          if(dir != down)
            dir = up;
          break;
        case VK_RIGHT:
          if(dir != left)
            dir = right;
          break;
        case VK_DOWN:
          if(dir != up)
            dir = down;
          break;
        case VK_LEFT:
          if(dir != right)
            dir = left;
          break;
      }
      
      //Flush all non-button presses from the buffer
      while(PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputBuffer, 1, &cNumRead))
      {
        if(cNumRead && !inputBuffer->Event.KeyEvent.bKeyDown)
          ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputBuffer, 1, &cNumRead);
        else break;
      }
      
      //See if there are more events left
      GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numEvents);
      if(numEvents)
      {
        //Discard all events except the next one
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputBuffer, 1, &cNumRead);
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputBuffer, 1, &cNumRead);
      }
    }
    return 1;
	}
	
	int step(int **decay, int rows, int cols)
	{
		if(!takeInput())
      return 0;
		switch(dir)
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
		return fillStep(decay, rows, cols, *this);
	}
};

int fillStep(int **decay, int rows, int cols, snake& snake)
{
	if(snake.xPos < 0 ||
	   snake.xPos >= rows ||
	   snake.yPos < 0 ||
	   snake.yPos >= cols ||
	   decay[snake.xPos][snake.yPos] > 0)
	{
		return 0;
	}
	else
	{
    int returnVal = 1;
		if(decay[snake.xPos][snake.yPos] == -1)
		{
			snake.feed();
      returnVal = 2;
		}
    for(int i = 0; i < rows; i++)
    {
      for(int j = 0; j < cols; j++)
      {
        if(decay[i][j] > 0)
        {
          decay[i][j]--;
          if(returnVal == 2)
            decay[i][j] += GROWTH_RATE;
        }
      }
    }
    decay[snake.xPos][snake.yPos] = snake.length;
    return returnVal;
	}
}