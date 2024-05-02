/*
 * player.h
 * COMP11 Sushi Go
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "vector.h"

using namespace std;


class Player{
    public:
    Player();
    ~Player();
    Vector *getPassingHand();
    Vector *getRevealedCards();
    int getScore();
    int getPuddingCount();
    void setHand(Card* card);
    void setPassingHand(Vector *newPassingHand);
    void setPuddingCount(int count);
    void addToScore(int points);
    void clearRevealedCards();

    private:
    int PuddingCount;
    int score;
    Vector passingHand;
    Vector revealedCards;
};

#endif