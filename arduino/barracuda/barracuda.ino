/*
   Battleship Game engine for hardware!
   originally the motivation behind building an engine for battleship

   This will solve randomly gen-ed boards, per the rules, of battleship
   Comes as is...

   Slash/Byte 2018

  Sketch uses 6914 bytes (21%) of program storage space. Maximum is 32256 bytes.
  Global variables use 390 bytes (19%) of dynamic memory, leaving 1658 bytes for local variables. Maximum is 2048 bytes.


  ...and yes, everything all crammed togeather like this is not very good codeing.
  but its open source, that counts for somethig right?

*/

//generates a random board state to solve
void genGame(uint8_t op[10][10]);
bool checkFit(uint8_t op[10][10], bool ort, uint8_t startY, uint8_t startX, uint8_t length);
//prints the internal board state over serial
void printBoard(uint8_t OP[10][10], int font = 96); //32 works too

//ship class for, ship stuff?
class Ships
{
  public:
    Ships(uint8_t length) //init all the things
    {
      _length = length;
      _sunk = 0;
    }

    //sink or unsink a ship
    void sink(bool a = 1) //sink or unsink a ship
    {
      _sunk = a;
    }

    //return sunk state
    bool sunk(void) //check sunk state of ship
    {
      return _sunk;
    }

    //uses the current board to find the ship probobility 'super'
    void target(uint8_t board[10][10], uint8_t super[10][10])
    {
      uint8_t j = 0;
      uint8_t ort = 0;
      for (uint8_t i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
          for (ort = 0; ort < 2; ort++)
            if (fitCell(j, i, board, ort) == 0) //horse
              setCell(j, i, board, super, ort); //setH(i, j, board, super);
    }

  private:
    uint8_t *bp; //board pointer
    uint8_t *sp; //super pointer, its super
    uint8_t _length; //length of the ship
    uint8_t _max;
    bool _sunk; //sunk state

    //vertical and horizontal, cell FIT CHECK, virtical, ort = true
    bool fitCell(uint8_t i, uint8_t j, uint8_t board[10][10], bool ort)
    {
      _max = i + _length;
      if (_max > 10) //check ship length and compare it to the board length
        return 1; //wont fit
      for (; i < _max; i++) //scan the total length of ship
      {
        ort ? bp = &board[i][j] : bp = &board[j][i];
        if ((*bp == 2) || (*bp == 3)) //if the space if full
          return 1; //wont fit
      }
      return 0;
    }

    //vertical and horizontal, cell FILL, virtical, ort = true
    void setCell(uint8_t i, uint8_t j, uint8_t board[10][10], uint8_t super[10][10], bool ort)
    {
      _max = i + _length;
      for (; i < _max; i++) //scan the total length of ship
      {
        ort ? bp = &board[i][j] : bp = &board[j][i];
        ort ? sp = &super[i][j] : sp = &super[j][i];
        if (*bp == 1) //if the space has been hit
        {
          *sp = 0; //zero it out
          if ((i + 1) <= 9) //noth & west check, 4 cells all round
          {
            ort ? bp = &board[i + 1][j] : bp = &board[j][i + 1];
            if (*bp == 0) //check if its free
            {
              ort ? sp = &super[i + 1][j] : sp = &super[j][i + 1];
              *sp += 20; //then heavy weight it
            }
          }
          if ((i - 1) >= 0) //south & east check, 4 cells all round
          {
            ort ? bp = &board[i - 1][j] : bp = &board[j][i - 1];
            if (*bp == 0) //check if its free
            {
              ort ? sp = &super[i - 1][j] : sp = &super[j][i - 1];
              *sp += 20; //then heavy weight it
            }
          }
        }
        else
          *sp += 1; //increment super
      }
    }
};

//Game class, dose game stuff
class Game
{
  public:
    Game()
    {
      //init ship length
      shipLength[0] = 5;
      shipLength[1] = 4;
      shipLength[2] = 3;
      shipLength[3] = 3;
      shipLength[4] = 2;

      //create all the ship objects with the right length
      for (i = 0; i < 5; i++)
        myShips[i] = new Ships(shipLength[i]); //create my ships
      newGame();
    }

    ~Game()
    {
      for (i = 0; i < 5; i++)
        delete[] myShips[i]; //duke nuke um
    }

    //start a new game, clear all vars
    void newGame(void)
    {
      memset(Xboard, 0, sizeof(Xboard)); //clear board
      memset(super, 0, sizeof(super)); //clear super
      for (i = 0; i < 5; i++)
        myShips[i]->sink(0); //unsink my ships
      lastMoveX = 0; //clear moves
      lastMoveY = 0; //clear moves
    }

    //update the board
    void updateBoard(bool hm)
    {
      if (hm) //if hit
        Xboard[lastMoveY][lastMoveX] = 1; //set hit cell
      else
        Xboard[lastMoveY][lastMoveX] = 2; //set miss cell
      //display for testing
      //printBoard(Xboard, 32);
    }

    //update the board
    void updateBoard(bool hm, uint8_t sunk) //sunk = ship index + 1
    {
      updateBoard(hm);
      if (sunk > 0)
      {
        myShips[sunk - 1]->sink(); //sink the ship
        findShip(sunk - 1); //locate the sunk ship
      }
    }

    //returns the best move in battleship format 'a7', 'd3'
    const char* getMove()
    {
      findSuper(); //find the super position for the game board
      findBest(); //find the highist rated move in the super position
      static char a[10] = {0};
      snprintf(a, 10, "%c%d", (char)lastMoveY + 97, lastMoveX + 1); //convert to c-string
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

  private:
    uint8_t i;
    uint8_t lastMoveX, lastMoveY; //move vars
    uint8_t shipLength[5]; //length of the ships in the game
    uint8_t super[10][10]; //super position
    //Xboard, hit=1, miss=2, sunk=3, free=0;
    uint8_t Xboard[10][10]; //game board
    Ships *myShips[5]; //array of ship objects

    //calulate new super position for the current board
    void findSuper(void)
    {
      memset(super, 0, sizeof(super)); //clear the super position
      for (i = 0; i < 5; i++)
      {
        if (!myShips[i]->sunk()) //if the ship is still in play
          myShips[i]->target(Xboard, super); //use it in the new super position
      }
    }

    //locate the sunk ship and change the state from hit to sunk
    bool findShip(uint8_t len)
    {
      uint8_t Y = 0, X = 0; //hit index

      for (i = 0; i < 10; i++)
      {
        Xboard[lastMoveY][i] == 1 ? Y++ : 0; //inc y hit count if cell is 1
        Xboard[i][lastMoveX] == 1 ? X++ : 0; //int x hit count if cell is 1
      }

      if ((Y == shipLength[len]) && (X == 1)) //figure out the orientation
      {
        X = 0;
        while (Xboard[lastMoveY][X] != 1) //count untill we find the hit ship
          X++;
        for (i = X; i < shipLength[len] + X; i++) //change the state, horizontal
          Xboard[lastMoveY][i] = 3; //use last move as ref
        return 0;
      }
      else if ((X == shipLength[len]) && (Y == 1)) //figure out the orientation
      {
        Y = 0;
        while (Xboard[Y][lastMoveX] != 1) //count untill we find first hit of the ship
          Y++;
        for (i = Y; i < shipLength[len] + Y; i++) //change the state, vertical
          Xboard[i][lastMoveX] = 3; //use last move as ref
        return 0;
      }
      return 1;
    }

    //find the best move in the super position
    void findBest()
    {
      uint8_t a = 0, b = 0;
      for (uint8_t j = 0; j < 5; j++)
        for (i = 0; i < 10; i++)
        {
          b = super[i][j] > super[i][j + 5] ? super[i][j] : super[i][j + 5]; //'b' is now the largist of the two numbers
          if (a < b) //if 'a' is bigger than the largist
          {
            a = b; //a becomes the largist
            lastMoveY = i; //'i' dont change between the two
            lastMoveX = a == super[i][j] ? j : j + 5; //check which number was assigned, give it a 'j' val.
          } //I save 50 loops, faster = better?
        }
    }
};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  /* open up serial to start the running everything */
  /* otherwise you may go insane with rage */
  while (!Serial) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
  randomSeed(analogRead(0)); //seed the 'not so' random number generator
  Serial.println("Battleship Game Engine for the arduino");
  Serial.println("Slash/Byte, 2018");
  Serial.println("--------------------------------------");
}

// the loop function runs over and over again forever
void loop()
{
  /* start of engine test */
  uint8_t max = 10, min = 100;
  uint8_t OP[10][10] = {{0}};
  uint8_t shipLength[5] = {5, 4, 3, 3, 2};
  uint8_t shipCount[5] = {0};

  Game myGame; //game object

  /* change to set amount of games played */
  int gameCnt = 10; //number of games to play
  Serial.println("Games started...");
  int freq[101] = {0};

  /* main game loop */
  for (int i = 0; i < gameCnt; i++)
  {
    uint8_t X = 0, Y = 0;
    uint8_t sunk = 0;
    uint8_t mv = 0;
    uint8_t z = 0;

    myGame.newGame();
    memset(OP, 0, sizeof(OP));
    memset(shipCount, 0, sizeof(shipCount));
    genGame(OP);

    //for displaying the gen-ed board
    //printBoard(OP, 96);

    /* 2nd game loop */
    while (sunk != 5)
    {
      mv++; //count moves made
      myGame.getMove(Y, X); //get move from engine
      z = OP[Y][X];
      if (z == 0) //check board to solve
        myGame.updateBoard(0); //send miss to engine
      else //send hit
      {
        shipCount[z - 1] += 1;
        if (shipCount[z - 1] == shipLength[z - 1]) //is the ship was a hink and sink
        {
          sunk++;
          myGame.updateBoard(1, z); //send which ship sunk
        }
        else
          myGame.updateBoard(1); //send hit to engine
      }
    }
    freq[mv]++; //add move count to frequency array
    /* uncomment to view some info */
    /*
      Serial.print("Game: ");
      Serial.print(i + 1);
      Serial.print("/");
      Serial.print(gameCnt);
      Serial.print(" Finished, ");
      Serial.print("Move Count: ");
      Serial.println(mv);
    */
  }
  int avg = 0;
  for (int i = 1; i < 101; i++) //calc average
    avg += (i * freq[i]);
  Serial.print("Average Moves till Victory: ");
  Serial.println((float)avg / (float)gameCnt, 2); //displays move average
  /* end of engine test */
  Serial.println("I'm gona stay in a loop now...");
  while (1)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
}

/* prints the board to serial */
void printBoard(uint8_t OP[10][10], int font)
{
  Serial.println("-------------------");
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      Serial.print((char)(OP[i][j] + font));
      Serial.print(" ");
    }
    Serial.println();
  }
}

/* gens a random board state to solve */
void genGame(uint8_t op[10][10])
{
  uint8_t length[5] = {5, 4, 3, 3, 2};
  bool ort[5];
  uint8_t A[5];
  uint8_t B[5];
  for (uint8_t i = 0; i < 5; i++)
  {
    do
    {
      ort[i] = random(2); //0-1
      A[i] = random(10); //0-9
      B[i] = random(10); //0-9
    }
    while (checkFit(op, ort[i], A[i], B[i], length[i]) == 1);

    for (uint8_t j = A[i]; j < A[i] + length[i]; j++)
      ort[i] == 1 ? op[j][B[i]] = i + 1 : op[B[i]][j] = i + 1;
  }
}

/* part of 'genGame()' */
bool checkFit(uint8_t op[10][10], bool ort, uint8_t A, uint8_t B, uint8_t length)
{
  if ((A + length) > 10)
    return 1; //wont fit

  uint8_t *a;
  for (uint8_t i = A; i < A + length; i++)
  {
    ort == 1 ? a = &op[i][B] : a = &op[B][i];
    if (*a > 0)
      return 1;
  }
  return 0;
}
