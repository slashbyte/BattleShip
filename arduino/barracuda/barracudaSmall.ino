/*
   Slash/Byte 2018
   Battleship Game engine for hardware! ...arduino
   it was originally the motivation for building a game engine for battleship
   used v2 code for this, its on my github somewhere.

   smaller, changed very little.
   Sketch uses 4028 bytes (12%) of program storage space. Maximum is 32256 bytes.
   Global variables use 294 bytes (14%) of dynamic memory, leaving 1754 bytes for local variables. Maximum is 2048 bytes.

   used this blogpost as a guide.
   http://www.datagenetics.com/blog/december32011/
*/

//prints the internal board state over serial
//void printBoard(uint8_t OP[10][10], int font = 96); //32 works too

class Barracuda
{
  public:
    Barracuda() {
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
    uint8_t _length[5] = {5, 4, 3, 3, 2};
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
        }
        else
          *sp += 1;
      }
    }

};

void Blink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  /* open up serial to start running everything */
  /* otherwise you may go insane with rage */
  while (!Serial)
    Blink();
  Serial.println("Battleship Game Engine for the arduino");
  Serial.println("Slash/Byte, 2018\n");
}

// the loop function runs over and over again forever
void loop() {
  /* start of engine test */
  uint8_t X = 0, Y = 0, sunk = 0, mv = 0, z = 0;
  uint8_t shipLength[5] = {5, 4, 3, 3, 2};
  uint8_t shipCount[5] = {0};
  //board to solve
  uint8_t OP[10][10] = {
    {0, 0, 4, 4, 4, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 2, 2, 2, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 3, 3, 3, 0},
    {0, 0, 5, 5, 0, 0, 0, 0, 0, 0}
  };
  Barracuda myGame; //game object
  Serial.print("Now Solving, ");
  unsigned long StartTime = millis(); //start the clock!
  /* main game loop */
  while (sunk != 5) {
    mv++; //count moves made
    myGame.getMove(Y, X); //get move from engine
    z = OP[Y][X];
    if (z == 0) //check board to solve
      myGame.updateBoard(0); //send miss to engine
    else { //send hit
      shipCount[z - 1] += 1;
      if (shipCount[z - 1] == shipLength[z - 1]) { //is the ship was a hit and sunk
        sunk++;
        myGame.updateBoard(1, z); //send which ship sunk
      }
      else
        myGame.updateBoard(1); //send hit to engine
    }
  }
  unsigned long CurrentTime = millis();
  Serial.print("Finished in ");
  Serial.print(CurrentTime - StartTime);
  Serial.println(" ms");
  Serial.print("Move Count: ");
  Serial.println(mv);
  /* end of engine test */
  while (1) //'NASA' approved while loop
    Blink();
}

/* prints the board to serial */
/*
  void printBoard(uint8_t OP[10][10], int font) {
  Serial.println("-------------------");
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      Serial.print((char)(OP[i][j] + font));
      Serial.print(" ");
    }
    Serial.println();
  }
  }
*/

//EOF
