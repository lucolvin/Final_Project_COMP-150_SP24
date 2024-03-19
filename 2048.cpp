#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

#define SIZE 4

using namespace std;

// For debugging purposes you can set one of the initial tiles to a specific number
int debug = 2;

int board[SIZE][SIZE];

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    if(ch == 27) { // if the first value is esc
        getchar(); // skip the [
        switch(getchar()) { // the real value
            case 'A':
                ch = 'w';
                break;
            case 'B':
                ch = 's';
                break;
            case 'C':
                ch = 'd';
                break;
            case 'D':
                ch = 'a';
                break;
        }
    }
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

void initializeGame() {
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            board[i][j] = 0;
        }
    }
    // Add a tile at a random position on the board for debugging
    int randIndexI = rand() % SIZE;
    int randIndexJ = rand() % SIZE;
    board[randIndexI][randIndexJ] = debug;
}

void drawBoard() {
    cout << "2048 GAME GO BRRRRR!!!!\n";
    cout << "Try to get to the 2048 tile!\n";
    cout << "W: UP, S: DOWN, A: LEFT, D: RIGHT, Q: QUIT\n";
    for(int i=0; i<SIZE; i++) {
        cout << "+----+----+----+----+\n|";
        for(int j=0; j<SIZE; j++) {
            if(board[i][j] == 0) {
                cout << setw(4) << "  " << "|";
            } else {
                string colorCode;
                switch(board[i][j]) {
                    case 2:    colorCode = "\033[47;30m"; break; // White background, Black text
                    case 4:    colorCode = "\033[44;37m"; break; // Blue background, White text
                    case 8:    colorCode = "\033[42;37m"; break; // Green background, White text
                    case 16:   colorCode = "\033[46;37m"; break; // Cyan background, White text
                    case 32:   colorCode = "\033[41;37m"; break; // Red background, White text
                    case 64:   colorCode = "\033[45;37m"; break; // Purple background, White text
                    case 128:  colorCode = "\033[43;37m"; break; // Brown background, White text
                    case 256:  colorCode = "\033[104;37m"; break; // Bright Blue background, White text
                    case 512:  colorCode = "\033[102;37m"; break; // Bright Green background, White text
                    case 1024: colorCode = "\033[106;37m"; break; // Bright Cyan background, White text
                    case 2048: colorCode = "\033[101;37m"; break; // Bright Red background, White text
                    // Add more cases as needed
                    default:   colorCode = "\033[0m";  // Reset
                }
                int num = board[i][j];
                int numLength = to_string(num).length();
                int padding = (4 - numLength) / 2;
                // If the number is greater than 1000 adjusts padding
                if (num > 1000) {
                    cout << colorCode << setw(padding + numLength) << right << num << "\033[0m|";
                } else {
                    cout << colorCode << setw(padding + numLength) << right << num << setw(4 - padding - numLength) << left << " " << "\033[0m|";
                }
            }
        }
        cout << "\n";
    }
    cout << "+----+----+----+----+\n";
}

void addNewNumber() {
    int available[SIZE*SIZE][2], numAvailable = 0;
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            if(board[i][j] == 0) {
                available[numAvailable][0] = i;
                available[numAvailable][1] = j;
                numAvailable++;
            }
        }
    }
    
    if(numAvailable > 0) {
        int randIndex = rand() % numAvailable;
        int randNumber = 2 * (rand() % 2 + 1);
        board[available[randIndex][0]][available[randIndex][1]] = randNumber;
    }
}

bool canDoMove() {
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            if(board[i][j] == 0) {
                return true;
            }
            if((i+1<SIZE && board[i][j] == board[i+1][j]) || (j+1<SIZE && board[i][j] == board[i][j+1])) {
                return true;
            }
        }
    }
    return false;
}

void slideLine(int array[SIZE]) {
    int x, y;
    for (x = 0, y = 0; y < SIZE; y++) {
        if (array[y] != 0) {
            if (x < y) {
                array[x] = array[y];
                array[y] = 0;
            }
            x++;
        }
    }
}

void mergeLine(int array[SIZE]) {
    for (int x = 0; x < SIZE - 1; x++) {
        if (array[x] == array[x + 1] && array[x] != 0) {
            array[x] *= 2;
            array[x + 1] = 0;
        }
    }
}

void applyMove(int direction) {
    int temp[SIZE];
    for (int i = 0; i < SIZE; i++) {
        // Copy the original line
        for (int j = 0; j < SIZE; j++) {
            switch (direction) {
                case 0: temp[j] = board[j][i]; break; // Up
                case 1: temp[j] = board[i][SIZE - j - 1]; break; // Right
                case 2: temp[j] = board[SIZE - j - 1][i]; break; // Down
                case 3: temp[j] = board[i][j]; break; // Left
            }
        }
        slideLine(temp);
        mergeLine(temp);
        slideLine(temp);
        // Copy the result back
        for (int j = 0; j < SIZE; j++) {
            switch (direction) {
                case 0: board[j][i] = temp[j]; break; // Up
                case 1: board[i][SIZE - j - 1] = temp[j]; break; // Right
                case 2: board[SIZE - j - 1][i] = temp[j]; break; // Down
                case 3: board[i][j] = temp[j]; break; // Left
            }
        }
    }
}

int main() {
    char commandToChar[128];
    commandToChar['w'] = 0; // up
    commandToChar['d'] = 1; // right
    commandToChar['s'] = 2; // down
    commandToChar['a'] = 3; // left
    
    srand(time(0));
    initializeGame();
    addNewNumber();
    while(true) {
        system("clear");
        drawBoard();
        if(canDoMove() == false) {
            cout << "\nGAME OVER\n";
            cout << "Do you want to play again? (y/n)\n";
            char playAgain;
            cin >> playAgain;
            if(playAgain == 'y') {
                main();
            }
            break;
        }
        char command = getch();
        if(command == 'q') {
            cout << "QUIT!\n";
            break;
        }
        int currentDirection = commandToChar[command];
        applyMove(currentDirection);
        addNewNumber();
    }
    return 0;
}