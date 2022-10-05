package deckOfCards;

/**
 * These are the "ranks" of cards taken from a standard deck of 52
 * cards.
 * 
 * @author Fawzi Emad (C)2020, University of Maryland
 */
public enum Rank {

	ACE(1), TWO(2), THREE(3), FOUR(4), FIVE(5), SIX(6), SEVEN(7), EIGHT(8),
	NINE(9), TEN(10), JACK(10), QUEEN(10), KING(10);
	
	private int value;  // Value of the card in a game like Blackjack
	
	/**
	 * Standard constructor.
	 * @param value the value of the card in a game like Blackjack.
	 * Aces are assigned the value 1.  Face cards (Jack, Queen, King)
	 * are assigned the value 10.  All other cards have a value equal
	 * to their rank.
	 */
	private Rank(int value) {
		this.value = value;
	}
	
	/**
	 * Getter for this card's value.
	 * @return the value of the card.  (Aces return 1, Face cards return
	 * 10, all other cards return a number equal to their rank.)
	 */
	public int getValue() {
		return value;
	}
}
