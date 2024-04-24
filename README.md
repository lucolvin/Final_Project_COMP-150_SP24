# 2048 Game

This is a simple implementation of the popular game 2048, written in C++ for the command line.

## Cross-Platform
Added cross-platform functionality. 
Keep in mind this program was made on and for Mac OS so the windows version is a bit sub par.
At the moment all that means is no unicode so it dosent look as good, but that may change in the future.
## How to Play

Use the following keys to move the tiles:

- 'w' for up
- 's' for down
- 'a' for left
- 'd' for right

- Uppercase letter and arrow keys are also now supported

The goal is to combine tiles until you create a tile with the number 2048.

## How to Run

To compile the game, use the following command:

```bash
g++ -o 2048 2048.cpp
```

> [!CAUTION]
> test