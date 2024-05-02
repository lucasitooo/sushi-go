/*
 * game.cpp
 * CS11 Sushi Go
 */

// game.cpp
// Purpose: run the game Sushi Go.
// Written by: Lucas Frota(lfrota01)
// Date: May 1st, 2024


#include "game.h"
#include <fstream>
#include <iostream>
#include <string>

//constructor
Game::Game(string filename, string play_chopsticks){
    //include chopsticks or not
    if(play_chopsticks == "true"){
        this->play_chopsticks = true;
    }else{
        this->play_chopsticks = false;
    }  
    ifstream infile(filename);
    if (infile.fail()){ 
        cerr << "Failed to open deck file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    string skip;
    
    infile >> skip >> skip;
    while(!infile.eof()){
        int maki_num;
        string type;
        infile >> type;

        if (type.empty()) break; 

        while(type == "Chopsticks"){
            infile >> type;
        }
        if(type == "Maki"){
            infile >> maki_num;
        }else{
            maki_num = 0;
        }
        Card *card_p = new Card(type, maki_num);
        deck.push_back(card_p);
    }
    infile.close();
}

// playGame
// Input: None
// Description: Plays a game of Sushi Go!
// Output: None
void Game::playGame(){
    int card_index;
    string play_on;

    //play three rounds
    for(int round = 0; round < ROUNDS; round++){
         
        //select and pass all 27 cards
        Card* selectedCards[PLAYER_COUNT]; // Declared outside the loop

        for(int card = 0; card < CARD_HAND; card++){
            Deal(round);
            for(int player = 0; player < PLAYER_COUNT; player++){
                board.drawBoard(players, player);
                cout << " Player " << player + 1 << ", select a card: ";
                cin >> card_index;
                if(!cin){ //bail out of game if they enter a non-number
                    cerr << " INVALID INPUT: No game for you!\n";
                    exit(EXIT_FAILURE);
                }
                while(card_index > (9 - card) || card_index < 1){
                    cout << "     Please enter a valid number between 1 and "
                    << 9 - card << ": ";
                    cin >> card_index;
                }

                Vector *passingHand = players[player].getPassingHand();
                // Store selected card in selectedCards array
                selectedCards[player] = passingHand->at(card_index - 1); 
                passingHand->erase(card_index - 1);
            }  

            // Swap passing hands
            Vector temp; 
            for(int i = 0; i < players[0].getPassingHand()->size(); i++){
                temp.push_back(players[0].getPassingHand()->at(i));
            }
            // Iterate and delete each component of the vector
            players[0].setPassingHand(players[2].getPassingHand());
            players[2].setPassingHand(players[1].getPassingHand());
            players[1].setPassingHand(&temp);

            // Add selected cards to each player's revealed cards
            for (int player = 0; player < PLAYER_COUNT; player++) {
                players[player].getRevealedCards()->
                push_back(selectedCards[player]);
            }

            
        }
        updateScore();
        board.drawScore(players);     
        if(round < 2){
            cout << " End of round! Ready for Round "
                 << round + 2 << " ? (y/n): ";
            cin >> play_on;
            if(play_on != "y"){
                break;
            }
        }                                                        

        for (int j = 0; j < PLAYER_COUNT; j++) {
            // Delete revealed cards
            players[j].clearRevealedCards();
        }

    }
    int puddingCounts[PLAYER_COUNT] = {0};

    // Pudding Points
    for(int j = 0; j < PLAYER_COUNT; j++){
        puddingCounts[j] = players[j].getPuddingCount();
    }

    int highestPudding = puddingCounts[0];
    int lowestPudding = puddingCounts[0];

    // Find highest and lowest pudding counts
    for (int j = 1; j < PLAYER_COUNT; j++) {
        if (puddingCounts[j] > highestPudding) {
            highestPudding = puddingCounts[j];
        }
        if (puddingCounts[j] < lowestPudding) {
            lowestPudding = puddingCounts[j];
        }
    }



    // Award points for the highest pudding count
    int highestPuddingPlayers = 0;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (puddingCounts[j] == highestPudding) {
            highestPuddingPlayers++;
        }
    }

    int pointsForHighestPudding = 6 / highestPuddingPlayers;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (puddingCounts[j] == highestPudding) {
            players[j].addToScore(pointsForHighestPudding);
        }
    }

    // Deduct points for the least pudding
    int lowestPuddingPlayers = 0;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (puddingCounts[j] == lowestPudding) {
            lowestPuddingPlayers++;
        }
    }

    int pointsForLowestPudding = -6 / lowestPuddingPlayers;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (puddingCounts[j] == lowestPudding) {
            players[j].addToScore(pointsForLowestPudding);
        }
    }

    int start_score = 0;
    int winner = -1;
    int tiebreak = 0;
    int score = 0;

    // Find Winner
    for(int i = 0; i < PLAYER_COUNT; i++){
        score = players[i].getScore();
        if(score > start_score){
            start_score = score;
            winner = i;
        }
    }

    // Determine tiebreaker
    for(int i = 0; i < PLAYER_COUNT; i++){
        if(players[i].getScore() == start_score){
            if(players[i].getPuddingCount() == players[(i+1)%3].getPuddingCount()
            || players[i].getPuddingCount() == players[(i+2)%3].getPuddingCount()) 
            tiebreak++;

            if(players[i].getScore() == start_score && 
            players[i].getPuddingCount() >
            players[winner].getPuddingCount()) winner = i;

        }
    }
    if(tiebreak > 1){
        winner = -1;
    }
    board.drawWinner(players, winner);
}

// Deal
// Input: integer representing the round the game is currently on
// Description: Deal cards to a player's hand at the start of a round
// Output: None
void Game::Deal(int round) {
    int size = deck.size();
    static const int initialSize = size - 27;

    for (int i = size - 1; i > initialSize - (27 * round); i-= 3) { 
        for (int j = 0; j < 3; j++) {
            Card* next_card = deck.back();
            players[j].setHand(next_card); // Assign the card to the hand
            trash.push_back(deck.at(i - j)); // 'trash' vector for valgrind
            deck.erase(i - j); // Erase the card from the deck
        }
    }
}

// updateScore
// Input: None
// Description: update each player's score according to their revealed cards
// Output: None
void Game::updateScore(){
    int makiCounts[PLAYER_COUNT] = {0};

    for(int j = 0; j < PLAYER_COUNT; j++){
        int makiCount = 0;
        int tempuraCount = 0;
        int sashimiCount = 0;
        int dumplingCount = 0;
        int salmonNigiriCount = 0;
        int eggNigiriCount = 0;
        int squidNigiriCount = 0;
        int wasabiCount = 0;
        int puddingCount = 0;       
    
        Vector *revealedCards = players[j].getRevealedCards();

        // Calculate counts for each sushi type
        for (int i = 0; i < revealedCards->size(); i++) {
            Card *card = revealedCards->at(i);
            if (card->getSushiType() == "Maki") {
                makiCount += card->getMakiCount();
            } else if (card->getSushiType() == "Tempura") {
                tempuraCount++;
            } else if (card->getSushiType() == "Sashimi") {
                sashimiCount++;
            } else if (card->getSushiType() == "Dumpling") {
                dumplingCount++;
            } else if (card->getSushiType() == "Egg-Nigiri") {
                eggNigiriCount++;
            } else if (card->getSushiType() == "Salmon-Nigiri") {
                salmonNigiriCount++;
            } else if (card->getSushiType() == "Squid-Nigiri") {
                squidNigiriCount++;
            } else if (card->getSushiType() == "Wasabi") {
                wasabiCount++;
            } else if (card->getSushiType() == "Pudding") {
                puddingCount++;
            }
            // Update scores based on sushi counts
            

            // Tempura
            if (tempuraCount == 2) {
                players[j].addToScore(5);
                tempuraCount = 0;
            }

            // Salmon-Nigiri
            if(salmonNigiriCount == 1){
                if(wasabiCount > 0){
                    players[j].addToScore(2*3);
                    salmonNigiriCount = 0;
                    wasabiCount--;
                } else{
                    players[j].addToScore(2);
                    salmonNigiriCount = 0;
                }
            }

            // Egg
            if(eggNigiriCount == 1){
                if(wasabiCount > 0){
                    players[j].addToScore(1*3);
                    eggNigiriCount = 0;
                    wasabiCount--;
                }else{
                    players[j].addToScore(1);
                    eggNigiriCount = 0;
                }
            }

            // Squid
            if(squidNigiriCount == 1){
                if(wasabiCount > 0){
                    players[j].addToScore(3*3);
                    squidNigiriCount = 0;
                    wasabiCount--;
                } else{
                    players[j].addToScore(3);
                    squidNigiriCount = 0;
                }
            }

            // Sashimi
            if (sashimiCount == 3) {
                players[j].addToScore(10);
                sashimiCount = 0;
            }

            // Dumpling
            if (i == revealedCards->size() - 1) {
                if(dumplingCount == 0){
                    players[j].addToScore(0);
                } else if(dumplingCount == 1){
                    players[j].addToScore(1);
                } else if(dumplingCount == 2){
                    players[j].addToScore(3);
                } else if(dumplingCount == 3){
                    players[j].addToScore(6);
                } else if(dumplingCount == 4){
                    players[j].addToScore(10);
                } else if(dumplingCount >= 5){
                    players[j].addToScore(15);
                }
            }

            if (i == revealedCards->size() - 1) {
                players[j].setPuddingCount(puddingCount);
            }

        }
        makiCounts[j] = makiCount;
    }
    // Maki Points
    int highestMaki = 0;
    int secondHighestMaki = 0;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (makiCounts[j] > highestMaki) {
            secondHighestMaki = highestMaki;
            highestMaki = makiCounts[j];
        } else if (makiCounts[j] != highestMaki &&
         makiCounts[j] > secondHighestMaki) {
            secondHighestMaki = makiCounts[j];
        }
    }
    // Award points for the highest Maki count
    int highestMakiPlayers = 0;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (makiCounts[j] == highestMaki) {
            highestMakiPlayers++;
        }
    }
    int pointsForHighestMaki = 6 / highestMakiPlayers;
    for (int j = 0; j < PLAYER_COUNT; j++) {
        if (makiCounts[j] == highestMaki) {
            players[j].addToScore(pointsForHighestMaki);
        }
    }

    // Award points for the second highest Maki count
    if(highestMakiPlayers == 1){
        int secondHighestMakiPlayers = 0;
        for (int j = 0; j < PLAYER_COUNT; j++) {
            if (makiCounts[j] == secondHighestMaki) {
                secondHighestMakiPlayers++;
            }
        }
        int pointsForSecondHighestMaki = 3 / secondHighestMakiPlayers;
        for (int j = 0; j < PLAYER_COUNT; j++) {
            if (makiCounts[j] == secondHighestMaki && highestMakiPlayers < 2){
                players[j].addToScore(pointsForSecondHighestMaki);
            }
        }
    }

}


//destructor
Game::~Game(){
    for (int i = 0; i < deck.size(); ++i) {
        delete deck.at(i);
    }

    for(int i = 0; i < trash.size(); i++){
        delete trash.at(i);
    }

}


