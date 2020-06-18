#include "Game.h"
#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <string>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;



Game::Game(int width, int height)
 : m_well(width, height), m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1), m_score(0), m_rowsLeft(5)
{
}

void Game::play()
{
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]				s

    for(;;)
    {
        if (!playOneLevel()) 
            break;
        displayPrompt("Good job!  Press the Enter key to start next level!");
        waitForEnter();     
		m_well.emptyWell();
        m_level++;
		m_rowsLeft = 5 * m_level;
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}



void Game::displayPrompt(std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

void Game::displayStatus(PieceType nextPiece)
{
	// If there is a next piece to display, display it
	if (nextPiece != NONE)
	{
		m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);
		m_screen.printString("Next piece:");
		printNextPiece(nextPiece);
	}

	// Display score
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printStringClearLine("Score:     ");
	printRightJustified(m_score, SCORE_X + 11, SCORE_Y);

	// Display rows left
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printStringClearLine("Rows left: ");	
	printRightJustified(m_rowsLeft, ROWS_LEFT_X + 11, ROWS_LEFT_Y);	

	// Display level
	m_screen.gotoXY(LEVEL_X, LEVEL_Y);
	m_screen.printStringClearLine("Level:     ");
	printRightJustified(m_level, LEVEL_X + 11, LEVEL_Y);
}


bool Game::playOneLevel()
{
	//	Return false if game over or quit
	bool playerQuit(false), firstTimeThrough(true), wellFull(false), levelComplete(false);
	PieceType current(chooseRandomPieceType()), next;

	// While Game is not over
	while (m_rowsLeft > 0 && !playerQuit && !wellFull)
	{
		// Randomly select current piece and next piece
		if (!firstTimeThrough)
		{
			current = next;
		}
		firstTimeThrough = false;
		next = chooseRandomPieceType();

		// Display stats
		displayStatus(next);

		// Add a piece and allow moves if it was successfully added
		if (m_well.addPiece(current))
		{
			// Play the current piece until it comes to rest
			if (!pieceInPlay(current))
			{
				playerQuit = true;
				break;
			}

			// Check if level is complete
			if (m_rowsLeft <= 0)
			{
				levelComplete = true;
				break;
			}
		}
		else // Well is full and Game is over
		{
			// Display the last added piece
			m_well.display(m_screen, WELL_X, WELL_Y);
			wellFull = true;
			break;
		}
	}
    return levelComplete; 
}

void Game::updateStats(int rowsVaporized)
{
	switch (rowsVaporized)
	{
	case 1:
		m_rowsLeft--;
		m_score += 100;
		break;
	case 2:
		m_rowsLeft -= 2;
		m_score += 200;
		break;
	case 3:
		m_rowsLeft -= 3;
		m_score += 400;
		break;
	case 4:
		m_rowsLeft -= 4;
		m_score += 800;
		break;
	case 5:
	default:
		m_rowsLeft -= 5;
		m_score += 1600;
		break;
	}
	// m_rowsLeft cannot be negative
	if (m_rowsLeft < 0)
		m_rowsLeft = 0;
}

void Game::printNextPiece(PieceType nextPiece)
{
	Piece next(nextPiece);
	for (int i = 0; i < BOX_LEN; i++)
	{
		for (int j = 0; j < BOX_LEN; j++)
		{
			m_screen.gotoXY(NEXT_PIECE_X + j, NEXT_PIECE_Y + i);
			m_screen.printChar(next.getBox(i, j));
		}
	}
}

void Game::printRightJustified(int stat, int x, int y)
{
	string status = to_string(stat);
	const int length = status.length();
	m_screen.gotoXY(x, y);
	
	// Print status right justified in a field 7 characters wide
	for (int i = 0; i < 7 - length; i++)
	{
		m_screen.printChar(' ');
		m_screen.gotoXY(x + i, y);
	}
	m_screen.printString(status);
}

bool Game::pieceInPlay(PieceType current)
{
	int rowsRemoved;
	bool playerDidntQuit(true);
	m_well.display(m_screen, WELL_X, WELL_Y);
	do
	{
		// Let the player do moves for 2000 milliseconds
		Timer timer;
		SpecialControls type(NOTHING);
		while (timer.elapsed() <= max(1000 - (100 * (m_level - 1)), 100))
		{
			// Check for moves and update screen if moves are successful
			char move('\0');

			if (getCharIfAny(move))		
			{
				type = controls(move, current);
				if (type == NORMAL)
					m_well.display(m_screen, WELL_X, WELL_Y);
				else if (type == DOWN || type == SPACEBAR || type == QUIT)
					break;
			}

		}
		// Deal with any special controls the user typed
		if (type == QUIT)
		{
			playerDidntQuit = false;
			break;
		}
		else if (type == SPACEBAR) // Move piece down until it comes to rest
		{
			while (!m_well.readyForNextPiece())
				rowsRemoved = m_well.updateAfterOneTimeUnit();
		}
		else // Update piece down after 1 time unit
		{
			rowsRemoved = m_well.updateAfterOneTimeUnit();
		}
		m_well.display(m_screen, WELL_X, WELL_Y);

		// If rows have been vaporized, update score and check if level is complete
		if (rowsRemoved != 0)
		{
			updateStats(rowsRemoved);
			displayStatus();
		}
		
	} while (!m_well.readyForNextPiece() && playerDidntQuit);
	// Repeat while piece has not come to rest
	return playerDidntQuit;
}

SpecialControls Game::controls(char move, PieceType current)
{
	SpecialControls control(NOTHING);
	// Carry out user's move, checking if current piece is the Crazy piece
	switch (move)
	{
	case ARROW_LEFT:
	case 'a':
		if (current == PIECE_CRAZY)
		{
			if (m_well.movePiece('r'))
				control = NORMAL;
		}
		else // It's a normal piece
		{
			if (m_well.movePiece('l'))
				control = NORMAL;
		}
		break;
	case ARROW_RIGHT:
	case 'd':
		if (current == PIECE_CRAZY)
		{
			if (m_well.movePiece('l'))
				control = NORMAL;
		}
		else // It's a normal piece
		{
			if (m_well.movePiece('r'))
				control = NORMAL;
		}
		break;
	case ARROW_UP:
	case 'w':
		if (m_well.rotatePiece())
			control = NORMAL;
		break;
	case ARROW_DOWN:
	case 's':
		control = DOWN;
		break;
	case ' ':
		control = SPACEBAR;
		break;
	case 'q':
	case 'Q':
		control =  QUIT;
		break;
	default:	// Player hit an invalid key
		control = NOTHING;
		break;
	}
	return control;
}
