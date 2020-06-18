#ifndef WELL_INCLUDED
#define WELL_INCLUDED

#include "Piece.h"
#include <vector>
#include <queue>
using namespace std;
class Screen;

class Well
{
  public:
	Well(int width, int height);
	~Well();

    void display(Screen& screen, int x, int y) const;
	// Displays m_board to the screen.
	bool readyForNextPiece() const { return m_currentPiece == nullptr; }
	// Returns true if no piece is currently in play(falling), otherwise  returns false.
	bool addPiece(PieceType current);
	// Adds new piece to board and returns true if piece was successfully added, or false if well is full.
	bool movePiece(char direction);
	// Moves piece to the left if passed 'l' or right if passed 'r', if possible.
	bool rotatePiece();
	// Rotates piece to next orientation.
	int updateAfterOneTimeUnit();
	// Shifts the current piece down one row and returns the number of rows vaporized if 
	//	the piece has come to rest.
	void emptyWell();
	// Makes the well empty.

private:
	// Class invariant:
	//	m_currentPiece points to the piece currently falling, or nullptr when there is no piece currently falling
	//	m_board is a 2D char vector representing the well, the current piece, and pieces at rest in the well
	//	m_depth is the depth of the well (including the bottom)
	//	m_width is the width of the well (including the walls)

	Piece* m_currentPiece;
	vector<vector<char>> m_board;
	int m_depth;
	int m_width;

	// Helper functions
	bool checkForOverlap(int newX, int newY, bool rotated = false) const;
	// Returns true if the current piece, with the passed x, y, and orientation, will overlap 
	//	with the well, else it returns false.
	void makePieceRest();
	void shiftRowsDownOne(int shiftUntilThisRow);
	int removeRows();
	void updatePieceOnBoard(bool lastPiece = false);
	// If it's the last piece, update the board to show where it would have been placed.
	void erasePiece();
	void selectPiece(PieceType piece);
};

#endif // WELL_INCLUDED