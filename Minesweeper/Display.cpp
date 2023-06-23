
#include <iostream>
using namespace std;

enum states {
    covered, uncovered, flagged
};

enum Difficulty {
    beginner, medium, pro
};

struct Cell {
    states state;
    bool mine = false;
    int adjacentMines = 0;
    int rowNum;
    int colNum;
    Cell() {
        state = covered;
    }
};

struct Location {
    int row;
    int column;
    Location() {
        row = 0;
        column = 0;
    }
    Location(int r, int c) {
        row = r;
        column = c;
    }
};

class Display {
private:
    void winningMessage() {
        cout << "\n\nGAME WON !" << endl;
    }

    void losingMessage() {
        cout << "\n\nGAME LOST !" << endl;
    }


public:
    void clrScr() { cout << "\033[2J\033[1;1H"; system("cls"); }

    void gameOverScreen(bool won) {
        clrScr();
        if (won) winningMessage();
        else losingMessage();
        cout << "\n\n\nGAME OVER" << endl;
    }

    void welcomeScreen() {
        char a;
        cout << "\n\n\t\t\t WELCOME TO \"MINESWEEPER\" GAME \n\n\n\n";
        cout << "Rules: \n\n";
        cout << "1. The board is divided into cells, with mines randomly distributed. \n";
        cout << "2. To win, you need to open all the cells, without opening mines.  \n";
        cout << "3. The number on a cell shows the number of mines adjacent to it. Using this information, you can determine cells that are safe, and cells that contain mines. \n";
        cout << "4. Interact, evolve and enjoy!! \n";
        cout << "\n\n\n";
        cout << "How To Play: \n\n";
        cout << "1. Game always start with all cells covered. \n";
        cout << "2. In each move type the row number and column number for that particular cell to uncover. \n";
        cout << "3. If you want to flag, enter F and type the row number and column number to flag or unflag \n";

        cout << "\n\n\n";
        cout << "Enter any character to continue...\n";
        cin >> a;
    }

    Difficulty chooseDifficulty() {
        
        cout << "\n\nchoose your difficulty level: \n";
        cout << "\'B\' - Beginner, \'I\' - Intermediate, \'P\' - pro, \n";
        char difficulty;
        cin >> difficulty;
        clrScr();
        switch (difficulty)
        {
        case 'B':
            return beginner;
            break;
        case 'I':
            return medium;
            break;
        case 'P':
            return pro;
            break;
        default:
            cout << "choose an available option\n";
            chooseDifficulty();
        }
    }

    Location getBoardSize() {
        int sizes[3] = {10,15,20};
        cout << "\n\nSelect the board size of your choice: \n";
        cout << "\'1\' - "<<sizes[0]<<" x "<<sizes[0]<<",      \'2\' - " << sizes[1] << " x " << sizes[1] << ",      \'3\' - " << sizes[2] << " x " << sizes[2] << " \n";
        char boardSize;
        cin >> boardSize;
        clrScr();
        switch (boardSize)
        {
        case '1':
            return Location(sizes[0], sizes[0]);
            break;
        case '2':
            return Location(sizes[1], sizes[1]);
            break;
        case '3':
            return Location(sizes[2], sizes[2]);
            break;
        default:
            cout << "please choose an available option\n";
            getBoardSize();
        }
    }


    char replayScreen() {
        char x;
        cout << "press \'x\' to exit or any other character to start again!!\n";
        cin >> x;
        return x;
    }

    char endScreen() {
        char a;
        cout << "\n\n\n\n\t\t\tTHANKS FOR PLAYING!!\n\n\n\n\n\n";
        cout << "\t\t\t\t\t\t\t\t\t\t\t- sajan sj\n";
        cin >> a;
        return a;
    }

    pair<states,Location> getInput() {
        char input;
        states action;
        Location loc;
        cout << "Enter 'F' if you want to flag or unflag, or enter any other character to uncover a cell.\n";
        cin >> input;
        if (input == 'F' || input == 'f') {
            cout << "Enter the cell location to flag or unflag! \n ";
            loc = getLocInput();
            action = flagged;
        }
        else {
            cout << "Enter the cell location to uncover! \n ";
            loc = getLocInput();
            action = uncovered;
        }
        return { action, loc } ;
    }

   

    Location getLocInput() {
        int r;
        int c;
        cout << "Select the row : ";
        cin >> r;
        cout << "\nSelect the column: ";
        cin >> c;
        return Location(r, c);
    }

    void viewTable(Cell** arr, const int rows, const int cols) {
        for (int i = 0; i < rows; i++) {
            if (i == 0) {
                cout << "\n\n     ";
                for (int j = 0; j < cols; j++) {// TOP ROW lines
                    if (j > 9) cout << " " << j << " ";
                    else cout << "  "<< j <<" ";    
                }
                cout << endl;
            }
            
            cout << "     ";
            for (int j = 0; j < cols; j++) {
                if (i == 0) { // TOP ROW lines
                    cout <<"____";
                    if (j == cols - 1) { // Top row end lines
                        cout << "_";
                    }
                }
                else if (j == cols - 1) { // RIGHT mid bottom lines
                    cout << "|___|";
                }
                else // MID lines
                    cout << "|___";
            }
            cout << endl;
            if (i > 9) cout << "  " << i << " ";
            else cout << "   " << i << " ";

            for (int j = 0; j < cols; j++) {
                Cell* cell = &arr[i][j];
                if (j == cols - 1) { // RIGHT ending lines with text
                    if (cell->state == uncovered && cell->mine)//___________________________________________________________TEST
                        cout << "| " << "\033[31mM\033[0m" << " |";
                    else if(cell->state == flagged)
                        cout << "| " << "\033[33mF\033[0m" << " |";
                    else if (cell->state == uncovered)
                        if (cell->adjacentMines == 0) cout << "| \033[32m0\033[0m |";
                        else cout << "| " << cell->adjacentMines << " |";
                    else  
                        cout << "|   |";
                }
                else // other starting lines with text
                    if (cell->state == uncovered && cell->mine)//___________________________________________________________TEST
                        cout << "| " << "\033[31mM\033[0m" << " ";
                    else if (cell->state == flagged)
                        cout << "| " << "\033[33mF\033[0m" << " ";
                    else  if (cell->state == uncovered)
                        if(cell->adjacentMines == 0) cout << "| \033[32m0\033[0m ";
                        else cout << "| " << cell->adjacentMines << " ";
                    else
                        cout << "|   ";
            }      
            cout << endl;
            
            if (i == rows - 1) { // BOTTOM most lines
                cout << "     ";
                for (int j = 0; j < cols; j++) {
                    if (j == cols - 1) { // RIGHT bottom most line
                        cout << "|___|";
                    }
                    else
                        cout << "|___";
                }
            }
            
        }
        cout << "\n\n" << endl;
    }
};



