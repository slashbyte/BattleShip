#include <windows.h>  //ewwww....
#include <stdio.h> 
#include <string>     // memset & stuff
#include <stdlib.h>   // srand, rand
#include <time.h>     // time 
#include <unistd.h>   //getopt

class Stream
{
//https://docs.microsoft.com/en-us/windows/desktop/fileio/opening-a-file-for-reading-or-writing	
public:
	
	int BUFSIZE;
	char fileName[256];
	
	HANDLE g_hChildStd_IN_Rd = NULL; /* child process's STDIN is the user input or data that you enter into the child process - READ */
	HANDLE g_hChildStd_IN_Wr = NULL; /* child process's STDIN is the user input or data that you enter into the child process - WRITE */
	HANDLE g_hChildStd_OUT_Rd = NULL; /* child process's STDOUT is the program output or data that child process returns - READ */
	HANDLE g_hChildStd_OUT_Wr = NULL; /* child process's STDOUT is the program output or data that child process returns - WRITE */
	
	SECURITY_ATTRIBUTES saAttr;
	
	Stream(char a[])
	{ 
		BUFSIZE = 1024; 
		snprintf(fileName, 256, "cmd.exe /c \"%s\"", a);
	}
	
	bool begin(void)
	{
		// Set the bInheritHandle flag so pipe handles are inherited. 
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;
		//child process's STDOUT is the program output or data that child process returns
   		// Create a pipe for the child process's STDOUT. 
	    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))	
			{ printf("StdoutRd CreatePipe Error\n"); return 1; }
	    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))	
			{ printf("Stdout SetHandleInformation Error\n"); return 1; }
		if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))	
			{ printf("Stdin CreatePipe Error\n"); return 1; }
	    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))	
			{ printf("Stdin SetHandleInformation Error\n"); return 1; }
	    if (CreateChildProcess())												
			{ printf("Cannot create child process.\n"); return 1; }
	    return 0;
	}
	
	// Read output from the child process's pipe for STDOUT
	// and write to the parent process's pipe for STDOUT. 
	// Stop when there is no more data.
	bool ReadFromPipe(char a[], int _size)
	{
	    DWORD dwRead, dwWritten;
	    char chBuf[BUFSIZE] = {0};
	    BOOL bSuccess = FALSE;
	    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	    WORD wResult = 0;
	    
	    bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
	    if (!bSuccess || dwRead == 0)
	    	return 1;
	    snprintf(a, _size, "%s", chBuf);
	    //printf("READ: %s", a);
	    return 0;		
	}
	
	// Read from a file and write its contents to the pipe for the child's STDIN.
	// Stop when there is no more data. 
	bool WriteToPipe(const char chBuf[])
	{
	    DWORD dwRead, dwWritten;
	    dwRead = strlen(chBuf);
	    BOOL bSuccess = FALSE;
	    //printf("WRITE: %s", chBuf);
	    bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
	    if (!bSuccess)
	    	return 1;
		return 0;
	}
	
	// Create a child process that uses the previously created pipes for STDIN and STDOUT.
	bool CreateChildProcess()
	{
	    PROCESS_INFORMATION piProcInfo;
	    STARTUPINFO siStartInfo;
	    BOOL bSuccess = FALSE;
	
	    // Set up members of the PROCESS_INFORMATION structure. 
	    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	
	    // Set up members of the STARTUPINFO structure. 
	    // This structure specifies the STDIN and STDOUT handles for redirection.
	    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	    siStartInfo.cb = sizeof(STARTUPINFO);
	    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	
	    // Create the child process. 
	    bSuccess = CreateProcess(NULL,
	    	fileName,      // command line 
	        NULL,          // process security attributes 
	        NULL,          // primary thread security attributes 
	        TRUE,          // handles are inherited 
	        0,             // creation flags 
	        NULL,          // use parent's environment 
	        NULL,          // use parent's current directory 
	        &siStartInfo,  // STARTUPINFO pointer 
	        &piProcInfo);  // receives PROCESS_INFORMATION 
	
	    if (!bSuccess)
		{
			printf("CreateProcess Error\n"); 
			return 1; // If an error occurs, exit the application. 
		}
	    else
		{
			CloseHandle(piProcInfo.hProcess); // Close handles to the child process and its primary thread.
			CloseHandle(piProcInfo.hThread); 
		}
	    return 0;
	}
};

//build a board for game play following the rules of the game
class buildBoard
{
public:
	int uBoard[10][10];
	int shipLength[5];
	
	buildBoard() //init all the things
	{
		//srand(5);
		srand (time(NULL)); //seed
		memset(uBoard, 0, sizeof(uBoard));
		shipLength[0] = 5; //init ship length
        shipLength[1] = 4;
        shipLength[2] = 3;
        shipLength[3] = 3;
        shipLength[4] = 2;	
	}
	
	void build(int a[10][10]) //generates a random board
	{
		memset(uBoard, 0, sizeof(uBoard));	
		genGame();
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < 10; j++)
				a[i][j] = uBoard[i][j];
	}
	
	void genGame(void) //man none of this is labled right lol
	{
		bool ort[5]; //orientation
		int startY[5];
		int startX[5];
	    for(int i = 0; i < 5; i++) //generate a bunch of randim start positions
	    {
	        do
	        {
	            ort[i] = rand() % 2; //0-1
	            startY[i] = rand() % 10; //0-9
	            startX[i] = rand() % 10; //0-9
	        }
	        while(checkFit(ort[i], startY[i], startX[i], shipLength[i]) == 1); //keeps gen till they fit
	
	        if(ort[i]) //if one orientation
	        {
	            for(int j = startY[i]; j < startY[i] + shipLength[i]; j++) //place the ship
	                uBoard[j][startX[i]] = i+1;
	        }
	        else //if another
	        {
	            for(int j = startY[i]; j < startY[i] + shipLength[i]; j++) //place the ship
	                uBoard[startX[i]][j] = i+1;
	        }
	    }
	}
	
	bool checkFit(bool ort, int startY, int startX, int length) //checks to see if the ships will fit
	{
	    if((startY + length) > 10)
	        return 1; //wont fit
	    if(ort)
	    {
	        for(int i = startY; i < startY + length; i++)
	            if(uBoard[i][startX] > 0)
	                return 1; //overlap
	    }
	    else
	    {
	        for(int i = startY; i < startY + length; i++)
	            if(uBoard[startX][i] > 0)
	                return 1; //overlap
	    }
	    return 0;
	}
};


class Board //keeps track of the game board
{
	public:
	int uBoard[10][10]; //board in play
	int shipLength[5]; //yes
	int hitCount[5]; //hit ships
	bool history[10][10]; //history of moves
	int moveCnt; //number of moves
	buildBoard GB; //board generator
	
	Board() //init all the things
	{
		shipLength[0] = 5; //init ship length
        shipLength[1] = 4;
        shipLength[2] = 3;
        shipLength[3] = 3;
        shipLength[4] = 2;
        newGame();
	}
	
	void newGame(void) //sets everything back to zero
	{
		memset(uBoard, 0, sizeof(uBoard));
		memset(hitCount, 0, sizeof(hitCount));
		memset(history, 0, sizeof(history));
		moveCnt = 0;
		GB.build(uBoard); //builds board
	}
	
	void newGame(int a[10][10]) //sets everything back to zero, use a pre-gen board
	{
		memset(uBoard, 0, sizeof(uBoard));
		memset(hitCount, 0, sizeof(hitCount));
		memset(history, 0, sizeof(history));
		moveCnt = 0;
		//copy a board
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < 10; j++)
				uBoard[i][j] = a[i][j];
	}
	
	bool updateBoard(int y, int x, char a[]) //updates the play board
	{
		int m = y<0 | y>9 | x<0 | x>9; //checks move syntax
		if(m)
		{
			printf("Bad Move Syntax\n");
			return 1; //bad move
		}
	
		if(history[y][x] > 1) //check if already there
		{
			printf("Move Already Played\n");
			return 1; //bad move
		}
	
		history[y][x]++; //update history

		int b = uBoard[y][x];
		if(b > 0) //hit ship
		{
			hitCount[b-1]++; //add to cnt
			if(hitCount[b-1] == shipLength[b-1]) //if sunk
				sprintf(a, "hit %d\n", b); //send to engine
			else
				sprintf(a, "hit\n"); //send to engine
		}
		else //missed ship
			sprintf(a, "miss\n"); //send to engine

		return 0;
	}
	
	bool checkGame(void) //checks the state of the game, if you still have ships
	{
		int m = 0;
		for(int i = 0; i < 5; i++)
			if(hitCount[i] == shipLength[i])
				m++;
		if(m==5)
			return 1;
		return 0; //true if all ships sunk
	}
	
	void madeMove(void) //inc move counter
	{
		moveCnt++;
	}
	
	int getMoveCnt(void) //get moves
	{
		return moveCnt;
	}
	
};


void printBoard(int a[10][10], bool b[10][10]) //print board with hit overlay
{
	int j = 0;	
    for(int i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
        	if((a[i][j] > 0) && (b[i][j] == 1))
        		printf("X "); //hit
        	else if(b[i][j] == 1)
        		printf("@ "); //miss
        	else if(a[i][j] == 0)
        		printf(". "); //blank
        	else
        		printf("%.1d ", a[i][j]);
		}
		printf("\n");
    }
}


int getY(char a[]) //get the y value from string
{
	char y = 0;
	if(sscanf(a, "Incoming: %c", &y) == 1)
		return (y - 97);
	return 99;
}

int getX(char a[]) //get the x value from string
{
	char y = 0;
	int x = 0;
	if(sscanf(a, "Incoming: %c%d", &y, &x) == 2)
		return x-1;
	return 99;	
}

//run move freq check, test, thing
bool runF(char engineA[100], int gameS, bool flagF, bool flagV)
{
	int moveCounter[102] = {0}; //setup move freq counter
	char ioBufA[256] = {0};
	Board boardA;
	Stream playerA(engineA);
	if(playerA.begin())
		return 1; //cant start "stuff", fail hard!
		
	printf("Starting Move Analysis\n");
	printf("%d Random games to be played\n", gameS);
		
	for(int i = 0; i < gameS; i++) //start a batch of games
	{
		boardA.newGame(); //gen new board
		if(playerA.WriteToPipe("new\n")) //tell the engine to astart a new game
			return 1; //cant write to pipe
		while(1)
		{
			if(playerA.WriteToPipe("go\n"))
				return 1; //cant write to pipe
			if(playerA.ReadFromPipe(ioBufA,100))
				return 1; //cant read from pipe
			else
			{
				if(!boardA.updateBoard(getY(ioBufA), getX(ioBufA), ioBufA))
				{
					if(playerA.WriteToPipe(ioBufA))
						return 1; //cant write to pipe
					boardA.madeMove();
				}
				else
				{
					printf("Something Broke\n");
					return 1; //bad move					
				}
			}
			if(boardA.checkGame())
				break;
		}
		moveCounter[boardA.getMoveCnt()]++;
		if(!flagV)
			printf("Game %d finished\n", i);
	}
	
	printf("Finished\n");
	printf("--------------------------------\n");
	printf("Results for: %s\n", engineA);
	printf("--------------------------------\n");
	int avg = 0;
	for(int i = 0; i < 101; i++) //calc average
		avg += (i*moveCounter[i]);
	printf("Average Moves till Victory: %.2f\n", (float)avg/(float)gameS);
	avg = 0;
	for(int i = 0; i < 101; i++) //find max
		if(moveCounter[i] > 0)
			avg = i;
	printf("Max moves per game: %d\n", avg);
	avg = 0;
	for(int i = 0; i < 101; i++) //find min
		if(moveCounter[i] > 0)
		{
			avg = i;
			break;
		}
	printf("Min moves per game: %d\n", avg);
	if(flagF)
	{
		printf("Move Frequency Count\n");
		for(int i = 0; i < 101; i++)
			printf("%d\t%d\n",i,moveCounter[i]); //display to much shit
	}
	printf("--------------------------------\n");
	return 0;
}

bool runS(char engineA[100], char engineB[100], int gameS, bool flagV)
{
	int boardShare[10][10] = {0};
	char ioBufA[256] = {0};
	char ioBufB[256] = {0};
	Board boardA;
	Board boardB;
	buildBoard GB;
	Stream playerA(engineA);
	Stream playerB(engineB);
	if(playerA.begin())
		return 1; //cant start "stuff", fail hard!	
	if(playerB.begin())
		return 1; //cant start "stuff", fail hard!		
		
	printf("Starting Analysis\n");
	printf("%d Random games to be played\n", gameS);
	printf("Each engine solves the same board.\n");
	printf("--------------------------------\n");
		
	for(int i = 0; i < gameS; i++)
	{
		GB.build(boardShare);
		
		boardA.newGame(boardShare);
		boardB.newGame(boardShare);
		
		if(playerA.WriteToPipe("new\n")) //tell the engine to start a new game
			return 1; //cant write to pipe
		if(playerB.WriteToPipe("new\n")) //tell the engine to start a new game
			return 1; //cant write to pipe
			
		while(1)
		{
			if(playerA.WriteToPipe("go\n"))
				return 1; //cant write to pipe
			if(playerA.ReadFromPipe(ioBufA,100))
				return 1; //cant read from pipe
			else
			{
				if(!boardA.updateBoard(getY(ioBufA), getX(ioBufA), ioBufA))
				{
					if(playerA.WriteToPipe(ioBufA))
						return 1; //cant write to pipe
					boardA.madeMove();
				}
				else
				{
					printf("Something Broke\n");
					return 1; //bad move					
				}
			}
			if(boardA.checkGame())
				break;
		}
		if(!flagV)
			printBoard(boardA.uBoard, boardA.history);
		printf("Solved in %d, %s\n",boardA.getMoveCnt(), engineA);
		
		while(1)
		{
			if(playerB.WriteToPipe("go\n"))
				return 1; //cant write to pipe
			if(playerB.ReadFromPipe(ioBufB,100))
				return 1; //cant read from pipe
			else
			{
				if(!boardB.updateBoard(getY(ioBufB), getX(ioBufB), ioBufB))
				{
					if(playerB.WriteToPipe(ioBufB))
						return 1; //cant write to pipe
					boardB.madeMove();
				}
				else
				{
					printf("Something Broke\n");
					return 1; //bad move					
				}
			}
			if(boardB.checkGame())
				break;
		}
		if(!flagV)
			printBoard(boardB.uBoard, boardB.history);
		printf("Solved in %d, %s\n",boardB.getMoveCnt(), engineB);
		printf("-Game %d finished\n", i);
	}		
	return 0;
}


bool runS(char engineA[100], int gameS, bool flagV)
{
	char ioBufA[256] = {0};
	Board boardA;
	Stream playerA(engineA);
	if(playerA.begin())
		return 1; //cant start "stuff", fail hard!			
		
	printf("Starting Analysis\n");
	printf("%d Random games to be played\n", gameS);
	printf("--------------------------------\n");
		
	for(int i = 0; i < gameS; i++)
	{
		boardA.newGame();
		
		if(playerA.WriteToPipe("new\n")) //tell the engine to astart a new game
			return 1; //cant write to pipe
		while(1)
		{
			if(playerA.WriteToPipe("go\n"))
				return 1; //cant write to pipe
			if(playerA.ReadFromPipe(ioBufA,100))
				return 1; //cant read from pipe
			else
			{
				if(!boardA.updateBoard(getY(ioBufA), getX(ioBufA), ioBufA))
				{
					if(playerA.WriteToPipe(ioBufA))
						return 1; //cant write to pipe
					boardA.madeMove();
				}
				else
				{
					printf("Something Broke\n");
					return 1; //bad move					
				}
			}
			if(boardA.checkGame())
				break;
		}
		if(!flagV)
			printBoard(boardA.uBoard, boardA.history);
		printf("Solved in %d, %s\n",boardA.getMoveCnt(), engineA);
		printf("-Game %d finished\n", i);
	}		
	return 0;
}

bool runD(char engineA[100], bool flagV)
{
	char ioBufA[256] = {0};
	Board boardA;
	Stream playerA(engineA);
	if(playerA.begin())
		return 1; //cant start "stuff", fail hard!			
		
	printf("Solving Random board\n");
	printf("--------------------------------\n");
		
	if(playerA.WriteToPipe("new\n")) //tell the engine to astart a new game
		return 1; //cant write to pipe
	while(1)
	{
		if(playerA.WriteToPipe("go\n"))
			return 1; //cant write to pipe
		if(playerA.ReadFromPipe(ioBufA,100))
			return 1; //cant read from pipe
		else
		{
			if(!boardA.updateBoard(getY(ioBufA), getX(ioBufA), ioBufA))
			{
				if(playerA.WriteToPipe(ioBufA))
					return 1; //cant write to pipe
				boardA.madeMove();
			}
			else
			{
				printf("Something Broke\n");
				return 1; //bad move					
			}
		}
		if(!flagV)
		{
			printf("\nMove: %d\n", boardA.getMoveCnt());
			printBoard(boardA.uBoard, boardA.history);
		}
		if(boardA.checkGame())
			break;
	}
	printf("Solved in %d, %s\n",boardA.getMoveCnt(), engineA);
	printf("-finished\n");
		
	return 0;
}

bool runM(char engineA[100], char engineB[100], int gameS, bool flagV)
{
	char ioBufA[256] = {0};
	char ioBufB[256] = {0};
	Board boardA;
	Board boardB;
	Stream playerA(engineA);
	Stream playerB(engineB);
	if(playerA.begin())
		return 1; //cant start "stuff", fail hard!	
	if(playerB.begin())
		return 1; //cant start "stuff", fail hard!		
		
	bool aFlag = 0;
	bool bFlag = 0;
	
	int aWin = 0;
	int bWin = 0;
		
	printf("Starting Analysis\n");
	printf("%d Random matches to be played\n", gameS);
	printf("Each engine solves a random board.\n");
	printf("--------------------------------\n");
		
	for(int i = 0; i < gameS; i++)
	{
		boardA.newGame();
		boardB.newGame();
		
		if(playerA.WriteToPipe("new\n")) //tell the engine to start a new game
			return 1; //cant write to pipe
		if(playerB.WriteToPipe("new\n")) //tell the engine to start a new game
			return 1; //cant write to pipe
		//a solve
		while(1)
		{
			if(playerA.WriteToPipe("go\n"))
				return 1; //cant write to pipe
			if(playerA.ReadFromPipe(ioBufA,100))
				return 1; //cant read from pipe
			else
			{
				if(!boardA.updateBoard(getY(ioBufA), getX(ioBufA), ioBufA))
				{
					if(playerA.WriteToPipe(ioBufA))
						return 1; //cant write to pipe
					boardA.madeMove();
				}
				else
				{
					printf("Something Broke\n");
					return 1; //bad move					
				}
			}
			if(boardA.checkGame())
			{
				aFlag = 1;
				aWin++;
				break;
			}
				
			if(playerB.WriteToPipe("go\n"))
				return 1; //cant write to pipe
			if(playerB.ReadFromPipe(ioBufB,100))
				return 1; //cant read from pipe
			else
			{
				if(!boardB.updateBoard(getY(ioBufB), getX(ioBufB), ioBufB))
				{
					if(playerB.WriteToPipe(ioBufB))
						return 1; //cant write to pipe
					boardB.madeMove();
				}
				else
				{
					printf("Something Broke\n");
					return 1; //bad move					
				}
			}
			if(boardB.checkGame())
			{
				bFlag = 1;
				bWin++;
				break;
			}			
		}
	
		if(!flagV)
		{
			printf("Victory goes to, ");
			if(aFlag)
				printf("%s\nCompleted in %d Moves\n", engineA, boardA.getMoveCnt());
			else
				printf("%s\nCompleted in %d Moves\n", engineB, boardB.getMoveCnt());
				
			printf("%s\n", engineB);
			printBoard(boardA.uBoard, boardA.history);
			printf("\n");
			printf("%s\n", engineA);
			printBoard(boardB.uBoard, boardB.history);		
			printf("\n");	
		}
		printf("--Game %d finished\n", i);
	}		
	
	printf("--------------------------------\n");
	printf("%s\n", engineA);
	printf("Wins %d\n", aWin);
	printf("Loss %d\n", gameS-aWin);
	printf("--------------------------------\n");	
	printf("%s\n", engineB);
	printf("Wins %d\n", bWin);
	printf("Loss %d\n", gameS-bWin);
	printf("--------------------------------\n");
	printf("The winning engine is,\n");
	if(aWin != bWin)
	{
		if(aWin > bWin)
			printf("%s, by %d wins\n", engineA, aWin-bWin);
		else
			printf("%s, by %d wins\n", engineB, bWin-aWin);
	}
	else
		printf("The engines are matched in skill\n");
	printf("--------------------------------\n");
	
	return 0;
}

int main(int argc, char** argv)
{
	printf("Slash/Byte                             2018\n");
    printf("Uboard, Battleship engine benchmark program\n");
	printf("-------------------------------------------\n");
	
	char engineA[100] = {0};
	char engineB[100] = {0};
	int gameS = 10;
	bool flagF=0, flagV=0, flagM=0;
	bool bSet=0, flagS=0, flagD=0, aSet=0;
		
	int c;
	while((c = getopt(argc, argv, "a:n:b:fhvmsd")) != -1)
	{
		switch(c)
		{
			case 'a':
				snprintf(engineA, 100, "%s", optarg);
				aSet = 1;
				break;
			case 'b':
				snprintf(engineB, 100, "%s", optarg);
				bSet = 1;
				break;				
			case 'n':
				sscanf(optarg, "%d", &gameS);
				break;
			case 'f':
				flagF = 1;
				break;
			case 'v':
				flagV = 1;
				break;
			case 'm':
				flagM = 1;
				break;
			case 's':
				flagS = 1;
				break;	
			case 'd':
				flagD = 1;
				break;												
			case 'h':
				printf("Usage: %s -a engineone.exe -n 100 [-f -v]\n", argv[0]);
				printf("       %s -a engineone.exe -n 1 -s [-v]\n", argv[0]);
				printf("       %s -a engineone.exe -b enginetwo.exe -n 100 -m [-v]\n", argv[0]);
				printf("       %s -a engineone.exe -b enginetwo.exe -n 1 -s [-v]\n", argv[0]);
				printf("       %s -a engineone.exe -d [-v]\n", argv[0]);
				return 0;
				break;
			case '?':
				printf("Unknown option %c\n", optopt);
				return 0;
			default:
				break;
		}
	}
	
	if(!aSet) //need one engine
	{
		printf("No EngineA, bye!\n");	
		return 0;
	}
	
	if(flagD)
	{
		if(runD(engineA, flagV)) //test move count and freq
		{
			printf("something broke!\n");
			return 1;
		}		
		printf("\nExiting Gracefully\n");
		return 0;		
	}
	
	if(flagS)
	{
		if(!bSet) //if one engine and sflag
		{
			if(runS(engineA, gameS, flagV)) //test move count and freq
			{
				printf("something broke!\n");
				return 1;
			}			
		}
		else //if two engins and sflag
		{
			if(runS(engineA, engineB, gameS, flagV)) //test move count and freq
			{
				printf("something broke!\n");
				return 1;
			}			
		}
		printf("\nExiting Gracefully\n");
		return 0;
	}
	
	if(flagM)
	{		
		if(bSet) //if two engines
		{
			if(runM(engineA, engineB, gameS, flagV)) //test move count and freq
			{
				printf("something broke!\n");
				return 1;
			}			
		}
		else
		{
			printf("No EngineB, bye!\n");
			return 0;
		}
		printf("\nExiting Gracefully\n");
		return 0;		
	}
	
	if(runF(engineA, gameS, flagF, flagV)) //test move count and freq
	{
		printf("something broke!\n");
		return 1;
	}	
	
	printf("\nExiting Gracefully\n");
	return 0;
}
