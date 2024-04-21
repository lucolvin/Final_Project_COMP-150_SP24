#include <iostream>
#include <iomanip> // For setw
#include <vector>

#ifdef _WIN32
#include <windows.h> 
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

void displayMenu() {
    // Added to try to get dynamic terminal width
    // struct winsize size;
    // ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    int terminalWidth;
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        terminalWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #else
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        terminalWidth = size.ws_col;
    #endif

    // Split ASCII art into separate lines
    vector<string> asciiArt = {
        "\033[31m                       .----------------.   \033[33m.----------------.   \033[94m.----------------.   \033[32m.----------------. \n",
        "\033[31m                      | .--------------. |\033[33m | .--------------. |\033[94m | .--------------. |\033[32m | .--------------. |\n",
        "\033[31m                      | |    _____     | |\033[33m | |     ____     | |\033[94m | |   _    _     | |\033[32m | |     ____     | |\n",
        "\033[31m                      | |   / ___ `.   | |\033[33m | |   .'    '.   | |\033[94m | |  | |  | |    | |\033[32m | |   .' __ '.   | |\n",
        "\033[31m                      | |  |_/___) |   | |\033[33m | |  |  .--.  |  | |\033[94m | |  | |__| |_   | |\033[32m | |   | (__) |   | |\n",
        "\033[31m                      | |   .'____.'   | |\033[33m | |  | |    | |  | |\033[94m | |  |____   _|  | |\033[32m | |   .`____'.   | |\n",
        "\033[31m                      | |  / /____     | |\033[33m | |  |  `--'  |  | |\033[94m | |      _| |_   | |\033[32m | |  | (____) |  | |\n",
        "\033[31m                      | |  |_______|   | |\033[33m | |   '.____.'   | |\033[94m | |     |_____|  | |\033[32m | |  `.______.'  | |\n",
        "\033[31m                      | |              | |\033[33m | |              | |\033[94m | |              | |\033[32m | |              | |\n",
        "\033[31m                      | '--------------' |\033[33m | '--------------' |\033[94m | '--------------' |\033[32m | '--------------' |\n",
        "\033[31m                          '----------------'   \033[33m'----------------'   \033[94m'----------------'   \033[32m'----------------' \n\033[0m"
    };

    // Get the maximum line length in the ASCII art
    int maxLineLength = 0;
    for (const string& line : asciiArt) {
        if (line.length() > maxLineLength) {
            maxLineLength = line.length();
        }
    }

    // Only display the ASCII art if it can fit into the terminal
    if (maxLineLength <= terminalWidth) {
        for (const string& line : asciiArt) {
            int padding = (terminalWidth - line.length()) / 2;
            cout << string(padding, ' ') << line; 
        }
    }

    //NOTE - Changed this to center the menu using setw() to center the text in the terminal
    string line1 = "2048 GAME GO BRRRRR!!!!";
    string line2 = "1. Start Game";
    string line3 = "2. High Scores";
    string line4 = "3. Help/Info";
    string line5 = "4. Quit";
    string line6 = "Enter your choice: ";
    cout << setw((terminalWidth + line1.length()) / 2) << line1 << endl;
    cout << setw((terminalWidth + line2.length()) / 2) << line2 << endl;
    cout << setw((terminalWidth + line2.length()) / 2) << line3 << endl;
    cout << setw((terminalWidth + line4.length()) / 2) << line4 << endl;
    cout << setw((terminalWidth + line5.length()) / 2) << line5 << endl;
    cout << setw((terminalWidth + line6.length()) / 2) << line6;
    cout << "Enter a number to end the program: ";
    cin.get();
}
int main() {
    displayMenu();
    return 0;
}