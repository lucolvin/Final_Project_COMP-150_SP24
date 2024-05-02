# 2048 Game

This is a simple implementation of the popular game 2048, written in C++ for the command line.

## Cross-Platform
Added cross-platform functionality. 
- Keep in mind this program was made on and for Mac OS so the windows version is a bit sub par.
- At the moment all that means is no unicode and instead uses ascii so it dosent look as good as the unicode counterpart.
## How to Play

Use the following keys to move the tiles:

- 'w' for up
- 's' for down
- 'a' for left
- 'd' for right<br>
- ↑ - up arrow for up
- ↓ - down arrow for down 
- ← - left arrow for left
- → - right arrow for right


- Uppercase letters also work for movement<br>

The goal is to combine tiles until you create a tile with the number 2048.<br><br>
<img width="482" alt="Screenshot 2024-05-02 at 3 25 56 PM" src="https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/b947c184-f021-481d-8537-e12f0202b814">

## Flow of the game
<img width="477" alt="image" src="https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/69250f87-82d3-4fc5-88da-f73bf25129c0">

## Main Menu
The menu is centered using the wdth of the terminal<br>
I want to center the ascii art and I have a working version of it but it is not implemented in the game yet.

<img width="781" alt="Screenshot 2024-05-02 at 3 29 52 PM" src="https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/b50e36c2-bef4-4f33-b2ef-6a9d7eac6dfc">

## In-game menu
Lists options other than the direction keys that the user has access to while playing the game 
<img width="461" alt="Screenshot 2024-05-02 at 3 47 43 PM" src="https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/239f3d6e-0655-4a0f-b6a9-e2e9aaf355b2">


## High scores
The high scores are read from high_scores.txt and sorted to the top 5 then displayed<br>
<img width="166" alt="Screenshot 2024-05-02 at 3 33 21 PM" src="https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/00a67a5d-2630-4eb9-9c8c-830d9c0f21a1">



## Game board Unix
Game board on Unix uses unicode characters to draw the board
<img width="461" alt="Screenshot 2024-05-02 at 3 30 02 PM" src="https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/3d59c0b8-ee8f-49c2-a54e-51bd5e1444a3">

## Game board Windows
The game board on windows uses ascii instead of unicode to avoid terminal issues. All other functionality is preserved between operating systems <br>
![windows_game_board](https://github.com/lucolvin/Final_Project_COMP-150_SP24/assets/97006355/20af8ee6-fa50-4256-8667-9e609c788d00)




## How to Run

To compile the game, use the following command:

```bash
g++ -o 2048 2048.cpp
```

> [!CAUTION]
> To compile the centered ascii art test use:
> ```bash
> g++ -std=c++11 ascii-art-test.cpp -o ascii-art-test 
> ```