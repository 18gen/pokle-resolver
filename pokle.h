#ifndef POKLE_H
#define POKLE_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };

enum Rank { 
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, 
    JACK,  // value 11
    QUEEN, // value 12
    KING,  // value 13
    ACE    // value 14
};

struct Card {
    Rank rank;
    Suit suit;
};

struct Player {
    string name;
    Card hand[2];
};

struct GameState {
    vector<Player> players;
    vector<vector<int>> rankings; // 3 different rankings (e.g., flop, turn, river)
};

// Returns a string representing the rank of the given hand.
// For example, "RF" for Royal Flush or "FK with 9" for Four of a Kind with 9.
string findRank(const vector<Card>& cards);

string rankToString(Rank r);
int getRankValue(Rank r);

#endif
