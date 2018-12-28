# Arduino Battleship Engine
A minimal, but very functional, battleship 'AI' that runs on arduino. Should compile and run just fine.
```
Sketch uses 6914 bytes (21%) of program storage space. Maximum is 32256 bytes.
Global variables use 390 bytes (19%) of dynamic memory, leaving 1658 bytes for local variables. Maximum is 2048 bytes.
```

## Serial Output of 'barracudaSmall.ino
```
Battleship Game Engine for the arduino
Slash/Byte, 2018

Now Solving, Finished in 381 ms
Move Count: 54
```
### Game Data
```
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
```
Solved with no knowledge of the "opponents" board 
## Notes
This is test code, so its not perfect, keep that in mind.

## License
This project is licensed under the MIT License
(https://opensource.org/licenses/MIT)
## Acknowledgments
* Thanks Science

