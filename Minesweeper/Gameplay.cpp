
#include "Display.cpp"
#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

class Gameplay {
private:
    int rows;
    int cols;
    Difficulty gameDifficulty;
    set<Cell*> mines;

    enum placement {
        left, right, top, bottom, topLeft, topRight, bottomLeft, bottomRight, others
    };

    bool validLocation(Location loc) {
        return (0 <= loc.row && loc.row < rows && 0 <= loc.column && loc.column < cols);
    }

    placement getPlacement(Location loc) {

        if (loc.row == 0 && loc.column > 0 && loc.column < cols - 1) return top;
        if (loc.row == rows - 1 && loc.column > 0 && loc.column < cols - 1) return bottom;
        if (loc.column == 0 && loc.row > 0 && loc.row < rows - 1) return left;
        if (loc.column == cols - 1 && loc.row > 0 && loc.row < rows - 1) return right;

        if (loc.row == 0 && loc.column == 0) return topLeft;
        if (loc.row == 0 && loc.column == cols - 1) return topRight;
        if (loc.row == rows - 1 && loc.column == 0) return bottomLeft;
        if (loc.row == rows - 1 && loc.column == cols - 1) return bottomRight;
        else return others;
    }

    int mineDetector(stack<Cell*> locations) {
        int mineCount = 0;
        while (!locations.empty()) {
            if (locations.top()->mine) mineCount++;
            locations.pop();
        }
        return mineCount;
    }

    stack<Cell*> getPossibilities(Cell** Arr, vector<int> rowPos, vector<int> colPos) {
        stack<Cell*> possibilities;
        for (int i = 0; i < rowPos.size(); i++) {
            for (int j = 0; j < colPos.size(); j++) {
                possibilities.push(&Arr[rowPos[i]][colPos[j]]);
            }
        }
        possibilities.pop();
        return possibilities;
    }

    int processCells(Cell** Arr, Location loc) {

        placement place = getPlacement(loc);
        vector<int> rowPossibilities;
        vector<int> colPossibilities;
        int adjacentMines = 0;
        stack<Cell*> possibilities;

        switch (place)
        {
        case Gameplay::left:
            rowPossibilities = { loc.row - 1, loc.row + 1,loc.row };
            colPossibilities = { loc.column + 1,loc.column };
            break;
        case Gameplay::right:
            rowPossibilities = { loc.row - 1, loc.row + 1,loc.row };
            colPossibilities = { loc.column - 1,loc.column };
            break;
        case Gameplay::top:
            rowPossibilities = { loc.row + 1,loc.row };
            colPossibilities = { loc.column - 1,loc.column + 1,loc.column };
            break;
        case Gameplay::bottom:
            rowPossibilities = { loc.row - 1,loc.row };
            colPossibilities = { loc.column - 1,loc.column + 1,loc.column };
            break;
        case Gameplay::topLeft:
            rowPossibilities = { loc.row + 1,loc.row };
            colPossibilities = { loc.column + 1,loc.column };
            break;
        case Gameplay::topRight:
            rowPossibilities = { loc.row + 1,loc.row };
            colPossibilities = { loc.column - 1,loc.column };
            break;
        case Gameplay::bottomLeft:
            rowPossibilities = { loc.row - 1,loc.row };
            colPossibilities = { loc.column + 1,loc.column };
            break;
        case Gameplay::bottomRight:
            rowPossibilities = { loc.row - 1,loc.row };
            colPossibilities = { loc.column - 1,loc.column };
            break;
        default:
            rowPossibilities = { loc.row - 1, loc.row + 1,loc.row };
            colPossibilities = { loc.column - 1,loc.column + 1,loc.column };
            break;
        }
        possibilities = getPossibilities(Arr, rowPossibilities, colPossibilities);
        adjacentMines = mineDetector(possibilities);
        
        if (adjacentMines == 0 ) {
            while (!possibilities.empty()) {
                if (!possibilities.top()->mine && possibilities.top()->state != uncovered) {
                    Location selected(possibilities.top()->rowNum, possibilities.top()->colNum);  
                    possibilities.top()->state = uncovered;
                    possibilities.top()->adjacentMines = processCells(Arr, selected);
                    toUncover -= 1;
                    if (gameDifficulty == medium ) {
                        if (possibilities.top()->adjacentMines > 2) possibilities.top()->state = covered;
                        toUncover += 1;
                    }
                    if (gameDifficulty == pro) {
                        if (possibilities.top()->adjacentMines > 1) possibilities.top()->state = covered;
                        toUncover += 1;
                    }
                    
                }
                    possibilities.pop();
            }
        }
        return adjacentMines;

    }

    int setMineCount() {
        if (gameDifficulty == medium) return rows * 2;
        else if (gameDifficulty == pro) return rows * 3;
        else return rows/2 * 2;
    }
    

public:

    bool gameOver = false;
    bool won = false;
    int toUncover = 0;

    Gameplay(const int r, const int c, Difficulty difficulty) {
        rows = r;
        cols = c;
        gameDifficulty = difficulty;
        toUncover = r * c;
    }



    void initializeMines(Cell** Arr) {
        int totalMines = setMineCount();
        Location firstLoc;
        Display display;
        do {
            cout << "Insert a cell location to start the first move !! \n";
            firstLoc = display.getLocInput();
        } while (!validLocation(firstLoc));

        Cell* firstCell = &Arr[firstLoc.row][firstLoc.column];
        int randRow = 0;
        int randCol = 0; 
        Cell* randomCell;
        mines.insert(firstCell);
        srand(time(NULL));
        while (mines.size() != totalMines + 1) {
            randRow = rand();
            randCol = rand();
            randRow %= rows;
            randCol %= cols;
            randomCell = &Arr[randRow][randCol];
            if (randomCell == firstCell) randomCell->mine = false;
            else randomCell->mine = true;
            mines.insert(randomCell);
        }
        toUncover -= totalMines ;
        toUncover--;
        processInput(Arr, uncovered, firstLoc);
    }

    void GameOverTable() {
        if (won == true) {
            for (Cell* mine : mines) {
                if(mine->mine)
                    mine->state = flagged;
            }
        }
        else {
            for (Cell* mine : mines) {
                mine->state = uncovered;
            }
        }
    }
    

    bool processInput(Cell** Arr, states action, Location loc) {
        Display display;
        if (validLocation(loc)) {
            Cell* selected = &Arr[loc.row][loc.column];
            if (selected->state == uncovered) {
                display.clrScr();
                display.viewTable(Arr, rows, cols);
                cout << "The cell is already un-covered, select a valid cell!!\n";
                return true;
            }
            else if (action == flagged) {
                if (selected->state == covered)
                    selected->state = action;
                else if (selected->state == flagged)
                    selected->state = covered;
            }
            else if (action == uncovered) {
                if (selected->mine) {
                    gameOver = true;
                }
                else {
                    selected->state = action;
                    selected->adjacentMines = processCells(Arr, loc);
                    toUncover -= 1;
                    if (toUncover == 0) {
                        won = true;
                        gameOver = true;
                    }
                }
                
            }
            return false;
        }
        else {
            display.clrScr();
            display.viewTable(Arr, rows, cols);
            cout << "The entered value are not within range, select a valid cell!!\n";
            return true;
        }
       
    }

    Cell** InitializeTable(Cell** Arr) {
        Arr = new Cell* [rows]; // Empty Table
        for (int i = 0; i < rows; i++) {
            Arr[i] = new Cell[cols];
            for (int j = 0; j < cols; j++) {
                Arr[i][j] = Cell();
                Arr[i][j].adjacentMines = 0;
                Arr[i][j].rowNum = i;
                Arr[i][j].colNum = j;
            }
        }
        return Arr;
    }

};