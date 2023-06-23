#include "Gameplay.cpp"
#include <iostream>

using namespace std;

int main() {
    Cell** Arr = new Cell*;
    Display display;
    
    do {
        display.clrScr();
        display.welcomeScreen();
        display.clrScr();
        Difficulty gameDifficulty = display.chooseDifficulty();
        display.clrScr();
        Location size = display.getBoardSize();

        Gameplay* game = new Gameplay(size.row, size.column , gameDifficulty);
        display.clrScr();
        
        Arr = game->InitializeTable(Arr);
        display.viewTable(Arr, size.row, size.column);
        
        game->initializeMines(Arr);
        while (!game->gameOver) {
            display.clrScr();
            cout << "to uncover " << game->toUncover << endl;
            pair<states, Location> input;
            display.viewTable(Arr, size.row, size.column);
            do {
                input = display.getInput();
            } while (game->processInput(Arr, input.first, input.second));
        }
        game->GameOverTable();
        display.gameOverScreen(game->won);
        delete game;
        display.viewTable(Arr, size.row, size.column);
    } while (display.replayScreen() != 'x');
    display.clrScr();
    cout << display.endScreen();
    
    return 0;

}
