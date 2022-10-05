package blackjack;

import java.util.ArrayList;
import java.util.Random;

import deckOfCards.*;

public class BlackjackModel {

	private ArrayList<Card> dealerCards;
	private ArrayList<Card> playerCards;
	private Deck deck;

	// REturns a copy of the dealerCards
	public ArrayList<Card> getDealerCards() {
		ArrayList<Card> dealersCards = new ArrayList<Card>();
		for (int i = 0; i < dealerCards.size(); i++) {
			dealersCards.add(new Card(dealerCards.get(i).getRank(),
					dealerCards.get(i).getSuit()));
		}

		return dealersCards;

	}

	// Returns a copy of the playersCards
	public ArrayList<Card> getPlayerCards() {
		ArrayList<Card> playersCards = new ArrayList<Card>();

		// Iterate through all of the cards and add a copy of that card onto
		// playerCards
		for (int i = 0; i < playerCards.size(); i++) {
			playersCards.add(new Card(playerCards.get(i).getRank(),
					playerCards.get(i).getSuit()));

		}

		return playersCards;

	}

	// Used for a test
	public Deck getDeck() {
		return deck;
	}

	// Set the dealerCards to the ArrayList called cards given in parameter
	public void setDealerCards(ArrayList<Card> cards) {
		ArrayList<Card> dealersCards = new ArrayList<Card>();

		// Iterate through all of the cards and add a copy of that card onto
		// dealerCards
		for (int i = 0; i < cards.size(); i++) {
			dealersCards.add(
					new Card(cards.get(i).getRank(), cards.get(i).getSuit()));
		}

		this.dealerCards = dealersCards;

	}

	// Set the playerCards to the ArrayList called cards given in parameter
	public void setPlayerCards(ArrayList<Card> cards) {
		ArrayList<Card> playersCards = new ArrayList<Card>();

		for (int i = 0; i < cards.size(); i++) {
			playersCards.add(
					new Card(cards.get(i).getRank(), cards.get(i).getSuit()));
		}
		playerCards = playersCards;
	}

	// Create a deck, and shuffle it using a random generator
	public void createAndShuffleDeck(Random random) {
		Deck deck = new Deck();
		deck.shuffle(random);
		this.deck = deck;
	}

	// Two cards are added onto the dealers cards
	public void initialDealerCards() {
		ArrayList<Card> dealerCards = new ArrayList<Card>();
		dealerCards.add(deck.dealOneCard());
		dealerCards.add(deck.dealOneCard());

		this.dealerCards = dealerCards;
	}

	// Two cards are added onto the players cards
	public void initialPlayerCards() {
		ArrayList<Card> playerCards = new ArrayList<Card>();
		playerCards.add(deck.dealOneCard());
		playerCards.add(deck.dealOneCard());

		this.playerCards = playerCards;

	}

	// The playerCards pile adds a card from the deck
	public void playerTakeCard() {
		playerCards.add(deck.dealOneCard());

	}

	// The dealerCards pile adds a card from the deck
	public void dealerTakeCard() {
		dealerCards.add(deck.dealOneCard());
	}

	/*
	 * When given a hand, this method provides all of the possible hand values
	 * that the player or dealer may have, using an ArrayList of integers
	 */
	public static ArrayList<Integer> possibleHandValues(ArrayList<Card> hand) {
		// The ArrayList of possible hand values
		ArrayList<Integer> handvalues = new ArrayList<Integer>();

		// The value of the card in the index of the hand
		int value = 0;

		// Goes through each card in the hand
		for (int i = 0; i < hand.size(); i++) {

			// Add value of every card in the hand
			value += hand.get(i).getRank().getValue();

		}

		// Add the value of the hand onto the ArrayList of possible values
		handvalues.add(value);

		// Goes through each card in the hand
		for (int i = 0; i < hand.size(); i++) {

			// In the case the card is an ace
			if (hand.get(i).getRank().getValue() == 1) {
				// To ensure that it is not a bust
				if (value + 10 < 22) {
					// Add the value if ACE was regarded as 11, not 1
					handvalues.add(value + 10);
					return handvalues;
				}
			}

		}
		return handvalues;
	}

	/*
	 * Returns a static variable from the Hand Assessment class by looking at
	 * the cards and interpret what the cards mean
	 */
	public static HandAssessment assessHand(ArrayList<Card> hand) {

		// There are insufficient cards if the hand has less than two cards
		if (hand.size() < 2) {
			return HandAssessment.INSUFFICIENT_CARDS;

			// If there are two cards and it equals 21, it's a natural blackjack
		} else if (hand.size() == 2 && largestHandValue(hand) == 21) {
			return HandAssessment.NATURAL_BLACKJACK;

			// If the smallest hand has a value of greater than 21, it's a bust
		} else if (smallestHandValue(hand) > 21) {
			return HandAssessment.BUST;

			// If nothing above, return normal
		} else {
			return HandAssessment.NORMAL;
		}
	}

	/*
	 * The gameAssessment will provide game results of either push, natural
	 * blackjack, player lost or player won. The results are due to what the
	 * assesHand method returned
	 */
	public GameResult gameAssessment() {
		
		//If both the dealer and player have a natural blackjack, return push
		if (assessHand(playerCards) == HandAssessment.NATURAL_BLACKJACK
				&& assessHand(
						dealerCards) == HandAssessment.NATURAL_BLACKJACK) {
			return GameResult.PUSH;

		//If both the player has a natural blackjack, return natural blackjack
		} else if (assessHand(
				playerCards) == HandAssessment.NATURAL_BLACKJACK) {
			return GameResult.NATURAL_BLACKJACK;

		//If the playerCards is a bust, the player lost
		} else if (assessHand(playerCards) == HandAssessment.BUST) {
			return GameResult.PLAYER_LOST;

		//If the dealers cards i a bust the player won
		} else if (assessHand(dealerCards) == HandAssessment.BUST) {
			return GameResult.PLAYER_WON;

		//Whoever is closest to twenty one wins, if its tied return push
		} else if (closestToTwentyOne(playerCards) > closestToTwentyOne(
				dealerCards)) {
			return GameResult.PLAYER_WON;

		} else if (closestToTwentyOne(playerCards) < closestToTwentyOne(
				dealerCards)) {
			return GameResult.PLAYER_LOST;

		} else {
			return GameResult.PUSH;
		}

	}

	/*
	 * Determines whether the dealer should take cards. If the dealers cards is
	 * 16 or less, it'll return true. If the largest value is greater than 18,
	 * the dealer must stop taking cards. If there is an ace, but the smallest
	 * hand is still less than 16, the dealer should take the card.
	 */
	public boolean dealerShouldTakeCard() {

		if (largestHandValue(dealerCards) <= 16) {
			return true;

		} else if (largestHandValue(dealerCards) >= 18) {
			return false;

		} else if (possibleHandValues(dealerCards).size() > 1
				&& smallestHandValue(dealerCards) <= 16) {
			return true;

		}

		return false;
	}

	// Made my own methods

	// finding smallest value in the Integer ArrayList
	public static int smallestHandValue(ArrayList<Card> hand) {
		return possibleHandValues(hand).get(0);
	}

	// returns largest value in the Integer ArrayList
	public static int largestHandValue(ArrayList<Card> hand) {
		int largestHandValue = 0;
		for (int i = 0; i < possibleHandValues(hand).size(); i++) {
			if (largestHandValue < possibleHandValues(hand).get(i)) {
				largestHandValue = possibleHandValues(hand).get(i);
			}
		}
		return largestHandValue;
	}

	// Provides the integer number that is closest to twenty one in the hand
	// But not larger than 21
	public static int closestToTwentyOne(ArrayList<Card> hand) {
		int value = 0;

		for (int i = 0; i < possibleHandValues(hand).size(); i++) {
			if (possibleHandValues(hand).get(i) <= 21) {
				value = possibleHandValues(hand).get(i);
			}
		}

		return value;
	}
}
