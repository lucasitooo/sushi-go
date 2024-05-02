#include "game.h"
#include <fstream>
#include <iostream>

using namespace std;

// card.cpp

// Card Constructor
Card::Card(string type, int count) {
    sushiType = type;
    makiCount = count;
}
// Card Destructor
Card::~Card(){
    
}
// getSushiType
// Input: None
// Description: find and return the sushi type of a given card
// Output: string representing the card's sushi type
string Card::getSushiType() {
    return sushiType;
}
// getMakiCount
// Input: None
// Description: get the makiCount of the current card, given it is a maki card
// Output: integer representing the maki count of the current maki
int Card::getMakiCount() {
    return makiCount;
}

