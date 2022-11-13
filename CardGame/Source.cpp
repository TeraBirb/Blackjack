#include <iostream>
#include <string>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <array>
#include <fstream>

#define RESET   "\033[0m"
#define RED		"\033[1m\033[31m"
#define BLACK	"\033[1m\033[30m"

using namespace std;
using Suit = string;

class Card {
private:
	Suit suit;
	string val;
public:
	void setSuit(Suit s) {
		suit = s;
	}
	Suit getSuit() {
		return suit;
	}
	void setVal(string v) {
		val = v;
	}
	string getVal() {
		return val;
	}
	Card() {
	}
	Card(string v, Suit s) {
		val = v;
		suit = s;
	}
};

class Deck {
private:
	string val;
	Suit suit;
	
	int actVal;
	Card deckArray[53];
	int aceCount = 0;
public:
	void incrAceCount(int n) { 
		if (n == 1 || n == -1) {
			aceCount += n;
		}
		else {
			cout << "ERROR: func incrAceCount parameter not 1 or -1" << endl;	// Exception handling
		}
	}

	int getAceCount() {
		return aceCount;
	}

	Deck() {
		int count = 0;
		for (int i = 1; i <= 4; i++) {
			for (int j = 1; j <= 13; j++) {
				if (i == 1) {				// Set the suit
					suit = BLACK u8"\u2660" RESET;	// spade
				}
				else if (i == 2) {
					suit = BLACK u8"\u2663" RESET;	// club
				}
				else if (i == 3) {
					suit = RED u8"\u2665" RESET;	// heart
				}
				else if (i == 4) {
					suit = RED u8"\u2666" RESET;	// diamond
				}
				if (j == 1) {				// Set the value
					val = "Ace";
				}
				else if (j == 11) {
					val = "Jack";
				}
				else if (j == 12) {
					val = "Queen";
				}
				else if (j == 13) {
					val = "King";
				}
				else {
					val = to_string(j);
				}
				actVal = j;

				deckArray[count] = Card(val, suit);
				count++;
			}
		}
	}
	void printDeck() {	// DEBUG FUNC
		for (int i = 0; i < 52; i++) {
			cout << "#" << i + 1 << " " << deckArray[i].getVal() << " " << hex << deckArray[i].getSuit() << dec << endl;
		}
	}
	string drawCard() {
		string suitVal;
		Card card;
		card = deckArray[rand() % 51];
		// cout << "DEBUG |||| " << deckArray[51].getVal() << endl;
		suitVal = card.getVal() +" of " + card.getSuit();
		return suitVal;
	}
};

void play(void (*func)(Deck, int&), Deck deck, int& total, double& bankroll, double& bet) {
	int selection;
	cout << "Play starts!" << endl;
	// system("pause");

	do {
		cout << "Enter in your wager: $";
		cin >> bet;
		if (bet > bankroll) {
			cout << "The wager you entered is higher than your bankroll of " << bankroll << ". Try again." << endl;
		}
		if (bet < 0) {
			cout << "The wager you entered is less than zero. Try again." << endl;
		}

	} while (bet > bankroll || bet < 0);

	if (bet == bankroll) {
		cout << "ALL IN! Is it just me or is it getting steamy in here..." << endl;
	}
	else if (bet == 0) {
		cout << "Practice round! Zero stakes in this one." << endl;
	}

	func(deck, total);		// Draw 2 cards at the start
	Sleep(500);
	func(deck, total);
	Sleep(500);
	cout << "Hand total: " << total << endl;

	while (total <= 21) {
		if (total == 21) {
			cout << "BLACKJACK. You win!" << endl;
			bankroll += bet;
			cout << "You won $" << bet << ". Your bankroll is now at $" << bankroll << "." << endl << endl;
			break;
		}
		cout << endl <<
			"1) Hit" << endl
			<< "2) Stand" << endl
			<< "Select: ";

		cin >> selection;
		cout << endl;

		if (selection == 1) {
			func(deck, total);
			cout << "Your hand total is " << total << endl;
		}
		else if (selection == 2) {
			cout << "Standing. Your hand's value is " << total << "." << endl << endl;
			break;
		}
	}

	if (total > 21) {
		cout << "You went over 21! You lose." << endl;
		bankroll -= bet;
		cout << "You lost $" << bet << ". Your bankroll is now at $" << bankroll << "." << endl << endl;
	}
}

void housePlay(void (*func)(Deck, int&), Deck deck, int& total, double& bankroll, double& bet) {
	cout << "House plays." << endl;
	func(deck, total);
	Sleep(650);
	func(deck, total);
	Sleep(650);

	while (total <= 17) {
		func(deck, total);
		Sleep(650);
	}

	cout << "The house's hand is " << total << endl;

	if (total == 21) {
		cout << "BLACKJACK! You lose..." << endl;
		bankroll -= bet;
		cout << "You lost $" << bet << ". Your bankroll is now at $" << bankroll << "." << endl << endl;
	}
	else if (total > 21) {
		cout << "The house went over 21. You win!" << endl;
		bankroll += bet;
		cout << "You won $" << bet << ". Your bankroll is now at $" << bankroll << "." << endl << endl;
	}

}

void hit(Deck deck, int& total) {
	string currentHand = deck.drawCard();
	cout << currentHand << endl;

	if (currentHand[0] == 'J' || currentHand[0] == 'Q' || currentHand[0] == 'K' || currentHand[0] == 'A' || currentHand[0] == '1') {
		if (currentHand[0] == 'A') {
			if ((total + 11) > 21) {
				total += 1;
			}
			else {
				total += 11;
				deck.incrAceCount(1);
			}
		}
		else {
			total += 10;
		}
	}
	else {
		total += (int)(currentHand[0] - 48);	// 48 offset ascii
	}

	if (total > 21 && deck.getAceCount() > 0) {	// handles aces in hand
		total -= 10;
		deck.incrAceCount(-1);
	}
	//cout << "Total: " << total << endl;

}

void results(int& total, int& houseTotal, double& bankroll, double& bet) {
	if (total < 21 && houseTotal < 21) {
		if (total > houseTotal) {
			cout << "Your " << total << " is GREATER than the house's " << houseTotal << ". YOU WIN!" << endl;
			bankroll += bet;
			cout << "You won $" << bet << ". Your bankroll is now at $" << bankroll << "." << endl << endl;
		}
		else if (total < houseTotal) {
			cout << "Your " << total << " is less than the house's " << houseTotal << "... you lose." << endl;
			bankroll -= bet;
			cout << "You lost $" << bet << ". Your bankroll is now at $" << bankroll << "." << endl << endl;
		}
		else if (total == houseTotal) {
			cout << "DRAW! " << total << " to " << houseTotal << "." << endl;
			cout << "Your bankroll of $" << bankroll << " stays the same." << endl << endl;
		}
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8); // Set console code page to UTF-8 so console known how to interpret string data
	setvbuf(stdout, nullptr, _IOFBF, 1000); // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	srand(time(NULL));	// Seed random

	Deck playerDeck, houseDeck;
	vector <string> handCards;
	int handTotal = 0;
	int houseHandTotal = 0;
	string userName;
	char keepPlaying = 'Y';

	double bankroll = 1000;
	double bet = 0;

	cout << "Welcome to Blackjack!" << endl <<
		"Please enter your name: ";
	cin >> userName;
	cout << "Thank you, " << userName << ", your bankroll is at $" << bankroll << ", enjoy!" << endl << endl;

	while (bankroll > 0 && toupper(keepPlaying) == 'Y') {

		play(&hit, playerDeck, handTotal, bankroll, bet);		// PLAY!!

		if (handTotal < 21) {
			housePlay(&hit, houseDeck, houseHandTotal, bankroll, bet);
		}

		results(handTotal, houseHandTotal, bankroll, bet);

		if (bankroll > 0) {
			cout << "Would you like to keep playing? Y or N: ";
			cin >> keepPlaying;
			while (toupper(keepPlaying) != 'Y' && toupper(keepPlaying) != 'N') {
				cout << "Invalid selection. Try again. Y, N, or S: ";		// exception handling
				cin >> keepPlaying;
			}
		}

		houseHandTotal = 0;
		handTotal = 0;

	}

	if (bankroll == 0) {
		cout << "GAME OVER! You lost all your money, " << userName << ". Better luck next time!" << endl;
	}

}
