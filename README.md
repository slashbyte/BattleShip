# Battleship Game Engine
What lives here, is a pretty awesome battleship game engine. But wait theres more, there are two engines that play battleship!

Codename **Barracuda** is an engine that uses "superpositions" to calculate the best possible move.

Codename **The Drunk Capitan** randomly picks a move and sticks to it, even though it only looked good three beers ago.

# Uboard (test program)
This program allows you to pin engines against each other, run tests, and other things. Since theres no standard battleship engine protocol, I winged it. 

### Help File
```
λ Uboard.exe -h
Slash/Byte                             2018
Uboard, Battleship engine benchmark program
-------------------------------------------
Usage: Uboard.exe -a engineone.exe -n 100 [-f -v]
       Uboard.exe -a engineone.exe -n 1 -s [-v]
       Uboard.exe -a engineone.exe -b enginetwo.exe -n 100 -m [-v]
       Uboard.exe -a engineone.exe -b enginetwo.exe -n 1 -s [-v]
       Uboard.exe -a engineone.exe -d [-v]
```

### Usage:
```
Uboard.exe -a engineone.exe -n 100 [-f -v]
```
This runs a "move analysis" on the engine.

* [-a] tells 'Uboard' what engine your running.
* [-f] Displays a move frequency list.
* [-n] Specifies the number of random games to be played.
* [-v] Suppresses some output.

Sample Output
```
Uboard.exe -a drunkcaptain.exe -n 10 -v         
Slash/Byte                             2018       
Uboard, Battleship engine benchmark program       
-------------------------------------------       
Starting Move Analysis                            
10 Random games to be played                      
Finished                                          
--------------------------------                  
Results for: drunkcaptain.exe                     
--------------------------------                  
Average Moves till Victory: 92.70                 
Max moves per game: 100                           
Min moves per game: 82                            
--------------------------------                  

Exiting Gracefully
```
***
```
Uboard.exe -a engineone.exe -b enginetwo.exe -n 1 -s [-v]
```
This runs a random test case on the engines, allowing you to see how well each engine preformed on the same game, side by side.

* [-a] tells 'Uboard' what engine your running.
* [-b] tells 'Uboard' what engine your running.
* [-s] use the same random test boards for the engine.
* [-n] Specifies the number of random games to be played.
* [-v] Suppresses some output.

Sample Output
```
Uboard.exe -a drunkcaptain.exe -b barracuda.exe -n 1 -s   
Slash/Byte                             2018                 
Uboard, Battleship engine benchmark program                 
-------------------------------------------                 
Starting Analysis                                           
1 Random games to be played                                 
Each engine solves the same board.                          
--------------------------------                            
@ @ @ @ @ @ @ . @ @                                         
@ @ @ @ @ @ @ @ @ @                                         
@ X X X X X @ @ @ @                                         
@ @ @ @ @ @ @ @ @ @                                         
@ @ @ @ @ @ @ @ @ @                                         
@ @ @ X @ @ @ @ X X                                         
@ @ @ X X X X X @ @                                         
@ @ @ X @ @ @ @ X @                                         
@ @ . . @ @ @ @ X @                                         
@ @ @ @ @ @ @ @ X @                                         
Solved in 97, drunkcaptain.exe                              
. . @ . . @ . . @ .                                         
. @ @ @ @ . . @ . @                                         
@ X X X X X @ . @ .                                         
. @ @ @ @ . . @ . @                                         
@ . . @ @ . @ . @ @                                         
. . @ X . @ @ . X X                                         
. @ . X X X X X @ .                                         
@ . @ X . . @ . X @                                         
. @ . @ . @ . @ X .                                         
. . . . @ . . . X .                                         
Solved in 55, barracuda.exe                                 
-Game 0 finished                                            

Exiting Gracefully   
```
***
```
Uboard.exe -a engineone.exe -b enginetwo.exe -n 100 -m [-v]
```
This plays engine against engine, AI battle royal.

* [-a] tells 'Uboard' what engine your running.
* [-b] tells 'Uboard' what engine your running.
* [-n] Specifies the number of random games to be played.
* [-v] Suppresses some output.
* [-m] Match mode.

Sample Output
```
Uboard.exe -a drunkcaptain.exe -b barracuda.exe -n 10 -m -v                  
Slash/Byte                             2018                                    
Uboard, Battleship engine benchmark program                                    
-------------------------------------------                                    
Starting Analysis                                                              
10 Random matches to be played                                                 
Each engine solves a random board.                                             
--------------------------------                                               
--Game 0 finished                                                              
--Game 1 finished                                                              
--Game 2 finished                                                              
--Game 3 finished                                                              
--Game 4 finished                                                              
--Game 5 finished                                                              
--Game 6 finished                                                              
--Game 7 finished                                                              
--Game 8 finished                                                              
--Game 9 finished                                                              
--------------------------------                                               
drunkcaptain.exe                                                               
Wins 0                                                                         
Loss 10                                                                        
--------------------------------                                               
barracuda.exe                                                                  
Wins 10                                                                        
Loss 0                                                                         
--------------------------------                                               
The winning engine is,                                                         
barracuda.exe, by 10 wins                                                      
--------------------------------                                               
                                                                               
Exiting Gracefully
```
***
```
Uboard.exe -a engineone.exe -d [-v]
```
This runs a game, turn by turn so you can see the engine performance.

* [-a] tells 'Uboard' what engine your running.
* [-d] Display game mode
* [-v] Suppresses some output.

Sample Output
```
Uboard.exe -a barracuda.exe -d               
Slash/Byte                             2018    
Uboard, Battleship engine benchmark program    
-------------------------------------------    
Solving Random board                           
--------------------------------               
                                               
Move: 1                                        
2 . . . . . . . . .                            
2 . . . . . . . . .                            
2 . . 1 5 . . . . .                            
2 . . 1 5 3 . . . .                            
. . . 1 @ 3 . . . .                            
. . . 1 . 3 . 4 4 4                            
. . . 1 . . . . . .                            
. . . . . . . . . .                            
. . . . . . . . . .                            
. . . . . . . . . .                            

(Alot more boards...)       

Move: 48                          
X . @ . . . @ @ . .               
X . . @ @ . . . @ .               
X @ . X X @ @ . . @               
X . @ X X X . @ . .               
@ @ @ X @ X . . @ .               
. . @ X @ X @ X X X               
@ . . X . @ . . . @               
. @ . @ . . . @ . .               
. . @ . @ . . . @ .               
. . @ . . . @ . . .               
Solved in 48, barracuda.exe       
-finished                         
                                  
Exiting Gracefully 
```

## License
This project is licensed under the MIT License
(https://opensource.org/licenses/MIT)
## Acknowledgments
* Thanks Science

