// OOP Kind of working need a private class for input handler
// Fixed kindda need to add more private fields and methods

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>

//MARK: this is a test
//NOTE - This adds cross platform support for getch() function... kinda
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#define SIZE 4

using namespace std;

// For debugging purposes you can set one of the initial two tiles to a specific number
int debug = 2;

// **Input Handler Class (Private)**
class InputHandler {
public:
    #ifdef _WIN32
    void clearScreen() {
        system("cls");
    }

    int getch() {
        return _getch();
    }
    #else
    void clearScreen() {
        system("clear");
    }
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
    #endif
    char getCommand() {
        int key = getch();
        // Conversion from key codes to wasd
        switch(key) {
            case 'q':
            case 'Q': return 'q'; 
            case 'r':
            case 'R': return 'r';
            case 'w':
            case 'W': return 'w'; 
            case 'a':
            case 'A': return 'a';
            case 's':
            case 'S': return 's';
            case 'd':
            case 'D': return 'd';
            default:  return 'x'; // To indicate an invalid key
        }
    }
};

class Game {
private:
    int board[SIZE][SIZE];
    int score;
    InputHandler inputHandler;
    char commandToChar[128];

public:
    void displayMenu() {
        // BTW, this took me way to long to get working correctly
        // Totally worth it though lol
        cout << "\033[31m .----------------.   \033[33m.----------------.   \033[93m.----------------.   \033[32m.----------------. \n"
                "\033[31m| .--------------. |\033[33m | .--------------. |\033[93m | .--------------. |\033[32m | .--------------. |\n"
                "\033[31m| |    _____     | |\033[33m | |     ____     | |\033[93m | |   _    _     | |\033[32m | |     ____     | |\n"
                "\033[31m| |   / ___ `.   | |\033[33m | |   .'    '.   | |\033[93m | |  | |  | |    | |\033[32m | |   .' __ '.   | |\n"
                "\033[31m| |  |_/___) |   | |\033[33m | |  |  .--.  |  | |\033[93m | |  | |__| |_   | |\033[32m | |   | (__) |   | |\n"
                "\033[31m| |   .'____.'   | |\033[33m | |  | |    | |  | |\033[93m | |  |____   _|  | |\033[32m | |   .`____'.   | |\n"
                "\033[31m| |  / /____     | |\033[33m | |  |  `--'  |  | |\033[93m | |      _| |_   | |\033[32m | |  | (____) |  | |\n"
                "\033[31m| |  |_______|   | |\033[33m | |   '.____.'   | |\033[93m | |     |_____|  | |\033[32m | |  `.______.'  | |\n"
                "\033[31m| |              | |\033[33m | |              | |\033[93m | |              | |\033[32m | |              | |\n"
                "\033[31m| '--------------' |\033[33m | '--------------' |\033[93m | '--------------' |\033[32m | '--------------' |\n"
                "\033[31m '----------------'   \033[33m'----------------'   \033[93m'----------------'   \033[32m'----------------' \n\033[0m";
        cout << "\n\n2048 GAME GO BRRRRR!!!!\n";
        cout << "1. Start Game\n";
        cout << "2. High Scores\n";
        cout << "3. Help/Info\n";
        cout << "4. Quit\n";
        cout << "Enter your choice: ";
    }
    Game() : score(0) { // Constructor to initialize score to 0
        initializeGame();
        addNewNumber();
        // Init commandToChar array
        for (int i = 0; i < 128; i++) {
            commandToChar[i] = -1;
        }
        commandToChar['w'] = 0; // up
        commandToChar['d'] = 1; // right
        commandToChar['s'] = 2; // down
        commandToChar['a'] = 3; // left
        // Uppercase commands because I kept hitting the Caps Lock key and breaking the program
        commandToChar['W'] = 0; // up
        commandToChar['D'] = 1; // right
        commandToChar['S'] = 2; // down
        commandToChar['A'] = 3; // left
    }

    void initializeGame() {
        for(int i=0; i<SIZE; i++) {
            for(int j=0; j<SIZE; j++) {
                board[i][j] = 0;
            }
        }
        // Add a tile at a random position for debugging
        int randIndexI = rand() % SIZE;
        int randIndexJ = rand() % SIZE;
        board[randIndexI][randIndexJ] = debug;
    }

    void drawBoard() {
        cout << "\e[0;31m2048 GAME GO BRRRRR!!!!\n";
        cout << "\e[0;32mTry to get to the 2048 tile!\n";
        cout << "\e[0;34mW: UP, S: DOWN, A: LEFT, D: RIGHT, R: RESTART, Q: QUIT\n\033[0m";
        // score
        cout << "\e[0;35mScore: " << score << "\n\033[0m";

        for(int i=0; i<SIZE; i++) {
            if (i == 0) {
                cout << "┌────┬────┬────┬────┐\n│"; // Top row
            } else {
                cout << "├────┼────┼────┼────┤\n│"; // Middle rows
            }
            for(int j=0; j<SIZE; j++) {
                if(board[i][j] == 0) {
                    cout << setw(4) << "  " << "│";
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
                        default:   colorCode = "\033[0m";  // Reset color to white
                    }
                    int num = board[i][j];
                    int numLength = to_string(num).length();
                    int padding = (4 - numLength) / 2;
                    if (num > 1000) {
                        cout << colorCode << setw(padding + numLength) << right << num << "\033[0m│";
                    } else {
                        cout << colorCode << setw(padding + numLength) << right << num << setw(4 - padding - numLength) << left << " " << "\033[0m│";
                    }
                }
            }
            cout << "\n";
        }
        cout << "└────┴────┴────┴────┘\n"; // Bottom row
    }

bool commands(char command) {
    switch(command) {
        case 'q':
        case 'Q':
            cout << "\e[0;31mAre you sure you want to quit? \e[0;33m(y/n) \033[0m";
            char quit;
            cin >> quit;
            if(quit == 'y') {
                exit(0);
            }
            return false;
        case 'r':
        case 'R':
            cout << "\e[0;31mAre you sure you want to restart? \e[0;33m(y/n) \033[0m";
            char restart;
            cin >> restart;
            if(restart == 'y') {
                score = 0;
                throw runtime_error("restart");
            }
            return false;
        case 'w':
        case 'W':
        case 'a':
        case 'A':
        case 's':
        case 'S':
        case 'd':
        case 'D':
            // These are valid commands, so a move was made.
            return true;
        default:
            // If the command is not one of the valid commands, no move was made.
            return false;
    }
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
            score += array[x]; // update score
        }
    }
}

int getScore() {
    return score;
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

int getMaxScore() {
    int max = 0;
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            if(board[i][j] > max) {
                max = board[i][j];
            }
        }
    }
    return max;
}

void writeHighScore(int score, string name) {
    ofstream file("high_scores.txt", ios_base::app);
    if (file.is_open()) {
        file << name << ": " << score << "\n";
        file.close();
    }
}

void printHighScore() {
    vector<pair<int, string > > highScores;
    ifstream inputFile("high_scores.txt");
    if(inputFile.is_open()) {
        string name;
        int score;
        while(inputFile >> name >> score) {
            highScores.push_back(make_pair(score, name));
        }
        inputFile.close();
    }
    sort(highScores.begin(), highScores.end(), greater<pair<int, string > >());
    cout << "\e[0;31mHigh Scores\n\033[0m";
    for (int i = 0; i < highScores.size() && i < 5; i++) {
        cout << i+1 << ". " << highScores[i].second << " " << highScores[i].first << " Points" << "\n";
    }
}

    void processInput() {
        char command = inputHandler.getCommand();
        bool moveMade = commands(command);
        if (moveMade) {
            int currentDirection = commandToChar[command];
            applyMove(currentDirection);
            addNewNumber();
        }
    }
    void helpMenu() {
        inputHandler.clearScreen();
        cout << "2048 is a single-player sliding block puzzle game designed by Italian web developer Gabriele Cirulli.\n";
        cout << "The game's objective is to slide numbered tiles on a grid to combine them to create a tile with the number 2048.\n";
        cout << "The game is won when a tile with a value of 2048 appears on the board, hence the name of the game.\n";
        cout << "After reaching the 2048 tile, players can continue to play to reach higher scores.\n";
        cout << "When the player has no legal moves (there are no empty spaces and no adjacent tiles with the same value), the game ends.\n";
        cout << "The game's source code can be found at... add github link later when/if I make it public\n";
        cout << "Press any key to return to the main menu.\n";
    }
    void runGame() {
    while(true) {
        int choice;
        int i = 0;
        displayMenu();
        cin >> choice;
        while(i < 1) {
            inputHandler.clearScreen();
            // Maybe fix for initial clear screen
            // Update YAY!!! it worked
            cout.flush();
            i++;
        }

        switch(choice) {
            case 1: // Start Game
                startGame();
                break;
            case 2: // View High Scores
                inputHandler.clearScreen();
                printHighScore();
                break;
            case 3: // Help Menu
                helpMenu();
                break;
            case 4: // Quit
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
    void startGame() {
        commandToChar['w'] = 0; // up
        commandToChar['d'] = 1; // right
        commandToChar['s'] = 2; // down
        commandToChar['a'] = 3; // left
        // Uppercase commands for convenience when caps lock is on
        commandToChar['W'] = 0; // up
        commandToChar['D'] = 1; // right
        commandToChar['S'] = 2; // down
        commandToChar['A'] = 3; // left

        while(true) {
            srand(time(0));
            score = 0;
            initializeGame();
            addNewNumber();
            while(true) {
                inputHandler.clearScreen();
                drawBoard();
                if(canDoMove() == false) {
                    cout << "\n\e[1;37mGAME OVER\n\033[0m";
                    cout << "\e[0;36mEnter your name: \033[0m" << endl;
                    string name;
                    cin >> name;
                    system("clear");
                    writeHighScore(score, name);
                    printHighScore();
                    cout << "\e[0;36mDo you want to play again? \e[0;33m(y/n) \033[0m";
                    char playAgain;
                    cin >> playAgain;
                    cin.ignore();
                    while (playAgain != 'y' && playAgain != 'n')
                    {
                        cout << "\e[0;31mInvalid input. Please enter \e[0;33m(y \033[0mor\e[0;33m n).\033[0m\n\n";
                        cin >> playAgain;
                    }
                    if(playAgain == 'y') {
                        break;
                    } else if (playAgain == 'n') {
                        return;
                    } else {
                        // Fallback. Used to be the invalid input message
                        cout << "\e[0;31mHuh?.\033[0m" << endl;
                        continue;
                    }
                }
                try {
                    char command = inputHandler.getch();
                    bool moveMade = commands(command);
                    if (moveMade) {
                        int currentDirection = commandToChar[static_cast<unsigned char>(command)];
                        applyMove(currentDirection);
                        addNewNumber();
                    }
                } catch (runtime_error& e) {
                    if (string(e.what()) == "restart") {
                        score = 0;
                        break;
                    }
                }
            }
        }
    }
};

int main() {
    Game game;
    game.runGame();
    return 0;
}