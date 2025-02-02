#include "pokle.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<Card> hand = {
        {ACE, HEARTS},
        {KING, HEARTS},
        {QUEEN, HEARTS},
        {JACK, HEARTS},
        {TEN, HEARTS}
    };
    vector<Card> hand2 = {
        {FIVE, CLUBS},
        {THREE, HEARTS},
        {NINE, DIAMONDS},
        {TWO, SPADES},
        {ACE, CLUBS}
    };
    
    cout << "Hand rank: " << findRank(hand) << "\n";
    cout << "Hand rank: " << findRank(hand2) << "\n";
    
    return 0;
}
