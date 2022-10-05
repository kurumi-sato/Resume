package deckOfCards;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class Deck {

	//ArrayList of Cards named cards
	private ArrayList<Card> cards;
	
	public Deck() {
		
		//Creating the ArrayList of 52 Card
		cards = new ArrayList<Card>(52);
		
		for (Suit suites : Suit.values()) {
			for (Rank ranks : Rank.values()) {
				cards.add(new Card(ranks, suites));
			}
		}

	}
	
	public void shuffle(Random randomNumberGenerator) {
		Collections.shuffle(cards, randomNumberGenerator);;
	}
	
	
	public Card dealOneCard() {
		return cards.remove(0);
	}
	
	public ArrayList<Card> getDeck() {
		return cards;
	}
	
}
