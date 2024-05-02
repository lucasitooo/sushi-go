/*
 * game.h
 * COMP11 Sushi Go
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "board.h"
#include "vector.h"
#include "player.h"
#include "card.h"

using namespace std;

class Game{
    public:
        //constructor/destructor
        Game(string filename, string play_chopsticks);
        ~Game();

        //main gameplay
        void playGame();
        void Deal(int round);
        void updateScore();

    private:
        //constants
        static const int PLAYER_COUNT = 3;
        static const int ROUNDS = 3;
        static const int CARD_HAND = 9;

        //chopsticks activated true/false
        bool play_chopsticks;

        //card deck
        Vector deck;
        // extras
        Vector trash;

        //game objects
        Board board;
        Player players[PLAYER_COUNT];
};

#endif
