#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> 
#include "termfuncs.h"

using namespace std;

class Card {
    public:
    // Constructors
    Card(string type, int count); 
    ~Card();
    string getSushiType();
    int getMakiCount();
    
    private:
   string sushiType;
   int makiCount; 
};

#endif