package GUI;
import blackjack.*;
import deckOfCards.*;

import java.awt.Graphics;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.WindowConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.JFrame;

public class BlackjackGUI extends JPanel {

	public static void main(String[] args) {
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				Thread.currentThread().setPriority(Thread.MAX_PRIORITY);		
				new BlackjackGUI();
			}
		});	
	}

	private int[] chips = new int[5];
	private int[] originalChips;
	private int[] bet = new int[5];

	private enum Mode {GAME_OVER, BETTING, PLAYER_TURN, DEALER_TURN};
	private Mode gameMode = Mode.BETTING;

	private static final int STARTING_CHIPS = 5000;
	private static final long serialVersionUID = 0L;
	private static final int TABLE_HEIGHT = 420;
	private static final int TABLE_WIDTH = 950;
	private static final int CARD_WIDTH = 71;
	private static final int CARD_HEIGHT = 96;
	private static final int CARD_PADDING = 10;
	private static final int BUTTON_HEIGHT = 35;
	private static final int BUTTON_WIDTH = 95;
	private static final int DEALER_CARD_V_OFFSET = 40;
	private static final int PLAYER_CARD_V_OFFSET = 260;
	private static final int LEFT_MARGIN = 300;
	private static final int CHIP_MARGIN = 20;
	private static final int BET_MARGIN = 60;

	private static final int TIMER_DELAY = 1200;

	private JButton nextHandButton = new JButton("Place Bet");
	private JButton hitButton = new JButton("Hit");
	private JButton stayButton = new JButton("Stay");
	private JButton dealButton = new JButton ("Deal");
	private JSlider slider = new JSlider();

	private BlackjackModel game;

	private static final Color GREEN_FELT_COLOR = new Color(20, 160, 20);
	private static final Color RED_COLOR = Color.ORANGE;
	private static final Color WIN_COLOR = new Color(100, 255, 100);
	private static final Color HAPPY_COLOR = Color.WHITE;

	private static final Font NORMAL_FONT = new Font("Arial", Font.BOLD, 20);
	private static final Font LARGE_FONT = new Font("Arial", Font.BOLD, 30);
	private static final Font SMALL_FONT = new Font("Courier", Font.BOLD, 12);

	private static int[] chipDenom = {500, 100, 50, 10, 5}; // each must be a factor of predecessor
	private static String[] chipColor = {"pink", "black", "green", "blue", "red"};

	private Color textColor;

	public BlackjackGUI() {
		greedyChipAllocation(STARTING_CHIPS);
		game = new BlackjackModel();
		setupWidgets();
		attachListeners();
		setupPanel();
		setupFrame();
	}

	private void setupWidgets() {
		nextHandButton.setBounds(LEFT_MARGIN + 20, PLAYER_CARD_V_OFFSET + CARD_HEIGHT + 15, BUTTON_WIDTH, BUTTON_HEIGHT);
		hitButton.setBounds(LEFT_MARGIN + 20, PLAYER_CARD_V_OFFSET + CARD_HEIGHT + 15, BUTTON_WIDTH, BUTTON_HEIGHT);
		stayButton.setBounds(LEFT_MARGIN + BUTTON_WIDTH + 30, PLAYER_CARD_V_OFFSET + CARD_HEIGHT + 15, BUTTON_WIDTH, BUTTON_HEIGHT);
		dealButton.setBounds(LEFT_MARGIN + 20, PLAYER_CARD_V_OFFSET + CARD_HEIGHT + 15, BUTTON_WIDTH, BUTTON_HEIGHT);
		nextHandButton.setVisible(false);
		hitButton.setVisible(false);
		stayButton.setVisible(false);
		dealButton.setVisible(true);
		int chipTotal = getTotalValue(chips);
		slider.setVisible(true);
		slider.setBounds(BET_MARGIN + 5, 210, 500, 20);
		slider.setBackground(GREEN_FELT_COLOR);
		slider.setMinimum(0);
		slider.setMaximum(chipTotal);
		slider.setValue(getBestSliderValue(chipTotal));
		slider.setFont(SMALL_FONT);
		originalChips = Arrays.copyOf(chips, 5);
		removeBetFromChips();
	}

	private void setupPanel() {
		setLayout(null);
		add(nextHandButton);
		add(hitButton);
		add(stayButton);
		add(dealButton);
		add(slider);
		setBackground(GREEN_FELT_COLOR);
		setSize(TABLE_WIDTH, TABLE_HEIGHT);
		setPreferredSize(new Dimension(TABLE_WIDTH, TABLE_HEIGHT));
		setMinimumSize(new Dimension(TABLE_WIDTH, TABLE_HEIGHT));
		setBounds(new java.awt.Rectangle(TABLE_WIDTH, TABLE_HEIGHT));
	}

	private void setupFrame() {
		JFrame frame = new JFrame("CMSC132  BlackJack Game");
		frame.setResizable(false);
		frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		frame.setContentPane(this);
		frame.pack();
		frame.setVisible(true);
	}

	private void attachListeners() {
		nextHandButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				gameMode = Mode.BETTING;
				originalChips = new int[chips.length];
				for (int i = 0; i < chips.length; i++) {
					originalChips[i] = chips[i];
				}
				nextHandButton.setVisible(false);
				dealButton.setVisible(true);
				slider.setVisible(true);
				calibrateSlider();
				removeBetFromChips();
				repaint();
			}		
		});

		hitButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				game.playerTakeCard();
				ArrayList<Card> hand = game.getPlayerCards();
				HandAssessment status = BlackjackModel.assessHand(hand);
				repaint();
				if (status == HandAssessment.BUST) {
					gameMode = Mode.DEALER_TURN;
					doDealerTurn();
				}
			}
		});

		stayButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				gameMode = Mode.DEALER_TURN;
				doDealerTurn();
			}
		});

		dealButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				gameMode = Mode.PLAYER_TURN;
				hitButton.setVisible(true);
				stayButton.setVisible(true);
				dealButton.setVisible(false);
				slider.setVisible(false);
				game.createAndShuffleDeck(new Random());
				game.initialDealerCards();
				game.initialPlayerCards();
				repaint();
			}
		});

		slider.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				removeBetFromChips();
				repaint();
			}
		});
	}

	private int getBestSliderValue(int chipTotal) {
		if (chipTotal == 0) {
			return 0;
		}
		if (chipTotal <= 50) {
			return 5;
		}
		if (chipTotal < 100) {
			return 10;
		}
		if (chipTotal < 1000) {
			return (chipTotal / 100) * 10;
		}
		return (chipTotal / 1000) * 100;
	}

	
	private void doDealerTurn() {
		hitButton.setVisible(false);
		stayButton.setVisible(false);
		repaint();
		new Timer().schedule(new DealerTask(), TIMER_DELAY);
	}

	private class DealerTask extends TimerTask {
		@Override
		public void run() {
			if (game.dealerShouldTakeCard()) {
				game.dealerTakeCard();
				repaint();
				new Timer().schedule(new DealerTask(), TIMER_DELAY);
			} else {
				gameMode = Mode.GAME_OVER;
				nextHandButton.setVisible(true);
				if (game.getDealerCards() != null && game.getPlayerCards() != null) {
					GameResult result = game.gameAssessment();
					if (result == GameResult.NATURAL_BLACKJACK) {
						greedyChipAllocation((int)(getTotalValue(chips) + getTotalValue(bet) * 2.5));
					} else if (result == GameResult.PLAYER_WON) {
						for (int i = 0; i < 5; i++) {
							chips[i] += 2 * bet[i];
						}
						reallocateIfNecessary();
					} else if (result == GameResult.PUSH) {
						for (int i = 0; i < 5; i ++) {
							chips[i] += bet[i];
						}
						reallocateIfNecessary();
					} else {  // lost money
						reallocateIfNecessary();
					}
				}
				repaint();
			}
		}
	}

	private void reallocateIfNecessary() {
		for (int i = 1; i < chips.length; i++) {
			if (chips[i - 1] == 0) {  // ignore leading stack
				continue;
			}
			int minNeededForChange = chipDenom[i - 1] / chipDenom[i];
			if (chips[i] > 18 || chips[i] < minNeededForChange) {
				greedyChipAllocation(getTotalValue(chips));
				return;
			}
		}
	}

	private void calibrateSlider() {
		int chipTotal = getTotalValue(chips);
		slider.setMinimum(0);
		slider.setMaximum(chipTotal);
		slider.setValue(getBestSliderValue(chipTotal));
	}

	public void paint(Graphics g) {
		super.paint(g);
		g.setFont(NORMAL_FONT);
		textColor = Color.BLACK;
		g.setColor(new Color(0, 110, 0));
		g.fillOval(BET_MARGIN, 12, 188, 188);
		g.setColor(Color.BLACK);
		if (game != null) {
			if (gameMode != Mode.BETTING) {
				ArrayList<Card> dealerHand = game.getDealerCards();
				ArrayList<Card> playerHand = game.getPlayerCards();
				if (dealerHand != null && dealerHand.size() > 0) {
					String toDraw = "Dealer ";
					if (gameMode == Mode.DEALER_TURN || gameMode == Mode.GAME_OVER) {
						toDraw += getValuesString(dealerHand);
					}
					g.setColor(textColor);
					g.drawString(toDraw, LEFT_MARGIN + 65, DEALER_CARD_V_OFFSET - 10);
					boolean downCard = false;
					if (gameMode == Mode.PLAYER_TURN) {
						downCard =  true;
					}
					drawCards(g, dealerHand, downCard, DEALER_CARD_V_OFFSET);
				}

				if (playerHand != null && playerHand.size() > 0) {
					String toDraw = getValuesString(playerHand);
					g.setColor(textColor);
					g.drawString(toDraw, LEFT_MARGIN + 70, PLAYER_CARD_V_OFFSET - 10);
					drawCards(g, playerHand, false, PLAYER_CARD_V_OFFSET);
				}
			}
			if (gameMode == Mode.GAME_OVER) {
				if (game.getDealerCards() != null && game.getPlayerCards() != null) {
					GameResult result = game.gameAssessment();
					g.setFont(LARGE_FONT);
					String resultString = getGameResultString(result);
					g.setColor(textColor);
					g.drawString(resultString, BET_MARGIN + 20, 120);
					g.setFont(NORMAL_FONT);
					g.setColor(Color.BLACK);
				}
			} else {
				drawBetChips(g);
				g.setFont(NORMAL_FONT);
				g.setColor(Color.BLACK);
				g.drawString("Bet: $" + getSliderValue(), BET_MARGIN + 50, 155);
			}
			drawChips(g);
		}
	}

	private int getSliderValue() {
		int total = getTotalValue(chips) + getTotalValue(bet);
		int factor;
		if (total > 49000) {
			factor = 1000;
		}
		if (total > 4900) {
			factor = 100;
		} else if (total > 490){
			factor = 10;
		} else {
			factor = 5;
		}
		return (slider.getValue() / factor) * factor;
	}
	
	private void drawBetChips(Graphics g) {
		for (int i = 0; i < chipDenom.length; i++) {
			drawChipStack(g, chipColor[i] + "ChipSmall.png", BET_MARGIN + 5 + i * 35, 100, 4, bet[i]);
		}
	}

	private void greedyChipAllocation(int toGo) {
		for (int i = 0; i < chips.length; i++) {
			chips[i] = 0;
		}
		for (int i = 0; i < chipDenom.length; i++) {
			while (toGo >= chipDenom[i]) {
				chips[i]++;
				toGo -= chipDenom[i];
			}
		}
		for (int i = 1; i < chips.length; i++) {
			int needed = (13 - 2 * i) - chips[i];
			while (needed > 0) {
				if (chips[i - 1] > 0) {
					chips[i - 1]--;
					int numberToAdd = chipDenom[i - 1] / chipDenom[i];
					chips[i] += numberToAdd;
					needed -= numberToAdd;
				} else {
					break;
				}
			}
		}
	}

	private void removeBetFromChips() {
		bet = new int[5];
		chips = Arrays.copyOf(originalChips, 5);
		int toGo = getSliderValue();
		for (int i = 0; i < chips.length; i++) {
			while (chipDenom[i] <= toGo && chips[i] > 0) {
				bet[i]++;
				chips[i]--;
				if (chips[i] < 0) {
					throw new RuntimeException("chip stack got negative");
				}
				toGo -= chipDenom[i];
			}
		}
	}
	
	private void drawChips(Graphics g) {
		g.setFont(SMALL_FONT);
		for (int i = 0; i < chipDenom.length; i++) {
			drawChipStack(g, chipColor[i] + "Chip.png", CHIP_MARGIN + i * 50, TABLE_HEIGHT - 110, 6, chips[i]);
			String padding = "";
			if (chipDenom[i] < 100) {
				padding = " ";
			} 
			g.drawString(padding + "$" + chipDenom[i], CHIP_MARGIN + i * 50 + 10, TABLE_HEIGHT - 55);
		}
		g.setFont(LARGE_FONT);
		g.drawString("$" + getTotalValue(chips), CHIP_MARGIN + 95, TABLE_HEIGHT - 20);
		g.setFont(NORMAL_FONT);
	}

	private String getGameResultString(GameResult result) {
		int totalBet = getTotalValue(bet);
		if (result == GameResult.NATURAL_BLACKJACK) {
			textColor = HAPPY_COLOR;
			return "Won $" + (int)(totalBet * 1.5);
		}
		if (result == GameResult.PLAYER_LOST) {
			textColor = RED_COLOR;
			return "Lost $" + totalBet;
		}
		if (result == GameResult.PLAYER_WON) {
			textColor = WIN_COLOR;
			return "Won $" + totalBet;
		}
		return "    Push";
	}

	private String getValuesString(ArrayList<Card> hand) {
		textColor = Color.BLACK;
		ArrayList<Integer> values = BlackjackModel.possibleHandValues(hand);
		String toDraw = " (" + values.get(0);
		if (values.size() == 2) {
			toDraw += "/" + values.get(1);
		}
		toDraw += ")";
		HandAssessment status = BlackjackModel.assessHand(hand);
		if (status == HandAssessment.NATURAL_BLACKJACK) {
			toDraw += "  BLACKJACK!";
			textColor = HAPPY_COLOR;
		} else if (status == HandAssessment.BUST) {
			toDraw += "  [BUST]";
			textColor = RED_COLOR;
		}
		return toDraw;
	}

	private void drawCards(Graphics g, ArrayList<Card> hand, boolean lastCardDown, int height) {
		int xPos = LEFT_MARGIN + 20;
		for (int i = 0; i < hand.size(); i++) {
			Card c = hand.get(i);
			if (lastCardDown && i == hand.size() - 1) {
				c = null;
			}
			drawCard(g, c, xPos, height);
			xPos += CARD_WIDTH + CARD_PADDING;
		}
	}

	public Dimension getPreferredSize() {
		return new Dimension(TABLE_WIDTH, TABLE_HEIGHT);
	}

	private void drawCard(Graphics g, Card c, int x, int y) {
		String cardName;
		if (c == null) {
			cardName = "images/b2fv.gif";  // face-down
		} else {
			cardName = getImageFileName(c);
		}
		g.drawImage(ImageLoader.getImage(cardName), x, y, this);
	}

	private static String[] suitPrefix = {"s", "d", "c", "h"};

	private static String getImageFileName(Card c) {
		String retValue;
		retValue = suitPrefix[c.getSuit().ordinal()];
		int value = c.getRank().ordinal();
		if (c.getRank().ordinal() <= 9) {
			retValue += c.getRank().ordinal() + 1;
		} else if (value == 10) {
			retValue += "j";
		} else if (value == 11) {
			retValue += "q";
		} else if (value == 12) {
			retValue += "k";
		} else { 
			retValue += "Unknown!";
		}
		return "images/" + retValue + ".gif";	
	}

	private void drawChipStack(Graphics g, String image, int x, int y, int separation, int count) {
		BufferedImage chip = ImageLoader.getImage("images/" + image);
		for (int i = 0; i < count; i++) {
			g.drawImage(chip, x, y - separation * i, this);
		}
	}

	private static int getTotalValue(int[] a) {
		int total = 0;
		for (int i = 0; i < a.length; i++) {
			total += a[i] * chipDenom[i];
		}
		return total;
	}

} 