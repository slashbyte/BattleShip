/*
 * Slash/Byte                    12-27-2018
 *
 * used this blogpost as a guide
 * http://www.datagenetics.com/blog/december32011/
 *
 * Barracuda Version 2, is a battleship playing engine
 * NOT set up for game play, just tests.
 * but she works, and she works well!
 * solves a game on average in 46 moves
 */

//#include <FlyingMonkeyProtocol.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


//gen random board
bool checkFit(uint8_t op[10][10], bool ort, uint8_t startY, uint8_t startX, uint8_t length);
void genGame(uint8_t op[10][10]);
//96 'abcde, 32 [space]!"#
//32 for game board playing, 96 for board with ships
void printBoard(uint8_t OP[10][10], int font = 96); //pretty print board


class Barracuda
{
public:
    Barracuda() {
        _length[0] = 5;
        _length[1] = 4;
        _length[2] = 3;
        _length[3] = 3;
        _length[4] = 2;
        newGame();
    }

    //start a new game, clear all vars
    void newGame(void) {
        memset(Xboard, 0, sizeof(Xboard)); //clear board
        memset(super, 0, sizeof(super)); //clear super
        memset(_sunk, 0, sizeof(_sunk)); //unsink my ships
        lastMoveX = 0; //clear moves
        lastMoveY = 0; //clear moves
    }

    //update the board, based on last pos
    void updateBoard(bool hm) {
        if (hm) //if hit
            Xboard[lastMoveY][lastMoveX] = 1; //set hit cell
        else
            Xboard[lastMoveY][lastMoveX] = 2; //set miss cell
        //uncomment for testing
        //printBoard(Xboard, 32); //debuging
    }

    //update the board
    void updateBoard(bool hm, uint8_t sunk) { //sunk = ship index + 1
        updateBoard(hm); //update based on last pos
        if (sunk > 0) { //'if the ship be sunk'... 'rrrrrr'
            _sunk[sunk - 1] = 1; //myShips[sunk-1]->sink(); //sink the ship
            findShip(sunk - 1); //locate the sunk ship
        }
    }

    //returns the best move in battleship format 'a7', 'd3'
    const char* getMove() {
        findSuper(); //find the super position for the game board
        findBest(); //find the highist rated move in the super position
        static char a[10] = {0};
        snprintf(a, 10, "%c%d", (char)lastMoveY + 97, lastMoveX + 1); //convert to c-string
        return a;
    }

    //gets the move in index format, not usefull unless testing
    void getMove(uint8_t &y, uint8_t &x) {
        findSuper(); //find the super position for the game board
        findBest(); //find the highist rated move in the super position
        y = lastMoveY; //sets the move index
        x = lastMoveX; //sets the move index
    }

private:
    uint8_t _i, _j, _max, lastMoveX, lastMoveY, *bp, *sp;
    uint8_t _length[5];
    uint8_t super[10][10], Xboard[10][10];
    bool _sunk[5];

    //calulate new super position for the current board
    void findSuper(void) {
        memset(super, 0, sizeof(super)); //clear the super position
        for (_i = 0; _i < 5; _i++) //cycle ships
            if (!_sunk[_i])
                target(_i);
    }

    //locate the sunk ship
    void findShip(uint8_t shipID) {
        uint8_t Y = 0, X = 0; //hit index
        for (_i = 0; _i < 10; _i++) {
            Xboard[lastMoveY][_i] == 1 ? Y++ : 0; //inc y hit count if cell is 1
            Xboard[_i][lastMoveX] == 1 ? X++ : 0; //int x hit count if cell is 1
        }
        //figure out which way they are
        if ((Y == _length[shipID]) && (X == 1))
            setShip(1, shipID);
        else if ((X == _length[shipID]) && (Y == 1))
            setShip(0, shipID);
    }

    // chnange from hit to sunk
    void setShip(bool ort, uint8_t shipID) {
        _j = 0;
        ort ? bp = &Xboard[lastMoveY][_j] : bp = &Xboard[_j][lastMoveX]; //flip
        while (*bp != 1) { //count untill we find the hit ship
            _j++;
            ort ? bp = &Xboard[lastMoveY][_j] : bp = &Xboard[_j][lastMoveX]; //flip
        }
        for (_i = _j; _i < _length[shipID] + _j; _i++) //change the state
            ort ? Xboard[lastMoveY][_i] = 3 : Xboard[_i][lastMoveX] = 3; //use last move as ref
    }

    //find the best move in the 'extra super position, of doom, from hell...'
    void findBest() {
        uint8_t a = 0, b = 0;
        for (_j = 0; _j < 5; _j++)
            for (_i = 0; _i < 10; _i++) {
                b = super[_i][_j] > super[_i][_j + 5] ? super[_i][_j] : super[_i][_j + 5]; //'b' is now the largist of the two numbers
                if (a < b) { //if 'a' is bigger than the largist
                    a = b; //a becomes the largist
                    lastMoveY = _i; //'i' dont change between the two
                    lastMoveX = a == super[_i][_j] ? _j : _j + 5; //check which number was assigned, give it a 'j' val.
                } //I save 50 loops, faster = better?
            }
    }

    //uses the current board to find the ship probobility 'super'
    void target(uint8_t shipID) {
        uint8_t j = 0, i = 0, ort = 0; //dont use _i or _j
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                for (ort = 0; ort < 2; ort++)
                    if (fitCell(j, i, ort, shipID) == 0) //horse
                        setCell(j, i, ort, shipID); //setH(i, j, board, super);
    }

    //vertical and horizontal, cell FIT CHECK, virtical, ort = true
    bool fitCell(uint8_t i, uint8_t j, bool ort, uint8_t shipID) {
        _max = i + _length[shipID]; //set fit check length
        if (_max > 10) //check ship length and compares it to the board edge
            return 1; //wont fit
        for (; i < _max; i++) { //scan the total length of ship
            ort ? bp = &Xboard[i][j] : bp = &Xboard[j][i]; //flip board
            if ((*bp == 2) || (*bp == 3)) //if the space if full
                return 1; //wont fit
        }
        return 0;
    }

    //vertical and horizontal, cell FILL, virtical, ort = true
    void setCell(uint8_t i, uint8_t j, bool ort, uint8_t shipId) {
        _max = i + _length[shipId]; //set fit check length
        for (; i < _max; i++) { //scan the total length of ship
            ort ? bp = &Xboard[i][j] : bp = &Xboard[j][i]; //flip board
            ort ? sp = &super[i][j] : sp = &super[j][i]; //flip board
            if (*bp == 1) { //if the space has been hit
                *sp = 0; //zero it out
                //check sourounding cells
                if ((i + 1) <= 9) { //noth & west check, 4 cells all round
                    ort ? bp = &Xboard[i + 1][j] : bp = &Xboard[j][i + 1]; //flip board
                    if (*bp == 0) { //check if its free
                        ort ? sp = &super[i + 1][j] : sp = &super[j][i + 1]; //flip board
                        *sp += 20; //then heavy weight it
                    }
                }
                if ((i - 1) >= 0) { //south & east check, 4 cells all round
                    ort ? bp = &Xboard[i - 1][j] : bp = &Xboard[j][i - 1]; //flip board
                    if (*bp == 0) { //check if its free
                        ort ? sp = &super[i - 1][j] : sp = &super[j][i - 1]; //flip board
                        *sp += 20; //then heavy weight it
                    }
                }
            } else
                *sp += 1;
        }
    }

};


int main(int argc, char** argv)
{

    // ship manifest
    // id 1 Aircraft Carrier 5
    // id 2 Battleship 4
    // id 3 Submarine 3
    // id 4 Cruiser 3
    // id 5 Destroyer 2

    //srand (time(NULL));
    srand(5); //used for debugging, damn bugs...

    //games to be played, change
    int gamen = 1000;

    //vars for stuff
    uint8_t max = 0, min = 100;
    uint8_t OP[10][10] = {{0}};
    uint8_t shipLength[5] = {5, 4, 3, 3, 2};
    uint8_t shipCount[5] = {0};

    Barracuda myGame; //game object, creates 5 'ship' objects, and things.

    int freq[101] = {0}; //var for frequency analisys, slash/byte, use spell check..

    printf("Started, Games to be solved: %d\n", gamen); //buys your mom a refrigerator

    // game loop
    for(int i = 0; i < gamen; i++) {
        //vars
        uint8_t X = 0, Y = 0;
        uint8_t sunk = 0;
        int mv = 0;

        myGame.newGame(); //sets game state to zero 'n' stuff
        memset(OP, 0, sizeof(OP)); //clears rand board to solve
        memset(shipCount, 0, sizeof(shipCount)); //clears ship hit counter
        genGame(OP); //gens new board to solve

        //printBoard(OP,96);

        // 2nd game loop
        while(sunk != 5) {
            mv++;
            //printf("Move#: %d, ", mv); //display curretn move count
            myGame.getMove(Y,X); // engine move
            //printf("Move: %s,...", myGame.getMove()); //diplay move
            uint8_t z = OP[Y][X]; //snag rnd board val
            if(z == 0) { //check rnd board val for hit or miss
                myGame.updateBoard(0); //update with reply, miss
                //printf("Miss\n"); //diplay miss
            } else { //hit
                shipCount[z-1] += 1; //inc ship hit count
                if(shipCount[z-1] == shipLength[z-1]) { //if sunk, hit count = ship length
                    sunk++; //inc sunk count, 5 to win
                    myGame.updateBoard(1, z); //update board based on reply, hit, sunk
                    //printf("Hit, you Sank boat %d\n", z); //display hit, sunk
                } else {
                    myGame.updateBoard(1); //update board with reply, hit
                    //printf("Hit\n"); //display hit
                }
            }
            //getchar(); //stream break
        }

        //find max, min moves
        mv < min ? min = mv : 0;
        mv > max ? max = mv : 0;

        freq[mv]++; //add to freq array
    }

    //prints all the pretty things
    printf("Solved %d Games\n", gamen);
    printf("Max Move Count: %d\n", max);
    printf("Min Move Count: %d\n", min);

    int avg = 0;
    for(int i = 1; i < 101; i++) //calc average
        avg += (i*freq[i]); //sum of all moves
    printf("Average Moves till Victory: %.2f\n", (float)avg/(float)gamen); //sum devided by games

    /*
    //display freq table
    printf("MoveCnt,Frequency\n");
    for(int i = 0; i < 101; i++) {
    	printf("%d,%d\n", i, freq[i]);
    }
    */
    return 0;
}

void printBoard(uint8_t OP[10][10], int font)
{
    printf("-------------------\n");
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++)
            printf("%c ", OP[i][j]+font);
        printf("\n");
    }
}

// all below is used for board setup for testing, not needed for the engine
// the engine dosent have knolage of the ship placement
void genGame(uint8_t op[10][10])
{
    bool ort[5];
    uint8_t length[5] = {5,4,3,3,2};
    uint8_t A[5], B[5];
    for(uint8_t i = 0; i < 5; i++) {
        do {
            ort[i] = rand() % 2; //0-1
            A[i] = rand() % 10; //0-9
            B[i] = rand() % 10; //0-9
        } while(checkFit(op, ort[i], A[i], B[i], length[i]) == 1); //brute force ship set

        for(uint8_t j = A[i]; j < A[i] + length[i]; j++) //add the ship to the rnd board
            ort[i] == 1 ? op[j][B[i]] = i+1 : op[B[i]][j] = i+1; //flip board
    }
}

bool checkFit(uint8_t op[10][10], bool ort, uint8_t A, uint8_t B, uint8_t length)
{
    if((A + length) > 10) //checks board edge
        return 1; //wont fit

    uint8_t *a; //a pointer for board flip
    for(uint8_t i = A; i < A + length; i++) { //checks for overla[
        ort == 1 ? a = &op[i][B] : a = &op[B][i]; //flip board
        if(*a > 0)
            return 1;
    }
    return 0;
}




