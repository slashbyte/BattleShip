# Arduino Battleship Engine
A minimal, but very functional, battleship 'AI' that runs on arduino. Should compile and run just fine.

## Testing 'barracudaSmall.ino'
seeduino v4.2, 16Mhz
### Compiler Output
```
Sketch uses 4028 bytes (12%) of program storage space. Maximum is 32256 bytes.
Global variables use 294 bytes (14%) of dynamic memory, leaving 1754 bytes for local variables. Maximum is 2048 bytes.
```
### Results
```
Battleship Game Engine for the arduino
Slash/Byte, 2018

Now Solving, Finished in 381 ms
Move Count: 54
```
### Game Data / Board State
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
Barracuda Solved this board state with no knowledge of the board, in 381 milliseconds, 54 consecutive moves.
## More Tests 'barracudaSmall.ino'
Adafruit ItsyBitsy M4 Express, 120 MHz. The same board state from above was used during testing.
### Compiler Output
```
Sketch uses 11984 bytes (2%) of program storage space. Maximum is 507904 bytes.
```
### Results
```
Battleship Game Engine for the arduino
Slash/Byte, 2018

Now Solving, Finished in 29 ms
Move Count: 54

```
## Notes
This is test code, so its not perfect, keep that in mind.

## License
This project is licensed under the MIT License
(https://opensource.org/licenses/MIT)
## Acknowledgments
* Thanks Science
* http://www.datagenetics.com/blog/december32011/
