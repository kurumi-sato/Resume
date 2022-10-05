package tests;

import deckOfCards.*;
import blackjack.*;

import java.util.ArrayList;
import java.util.Random;

import static org.junit.Assert.*;
import org.junit.Test;

public class PublicTests {

	@Test
	public void testDeckConstructorAndDealOneCard() {
		Deck deck = new Deck();
		for (int suitCounter = 0; suitCounter < 4; suitCounter++) {
			for (int valueCounter = 0; valueCounter < 13; valueCounter++) {
				Card card = deck.dealOneCard();
				assertEquals(card.getSuit().ordinal(), suitCounter);
				assertEquals(card.getRank().ordinal(), valueCounter);
			}
		}
	}

	/* This test will pass only if an IndexOutOfBoundsException is thrown */
	@Test(expected = IndexOutOfBoundsException.class)
	public void testDeckSize() {
		Deck deck = new Deck();
		for (int i = 0; i < 53; i++) { // one too many -- should throw exception
			deck.dealOneCard();
		}
	}

	@Test
	public void testDeckShuffle() {
		Deck deck = new Deck();
		Random random = new Random(1234);
		deck.shuffle(random);
		assertEquals(new Card(Rank.KING, Suit.CLUBS), deck.dealOneCard());
		assertEquals(new Card(Rank.TEN, Suit.CLUBS), deck.dealOneCard());
		assertEquals(new Card(Rank.JACK, Suit.SPADES), deck.dealOneCard());
		for (int i = 0; i < 20; i++) {
			deck.dealOneCard();
		}
		assertEquals(new Card(Rank.SIX, Suit.CLUBS), deck.dealOneCard());
		assertEquals(new Card(Rank.FIVE, Suit.CLUBS), deck.dealOneCard());
		for (int i = 0; i < 24; i++) {
			deck.dealOneCard();
		}
		assertEquals(new Card(Rank.EIGHT, Suit.CLUBS), deck.dealOneCard());
		assertEquals(new Card(Rank.JACK, Suit.HEARTS), deck.dealOneCard());
		assertEquals(new Card(Rank.JACK, Suit.CLUBS), deck.dealOneCard());
	}

	@Test
	public void testGameBasics() {
		Random random = new Random(3723);
		BlackjackModel game = new BlackjackModel();
		game.createAndShuffleDeck(random);
		game.initialPlayerCards();
		game.initialDealerCards();
		game.playerTakeCard();
		game.dealerTakeCard();
		ArrayList<Card> playerCards = game.getPlayerCards();
		ArrayList<Card> dealerCards = game.getDealerCards();
		assertTrue(
				playerCards.get(0).equals(new Card(Rank.QUEEN, Suit.HEARTS)));
		assertTrue(
				playerCards.get(1).equals(new Card(Rank.SIX, Suit.DIAMONDS)));
		assertTrue(
				playerCards.get(2).equals(new Card(Rank.EIGHT, Suit.HEARTS)));
		assertTrue(dealerCards.get(0).equals(new Card(Rank.THREE, Suit.CLUBS)));
		assertTrue(dealerCards.get(1).equals(new Card(Rank.NINE, Suit.SPADES)));
		assertTrue(dealerCards.get(2).equals(new Card(Rank.FIVE, Suit.CLUBS)));
	}

	// Kurumi's Tests

	/*
	 * @Test public void testGame() { Random random = new Random(3723);
	 * BlackjackModel game = new BlackjackModel();
	 * game.createAndShuffleDeck(random); game.initialPlayerCards();
	 * game.initialDealerCards(); ArrayList<Card> playerCards =
	 * game.getPlayerCards(); ArrayList<Card> dealerCards =
	 * game.getDealerCards(); Deck deck = game.getDeck();
	 * 
	 * System.out.println("initial hand");
	 * System.out.println("number of player cards: " + playerCards.size());
	 * System.out.println("number of dealer cards: " + dealerCards.size());
	 * System.out.println("number of deck cards: " + deck.getDeck().size());
	 * 
	 * game.playerTakeCard(); game.dealerTakeCard();
	 * 
	 * System.out.println(); System.out.println("After a player takes a card");
	 * 
	 * System.out.println("number of player cards: " + playerCards.size());
	 * System.out.println("number of dealer cards: " + dealerCards.size());
	 * System.out.println("number of deck cards: " + deck.getDeck().size());
	 * 
	 * 
	 * }
	 */

	// @Test
	public void testPossibleHandAssessment() {
		Random random = new Random(23);
		BlackjackModel game = new BlackjackModel();
		game.createAndShuffleDeck(random);
		game.initialPlayerCards();
		game.initialDealerCards();
		ArrayList<Card> playerCards = game.getPlayerCards();
		ArrayList<Card> dealerCards = game.getDealerCards();
		Deck deck = game.getDeck();

		System.out.println("initial hand");
		System.out.println("deck index 0: " + deck.getDeck().get(0));
		System.out.println("Hand: [ <" + playerCards.get(0).getRank() + " OF "
				+ playerCards.get(0).getSuit() + ">, <"
				+ playerCards.get(1).getRank() + " OF "
				+ playerCards.get(1).getSuit() + "> ]");

		System.out.println("Hand Value: "
				+ BlackjackModel.possibleHandValues(playerCards));

		game.playerTakeCard();

		System.out.println("take another card");
		System.out.println("Number of Cards: " + game.getPlayerCards().size());

		System.out.println("Hand: [ <" + playerCards.get(0).getRank() + " OF "
				+ game.getPlayerCards().get(0).getSuit() + ">, <"
				+ game.getPlayerCards().get(1).getRank() + " OF "
				+ game.getPlayerCards().get(1).getSuit() + ">, <"
				+ game.getPlayerCards().get(2).getRank() + " OF "
				+ game.getPlayerCards().get(2).getSuit() + "> ]");

		System.out.println("Hand Value: "
				+ BlackjackModel.possibleHandValues(game.getPlayerCards()));

	}

	@Test
	public void testCornerCaseHandAssessment() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.ACE, Suit.CLUBS);
		Card card2 = new Card(Rank.ACE, Suit.HEARTS);
		Card card3 = new Card(Rank.ACE, Suit.SPADES);
		Card card4 = new Card(Rank.ACE, Suit.DIAMONDS);
		Card card5 = new Card(Rank.FIVE, Suit.CLUBS);
		Card card6 = new Card(Rank.EIGHT, Suit.CLUBS);
		Card card7 = new Card(Rank.JACK, Suit.HEARTS);
		Card card8 = new Card(Rank.JACK, Suit.CLUBS);

		playerCards.add(card1);
		playerCards.add(card2);
		playerCards.add(card3);
		playerCards.add(card4);

		System.out.println("initial hand");
		System.out.println("Hand: [ <" + playerCards.get(0).getRank() + " OF "
				+ playerCards.get(0).getSuit() + ">, <"
				+ playerCards.get(1).getRank() + " OF "
				+ playerCards.get(1).getSuit() + ">, <"
				+ playerCards.get(2).getRank() + " OF "
				+ playerCards.get(2).getSuit() + ">, <"
				+ playerCards.get(3).getRank() + " OF "
				+ playerCards.get(3).getSuit() + "> ]");

		System.out.println("Hand Value: "
				+ BlackjackModel.possibleHandValues(playerCards));

	}

	@Test
	public void testEdgeCaseHandAssessment() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.ACE, Suit.CLUBS);
		Card card2 = new Card(Rank.ACE, Suit.HEARTS);
		Card card3 = new Card(Rank.ACE, Suit.SPADES);
		Card card4 = new Card(Rank.ACE, Suit.DIAMONDS);
		Card card5 = new Card(Rank.FOUR, Suit.CLUBS);
		Card card6 = new Card(Rank.EIGHT, Suit.CLUBS);
		Card card7 = new Card(Rank.JACK, Suit.HEARTS);
		Card card8 = new Card(Rank.JACK, Suit.CLUBS);

		playerCards.add(card1);
		playerCards.add(card6);
		playerCards.add(card5);
		playerCards.add(card8);

		System.out.println("initial hand");
		System.out.println("Hand: [ <" + playerCards.get(0).getRank() + " OF "
				+ playerCards.get(0).getSuit() + ">, <"
				+ playerCards.get(1).getRank() + " OF "
				+ playerCards.get(1).getSuit() + ">, <"
				+ playerCards.get(2).getRank() + " OF "
				+ playerCards.get(2).getSuit() + ">, <"
				+ playerCards.get(3).getRank() + " OF "
				+ playerCards.get(3).getSuit() + "> ]");

		System.out.println("Hand Value: "
				+ BlackjackModel.possibleHandValues(playerCards));

	}
	
	@Test
	public void testExtraCaseHandAssessment() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.ACE, Suit.CLUBS);
		Card card2 = new Card(Rank.SIX, Suit.HEARTS);
		Card card3 = new Card(Rank.FIVE, Suit.SPADES);
		Card card4 = new Card(Rank.ACE, Suit.DIAMONDS);

		playerCards.add(card1);
		playerCards.add(card2);
		playerCards.add(card3);


		System.out.println("EXTRA CASE");
		System.out.println("Hand: [ <" + playerCards.get(0).getRank() + " OF "
				+ playerCards.get(0).getSuit() + ">, <"
				+ playerCards.get(1).getRank() + " OF "
				+ playerCards.get(1).getSuit() + ">, <"
				+ playerCards.get(2).getRank() + " OF "
				+ playerCards.get(2).getSuit() + "> ]");

		System.out.println("Hand Value: "
				+ BlackjackModel.possibleHandValues(playerCards));

	}
	
	@Test
	public void testGameAssessmentWin() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.KING, Suit.CLUBS);
		Card card2 = new Card(Rank.TEN, Suit.CLUBS);
		Card card3 = new Card(Rank.JACK, Suit.SPADES);
		Card card4 = new Card(Rank.SIX, Suit.CLUBS);
		Card card5 = new Card(Rank.FIVE, Suit.CLUBS);
		Card card6 = new Card(Rank.EIGHT, Suit.CLUBS);
		Card card7 = new Card(Rank.JACK, Suit.HEARTS);
		Card card8 = new Card(Rank.JACK, Suit.CLUBS);

		playerCards.add(card1);
		playerCards.add(card2);

		dealerCards.add(card4);
		dealerCards.add(card8);

		BlackjackModel game = new BlackjackModel();
		game.setDealerCards(dealerCards);
		game.setPlayerCards(playerCards);
		assertTrue(game.gameAssessment() == GameResult.PLAYER_WON);
	}
	
	@Test
	public void testGameAssessmentLose() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.KING, Suit.CLUBS);
		Card card2 = new Card(Rank.TEN, Suit.CLUBS);
		Card card3 = new Card(Rank.JACK, Suit.SPADES);
		Card card4 = new Card(Rank.SIX, Suit.CLUBS);
		Card card5 = new Card(Rank.FIVE, Suit.CLUBS);
		Card card6 = new Card(Rank.EIGHT, Suit.CLUBS);
		Card card7 = new Card(Rank.JACK, Suit.HEARTS);
		Card card8 = new Card(Rank.JACK, Suit.CLUBS);

		playerCards.add(card1);
		playerCards.add(card2);

		dealerCards.add(card5);
		dealerCards.add(card4);
		dealerCards.add(card8);

		BlackjackModel game = new BlackjackModel();
		game.setDealerCards(dealerCards);
		game.setPlayerCards(playerCards);
		assertTrue(game.gameAssessment() == GameResult.PLAYER_LOST);
	}
	
	@Test
	public void testGameAssessmentNatural() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.KING, Suit.CLUBS);
		Card card2 = new Card(Rank.TEN, Suit.CLUBS);
		Card card3 = new Card(Rank.JACK, Suit.SPADES);
		Card card4 = new Card(Rank.SIX, Suit.CLUBS);
		Card card5 = new Card(Rank.FIVE, Suit.CLUBS);
		Card card6 = new Card(Rank.EIGHT, Suit.CLUBS);
		Card card7 = new Card(Rank.ACE, Suit.HEARTS);
		Card card8 = new Card(Rank.JACK, Suit.CLUBS);

		playerCards.add(card7);
		playerCards.add(card8);

		dealerCards.add(card1);
		dealerCards.add(card2);

		BlackjackModel game = new BlackjackModel();
		game.setDealerCards(dealerCards);
		game.setPlayerCards(playerCards);
		assertTrue(game.gameAssessment() == GameResult.NATURAL_BLACKJACK);
	}
	
	@Test
	public void testGameAssessmentPush() {
		ArrayList<Card> playerCards = new ArrayList<>();
		ArrayList<Card> dealerCards = new ArrayList<>();

		Card card1 = new Card(Rank.KING, Suit.CLUBS);
		Card card2 = new Card(Rank.TEN, Suit.CLUBS);
		Card card3 = new Card(Rank.JACK, Suit.SPADES);
		Card card4 = new Card(Rank.ACE, Suit.CLUBS);
		Card card5 = new Card(Rank.FIVE, Suit.CLUBS);
		Card card6 = new Card(Rank.EIGHT, Suit.CLUBS);
		Card card7 = new Card(Rank.ACE, Suit.HEARTS);
		Card card8 = new Card(Rank.JACK, Suit.CLUBS);

		playerCards.add(card1);
		playerCards.add(card2);
		playerCards.add(card4);

		dealerCards.add(card7);
		dealerCards.add(card8);

		BlackjackModel game = new BlackjackModel();
		game.setDealerCards(dealerCards);
		game.setPlayerCards(playerCards);
		assertTrue(game.gameAssessment() == GameResult.PUSH);
		
	}

}
