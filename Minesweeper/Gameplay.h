
#include "display.cpp"
#include <iostream>

using namespace std;

class Gameplay {
private:
    int rows;
    int cols;
    int gameCheck = 0;
    bool winCheck = false;

    void fillSlot() { gameCheck += -1; }

    void vericalCheck(char** Arr, int* slot, char player, int sel) {
        int winCount = 0;
        if (rows - 1 - slot[sel] >= 3) {
            for (int i = slot[sel]; i < rows; i++) {
                if (Arr[i][sel] == player) {
                    winCount++;
                }
                else
                    break;
            }
            if (winCount == 4) {
                winCheck = true;
            }
        }
    }

    void horizontalCheck(char** Arr, int* slot, char player, int sel) {
        int winCount = 1;
        // char a;
        bool leftDead = false;
        bool rightDead = false;
        int lPtr = sel - 1;
        int rPtr = sel + 1;
        while (!leftDead || !rightDead) {
            if (lPtr >= 0 && Arr[slot[sel]][lPtr] == player) {
                --lPtr;
                ++winCount;
            }
            else if (lPtr < 0 || Arr[slot[sel]][lPtr] != player)
                leftDead = true;
            if (rPtr <= cols - 1 && Arr[slot[sel]][rPtr] == player) {
                ++rPtr;
                ++winCount;
            }
            else if (rPtr > cols - 1 || Arr[slot[sel]][rPtr] != player)
                rightDead = true;
        }
        // cout << "wincount " << winCount << endl;
        //   cout << Arr[slot[sel]][lPtr] << "lptr " << lPtr << endl;
        //   cout << Arr[slot[sel]][rPtr] << "rptr " << rPtr << endl;
        //   cin >> a;
        if (winCount == 4) {
            winCheck = true;
        }
    }

    void backwDiagonalCheck(char** Arr, int* slot, char player, int sel) {
        int winCount = 1;
        bool leftDead = false;
        bool rightDead = false;
        int lColPtr = sel - 1;
        int lRowPtr = slot[sel] - 1;
        int rColPtr = sel + 1;
        int rRowPtr = slot[sel] + 1;
        while (!leftDead || !rightDead) {
            if (lColPtr >= 0 && lRowPtr >= 0 && Arr[lRowPtr][lColPtr] == player) {
                --lColPtr;
                --lRowPtr;
                ++winCount;
            }
            if (rColPtr <= cols - 1 && rRowPtr <= rows - 1 &&
                Arr[rRowPtr][rColPtr] == player) {
                ++rColPtr;
                ++rRowPtr;
                ++winCount;
            }
            if (lColPtr < 0 || lRowPtr < 0 || Arr[lRowPtr][lColPtr] != player)
                leftDead = true;
            if (rColPtr > cols - 1 || rRowPtr > rows - 1 ||
                Arr[rRowPtr][rColPtr] != player)
                rightDead = true;
        }
        if (winCount == 4) {
            winCheck = true;
        }
    }

    void forwDiagonalCheck(char** Arr, int* slot, char player, int sel) {
        int winCount = 1;
        bool leftDead = false;
        bool rightDead = false;
        int lColPtr = sel - 1;
        int lRowPtr = slot[sel] + 1;
        int rColPtr = sel + 1;
        int rRowPtr = slot[sel] - 1;
        while (!leftDead || !rightDead) {
            if (lColPtr >= 0 && lRowPtr <= rows - 1 &&
                Arr[lRowPtr][lColPtr] == player) {
                --lColPtr;
                ++lRowPtr;
                ++winCount;
            }
            if (rColPtr <= cols - 1 && rRowPtr >= 0 &&
                Arr[rRowPtr][rColPtr] == player) {
                ++rColPtr;
                --rRowPtr;
                ++winCount;
            }
            if (lColPtr < 0 || lRowPtr > rows - 1 || Arr[lRowPtr][lColPtr] != player)
                leftDead = true;
            if (rColPtr > cols - 1 || rRowPtr < 0 || Arr[rRowPtr][rColPtr] != player)
                rightDead = true;
        }
        if (winCount == 4) {
            winCheck = true;
        }
    }

    void checkRules(char** Arr, int* slot, char player, int sel) {
        vericalCheck(Arr, slot, player, sel);
        horizontalCheck(Arr, slot, player, sel);
        forwDiagonalCheck(Arr, slot, player, sel);
        backwDiagonalCheck(Arr, slot, player, sel);
    }

    void insertSlot(char** Arr, int slot[], char player, const int sel) {
        Arr[slot[sel]][sel] = player;
        checkRules(Arr, slot, player, sel);
        slot[sel] -= 1;
        if (slot[sel] == -1)
            fillSlot();
    }
    bool verify(int slots[], int choice) {
        if (choice < 0 || slots[choice] == -1 || choice > cols - 1) {
            cout << "Pick from the Available Slots!!!";
            return true;
        }
        else
            return false;
    }

public:
    Gameplay(const int r, const int c) {
        rows = r;
        cols = c;
    }

    char** InitializeTable(char** Arr) {
        Arr = new char* [rows]; // Empty Table
        for (int i = 0; i < rows; i++) {
            Arr[i] = new char[cols];
            for (int j = 0; j < cols; j++) {
                Arr[i][j] = ' ';
            }
        }
        return Arr;
    }
    void InitializeSlots(int* slots) {
        for (int i = 0; i < cols; i++) { // All Slots open
            slots[i] = rows - 1;
        }
    }

    bool gameOver(char player) {
        if (winCheck || gameCheck == -cols) {
            Display* endDisplay = new Display;
            if (winCheck) {
                endDisplay->winningMessage(" WON", player);
            }
            else if (gameCheck == -cols) {
                endDisplay->winningMessage("IT'S A DRAW");
            }
            delete endDisplay;
            return true;
        }

        return false;
    }

    void getSlot(char** Arr, char16_t player, int slot[]) {
        int choice;
        do {
            cin >> choice;
        } while (verify(slot, choice));
        insertSlot(Arr, slot, player, choice);
    }
};