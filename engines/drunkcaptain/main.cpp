/*
 * 	.__ · __¦   ___· .__ ·  _ ._____·  _· _¦________ .
 *  ¦¦ ¯. ¦¦•  ¦¦ ¯¦ ¦¦ ¯. ¦¦?¦¦¦¦ ¯¦?¦¦?¦¦¦•¦¦  ¯_.¯·
 *  _¯¯¯¦_¦¦?  _¦¯¯¦ _¯¯¯¦_¦¦¯¦¦¦¦¯¯¦_¦¦¦¦¦? ¦¦.?¦¯¯?_
 *  ¦¦_?¦¦¦¦¦¦¦¦¦ ?¦¦¦¦_?¦¦¦¦¦¦¯¦¦_?¦¦ ¦¦¯·. ¦¦¦·¦¦__¦
 *   ¯¯¯¯ .¯¯¯  ¯  ¯  ¯¯¯¯ ¯¯¯ ··¯¯¯¯   ¯ •  ¯¯¯  ¯¯¯
 *                                         12-24-2018
 *
 *
 * drunkcaptain is a battleship playing engine, it requires some sort of interface.
 * but she works, but not so well...
 * solves a game on average in 96 moves
 */

#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>    //srand, rand
#include <time.h>      //time

//light on the comments, i know
class RNDmove
{
	public:
	uint8_t Xboard[10][10];
    uint8_t lastMoveX, lastMoveY;
    RNDmove()
    {
    	srand(time(NULL));
		newGame();
	}
	
	void newGame(void)
	{
    	memset(Xboard, 0, sizeof(Xboard)); //clear board
    	lastMoveX = 0;
		lastMoveY = 0;		
	}
	
	//find a random position to shoot at, like a drunk captain...
	void moveIT(void)
	{
		uint8_t y, x;
		do
		{
			y = rand() % 10; //0-9
			x = rand() % 10; //0-9
		}while(Xboard[y][x] != 0);
		lastMoveY = y;
		lastMoveX = x;
	}
	
	//gets the move in index format, not usefull unless testing
    void getMove(uint8_t &y, uint8_t &x)
    {
    	moveIT();
        y = lastMoveY;
        x = lastMoveX;
    }	
    
    //returns the best move in battleship format 'a7', 'd3'
    const char* getMove() 
    {
    	moveIT();
    	static char a[10] = {0};
    	snprintf(a, 10, "%c%d", (char)lastMoveY+97, lastMoveX+1); //convert to c-string
    	return a;
	}
	
    void updateBoard(bool hm)
    {
        if(hm) //if hit
            Xboard[lastMoveY][lastMoveX] = 1; //hit
        else
            Xboard[lastMoveY][lastMoveX] = 2; //miss
    }

    void updateBoard(bool hm, uint8_t sunk) //sunk = ship index + 1
    {
        updateBoard(hm);
    }		
};


int main(int argc, char** argv)
{
	char line[256], command[256];
	bool Side = 0;
	uint8_t m, x;

	RNDmove myGame;

	while(1)
	{
		fflush(stdout); //flush the stream
		if(Side)
		{
			printf("Incoming: %s\n", myGame.getMove()); //send the move to the stream
			Side = !Side; //flip sides
		}
		else
		{
			//list of commands for the engine 'chess inspired'
			if (!fgets(line, 256, stdin))
				return 1;
			if (line[0] == '\n') //enter
				continue;
			sscanf(line, "%s", command);
			if (!strcmp(command, "disp")) //show board
			{
				continue;			
			}
			if (!strcmp(command, "new")) //new game
			{
				myGame.newGame(); //starts a new game
				Side = 0;
				continue;			
			}
			if (!strcmp(command, "quit")) //quit
				return 0;			
			if (!strcmp(command, "hit")) //hit
			{
				if(sscanf(line, "hit %d", &m) < 0)
					myGame.updateBoard(1);
				else if((m > 0) && (m < 6))
					myGame.updateBoard(1, m);
				else
					printf("unknown ship: %s\n", line);
				continue;
			}
			if (!strcmp(command, "miss")) //miss
			{
				myGame.updateBoard(0);
				continue;			
			}
			if (!strcmp(command, "go")) //starts the engine
			{
				Side = !Side; //flip sides
				continue;			
			}
			printf("unknown command: %s\n", command);
		}
	}
	return 0;
}


