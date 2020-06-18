#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"

///////////////////////////////////////////////////////////////
// Interface functions
///////////////////////////////////////////////////////////////

Well::Well(int width, int height)
	:m_currentPiece(nullptr), m_board(height + 4, vector<char>(width + 5, ' ')), m_depth(height + 1), m_width(width + 2)
{
	// Create walls of well
	for (int i = 0; i < m_depth; i++)
	{
		m_board[i][0] = '@';
		m_board[i][m_width - 1] = '@';
	}
	// Create bottom of well
	for (int i = 0; i < m_width; i++)
	{
		m_board[m_depth - 1][i] = '@';
	}
}

Well::~Well()
{
	if (m_currentPiece != nullptr)
		delete m_currentPiece;
}

void Well::display(Screen& screen, int x, int y) const
{
	// Print out well
	for (int i = 0; i < m_depth; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			screen.gotoXY(x + j, y + i);
			screen.printChar(m_board[i][j]);
		}
	}
}

bool Well::addPiece(PieceType current)
{
	bool successfullyAdded(false);

	// Set new piece as current piece and update board
	selectPiece(current);

	// Check for overlap between new piece and well
	if (checkForOverlap(m_currentPiece->getX(), m_currentPiece->getY(), false))
	{
		// Draw piece on board
		updatePieceOnBoard(true);
		// Deallocate piece
		delete m_currentPiece;
		m_currentPiece = nullptr;
	}
	else // Piece does not overlap
	{
		updatePieceOnBoard();
		successfullyAdded = true;
	}
	return successfullyAdded;
}

bool Well::movePiece(char direction)
{
	bool successfulMove(false);

	// Check that shifting piece does not cause overlap, and if does not, shift the piece
	switch (direction)
	{
	case 'l':
		if (!checkForOverlap(m_currentPiece->getX() - 1, m_currentPiece->getY()))
		{
			m_currentPiece->moveLeft();
			successfulMove = true;
		}
		break;
	case 'r':
		if (!checkForOverlap(m_currentPiece->getX() + 1, m_currentPiece->getY()))
		{
			m_currentPiece->moveRight();
			successfulMove = true;
		}
		break;
	}
	updatePieceOnBoard();
	return successfulMove;
}

bool Well::rotatePiece()
{
	bool successfulRotation(false);

	// Check that rotating piece does not cause overlap, if it doesn't, rotate the piece
	if (!checkForOverlap(m_currentPiece->getX(), m_currentPiece->getY(), true))
	{
		m_currentPiece->nextOrientation();
		successfulRotation = true;
	}
	updatePieceOnBoard();
	return successfulRotation;
}

int Well::updateAfterOneTimeUnit()
{
	int rowsRemoved(0);

	// Check for overlap with the piece's new position
	if (checkForOverlap(m_currentPiece->getX(), m_currentPiece->getY() + 1))
	{
		// Make the piece come to rest
		makePieceRest();
		// Check if any rows need to be vaporized
		rowsRemoved = removeRows();
	}
	else // No overlap 
	{
		// Move the current piece down 1 unit
		m_currentPiece->moveDown();
		updatePieceOnBoard();
	}
	return rowsRemoved;
}

void Well::emptyWell()
{
	// Delete old well's dynamic variable to avoid a memory leak
	if (m_currentPiece != nullptr)
		delete m_currentPiece;

	// Create empty well
	for (int row = 0; row < m_depth - 1; row++)
	{
		for (int col = 1; col < m_width - 1; col++)
		{
			m_board[row][col] = ' ';
		}
	}
}

///////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////

bool Well::checkForOverlap(int newX, int newY, bool rotated) const
{
	bool overlap(false);
	if (rotated)
		m_currentPiece->nextOrientation();
	// Check for overlap given piece's theoretical passed position and orientation
	for (int row = newY; row < newY + BOX_LEN; row++)
	{
		for (int col = newX; col < newX + BOX_LEN; col++)
		{
			// Do not try to access outside of board
			if (col < 0)
				continue;

			// The piece can only appear on tiles that are currently open on the board
			if ((m_board[row][col] == '$' || m_board[row][col] == '@' || m_board[row][col] == '*') && m_currentPiece->getBox(row - newY, col - newX) == '#')
			{
				overlap = true;
				break;
			}
		}
	}
	// Rotate piece back to original position
	if (rotated)
		m_currentPiece->prevOrientation();
	return overlap;
}

void Well::makePieceRest()
{
	// Deal with foam bomb and vapor bomb
	if (m_currentPiece->performSpecialAction(m_board))
		;
	else // If the piece is not a special piece
	{
		// Change current piece from #'s to $'s to show it has come to rest
		int pieceX = m_currentPiece->getX();
		int pieceY = m_currentPiece->getY();
		for (int row = pieceY; row < pieceY + BOX_LEN; row++)
		{
			for (int col = pieceX; col < pieceX + BOX_LEN; col++)
			{
				if (m_board[row][col] == '#')
					m_board[row][col] = '$';
			}
		}
	}
	// Deallocate current piece and make it nullptr
	delete m_currentPiece;
	m_currentPiece = nullptr;
}

void Well::shiftRowsDownOne(int shiftUntilThisRow)
{
	for (int row = shiftUntilThisRow; row > 0; row--)
	{
		for (int col = 1; col < m_width - 1; col++)
		{
			if (m_board[row - 1][col] == '#')
				m_board[row][col] = '#';
			else if (m_board[row - 1][col] == '*')
				m_board[row][col] = '*';
			else if (m_board[row - 1][col] == ' ')
				m_board[row][col] = ' ';
		}
	}
}

int Well::removeRows()
{
	int rowsRemoved(0);

	// Removes piece from board
	for (int row = 0; row < m_depth; row++)
	{
		int blocksInRow(0);
		for (int col = 1; col < m_width - 1; col++)
		{
			if (m_board[row][col] == '$' || m_board[row][col] == '*')
			{
				blocksInRow++;
			}
		}
		// If entire row is full, shift rows above down by one
		if (blocksInRow == m_width - 2)
		{
			shiftRowsDownOne(row);
			rowsRemoved++;
		}
	}
	return rowsRemoved;
}

void Well::updatePieceOnBoard(bool lastPiece)
{
	// Erase Piece's current position
	erasePiece();

	// Updates board according to piece's new position
	int newPieceX = m_currentPiece->getX();
	int newPieceY = m_currentPiece->getY();

	for (int row = newPieceY; row < newPieceY + BOX_LEN; row++)
	{
		for (int col = newPieceX; col < newPieceX + BOX_LEN; col++)
		{
			if (row < 0 || row >= m_depth - 1 || col < 0 || col >= m_width - 1)	// Don't check out of the well
				break;
			// Only change what's on the board if it's an open space, except if it's the last piece
			if (lastPiece && m_currentPiece->getBox(row - newPieceY, col - newPieceX) == '#')
				m_board[row][col] = m_currentPiece->getBox(row - newPieceY, col - newPieceX);
			else if (m_board[row][col] == ' ')
				m_board[row][col] = m_currentPiece->getBox(row - newPieceY, col - newPieceX);
		}
	}
}

void Well::erasePiece()
{
	// Removes piece from board
	for (int row = 0; row < m_depth - 1; row++)
	{
		for (int col = 1; col < m_width - 1; col++)
		{
			if (m_board[row][col] == '#')
				m_board[row][col] = ' ';
		}
	}
}

void Well::selectPiece(PieceType piece)
{
	switch (piece)
	{
	case PIECE_VAPOR:
		m_currentPiece = new VaporBomb;
		break;
	case PIECE_FOAM:
		m_currentPiece = new FoamBomb;
		break;
	default: // It's normal piece
		m_currentPiece = new Piece(piece);
	}
}