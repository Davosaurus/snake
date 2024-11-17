#include "snek.h"


int main()
{
	bool gameOver = false;
	int xSize = 30;
	int ySize = 60;
	
	int **field = buildIntMatrix(xSize,ySize);
	fillIntMatrix(xSize,ySize,field, 0);
	printIntMatrix(xSize,ySize,field);
	
	snake snek(xSize, ySize);
	
	
	while(!gameOver)
	{
		cout << "Score: " << snek.length * 10 - 20 << endl;
		if(snek.step(field, xSize, ySize))
		{
			musleep(200);
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
			printMatrix(xSize,ySize,field);
		}
		else
		{
			gameOver = true;
		}
	}
	cout << "GAME OVER!" << endl;
	
	return 0;
}