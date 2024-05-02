// player.cpp
#include "game.h"
#include <fstream>
#include <iostream>
#include "player.h"


// Player constructor
Player::Player(){
    PuddingCount = 0;
    score = 0;
}

/* Player::getPassingHand
 * Input: None.
 * Description: Retrieves the passing hand of the player.
 * Output: A pointer to the vector containing the passing hand of the player.
 */
Vector* Player::getPassingHand() {
    return &passingHand;
}

/* Player::getRevealedCards
 * Input: None.
 * Description: Retrieves the revealed cards of the player.
 * Output: A pointer to the vector containing the revealed cards of the player.
 */

Vector* Player::getRevealedCards() {
    return &revealedCards;
}

/* Player::getScore
 * Input: None.
 * Description: Retrieves the current score of the player.
 * Output: The current score of the player.
 */

int Player::getScore() {
    return score;
}

/* Player::getPuddingCount
 * Input: None.
 * Description: Retrieves the count of pudding cards collected by the player.
 * Output: The count of pudding cards collected by the player.
 */

int Player::getPuddingCount() {
    return PuddingCount;
}

/* Player::setHand
 * Input: A pointer to a Card object representing the card to be added to the player's hand.
 * Description: Adds a card to the player's passing hand.
 * Output: None.
 */

void Player::setHand(Card *card){
    passingHand.push_back(card);
}

/* Player::setPassingHand
 * Input: A pointer to a Vector object representing the new passing hand.
 * Description: Sets the passing hand of the player to the given passing hand.
 * Output: None.
 */

void Player::setPassingHand(Vector* newPassingHand) {
    for(int i = 0; i < passingHand.size(); i++){
        passingHand.erase(i);
        passingHand.insert(i, newPassingHand->at(i));
    }
}

/* Player::addToScore
 * Input: An integer representing the points to be added to the player's score.
 * Description: Adds points to the player's score.
 * Output: None.
 */

void Player::addToScore(int points) {
    score += points;
}

/* Player::setPuddingCount
 * Input: An integer representing the new count of pudding cards collected by the player.
 * Description: Sets the count of pudding cards collected by the player.
 * Output: None.
 */

void Player::setPuddingCount(int count) {
    PuddingCount += count;
}

/* Player::clearRevealedCards
 * Input: None.
 * Description: Clears the revealed cards of the player.
 * Output: None.
 */

void Player::clearRevealedCards() {
    revealedCards.clear();
}

// player destructor
Player::~Player(){
    
}
