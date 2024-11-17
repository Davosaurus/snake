#define _WIN32_WINNT 0x0601
#include "snek.h"

int main()
{
  SetConsoleTitle("Snake");
  
	//Set font size
	CONSOLE_FONT_INFOEX font={0};
	font.cbSize=sizeof(font);
	font.dwFontSize.X = 15;
	font.dwFontSize.Y = 15;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
	
  //Disable QuickEdit mode (to prevent pausing when clicking in window)
  DWORD prev_mode;
  GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode);
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));
  
  //Lock window resizing
  SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
  
	//Set window size
	SetWindow(1, 1);
	SetWindow(42, 44);
	
	system("cls");
	ShowConsoleCursor(false);
	
  const int GAME_SPEED = 350000;
	const int xSize = 40;
	const int ySize = 40;
	int hiScore = loadScore();
	int gameTime = GAME_SPEED;
  bool restart = true;
		
	int ** field = buildIntMatrix(xSize,ySize);
	
	do
	{
		fillIntMatrix(xSize,ySize,field, 0);
		snake snek(xSize, ySize);
		bool gameOver = false;
		bool food = false;
		int eaten = 0;
		
		int count = 0;
		while(!_kbhit())
		{
			if(count % 2)
				printScreen(0, 0, ySize, "Press any key to begin!");
			else
				printScreen(0, 0, ySize, "");
			printScreen(0, 1, ySize, "Score: \x1B[93m" + to_string(eaten * 10) + "\033[0m");
			printScreen(ySize / 2, 1, ySize, "High Score: \x1B[93m" + to_string(hiScore) + "\033[0m");
			printScreen(0, 2, ySize, toString(xSize, ySize, field));
			usleep(gameTime);
			count++;
		}
		printScreen(0, 0, ySize, "");
		while(!gameOver)
		{
			//gameTime = GAME_SPEED*(1/(2+pow(1.8,(eaten-5)))+.15);
			gameTime = GAME_SPEED*(1/(3+pow(1.5,(eaten-10)))+.2);
			while(!food)
			{
				srand(time(NULL));
				int rX = rand() % xSize;
				int rY = rand() % ySize;
				if(field[rX][rY] == 0)
				{
					field[rX][rY] = -1;
					food = true;
				}
			}
			switch(snek.step(field, xSize, ySize))
			{
				case 0:
				{
					gameOver = true;
					break;
				}
				case 2:
				{
					eaten++;
					food = false;
				}
				case 1:
				{
					printScreen(0, 1, ySize, "Score: \x1B[93m" + to_string(eaten * 10) + "\033[0m");
					printScreen(ySize / 2, 1, ySize, "High Score: \x1B[93m" + to_string(hiScore) + "\033[0m");
					printScreen(0, 2, ySize, toString(xSize, ySize, field));
					usleep(gameTime);
				}
			}
		}
		
		if(eaten * 10 > hiScore)
			saveScore(eaten * 10);
		
		//lose screen:
		count = 0;
    GetAsyncKeyState(VK_ESCAPE);
		while(1)
		{
			while(kbhit()) getch();
			
			printScreen(0, 2, ySize, toString(xSize, ySize, field));
			if(count % 2)
			{
				printScreen(0, 0, ySize, "GAME OVER!");
				printScreen(0, 1, ySize, "Score: \x1B[93m" + to_string(eaten * 10) + "\033[0m");
				printScreen(ySize / 2, 1, ySize, "High Score: \x1B[93m" + to_string(max(hiScore, eaten * 10)) + "\033[0m");
			}
			else
			{
				printScreen(0, 0, ySize, "");
				if(eaten * 10 > hiScore)
					printScreen(ySize / 2, 1, ySize, "");
			}
			usleep(800000);
			count++;
			if(GetAsyncKeyState(VK_ESCAPE))
			{
				restart = false;
				break;
			}
			else if(_kbhit())
			{
				if(count > 3)
					break;
			}
		}
		
		if(eaten * 10 > hiScore)
			hiScore = eaten * 10;
		
	}while(restart);
	
	deleteIntMatrix(xSize,field);
	ShowConsoleCursor(true);
	system("cls");
	
	return 0;
}