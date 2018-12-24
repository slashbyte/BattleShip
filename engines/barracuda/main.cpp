/*
 * 	.__ · __¦   ___· .__ ·  _ ._____·  _· _¦________ .
 *  ¦¦ ¯. ¦¦•  ¦¦ ¯¦ ¦¦ ¯. ¦¦?¦¦¦¦ ¯¦?¦¦?¦¦¦•¦¦  ¯_.¯·
 *  _¯¯¯¦_¦¦?  _¦¯¯¦ _¯¯¯¦_¦¦¯¦¦¦¦¯¯¦_¦¦¦¦¦? ¦¦.?¦¯¯?_
 *  ¦¦_?¦¦¦¦¦¦¦¦¦ ?¦¦¦¦_?¦¦¦¦¦¦¯¦¦_?¦¦ ¦¦¯·. ¦¦¦·¦¦__¦
 *   ¯¯¯¯ .¯¯¯  ¯  ¯  ¯¯¯¯ ¯¯¯ ··¯¯¯¯   ¯ •  ¯¯¯  ¯¯¯
 *                                         12-24-2018
 *
 * used this blogpost as a guide
 * http://www.datagenetics.com/blog/december32011/
 *
 * Barracuda is a battleship playing engine, it requires some sort of interface.
 * but she works, and she works well!
 * solves a game on average in 46 moves
 */

#include <iostream>
#include <string.h>
#include <stdint.h>


class Ships
{
public:
    uint8_t _length; //length of the ship
    bool _sunk; //sunk state

    Ships(uint8_t length) //init all the things
    {
        _length = length;
        _sunk = 0;
    }

    void sink(bool a = 1) //sink or unsink a ship
    {
    	_sunk = a;
	}

    bool sunk(void) //check sunk state of ship
    {
        return _sunk;
    }

	//uses the current board to find the ship probobility 'super'
    void target(uint8_t board[10][10], uint8_t super[10][10]) 
    {
    	uint8_t j = 0;
    	for(uint8_t i = 0; i < 10; i++)
    	{
    		for(j = 0; j < 10; j++)
    		{
    			if(fitH(i, j, board) == 0) //checks if the ship will fit on the board for that position
    				setH(i, j, board, super); //updates the probobility
    			if(fitV(i, j, board) == 0)
    				setV(i, j, board, super);
			}
		}                
	}
	
	//set the vertical position
	//this upticks the cells next to the hit cell, if there is a hit cell
	void setV(uint8_t i, uint8_t j, uint8_t board[10][10], uint8_t super[10][10])
	{
        for(uint8_t k = j; k < _length+j; k++) //for the length of the ship
        {
            if(board[k][i] == 1) //if the space is hit
            {
                super[k][i] = 0; //zero the super cell
                if((k+1) <= 9) //if the space sourounding the cell is there, NORTH
                    if(board[k+1][i] == 0) //and its free
                        super[k+1][i] += 20; //heavy weight the cell
                if((k-1) >= 0) //if the space sourounding the cell is there. SOUTH
                	if(board[k-1][i] == 0) //and its free
                            super[k-1][i] += 20; //heavy weight the cell
            }
            else
                super[k][i]++;
        }		
	}
	
	//set the horizontal position 
	//this upticks the cells next to the hit cell, if there is a hit cell
	void setH(uint8_t i, uint8_t j, uint8_t board[10][10], uint8_t super[10][10])
	{
	    for(uint8_t k = j; k < _length+j; k++) //for the length of the ship
        {
	        if(board[i][k] == 1) //if the space is hit
	        {
	            super[i][k] = 0; //zero the super cell
	            if((k+1) <= 9) //if the space sourounding the cell is there, WEST
	                if(board[i][k+1] == 0) //and its free
	                    super[i][k+1] += 20; //heavy weight the cell
	            if((k-1) >= 0) //if the space sourounding the cell is there. EAST
	                if(board[i][k-1] == 0) //and its free
	                    super[i][k-1] += 20; //heavy weight the cell
	        }
	        else
	            super[i][k]++;
        }	
	}
	
	//vertical, test fits the ship, returns true if the ship could fit
    bool fitV(uint8_t startX, uint8_t startY, uint8_t board[10][10])
    {
        if((startY + _length) > 10)
            return 1; //wont fit
        if(startX > 10)
            return 1; //wont fit
        uint8_t state = 0;
        for(uint8_t i = startY; i < _length+startY; i++)
        {
            state = board[i][startX]; //checks board cells
            if((state == 2) || (state == 3)) //missed or sunk cells
                return 1;
        }
        return 0;
    }

	//horizontal, test fits the shiP, returns true if the ship could fit
    bool fitH(uint8_t startY, uint8_t startX, uint8_t board[10][10])
    {
        if((startX + _length) > 10)
            return 1; //wont fit
        if(startY > 10)
            return 1; //wont fit
        uint8_t state = 0;
        for(uint8_t i = startX; i < _length+startX; i++)
        {
            state = board[startY][i]; //checks board cells
            if((state == 2) || (state == 3)) //missed or sunk cells
                return 1;
        }
        return 0;
    }
};

class Game
{
public:
    uint8_t shipLength[5]; //length of the ships in the game
    uint8_t Xboard[10][10], super[10][10]; // game board and super position
    uint8_t lastMoveX, lastMoveY; //move vars
    Ships *myShips[5]; //array of ship objects

    Game()
    {
        shipLength[0] = 5; //init ship length
        shipLength[1] = 4;
        shipLength[2] = 3;
        shipLength[3] = 3;
        shipLength[4] = 2; 
        
        //create all the ship objects with the right length
        for(uint8_t i = 0; i < 5; i++)
            myShips[i] = new Ships(shipLength[i]); //create my ships
        newGame();
    }
    
    ~Game()
    {
    	for(uint8_t i = 0; i < 5; i++)
    		delete[] myShips[i]; //duke nuke um
	}

	//start a new game, clear all vars
	void newGame(void)
	{
		memset(Xboard, 0, sizeof(Xboard)); //clear board
        memset(super, 0, sizeof(super)); //clear super
        for(uint8_t i = 0; i < 5; i++)
        	myShips[i]->sink(0); //unsink my ships
        lastMoveX = 0; //clear moves
        lastMoveY = 0; //clear moves
	}

	//update the board
    void updateBoard(bool hm)
    {
        if(hm) //if hit
            Xboard[lastMoveY][lastMoveX] = 1; //set hit cell
        else
            Xboard[lastMoveY][lastMoveX] = 2; //set miss cell
    }

	//update the board
    void updateBoard(bool hm, uint8_t sunk) //sunk = ship index + 1
    {
        updateBoard(hm);
        if(sunk > 0)
        {
            myShips[sunk-1]->sink(); //sink the ship
            findShip(sunk-1); //locate the sunk ship
        }
    }
	
	//calulate new super position for the current board
    void findSuper(void)
    {
        memset(super, 0, sizeof(super)); //clear the super position
        for(uint8_t i = 0; i < 5; i++)
        {
            if(!myShips[i]->sunk()) //if the ship is still in play
                myShips[i]->target(Xboard, super); //use it in the new super position
        }
    }

	//locate the sunk ship and change the state from hit to sunk
    bool findShip(uint8_t len)
    {
        uint8_t hitY = 0, hitX = 0; //hit index
        uint8_t ort = 0; //orientation
        uint8_t i = 0;

        for(i = 0; i < 10; i++)
            if(Xboard[lastMoveY][i] == 1) //find horizontal hit count
                hitY++;
        for(i = 0; i < 10; i++)
            if(Xboard[i][lastMoveX] == 1) //find virtical hit count
                hitX++;

        if((hitY == shipLength[len]) && (hitX == 1)) //figure out the orientation
            ort = 1;
        else if((hitX == shipLength[len]) && (hitY == 1)) //figure out the orientation
            ort = 0;
        else
            return 1; //if we arnt 100% sure, it stays hit, not sunk

        int buf = 0;
        if(ort)
        {
            while(Xboard[lastMoveY][buf] != 1) //count untill we find first hit of the ship
                buf++;
            for(i = buf; i < shipLength[len] + buf; i++) //change the state, horizontal
                Xboard[lastMoveY][i] = 3; //use last move as ref
        }
        else
        {
            while(Xboard[buf][lastMoveX] != 1) //count untill we find first hit of the ship
                buf++;
            for(i = buf; i < shipLength[len] + buf; i++) //change the state, vertical
                Xboard[i][lastMoveX] = 3; //use last move as ref
        }
        return 0;
    }
    
    //returns the best move in battleship format 'a7', 'd3'
    const char* getMove() 
    {
    	findSuper(); //find the super position for the game board
    	findBest(); //find the highist rated move in the super position
    	static char a[10] = {0};
    	snprintf(a, 10, "%c%d", (char)lastMoveY+97, lastMoveX+1); //convert to c-string
    	return a;
	}
    
    //gets the move in index format, not usefull unless testing
    void getMove(uint8_t &y, uint8_t &x)
    {
        findSuper(); //find the super position for the game board
        findBest(); //find the highist rated move in the super position
        y = lastMoveY; //sets the move index
        x = lastMoveX; //sets the move index
    }
	
	//find the best move in the super position
    void findBest()
    {
        uint8_t j = 0, a = 0, b = 0;
        for(uint8_t i = 0; i < 10; i++) //scan all cells
            for(j = 0; j < 10; j++)
            {
                b = super[i][j];
                if(b > a) //the cell with the highist rateing gets picked
                {
                    a = b; 
                    lastMoveY = i;
                    lastMoveX = j;
                }
            }
    }

};

int main(int argc, char** argv)
{
	char line[256], command[256];
	bool Side = 0;
	uint8_t m, x;

	Game myGame;

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

