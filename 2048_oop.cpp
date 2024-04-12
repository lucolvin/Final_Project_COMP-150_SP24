class Game {
private:
    int debug = 2;
    int score = 0;
    bool moveMade = false;
    int board[SIZE][SIZE];
    char commandToChar[128];

    void initializeGame() {
        for(int i=0; i<SIZE; i++) {
            for(int j=0; j<SIZE; j++) {
                board[i][j] = 0;
            }
        }
        score = 0;
        int randIndexI = rand() % SIZE;
        int randIndexJ = rand() % SIZE;
        board[randIndexI][randIndexJ] = debug;
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

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    int getch() {
        #ifdef _WIN32
            return _getch();
        #else
            struct termios oldt, newt;
            int ch;
            tcgetattr( STDIN_FILENO, &oldt );
            newt = oldt;
            newt.c_lflag &= ~( ICANON | ECHO );
            tcsetattr( STDIN_FILENO, TCSANOW, &newt );
            ch = getchar();
            if(ch == 27) {
                getchar();
                switch(getchar()) {
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
        #endif
    }

    bool commands(char command) {
        switch(command) {
            case 'q':
            case 'Q':
                cout << "Are you sure you want to quit? (y/n) ";
                char quit;
                cin >> quit;
                if(quit == 'y') {
                    exit(0);
                }
                return false;
            case 'r':
            case 'R':
                cout << "Are you sure you want to restart? (y/n) ";
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
                return true;
            default:
                return false;
        }
    }

    void applyMove(int direction) {
        int temp[SIZE];
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                switch (direction) {
                    case 0: temp[j] = board[j][i]; break;
                    case 1: temp[j] = board[i][SIZE - j - 1]; break;
                    case 2: temp[j] = board[SIZE - j - 1][i]; break;
                    case 3: temp[j] = board[i][j]; break;
                }
            }
            slideLine(temp);
            mergeLine(temp);
            slideLine(temp);
            for (int j = 0; j < SIZE; j++) {
                switch (direction) {
                    case 0: board[j][i] = temp[j]; break;
                    case 1: board[i][SIZE - j - 1] = temp[j]; break;
                    case 2: board[SIZE - j - 1][i] = temp[j]; break;
                    case 3: board[i][j] = temp[j]; break;
                }
            }
        }
    }

    // Other methods from the provided code go here...

public:
    Game() {
        commandToChar['w'] = 0;
        commandToChar['d'] = 1;
        commandToChar['s'] = 2;
        commandToChar['a'] = 3;
        commandToChar['W'] = 0;
        commandToChar['D'] = 1;
        commandToChar['S'] = 2;
        commandToChar['A'] = 3;
    }

    void run() {
        while(true) {
            srand(time(0));
            score = 0;
            initializeGame();
            addNewNumber();
            while(true) {
                clearScreen();
                drawBoard();
                if(canDoMove() == false) {
                    cout << "\nGAME OVER\n";
                    cout << "Enter your name: " << endl;
                    string name;
                    cin >> name;
                    system("clear");
                    writeHighScore(score, name);
                    printHighScore();
                    cout << "Do you want to play again? (y/n) ";
                    char playAgain;
                    cin >> playAgain;
                    cin.ignore();
                    while (playAgain != 'y' && playAgain != 'n')
                    {
                        cout << "Invalid input. Please enter (y or n).\n\n";
                        cin >> playAgain;
                    }
                    if(playAgain == 'y') {
                        break;
                    } else if (playAgain == 'n') {
                        return;
                    } else {
                        cout << "Huh?.\n" << endl;
                        continue;
                    }
                }
                try {
                    char command = getch();
                    bool moveMade = commands(command);
                    if (moveMade) {
                        int currentDirection = commandToChar[command];
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
    game.run();
    return 0;
}