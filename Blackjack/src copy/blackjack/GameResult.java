package blackjack;

/** These constants characterize the state of the 
 *  blackjack game at the end.  
 *  They are used by the GUI to determine
 *  how much money to give or take away from the player.
 * 
 * @author Fawzi Emad (C)2020, University of Maryland
 */
public enum GameResult {
	PLAYER_WON, PLAYER_LOST, PUSH, NATURAL_BLACKJACK;
}
