#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

/* run this program using the console pause or add your own getch, system("pause") or input loop */

//more of slash/bytes badly written code

//used this blogpost as a guide
//http://www.datagenetics.com/blog/december32011/

//miss = 2
//hit = 1
//unvisited  = 0

// id 1 Aircraft Carrier 5
// id 2 Battleship 4
// id 3 Submarine 3
// id 4 Cruiser 3
// id 5 Destroyer 2

void printBoard(uint8_t a[10][10])
{
    for(uint8_t i = 0; i < 10; i++)
    {
        printf("\n");
        for(uint8_t j = 0; j < 10; j++)
            printf("%.1d ", a[i][j]);
    }
    printf("\n");
}

void printBoardX(uint8_t a[10][10])
{
    for(uint8_t i = 0; i < 10; i++)
    {
        printf("\n");
        for(uint8_t j = 0; j < 10; j++)
        {
            uint8_t temp = a[i][j];
            if(temp == 1)
                printf("X ");
            else if(temp == 2)
                printf("0 ");
            else if(temp == 3)
                printf("# ");
            else
                printf(". ");
        }
    }
    printf("\n");
}

class Ships
{
public:
    uint8_t _length;
    bool _sunk;

    Ships(uint8_t length)
    {
        _length = length;
        _sunk = 0;
    }

    void sink(void)
    {
        _sunk = 1;
    }

    bool sunk(void)
    {
        return _sunk;
    }

    void target(uint8_t board[10][10], uint8_t super[10][10])
    {
        for(uint8_t i = 0; i < 10; i++)
            for(uint8_t j = 0; j < 10; j++)
                if(fitH(i, j, board) == 0)
                    for(uint8_t k = j; k < _length+j; k++)
                    {
                        if(board[i][k] == 1) //if hit
                        {
                            super[i][k] = 0;
                            if((k+1) <= 9)
                                if(board[i][k+1] == 0)
                                    super[i][k+1] += 20;
                            if((k-1) >= 0)
                                if(board[i][k-1] == 0)
                                    super[i][k-1] += 20;
                        }
                        else
                            super[i][k]++;
                    }

        for(uint8_t i = 0; i < 10; i++)
            for(uint8_t j = 0; j < 10; j++)
                if(fitV(i, j, board) == 0)
                    for(uint8_t k = j; k < _length+j; k++)
                    {
                        if(board[k][i] == 1)
                        {
                            super[k][i] = 0;
                            if((k+1) <= 9)
                                if(board[k+1][i] == 0)
                                    super[k+1][i] += 20;
                            if((k-1) >= 0)
                                if(board[k-1][i] == 0)
                                    super[k-1][i] += 20;
                        }
                        else
                            super[k][i]++;
                    }
    }

    bool fitV(uint8_t startX, uint8_t startY, uint8_t board[10][10])
    {
        if((startY + _length) > 10)
            return 1; //wont fit
        if(startX > 10)
            return 1; //wont fit
        for(uint8_t i = startY; i < _length+startY; i++)
        {
            uint8_t state = board[i][startX];
            if((state == 2) || (state == 3)) //missed sunk
                return 1;
        }
        return 0;
    }

    bool fitH(uint8_t startY, uint8_t startX, uint8_t board[10][10])
    {
        if((startX + _length) > 10)
            return 1; //wont fit
        if(startY > 10)
            return 1; //wont fit
        for(uint8_t i = startX; i < _length+startX; i++)
        {
            uint8_t state = board[startY][i];
            if((state == 2) || (state == 3)) //missed sunk
                return 1;
        }
        return 0;
    }

};

class Game
{
public:
    uint8_t shipLength[5];
    uint8_t Xboard[10][10], super[10][10];
    uint8_t lastMoveX, lastMoveY;
    Ships *myShips[5];

    Game()
    {
        memset(Xboard, 0, sizeof(Xboard)); //clear board
        memset(super, 0, sizeof(super)); //clear super
        shipLength[0] = 5; //init ship length
        shipLength[1] = 4;
        shipLength[2] = 3;
        shipLength[3] = 3;
        shipLength[4] = 2; 
        for(uint8_t i = 0; i < 5; i++)
            myShips[i] = new Ships(shipLength[i]); //create my ships
        lastMoveX = 0;
        lastMoveY = 0; //init moves
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
        if(sunk > 0)
        {
            myShips[sunk-1]->sink(); //sink the ship
            findShip(sunk-1);
        }
    }

    void findSuper(void)
    {
        memset(super, 0, sizeof(super));
        for(uint8_t i = 0; i < 5; i++)
        {
            if(!myShips[i]->sunk())
                myShips[i]->target(Xboard, super);
        }
    }

    bool findShip(uint8_t len)
    {
        int hitY = 0, hitX = 0;
        int ort = 0;

        for(int i = 0; i < 10; i++)
            if(Xboard[lastMoveY][i] == 1)
                hitY++;
        for(int i = 0; i < 10; i++)
            if(Xboard[i][lastMoveX] == 1)
                hitX++;

        if((hitY == shipLength[len]) && (hitX == 1))
            ort = 1;
        else if((hitX == shipLength[len]) && (hitY == 1))
            ort = 0;
        else
            return 1;

        int buf = 0;
        if(ort)
        {
            while(Xboard[lastMoveY][buf] != 1)
                buf++;
            for(int i = buf; i < shipLength[len] + buf; i++)
                Xboard[lastMoveY][i] = 3;
        }
        else
        {
            while(Xboard[buf][lastMoveX] != 1)
                buf++;
            for(int i = buf; i < shipLength[len] + buf; i++)
                Xboard[i][lastMoveX] = 3;
        }

        return 0;
    }

    /*
    std::string getMove()
    {
    	findSuper();
    	findBest();
    	std::string a;
    	a += (char)(lastMoveY+97);
    	a += (char)(lastMoveX+49);
    	return a;
    }
    */
    void getMove(uint8_t &y, uint8_t &x)
    {
        findSuper();
        findBest();
        y = lastMoveY;
        x = lastMoveX;
    }

    void findBest()
    {
        uint8_t a = 0;
        for(uint8_t i = 0; i < 10; i++)
            for(uint8_t j = 0; j < 10; j++)
            {
                uint8_t b = super[i][j];
                if(b > a)
                {
                    a = b;
                    lastMoveY = i;
                    lastMoveX = j;
                }
            }
    }

};


bool checkFit(uint8_t op[10][10], bool ort, uint8_t startY, uint8_t startX, uint8_t length)
{
    if((startY + length) > 10)
        return 1; //wont fit
    if(ort)
    {
        for(uint8_t i = startY; i < startY + length; i++)
            if(op[i][startX] > 0)
                return 1; //overlap
    }
    else
    {
        for(uint8_t i = startY; i < startY + length; i++)
            if(op[startX][i] > 0)
                return 1; //overlap
    }
    return 0;
}

void genGame(uint8_t op[10][10])
{
    uint8_t length[5] = {5,4,3,3,2};
    bool ort[5];
    uint8_t startY[5];
    uint8_t startX[5];
    for(uint8_t i = 0; i < 5; i++)
    {
        do
        {
            ort[i] = rand() % 2; //0-1
            startY[i] = rand() % 10; //0-9
            startX[i] = rand() % 10; //0-9
        }
        while(checkFit(op, ort[i], startY[i], startX[i], length[i]) == 1);

        if(ort[i])
        {
            for(uint8_t j = startY[i]; j < startY[i] + length[i]; j++)
                op[j][startX[i]] = i+1;
        }
        else
        {
            for(uint8_t j = startY[i]; j < startY[i] + length[i]; j++)
                op[startX[i]][j] = i+1;
        }
    }
}


int main(int argc, char** argv)
{

// id 1 Aircraft Carrier 5
// id 2 Battleship 4
// id 3 Submarine 3
// id 4 Cruiser 3
// id 5 Destroyer 2
    srand (time(NULL));
    //srand(5);

    int gamen = 1000;
    uint8_t max = 10, min = 100;

    printf("Started, Games to be solved: %d\n", gamen);

    for(int i = 0; i < gamen; i++)
    {
        uint8_t OP[10][10] = {{0}};
        genGame(OP);

        Game myGame;

        uint8_t X = 0, Y = 0;
        uint8_t shipCount[5] = {0};
        uint8_t shipLength[5] = {5, 4, 3, 3, 2};
        uint8_t sunk = 0;
        int mv = 0;
        while(sunk != 5)
        {
            mv++;
            myGame.getMove(Y,X);
            uint8_t z = OP[Y][X];
            if(z == 0)
                myGame.updateBoard(0);
            else
            {
                shipCount[z-1] += 1;
                if(shipCount[z-1] == shipLength[z-1])
                {
                    sunk++;
                    myGame.updateBoard(1, z);
                }
                else
                    myGame.updateBoard(1);
            }
            //printBoardX(myGame.Xboard);
            //getchar();
        }

        if(mv < min)
            min = mv;
        if(mv > max)
            max = mv;

        printf("%d, %d,\n", i, mv);
    }

    printf("Solved %d Games\n", gamen);
    printf("Max Move Count: %d\n", max);
    printf("Min Move Count: %d\n", min);
    printf("Approx Average Moves per Game: %d\n", (max+min)/2);

    return 0;
}

