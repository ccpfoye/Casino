/*
        Blackjack!
        A programming exercise by Clay Foye
*/

/* TODO's

BUGS:
- It currently quits when you enter unexpected input during hitting/standing

FEATURES:
- Splitting
- Betting!
- Cheat modes
    - Card Counting?
        - Programmatic 
        - Using that method of the table
    - Edge identifying?
- Save States
    - Money amounts
    - progression
*/

// Includes:
#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
using namespace std;

// Defs
#define N 52
#define hand_size 22

// Recursive to find a valid card
int draw_card_helper(bitset<N> deck, int randint)
{
    if (deck[randint] == 0) { return randint; }
    else
    {
        int value = (randint + 1) % N;
        return draw_card_helper(deck, value);
    }
}

/* Search through the deck for a valid card index, and return the index*/
int draw_card(bitset<N> deck, int randint)
{
    return draw_card_helper(deck, randint);
}

// Get the suit name!
string get_card_suit(int card_index)
{
    if (card_index < 13){ return "Hearts"; }   
    if (card_index < 26) { return "Diamonds"; }
    if (card_index < 39) { return "Spades"; }
    return "Clubs";
}

/*Get the value of a card
card_index: an integer representing the index of the card in the deck
do_clamp: whether or not to clamp Ace and facecard values to 11 and 10 */
int get_card_value(int card_index, bool do_clamp)
{
    int card_value = (card_index % 13) + 2;
    if (do_clamp) {
        if (card_value > 10 && card_value < 14) { card_value = 10; }
        if (card_value == 14) { card_value = 11; }
    }
    return card_value;
}


/* Get the name of a card given an index in the deck! */
string get_card_name(int card_index)
{
    string name;                                 // to store name
    string suit = get_card_suit(card_index);     // get the suit
    int card_value = get_card_value(card_index, false); // convert from index to value
    switch (card_value)
    {
    case 14:
        name = "Ace of ";
        break;
    case 13:
        name = "King of ";
        break;
    case 12:
        name = "Queen of ";
        break;
    case 11:
        name = "Jack of ";
        break;
    default:
        name = to_string(card_value) + " of ";
        break;
    }
    name = name + suit; // concatenate the number and suit
    return name;        // return card name
}

// Run the game!
int main(int argc, char const *argv[])
{
    // Declare the deck as a bit_set
    bitset<N> deck;

    // reseed!
    srand(time(NULL));

    int randint = rand() % 52;
    int card_index = 0;
    int player_index = 2;
    int dealer_index = 2;
    string card_name = "";
    string hidden_dealer_card = "";
    vector<int> dealer_cards(hand_size, 0);
    vector<int> player_cards(hand_size, 0);
    int player_sum = 0;
    int dealer_sum = 0;

    // You want to play?
    while (true)
    {
        deck.reset();
        // store the answers
        string answer;

        // new game line:
        cout << "|----------------------------------------------------|\n";
        // Ask if you want to play:
        cout << "Want to play? (y/n)\n";

        // Get answer
        getline(cin, answer);

        // no:
        if (!answer.compare("n"))
        {
            cout << "See you next time!\n";
            break;
        }

        if (answer.compare("y"))
        {
            cout << "Unknown Answer! Please enter (y/n).\n";
            deck.reset();
            continue;
        }

        // answer was "y"!

        // reset draw indexes
        player_index = 2;
        dealer_index = 2;
        // reset hand values
        fill(player_cards.begin(), player_cards.end(), 0);
        fill(dealer_cards.begin(), dealer_cards.end(), 0);

        // Draw for dealer!
        cout << "Dealer draws:\n";
        card_index = draw_card(deck, randint);
        dealer_cards[0] = get_card_value(card_index, true);
        cout << get_card_name(card_index) << "\n";
        deck[card_index] = 1;

        randint = rand() % 51;

        card_index = draw_card(deck, randint);
        dealer_cards[1] = get_card_value(card_index, true);
        hidden_dealer_card = get_card_name(card_index);
        cout << "Dealer draws hidden card.\n";
        deck[card_index] = 1;

        // Your Draw:
        cout << "\nYou draw:\n";

        randint = rand() % 51;
        card_index = draw_card(deck, randint);
        player_cards[0] = get_card_value(card_index, true);
        cout << get_card_name(card_index) << "\n";
        deck[card_index] = 1;

        randint = rand() % 51;
        card_index = draw_card(deck, randint);
        player_cards[1] = get_card_value(card_index, true);
        cout << get_card_name(card_index) << "\n";
        deck[card_index] = 1;

        cout << "Dealer shows: " << dealer_cards[0] << "\n";

        player_sum = accumulate(player_cards.begin(), player_cards.end(), 0);
        cout << "You now show: " << player_sum << "\n";

        // Check for Blackjack:
        if (player_sum == 21)
        {
            cout << "Blackjack!\n";
            deck.reset();
            continue;
        }

        cout << "Hit? (y/n)\n" << "|-------|\n";
        getline(cin, answer);

        while (!answer.compare("y"))
        {
            randint = rand() % 51;
            card_index = draw_card(deck, randint);
            player_cards[player_index] = get_card_value(card_index, true);
            player_index++;
            cout << "You draw: " << get_card_name(card_index) << "\n";
            player_sum = accumulate(player_cards.begin(), player_cards.end(), 0);
            if (player_sum > 21)
            {
                for (int i = 0; i < hand_size; i++)
                {
                    if (player_cards[i] == 11)
                    {
                        // cout << "ACE FOUND!\n";
                        player_cards[i] = 1;
                        break;
                    }
                }
            }
            player_sum = accumulate(player_cards.begin(), player_cards.end(), 0);
            cout << "You now show: " << player_sum << "\n";
            deck[card_index] = 1;

            if (player_sum == 21)
            {
                cout << "Blackjack!\n";
                deck.reset();
                break;
            }

            if (player_sum > 21)
            {
                for (int i = 0; i < hand_size; i++)
                {
                    if (player_cards[i] == 11)
                    {
                        player_cards[i] = 1;
                        break;
                    }
                }
                player_sum = accumulate(player_cards.begin(), player_cards.end(), 0);
                if (player_sum > 21)
                {
                    // Check for Aces!!
                    cout << "BUST! Game Over. \n";
                    deck.reset();
                    break;
                }
            }

            cout << "Hit? (y/n)\n" << "|-------|\n";
            getline(cin, answer);
        }

        if (!answer.compare("n"))
        {
            // Reveal the dealer's hidden card
            cout << "Dealer turns over: " << hidden_dealer_card << "\n";
            dealer_sum = accumulate(dealer_cards.begin(), dealer_cards.end(), 0);
            cout << "Dealer shows: " << dealer_sum << "\n";

            // while check for 17
            while (dealer_sum < 17)
            {
                randint = rand() % 51;
                card_index = draw_card(deck, randint);
                dealer_cards[dealer_index] = get_card_value(card_index, true);
                card_name = get_card_name(card_index);
                cout << "Dealer draws: " << card_name << "\n";
                dealer_sum = accumulate(dealer_cards.begin(), dealer_cards.end(), 0);

                dealer_index++;
                deck[card_index] = 1;

                if (dealer_sum > 17)
                {
                    for (int i = 0; i < hand_size; i++)
                    {
                        if (dealer_cards[i] == 11)
                        {
                            // cout << "DEALER ACE FOUND!\n";
                            dealer_cards[i] = 1;
                            break;
                        }
                    }
                }
                dealer_sum = accumulate(dealer_cards.begin(), dealer_cards.end(), 0);
                cout << "Dealer shows: " << dealer_sum << "\n";
            }

            
            // compare against player score
            if (player_sum > dealer_sum || dealer_sum > 21) { cout << "You win!\n"; }
            else if (dealer_sum == player_sum) { cout << "Push!\n"; }
            else { cout << "You lose!\n"; }
        }
    }

    return 0;
}
