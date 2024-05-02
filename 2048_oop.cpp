/*
COMP-150 Final Project
2048 Game
Author: Luke A. Colvin
Date: 05/03/2024
*/

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>

// Adding this to try to get dynamic terminal size integration
// #include <sys/ioctl.h>

//NOTE - This adds cross platform support for getch() function... kinda
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
#endif

// Define the size of the board
// I wanted to add functionality to change the size of the board but I did not get to it
// Functionality to change the size of the board is here and the game runs fine but the board is not displayed correctly
#define SIZE 4

// Using the standard namespace
using namespace std;

// For debugging purposes you can set one of the initial two tiles to a specific number
// Set to 2 for default behavior
int debug = 2;


// InputHandler class to handle input
class InputHandler {
// Public functions
public:
    // Function to clear the screen with a preprocessor for Windows and Unix system detection
    #ifdef _WIN32
    // Clear screen function for Windows
    void clearScreen() {
        // Clears the screen for Windows systems with the correct command
        system("cls");
    }

    int getch() {
        // _getch() function for Windows
        return _getch();
    }
    #else // Else is unix system
    // Clear screen function for Unix
    void clearScreen() {
        // Clears the screen for Unix systems with the correct command
        system("clear");
    }

    // Function to get a character from the user for the user input while in the game
    // I used this so that the user does not have to press enter after each move
    int getch() {
        // Structs to store the terminal settings
        struct termios oldt, newt;
        // int to store the character
        int ch;
        // Get the terminal settings
        tcgetattr( STDIN_FILENO, &oldt );
        // set the new terminal settings
        newt = oldt;
        // Set the new terminal settings to not require enter key press
        newt.c_lflag &= ~( ICANON | ECHO );
        // Apply the new terminal settings
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
        // Get the character from the user
        ch = getchar();
        if(ch == 27) { // if the first value is esc
            getchar(); // skip the [
            // Uses a switch statement to get the input from the arrow keys
            switch(getchar()) { // the real value of the arrow key
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
        // reset the terminal settings
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
        return ch;
    }
    // Ends the preprocessor for Unix systems
    #endif

    // Function to get the command from the user
    char getCommand() {
        // sets the key to the getch() function
        int key = getch();
        // Conversion from key codes to wasd and q for quit, r for restart, and sets an invalid key to x
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

// Tile class to handle the tiles on the board
class tile {
// Private integer value for the tile
private:
    int value;
public:
    // Constructor to initialize the value to 0
    tile() : value(0) {}

    // getter for the value
    int getValue() {
        return value;
    }

    // setter for the value
    void setValue(int newValue) {
        // sets the value to the new value after a move
        value = newValue;
    }
};

// Game class to handle the game logic
class Game {
// private variables for the game class
private:
    // creates a 2D array of tiles for the board
    tile board[SIZE][SIZE];
    // initializes the score to 0
    int score;
    // creates an instance of the InputHandler class
    InputHandler inputHandler;
    // creates an array to convert the commands to characters
    char commandToChar[128];

// Public functions for the game class
public:
    void displayMenu() {
        // Added to try to get dynamic terminal width
        // struct winsize size;
        // ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        // Init an int for terminal width to be able to center the menu
        int terminalWidth;
        // preprocessor for Windows detection to get the terminal width on Windows to work
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            terminalWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        #else
            // init a struct for the terminal width on Unix systems
            struct winsize size;
            // get the terminal width using ioctl on Unix systems
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
            // set the terminal width variable to be the width of the terminal
            terminalWidth = size.ws_col;
        #endif
        // Init an int for terminal width to center the menu
        // Changed to try to get dynamic terminal width
        // Woop! Woop! it worked
        // int terminalWidth = size.ws_col;
        // Clears the screen before the first menu is displayed
        // inputHandler.clearScreen();
        // BTW, this took me way to long to get working correctly
        // Totally worth it though lol
        cout << "\033[31m .----------------.   \033[33m.----------------.   \033[94m.----------------.   \033[32m.----------------. \n"
                "\033[31m| .--------------. |\033[33m | .--------------. |\033[94m | .--------------. |\033[32m | .--------------. |\n"
                "\033[31m| |    _____     | |\033[33m | |     ____     | |\033[94m | |   _    _     | |\033[32m | |     ____     | |\n"
                "\033[31m| |   / ___ `.   | |\033[33m | |   .'    '.   | |\033[94m | |  | |  | |    | |\033[32m | |   .' __ '.   | |\n"
                "\033[31m| |  |_/___) |   | |\033[33m | |  |  .--.  |  | |\033[94m | |  | |__| |_   | |\033[32m | |   | (__) |   | |\n"
                "\033[31m| |   .'____.'   | |\033[33m | |  | |    | |  | |\033[94m | |  |____   _|  | |\033[32m | |   .`____'.   | |\n"
                "\033[31m| |  / /____     | |\033[33m | |  |  `--'  |  | |\033[94m | |      _| |_   | |\033[32m | |  | (____) |  | |\n"
                "\033[31m| |  |_______|   | |\033[33m | |   '.____.'   | |\033[94m | |     |_____|  | |\033[32m | |  `.______.'  | |\n"
                "\033[31m| |              | |\033[33m | |              | |\033[94m | |              | |\033[32m | |              | |\n"
                "\033[31m| '--------------' |\033[33m | '--------------' |\033[94m | '--------------' |\033[32m | '--------------' |\n"
                "\033[31m '----------------'   \033[33m'----------------'   \033[94m'----------------'   \033[32m'----------------' \n\033[0m";
        // cout << "\n\n2048 GAME GO BRRRRR!!!!\n";
        // cout << "1. Start Game\n";
        // cout << "2. High Scores\n";
        // cout << "3. Help/Info\n";
        // cout << "4. Quit\n";
        // cout << "Enter your choice: \n";

        //NOTE - Changed this to center the menu using setw() to center the text in the terminal
        // makes the menu centered in the terminal by using the terminal width and changing the output of each line to its own string
        string line1 = "2048 GAME GO BRRRRR!!!!";
        string line2 = "1. Start Game";
        string line3 = "2. High Scores";
        string line4 = "3. Help/Info";
        string line5 = "4. Quit";
        string line6 = "Enter your choice: ";
        // Outputs the menu centered in the terminal
        // It centers the menu by using the terminal width plus the length of the line and divided by 2
        cout << setw((terminalWidth + line1.length()) / 2) << line1 << endl;
        cout << setw((terminalWidth + line2.length()) / 2) << line2 << endl;
        cout << setw((terminalWidth + line2.length()) / 2) << line3 << endl;
        cout << setw((terminalWidth + line4.length()) / 2) << line4 << endl;
        cout << setw((terminalWidth + line5.length()) / 2) << line5 << endl;
        cout << setw((terminalWidth + line6.length()) / 2) << line6;
    }
    Game() : score(0) { // Constructor to initialize score to 0
        initializeGame();
        addNewNumber();
        // Init commandToChar array
        for (int i = 0; i < 128; i++) {
            commandToChar[i] = -1;
        }
        // I am aware that it doesn't like that im using a char as an index but it works and the validations prevent any errors
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

    // Initializes the game board to all 0's
    void initializeGame() {
        for(int i=0; i<SIZE; i++) {
            for(int j=0; j<SIZE; j++) {
                board[i][j].setValue(0);
            }
        }
        // Adds one random number to the board and also the debug number if it is set
        int randIndexI = rand() % SIZE;
        int randIndexJ = rand() % SIZE;
        // This is the debug number that can be set at the top of the file
        board[randIndexI][randIndexJ].setValue(debug);
    }

    // Function to draw the board is set to void because it does not return anything
    void drawBoard() {
        // Added to implement cross platform support
        char* os_env = getenv("OS");
        // gets the os string from the environment variable
        string os_str = os_env ? string(os_env) : "";
        // If the os string is not empty and it contains Windows, then it is a Windows system
        bool isWindows = !os_str.empty() && os_str.find("Windows") != string::npos;
        
        // Added to implement cross platform support
        // And it worked... Shocker...
        // The strings are used to draw the board and can switch from unicode to ascii characters
        string cellDivider = isWindows ? "|" : "│";
        string topRow = isWindows ? "+----+----+----+----+\n|" : "┌────┬────┬────┬────┐\n│";
        string middleRow = isWindows ? "+----+----+----+----+\n|" : "├────┼────┼────┼────┤\n│";
        string bottomRow = isWindows ? "+----+----+----+----+\n" : "└────┴────┴────┴────┘\n";

        // This shows the options available to the user while in the game
        cout << "\e[0;31m2048 GAME GO BRRRRR!!!!\n";
        cout << "\e[0;32mTry to get to the 2048 tile!\n";
        //TODO - Add different colors to the different options
        cout << "\e[0;34mW: UP, S: DOWN, A: LEFT, D: RIGHT, R: RESTART, Q: QUIT, M: Menu\n\033[0m";
        // Outputs the score to the user while in the game
        cout << "\e[0;35mScore: " << score << "\n\033[0m";

        // for loop for the initial row of the board
        for(int i = 0; i < SIZE; i++) {
            if (i == 0) {
                // Outputs the top row of the board only for the first row
                cout << topRow;
            } else {
                // Outputs the middle row of the board for all other rows except the last row
                cout << middleRow;
            }
            // for loop to draw the board using the string variables for the board
            for(int j=0; j<SIZE; j++) {
                // If the value of the tile is 0, then it outputs a space that is output as a blank tile
                if(board[i][j].getValue() == 0) {
                    cout << setw(4) << "  " << cellDivider;
                } else {
                    // initializes the color code for the tiles
                    string colorCode;
                    // sets the color code based on the value of the tile
                    int tileValue = board[i][j].getValue();
                    // switch statement to set the color code based on the value of the tile
                    switch(tileValue) {
                        // 30m is black text, 37m is white text
                        case 2:    colorCode = "\033[47;30m"; break; // White background, Black text
                        case 4:    colorCode = "\033[44;37m"; break; // Blue background, White text
                        case 8:    colorCode = "\033[42;30m"; break; // Green background, Black text
                        case 16:   colorCode = "\033[46;30m"; break; // Cyan background, Black text
                        case 32:   colorCode = "\033[41;37m"; break; // Red background, White text
                        case 64:   colorCode = "\033[45;37m"; break; // Purple background, White text
                        case 128:  colorCode = "\033[43;30m"; break; // Brown background, Black text
                        case 256:  colorCode = "\033[104;30m"; break; // Bright Blue background, Black text
                        case 512:  colorCode = "\033[102;30m"; break; // Bright Green background, Black text
                        case 1024: colorCode = "\033[106;37m"; break; // Bright Cyan background, White text
                        case 2048: colorCode = "\033[101;30m"; break; // Bright Red background, Black text
                        default:   colorCode = "\033[0m";  // Reset color to white
                    }
                    // sets the number of the tile to the value of the tile
                    int num = tileValue;
                    // sets the length of the number to the length of the number converted to a string to get the padding for the tile correctly
                    int numLength = to_string(num).length();
                    // sets the padding for the tile to center the number in the tile
                    int padding = (4 - numLength) / 2;
                    // outputs the tile with the color code, padding, number, and cell divider
                    if (num > 1000) {
                        // Outputs the color code, padding, number, and cell divider
                        cout << colorCode << setw(padding + numLength) << right << num << "\033[0m" << cellDivider;
                    } else {
                        // Outputs the color code, padding, number, and cell divider with fixed padding for numbers larger than 1000
                        cout << colorCode << setw(padding + numLength) << right << num << setw(4 - padding - numLength) << left << " " << "\033[0m" << cellDivider;
                    }
                }
            }
            // new line
            cout << "\n";
        }
        // outputs the bottom row of the board only for the last row
        cout << bottomRow << "\n";
    }

// This function handles the commands that the user can input while in the game and returns a boolean value
bool commands(char command) {
    // swoitch statement to handle the commands that the user can input
    switch(command) {
        // case for the quit command
        case 'q':
        case 'Q':
            // Gives the user a choice to quit the game or continue playing
            cout << "\e[0;31mAre you sure you want to quit? \e[0;33m(y/n) \033[0m";
            char quit;
            cin >> quit;
            // if the user enters y, then the program exits and if not it continues
            //TODO - Add validation for the quit command to only accept y or n
            if(quit == 'y') {
                exit(0);
            }
            return false;
        // case for the restart command
        case 'r':
        case 'R':
            // Gives the user a choice to restart the game or continue playing
            cout << "\e[0;31mAre you sure you want to restart? \e[0;33m(y/n) \033[0m";
            char restart;
            cin >> restart;
            // if the user enters y, then the game restarts and if not it continues
            //TODO - Add validation for the restart command to only accept y or n
            if(restart == 'y') {
                score = 0;
                throw runtime_error("restart");
            }
            return false;
        // cases for the valid move commands
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
        // case for the main menu command
        case 'm':
        case 'M':
            // Gives the user a choice to return to the main menu or continue playing
            cout << "\e[0;31mAre you sure you want to return to the main menu? \e[0;33m(y/n) \033[0m";
            char menu;
            cin >> menu;
            // if the user enters y, then the program returns to the main menu and if not it continues
            //TODO - Add validation for the main menu command to only accept y or n
            if(menu == 'y') {
                runGame();
            }
            // return false;
        default:
            // If the command is not one of the valid commands, no move was made.
            return false;
    }
}

// Function to add a new number to the board after a move returns void because it does not return anything
void addNewNumber() {
    // initializes the array of available spots to move to and the number of available spots to 0
    int available[SIZE*SIZE][2], numAvailable = 0;
    // for loop to get the available spots to move to
    for(int i=0; i<SIZE; i++) {
        // for loop to get the available spots to move to
        for(int j=0; j<SIZE; j++) {
            // this checks if the value of the tile is 0 and if it is then it adds the spot to the available spots array
            if(board[i][j].getValue() == 0) {
                available[numAvailable][0] = i;
                available[numAvailable][1] = j;
                // itterates the number of available spots
                numAvailable++;
            }
        }
    }
    
    // if an available spot is found, then it adds a new number to the board
    if(numAvailable > 0) {
        // randomizes the index of the available spots to move to
        int randIndex = rand() % numAvailable;
        // randomizes the number to add to the board between 2 and 4
        int randNumber = 2 * (rand() % 2 + 1);
        // sets the value of the tile to the random number
        board[available[randIndex][0]][available[randIndex][1]].setValue(randNumber);
    }
}

// this function contains the logic to check if a move can be made and returns a boolean value
bool canDoMove() {
    // for loop to check if a move can be made
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            // if the value of the tile is 0, then a move can be made and it returns true
            if(board[i][j].getValue() == 0) {
                return true;
            }
            // if the value of the tile is the same as the value of the tile to the right, left, above, or below it, then a move can be made and it returns true
            if((i+1<SIZE && board[i][j].getValue() == board[i+1][j].getValue()) || (j+1<SIZE && board[i][j].getValue() == board[i][j+1].getValue())) {
                return true;
            }
        }
    }
    // if no move can be made then it returns false
    return false;
}

// function to slide the line of tiles and returns void because it does not return anything
void slideLine(tile array[SIZE]) {
    // inits integers x and y
    int x, y;
    // for loop to slide the line of tiles
    for (x = 0, y = 0; y < SIZE; y++) {
        // if the value of the tile is not 0, then it slides the tile to the left
        if (array[y].getValue() != 0) {
            // if x is less than y, then it sets the value of the tile at x to the value of the tile at y and sets the value of the tile at y to 0
            if (x < y) {
                array[x].setValue(array[y].getValue());
                array[y].setValue(0);
            }
            // iterates x
            x++;
        }
    }
}

// this function contains the logic to merge the line of tiles and returns void because it does not return anything
void mergeLine(tile array[SIZE]) {
    // for loop to merge the line of tiles
    for (int x = 0; x < SIZE - 1; x++) {
        // if the value of the tile at x is the same as the value of the tile at x+1 and the value of the tile at x is not 0, then it merges the tiles
        if (array[x].getValue() == array[x + 1].getValue() && array[x].getValue() != 0) {
            // sets the value of the tile at x to the value of the tile at x times 2 and sets the value of the tile at x+1 to 0
            array[x].setValue(array[x].getValue() * 2);
            array[x + 1].setValue(0);
            // updates the score
            score += array[x].getValue();
        }
    }
}

// getter for the score
int getScore() {
    return score;
}

// function to apply the move to the board and returns void because it does not return anything
// Type type type ... copy paste ... I'm so tired ... and I'm going crazy ... I should go to bed
void applyMove(int direction) {
    // creates a temporary array of tiles to store the board
    tile temp[SIZE];
    // for loop to apply the move to the board
    for (int i = 0; i < SIZE; i++) {
        // Copy the original line
        // for loop to copy the original line to the temporary array
        for (int j = 0; j < SIZE; j++) {
            // switch statement to copy the original line to the temporary array based on the direction
            switch (direction) {
                case 0: temp[j] = board[j][i]; break; // Up
                case 1: temp[j] = board[i][SIZE - j - 1]; break; // Right
                case 2: temp[j] = board[SIZE - j - 1][i]; break; // Down
                case 3: temp[j] = board[i][j]; break; // Left
            }
        }
        // Slide the line
        slideLine(temp);
        mergeLine(temp);
        slideLine(temp);
        // Copy the result back
        for (int j = 0; j < SIZE; j++) {
            // switch statement to copy the result back to the board based on the direction
            switch (direction) {
                case 0: board[j][i] = temp[j]; break; // Up
                case 1: board[i][SIZE - j - 1] = temp[j]; break; // Right
                case 2: board[SIZE - j - 1][i] = temp[j]; break; // Down
                case 3: board[i][j] = temp[j]; break; // Left
            }
        }
    }
}

// function to get the max score and returns an integer value for the max score
int getMaxScore() {
    // inits the max score to 0
    int max = 0;
    // for loop to get the max score
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            // gets the max score by comparing the value of the tiles to the max score
            if(board[i][j].getValue() > max) {
                max = board[i][j].getValue();
            }
        }
    }
    // returns the max score
    return max;
}

// function to check if the user has achieved a 2048 tile and returns a boolean value
bool hasWon() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j].getValue() == 2048) {
                return true;
            }
        }
    }
    return false;
}

// function to write the high score to a file and returns void because it does not return anything
void writeHighScore(int score, string name) {
    // uses the ofstream class to write the high score to a file named high_scores.txt
    ofstream file("high_scores.txt", ios_base::app);
    // if the file is open, then it writes the high score to the file and formats it with the name and score separated by a :
    if (file.is_open()) {
        file << name << ": " << score << "\n";
        // closes the file
        file.close();
    }
}

// Function to print the high score to the user and returns void because it does not return anything
void printHighScore() {
    // uses a vector of pairs to store the high scores along with the names
    vector<pair<int, string > > highScores;
    // Inports the high scores from the file high_scores.txt
    ifstream inputFile("high_scores.txt");
    // Checks if the file is open and if it is then it reads the high scores from the file and stores them in the vector
    if(inputFile.is_open()) {
        // initializes the string name and the int score
        string name;
        int score;
        // while loop to read the high scores from the file and store them in the vector
        while(inputFile >> name >> score) {
            highScores.push_back(make_pair(score, name));
        }
        // closes the file
        inputFile.close();
    }
    // functionality to sort the high scores in descending order
    sort(highScores.begin(), highScores.end(), greater<pair<int, string > >());
    // outputs title at the top of the list of high scores
    cout << "\e[0;31mHigh Scores\n\033[0m";
    // for loop to print the high scores to the user
    for (int i = 0; i < highScores.size() && i < 5; i++) {
        // outputs the high scores to the user with the name and score
        cout << i+1 << ". " << highScores[i].second << " " << highScores[i].first << " Points" << "\n";
    }
}

    // function to process the input from the user and returns void because it does not return anything
    void processInput() {
        // calls the getCommand function from the inputHandler class to get the command from the user
        char command = inputHandler.getCommand();
        // calls the commands function with the users command as an argument returns a boolean value
        bool moveMade = commands(command);
        // checks if a move was made and if it was then it applies the move and adds a new number to the board
        if (moveMade) {
            int currentDirection = commandToChar[command];
            applyMove(currentDirection);
            addNewNumber();
        }
    }

    // Simple help menu to explain the game to the user
    //TODO - Add a better help menu
    void helpMenu() {
        inputHandler.clearScreen();
        cout << "2048 is a single-player sliding block puzzle game originally designed by Italian web developer Gabriele Cirulli.\n";
        cout << "The game's objective is to slide numbered tiles on a grid to combine them to create a tile with the number 2048.\n";
        cout << "The game is won when a tile with a value of 2048 appears on the board, hence the name of the game.\n";
        cout << "After reaching the 2048 tile, players can continue to play to reach higher scores.\n";
        cout << "When the player has no legal moves (there are no empty spaces and no adjacent tiles with the same value), the game ends.\n";
        cout << "The game's source code can be found at... add github link later when/if I make it public\n";
        cout << "Press any key to return to the main menu.\n";
    }

    // function to run the game and returns void because it does not return anything
    void runGame() {
    // while loop to run the game
    while(true) {
        // inits an int for the user's choice
        int choice;
        // inits int i to 0
        int i = 0;
        // calls the displayMenu function to display the menu to the user
        displayMenu();
        // gets the user's choice
        cin >> choice;
        // clears the screen on the first iteration of the loop
        while(i < 1) {
            inputHandler.clearScreen();
            // Maybe fix for initial clear screen
            // Update YAY!!! it worked
            cout.flush();
            i++;
        }

        // This fixes the infinite loop bug when an invalid character is entered in the menu
        // checks for invalid input and clears the cin buffer so that the program does not get stuck in an infinite loop
        if(cin.fail()) {
            cin.clear();
            cin.ignore();
            continue;
        }

        // switch statement to handle the user's choice in the main menu
        switch(choice) {
            // case for the start game option
            case 1: // Start Game
                startGame();
                break;
            // case for the view high scores option
            case 2: // View High Scores
                inputHandler.clearScreen();
                printHighScore();
                break;
            // case for the help menu option
            case 3: // Help Menu
                helpMenu();
                break;
            // case for the quit option
            case 4: // Quit
                exit(0); // Fixes the bug where the program would not exit and would return to the game instead
            // default case which handles invalid input
            //TODO - Move this to the bottom instead of the top when it is output
            default:
                cout << "\n\033[31m ! Invalid choice. Please try again. !\033[0m\n\n";
        }
    }
}

    // function to start the game and returns void because it does not return anything
    void startGame() {
        // converts the commands to characters
        commandToChar['w'] = 0; // up
        commandToChar['d'] = 1; // right
        commandToChar['s'] = 2; // down
        commandToChar['a'] = 3; // left
        // Uppercase commands for convenience when caps lock is on
        commandToChar['W'] = 0; // up
        commandToChar['D'] = 1; // right
        commandToChar['S'] = 2; // down
        commandToChar['A'] = 3; // left

        // inits bools for the end game and the first run
        // for the win game functionality
        bool endGame = false;
        bool firstRun = true;

        // while loop for the main game loop
        while(true) {
            // initializes the random seed to get a pseudo-random number
            srand(time(0));
            // sets the score to 0
            score = 0;
            // calls the initializeGame function to initialize the game board on the initial run
            initializeGame();
            // calls the addNewNumber function to add a new number to the board in the initial run
            addNewNumber();
            // while loop for the game loop
            while(true) {
                // calls the clearScreen from the inputHandler to clear the screen
                inputHandler.clearScreen();
                // calls the drawBoard function to draw the board to the user
                drawBoard();
                // for (int i = 0; i < 4; i++) {
                // You cant do that Luke... facepalm
                // Functionality to check if the user has won the game ie. reached 2048
                if (hasWon() && firstRun == true) {
                    // sets the first run to false so that the message only appears once
                    firstRun = false;
                    // inits a char to store the user's choice
                    char choice;
                    // I freaking love this bit of code!!!
                    // makes the message a string
                    string message = "You've reached 2048! Would you like to continue playing? (y/n): ";
                    // inits an array of color codes to change the color of the message
                    int colorCodes[] = {31, 33, 32, 36, 34, 35}; // Red, Yellow, Green, Cyan, Blue, Magenta
                    // outputs the message to the user as a rainbow
                    for (int i = 0; i < message.size(); ++i) {
                        // outputs the message by changing the color of each character in the message
                        // uses the modulus operator to loop through the color codes
                        cout << "\033[" << colorCodes[i % 6] << "m" << message[i];
                    }
                    // resets the color to default
                    cout << "\033[0m" << endl;
                    // gets the user's input
                    cin >> choice;
                    // if the user enters n, then the endGame bool is set to false which triggers the game over message
                    if (choice == 'n' || choice == 'N') {
                        endGame = true;
                    // if the user enters y, then the game continues
                    } else if (choice == 'y' || choice == 'Y') {
                            // firstRun = true;
                            continue;
                    }
                }
                // checks if a move can be made by checking the canDoMove function
                if(endGame || canDoMove() == false) {
                    // outputs the game over message to the user if no move can be made
                    cout << "\n\e[1;37mGAME OVER\n\033[0m";
                    // gets the user's name to write to the high scores file
                    cout << "\e[0;36mEnter your name: \033[0m" << endl;
                    // initializes the string name to store the user's name
                    string name;
                    // gets the user's name
                    cin >> name;
                    // clears the screen
                    inputHandler.clearScreen();
                    // writes the high score to the file using the writeHighScore function with the user's name and score as arguments
                    writeHighScore(score, name);
                    // call the printHighScore function to print the high scores to the user
                    printHighScore();
                    // asks the user if they want to play again
                    cout << "\e[0;36mDo you want to play again? \e[0;33m(y/n) \033[0m";
                    // inits a char to store the user's choice to play again
                    char playAgain;
                    // gets the user's choice to play again
                    cin >> playAgain;
                    // clears the cin buffer
                    cin.ignore();
                    // checks if the user's choice is valid
                    while (playAgain != 'y' && playAgain != 'n')
                    {
                        // outputs an invalid input message to the user if the input is not a y or n
                        cout << "\e[0;31mInvalid input. Please enter \e[0;33m(y \033[0mor\e[0;33m n).\033[0m\n\n";
                        // goes back to the play again prompt
                        cin >> playAgain;
                    }
                    // if the user enters y and if it is then it clears the screen and breaks the loop to play again
                    if(playAgain == 'y') {
                        // clears the screen to ensure the screen only shows the game board when starting a new game
                        inputHandler.clearScreen();
                        break;
                    // If the user enters n, then it clears the screen and returns to the main menu
                    } else if (playAgain == 'n') {
                        inputHandler.clearScreen();
                        return;
                    } else {
                        // Fallback. Used to be the invalid input message
                        // currently does nothing but ignore the invalid input the output is canceled by the clearScreen
                        cout << "\e[0;31mHuh?.\033[0m" << endl;
                        continue;
                    }
                }
                // calls the processInput function to process the user's input
                // uses a try catch block to catch the restart exception
                try {
                    char command = inputHandler.getch();
                    bool moveMade = commands(command);
                    if (moveMade) {
                        // uses unsigned char to convert the command to a character because I was getting a warning and this was the fix
                        int currentDirection = commandToChar[static_cast<unsigned char>(command)];
                        // applies the move to the board
                        applyMove(currentDirection);
                        // adds a new number to the board
                        addNewNumber();
                    }
                // if the user enters r, then it throws a runtime error to restart the game
                // this fixed a bug that I had that if I canceled the restart it would still add a tile which made it weird
                } catch (runtime_error& e) {
                    if (string(e.what()) == "restart") {
                        // resets the score to 0 if the user chooses to restart the game
                        score = 0;
                        break;
                    }
                }
            }
        }
    }
};

// Main function
int main() {
    // initializes the game class
    Game game;
    // calls the runGame function to run the game
    game.runGame();
    // returns 0 to end the program
    return 0;
}