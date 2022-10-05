package deckOfCards;

/** This class represents a playing card that would be included in 
 * a standard deck of 52 cards.  
 * 
 * @author Fawzi Emad (C)2020, University of Maryland.
 *
 */
public class Card {
	
	private Suit suit;
	private Rank rank;
	
	public Card(Rank rank, Suit suit) {
		this.suit = suit;
		this.rank = rank;
	}
	
	public Rank getRank() {
		return rank;
	}
	
	public Suit getSuit() {
		return suit;
	}
	
	@Override
	public String toString() {
		return rank + " of " + suit;
	}
	
	@Override
	public boolean equals(Object other) {
		if (!(other instanceof Card)) {
			return false;
		}
		Card card = (Card)other;
		return card.suit == suit && card.rank == rank;
	}
} 
