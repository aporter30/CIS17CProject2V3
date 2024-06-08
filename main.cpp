/* 
 * File:   main.cpp
 * Author: ajporter
 *
 *      Created: May 21, 2024, 11:08 AM
 *      Purpose: Project 2 - Building on Project 1 Blackjack game with trees and
 *                           graphs concepts from past month
 */

//System Libraries
#include <iostream>
#include <iomanip>  //setprecision
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>

#include "deck.h"
#include "player.h"
#include "gameStats.h"
#include "graph.h"

using namespace std;

// Function prototypes
void prntTitle();       //Function to print the title of the game
int factorial(int n);   //Function to calculate the factorial of a number
int fibonacci(int n);   //Function to calculate the Fibonacci series
void quicksort(vector<Card>& cards, int low, int high); //Function to perform quicksort on a vector of cards
int partition(vector<Card>& cards, int low, int high);  //Function to partition the vector for quicksort
void inorderTraversal(Node* root);      //Function to perform inorder traversal of a binary tree

int main() {
    // Initialize objects
    Deck deck;
    Player player("Gambler", 1000);
    GameStats stats;
    
    Graph gameGraph;
    gameGraph.addEdge("Start", "Player Turn");
    gameGraph.addEdge("Player Turn", "Dealer Turn");
    gameGraph.addEdge("Dealer Turn", "Determine Winner");
    gameGraph.addEdge("Determine Winner", "End");

    // Traverse graph using BFS and DFS
    gameGraph.BFS("Start");
    gameGraph.DFS("Start");

    // Display welcome message and rules
    prntTitle();
    deck.displayRules();
    
    char playAgain;
    do {
        // Place bet
        player.placeBet();
        
        // Initialize hands
        list<Card> playerHand;
        list<Card> dealerHand;
        
        //Deal initial cards
        deck.dealInitialCards(playerHand, dealerHand);
        deck.displayInitialHands(playerHand, dealerHand);
        //Player's and Dealer's turn
        deck.playerTurn(playerHand, player);
        deck.dealerTurn(dealerHand, &Deck::calculateHandValue);
        //Determine winner and update score
        deck.determineWinner(playerHand, dealerHand);
        deck.updateScore(player.balance, playerHand, dealerHand);
        
        //Record game result
        if (deck.calculateHandValue(playerHand) > 21) {
            stats.recordLoss();
        } else if (deck.calculateHandValue(dealerHand) > 21 || deck.calculateHandValue(playerHand) > deck.calculateHandValue(dealerHand)) {
            stats.recordWin();
            player.winBet();
        } else {
            stats.recordLoss();
            player.loseBet();
        }
        //Display current balance
        cout << "Your current balance: $" << player.balance << endl;
        if(player.balance <= 0){
            cout<<"Pit Boss: Buddy, you lost all of your money. Get outta here!\n";
            cout<<"\nPLAYER STATS\n";
            break;
        }
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    } 
    while (playAgain == 'y' && player.balance > 0);
    //Displaying player stats and game stats and leaving message
    if (playAgain != 'y' || player.balance <= 0) {
        player.displayStats();
    }
    stats.displayStats();
    deck.displayGoodbyeMessage();
    //Exit stage right
    return 0;
}
//Function to print the title of the game
void prntTitle() {
    cout << "WELCOME TO BLACKJACK AT PORTER'S CASINO\n";
    cout << " ___   ___\n";
    cout << "|A  | |10 |\n";
    cout << "| ♣ | | ♦ |\n";
    cout << "|__A| |_10|\n";
}
//Function to calculate the factorial of a number
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
//Function to calculate the Fibonacci series
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
//Function to perform quicksort on a vector of cards
void quicksort(vector<Card>& cards, int low, int high) {
    if (low < high) {
        int pi = partition(cards, low, high);
        quicksort(cards, low, pi - 1);
        quicksort(cards, pi + 1, high);
    }
}
//Function to partition the vector for quicksort
int partition(vector<Card>& cards, int low, int high) {
    Card pivot = cards[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (cards[j].value < pivot.value) {
            i++;
            swap(cards[i], cards[j]);
        }
    }
    swap(cards[i + 1], cards[high]);
    return (i + 1);
}
//Function to perform inorder traversal of a binary tree
void inorderTraversal(Node* root) {
    if (root == nullptr) return;
    inorderTraversal(root->left);
    cout << root->card.rank << " of " << root->card.suit << endl;
    inorderTraversal(root->right);
}
//PLAYER FUNCTIONS
//Function to place a bet
void Player::placeBet() {
    cout << name << ", you have $" << balance << ". Enter your bet amount: ";
    cin >> currentBet;
    while (currentBet > balance || currentBet <= 0) {
        cout << "Invalid bet amount. Enter a valid bet amount: ";
        cin >> currentBet;
    }
    balance -= currentBet;
}
//Function to win a bet
void Player::winBet() {
    balance += 2 * currentBet;
    currentBet = 0;
}
//Function to lose a bet
void Player::loseBet() {
    currentBet = 0;
}
//Function to record hit
void Player::recordHit() {
    hits++;
}
//Function to record stands
void Player::recordStand() {
    stands++;
}
//Function to record busts
void Player::recordBust() {
    busts++;
}
//Function to display recorded stats above
void Player::displayStats() const {
    cout << "\nPLAYER STATS:" << endl;
    cout << "Hits: " << hits << endl;
    cout << "Stands: " << stands << endl;
    cout << "Busts: " << busts << endl;
}
//GAMESTATS FUNCTIONS
//Record a win for w/l %
void GameStats::recordWin() {
    gamesPlayed++;
    gamesWon++;
}
//Record a loss for w/l %
void GameStats::recordLoss() {
    gamesPlayed++;
    gamesLost++;
}
//Display game statistics and w/l %
void GameStats::displayStats() const {
    cout << "Games Played: " << gamesPlayed << endl;
    cout << "Games Won: " << gamesWon << endl;
    cout << "Games Lost: " << gamesLost << endl;
    cout << "Win/Loss Percentage: " << setprecision(4) << static_cast<float>(gamesWon) / gamesPlayed << endl;
}
//DECK FUNCTIONS
//Deck constructor
Deck::Deck() {
    string suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
    string vals[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    for (const auto& suit : suits) {
        for (const auto& value : vals) {
            Card card;
            card.suit = suit;
            card.rank = value;
            deck.push_back(card);
            remainingCards[card.rank + card.suit]++;
            addCardToHashTable(card);
        }
    }
}
//Shuffle deck function
void Deck::shuffleDeck() {
    random_device rd;
    mt19937 rng(rd());
    list<Card> shuffledDeck;
    while (!deck.empty()) {
        auto it = next(deck.begin(), rng() % deck.size());
        shuffledDeck.splice(shuffledDeck.end(), deck, it);
    }
    deck.swap(shuffledDeck);
}
//Dealing Initial cards for start of game
void Deck::dealInitialCards(list<Card>& playerHand, list<Card>& dealerHand) {
    shuffleDeck();

    playerHand.push_back(deck.front());
    usedCards.insert(deck.front().rank + deck.front().suit);
    deck.pop_front();
    updateRemainingCards(playerHand.back());
    playerDeck.push(playerHand.back());

    dealerHand.push_back(deck.front());
    usedCards.insert(deck.front().rank + deck.front().suit);
    deck.pop_front();
    updateRemainingCards(dealerHand.back());
    dealerDeck.push(dealerHand.back());

    playerHand.push_back(deck.front());
    usedCards.insert(deck.front().rank + deck.front().suit);
    deck.pop_front();
    updateRemainingCards(playerHand.back());
    playerDeck.push(playerHand.back());

    while (true) {
        const Card& dealerCard = deck.front();
        if (usedCards.find(dealerCard.rank + dealerCard.suit) == usedCards.end()) {
            dealerHand.push_back(dealerCard);
            usedCards.insert(dealerCard.rank + dealerCard.suit);
            deck.pop_front();
            updateRemainingCards(dealerHand.back());
            dealerDeck.push(dealerHand.back());
            break;
        }
        shuffleDeck();
    }
}
//Update remaining cards
void Deck::updateRemainingCards(const Card& card) {
    remainingCards[card.rank + card.suit]--;
}
//Display starting hands for player and dealer
void Deck::displayInitialHands(const list<Card>& playerHand, const list<Card>& dealerHand) const {
    cout << "PLAYER'S HAND:" << endl;
    for (const auto& card : playerHand) {
        cout << card.rank << " of " << card.suit << endl;
    }
    cout << endl;

    cout << "DEALER'S HAND:" << endl;
    const Card& firstCard = dealerHand.front();
    cout << firstCard.rank << " of " << firstCard.suit << endl;
    cout << "[Hidden Card]" << endl;
}
//Facilitates player turn
void Deck::playerTurn(list<Card>& playerHand, Player& player) {
    char choice;
    do {
        cout << "\nDo you want to hit (h) or stand (s)?" << endl;
        cin >> choice;
        cout << "RESULT\n";
        cout << "=================================" << endl;
        if (choice == 'h') {
            player.recordHit();  //Record hit
            playerHand.push_back(deck.front());
            deck.pop_front();
            updateRemainingCards(playerHand.back());
            
            cout << "PLAYER'S HAND:" << endl;
            for (const auto& card : playerHand) {
                cout << card.rank << " of " << card.suit << endl;
            }
            cout << endl;
            //calc hand value and check it
            int handValue = calculateHandValue(playerHand);
            if (handValue > 21) {
                cout << "Player busts! Your hand value is over 21." << endl;
                player.recordBust();  //Record bust 
                break;
            }
        } else if (choice == 's') {
            player.recordStand();  //Record stand 
        }
    } while (choice == 'h');
}
//Facilitates dealer's turn
void Deck::dealerTurn(list<Card>& dealerHand, int (Deck::*calculateHandValue)(const list<Card>&) const) {
    cout << "DEALER'S HAND:" << endl;
    for (const auto& card : dealerHand) {
        cout << card.rank << " of " << card.suit << endl;
    }
    cout << endl;

    while ((this->*calculateHandValue)(dealerHand) < 17) {
        if (dealerDeck.empty()) {
            cout << "Dealer's deck is empty!" << endl;
            break;
        }

        dealerHand.push_back(dealerDeck.top());
        dealerDeck.pop();
        updateRemainingCards(dealerHand.back());

        cout << "Dealer hits and receives a card." << endl;
        cout << "Dealer's Hand:" << endl;
        for (const auto& card : dealerHand) {
            cout << card.rank << " of " << card.suit << endl;
        }
        cout << endl;

        break;
    }
}
//Checks if string is numeric
bool Deck::isNumeric(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}
//Calculating hand value
int Deck::calculateHandValue(const list<Card>& hand) const {
    int sum = 0;
    int numAces = 0;

    for (const auto& card : hand) {
        if (card.rank == "A") {
            numAces++;
        } else if (card.rank == "J" || card.rank == "Q" || card.rank == "K") {
            sum += 10;
        } else if (isNumeric(card.rank)) {
            sum += stoi(card.rank);
        }
    }
    
    //Consider special case for Aces
    for (int i = 0; i < numAces; ++i) {
        if (sum + 11 <= 21) {
            sum += 11;
        } else {
            sum += 1;
        }
    }

    return sum;
}
//Determine winner (for each round)
void Deck::determineWinner(const list<Card>& playerHand, const list<Card>& dealerHand) const {
    int playerValue = calculateHandValue(playerHand);
    int dealerValue = calculateHandValue(dealerHand);

    if (playerValue > 21) {
        cout << "Player busts! Dealer wins." << endl;
    } else if (dealerValue > 21) {
        cout << "Dealer busts! Player wins." << endl;
    } else {
        if (playerValue > dealerValue) {
            cout << "Player wins!" << endl;
        } else if (playerValue < dealerValue) {
            cout << "Dealer wins!" << endl;
        } else {
            cout << "It's a push! No one wins." << endl;
        }
    }
}
//Update player score for stats
void Deck::updateScore(int& playerScore, const list<Card>& playerHand, const list<Card>& dealerHand) const {
    int playerValue = calculateHandValue(playerHand);
    int dealerValue = calculateHandValue(dealerHand);

    if (playerValue > 21 || (playerValue < dealerValue && dealerValue <= 21)) {
        cout << "Player loses this round." << endl;
        playerScore--;
    } else if ((dealerValue > 21 || playerValue > dealerValue) && playerValue <= 21) {
        cout << "Player wins this round!" << endl;
        playerScore++;
    } else {
        cout << "It's a tie! No change in score." << endl;
    }

    cout << "Player's score: " << playerScore << endl;
}
//Function to compare two cards based on their rank values
bool Deck::compareCards(const Card& card1, const Card& card2) {
    //Determine values for card ranks, considering Ace as 1 and numeric ranks as their values and face cards as 10
    int value1 = card1.rank == "A" ? 1 : (isNumeric(card1.rank) ? stoi(card1.rank) : 10);
    int value2 = card2.rank == "A" ? 1 : (isNumeric(card2.rank) ? stoi(card2.rank) : 10);
    //compare values
    return value1 < value2;
}
//Print Deck
void Deck::printDeck() const {
    for (const auto& card : deck) {
        cout << card.rank << " of " << card.suit << endl;
    }
}
//Welcome message to greet the player
void Deck::displayRules() {
    cout << "=================================" << endl;
    cout << "Rules:" << endl;
    cout << "1. Try to get as close to 21 without going over." << endl;
    cout << "2. Face cards are worth 10. Aces are worth 1 or 11." << endl;
    cout << "3. You start with two cards. The dealer has one card hidden." << endl;
    cout << "4. Type 'h' to hit (get another card)." << endl;
    cout << "5. Type 's' to stand (end your turn)." << endl;
    cout << "Good luck!" << endl << endl;
}
//Goodbye message to thank the player
void Deck::displayGoodbyeMessage() {
    cout << "Thank you for playing Blackjack at Porter's Casino!" << endl;
    cout << "Hope to see you again soon with more money!" << endl;
}
//Function to add card to hash table
void Deck::addCardToHashTable(const Card& card) {
    string key = card.rank + card.suit;
    cardHashTable[key] = card;
}
//Get card from hash table
Card Deck::getCardFromHashTable(const string& cardKey) {
    if (cardHashTable.find(cardKey) != cardHashTable.end()) {
        return cardHashTable[cardKey];
    }
    // Return a default Card object
    Card defaultCard;
    defaultCard.suit = "";
    defaultCard.rank = "";
    defaultCard.value = 0;
    return defaultCard;
}
//GRAPH FUNCTIONS
//Adds edge to graph
void Graph::addEdge(const string& v1, const string& v2) {
    adjList[v1].push_back(v2);
    adjList[v2].push_back(v1);  // Assuming an undirected graph
}
//Function to perform Breadth First Search (BFS) traversal starting from given vertex
void Graph::BFS(const string& start) {
    //Map to track visited vertices
    unordered_map<string, bool> visited;
    //Initialize all vertices as not visited
    for (const auto& pair : adjList) {
        visited[pair.first] = false;
    }
    //Queue for BFS traversal
    queue<string> q;
    //Mark starting vertex as visited
    visited[start] = true;
    q.push(start);
    //BFS traversal loop
    while (!q.empty()) {
        string v = q.front();
        q.pop();

        //Traverse all adjacent vertices of vertex v
        for (const auto& neighbor : adjList[v]) {
            //If neighbor is not visited, mark it as visited
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}

//Utility function for Depth First Search (DFS) traversal
void Graph::DFSUtil(const string& v, unordered_map<string, bool>& visited) {
    //Mark current vertex as visited
    visited[v] = true;
    //Traverse all adjacent vertices of vertex v
    for (const auto& neighbor : adjList[v]) {
        //If neighbor is not visited, recursively call DFSUtil
        if (!visited[neighbor]) {
            DFSUtil(neighbor, visited);
        }
    }
}

//Function to perform Depth First Search (DFS) traversal starting from a given vertex
void Graph::DFS(const string& start) {
    //Map to keep track of visited vertices
    unordered_map<string, bool> visited;
    //Initialize all vertices as not visited
    for (const auto& pair : adjList) {
        visited[pair.first] = false;
    }
    //Call DFSUtil to perform DFS traversal
    DFSUtil(start, visited);
    cout << endl;
}
