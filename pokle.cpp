#include "pokle.h"
#include <algorithm>
#include <sstream>

string rankToString(Rank r) {
    switch (r) {
        case TWO:   return "2";
        case THREE: return "3";
        case FOUR:  return "4";
        case FIVE:  return "5";
        case SIX:   return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE:  return "9";
        case TEN:   return "10";
        case JACK:  return "J";
        case QUEEN: return "Q";
        case KING:  return "K";
        case ACE:   return "A";
        default:    return "";
    }
}

// return number between 2 to 14 based on Rank
int getRankValue(Rank r) {
    return static_cast<int>(r);
}

string findRank(const vector<Card>& cards) {
    if (cards.size() < 5) {
        cerr << "ERROR: cards not enough";
        return "";
    }
    
    // Frequency of each rank (using indices 0 and 1 unused; ranks 2..14)
    vector<int> freq(15, 0);
    for (auto card : cards) {
        freq[getRankValue(card.rank)]++;
    }

    // Frequency for suits (0: HEARTS, 1: DIAMONDS, 2: CLUBS, 3: SPADES)
    vector<int> suitCount(4, 0);
    for (auto card : cards) {
        suitCount[card.suit]++;
    }
    
    // Check for flush: does any suit appear 5 times?
    bool isFlush = false;
    Suit flushSuit;
    for (int s = 0; s < 4; s++) {
        if (suitCount[s] >= 5) {
            isFlush = true;
            flushSuit = static_cast<Suit>(s);
            break;
        }
    }
    
    // Build a boolean vector indicating which ranks are present.
    vector<bool> present(15, false);
    for (int r = 2; r <= 14; r++) {
        if (freq[r] > 0)
            present[r] = true;
    }
    
    // Check for straight.
    int straightHigh = 0;
    int consecutive = 0;
    for (int r = 2; r <= 14; r++) {
        if (present[r]) {
            consecutive++;
            if (consecutive >= 5)
                straightHigh = r;
        } else {
            consecutive = 0;
        }
    }
    
    // Special case: Ace can be low (A,2,3,4,5)
    if (present[14] && present[2] && present[3] && present[4] && present[5])
        straightHigh = max(straightHigh, 5);
    
    // Check for straight flush.
    int straightFlushHigh = 0;
    if (isFlush) {
        // Only consider cards with flushSuit.
        vector<bool> flushPresent(15, false);
        for (auto card : cards) {
            if (card.suit == flushSuit)
                flushPresent[getRankValue(card.rank)] = true;
        }
        int consecutiveFlush = 0;
        for (int r = 2; r <= 14; r++) {
            if (flushPresent[r]) {
                consecutiveFlush++;
                if (consecutiveFlush >= 5)
                    straightFlushHigh = r;
            } else {
                consecutiveFlush = 0;
            }
        }
        // Ace-low straight flush check.
        if (flushPresent[14] && flushPresent[2] && flushPresent[3] && flushPresent[4] && flushPresent[5])
            straightFlushHigh = max(straightFlushHigh, 5);
    }
    
    // Check for Royal Flush (a straight flush that is 10,J,Q,K,A).
    if (straightFlushHigh == 14) {
        bool has10 = false, hasJ = false, hasQ = false, hasK = false;
        for (auto card : cards) {
            if (card.suit == flushSuit) {
                int val = getRankValue(card.rank);
                if (val == 10) has10 = true;
                if (val == 11) hasJ = true;
                if (val == 12) hasQ = true;
                if (val == 13) hasK = true;
            }
        }
        if (has10 && hasJ && hasQ && hasK)
            return "RF";
    }
    
    if (straightFlushHigh != 0)
        return "SF with " + rankToString(static_cast<Rank>(straightFlushHigh));
    
    // Four of a Kind.
    int fourRank = 0;
    for (int r = 2; r <= 14; r++) {
        if (freq[r] == 4) {
            fourRank = r;
            break;
        }
    }
    if (fourRank != 0)
        return "FK with " + rankToString(static_cast<Rank>(fourRank));
    
    // Full House.
    int threeRank = 0;
    int pairRank = 0;
    for (int r = 2; r <= 14; r++) {
        if (freq[r] >= 3)
            threeRank = max(threeRank, r);
    }
    for (int r = 2; r <= 14; r++) {
        if (r != threeRank && freq[r] >= 2)
            pairRank = max(pairRank, r);
    }
    if (threeRank != 0 && pairRank != 0)
        return "FH with " + rankToString(static_cast<Rank>(threeRank))
               + " over " + rankToString(static_cast<Rank>(pairRank));
    
    // Flush.
    if (isFlush) {
        int highestFlush = 0;
        for (auto card : cards) {
            if (card.suit == flushSuit)
                highestFlush = max(highestFlush, getRankValue(card.rank));
        }
        return "FL with " + rankToString(static_cast<Rank>(highestFlush));
    }
    
    // Straight.
    if (straightHigh != 0)
        return "ST with " + rankToString(static_cast<Rank>(straightHigh));
    
    // Three of a Kind.
    if (threeRank != 0)
        return "TK with " + rankToString(static_cast<Rank>(threeRank));
    
    // Two Pair.
    vector<int> pairs;
    for (int r = 2; r <= 14; r++) {
        if (freq[r] == 2)
            pairs.push_back(r);
    }
    if (pairs.size() >= 2) {
        int highPair = *max_element(pairs.begin(), pairs.end());
        return "TP with " + rankToString(static_cast<Rank>(highPair));
    }
    
    // One Pair.
    if (pairs.size() == 1)
        return "OP with " + rankToString(static_cast<Rank>(pairs[0]));
    
    // High Card.
    int highCard = 0;
    for (auto card : cards)
        highCard = max(highCard, getRankValue(card.rank));
    return "HC with " + rankToString(static_cast<Rank>(highCard));
}
