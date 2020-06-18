#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

#include <string>
#include <vector>
using std::vector;

const int BOX_LEN = 4;
enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES, NONE
};

class Piece
{
public:
	Piece(PieceType piece);
	virtual ~Piece() {}

	// Mutators
	void prevOrientation() { m_orientation--; }
	void nextOrientation() { m_orientation++; }
	void moveLeft() { m_x--; }
	void moveRight() { m_x++; }
	void moveDown() { m_y++; }

	// Accessors
	int getOrientation() const { return m_orientation; }
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	char getBox(int row, int col) const { return m_boxOrientations[m_orientation % 4][row][col]; }
	// Returns the character in a row, column of a piece's bounding box.
	virtual bool performSpecialAction(vector<vector<char>>& board) const { return false; }

private:

	int m_x;
	int m_y;
	char m_boxOrientations[4][BOX_LEN][BOX_LEN];
	int m_orientation;

	// Helpers
	void createBoxes(const char box0[BOX_LEN][BOX_LEN],const char box1[BOX_LEN][BOX_LEN] = nullptr,
	const char box2[BOX_LEN][BOX_LEN] = nullptr, const char box3[BOX_LEN][BOX_LEN] = nullptr);
};

class VaporBomb : public Piece
{
public:
	VaporBomb() : Piece(PIECE_VAPOR) {}
	virtual bool performSpecialAction(vector<vector<char>>& board) const;
	
};

class FoamBomb : public Piece
{
public:
	FoamBomb() : Piece(PIECE_FOAM) {}
	virtual bool performSpecialAction(vector<vector<char>>& board) const;
	

private:
	void foamBombHelper(vector<vector<char>>& board, int x, int y) const;
};
PieceType chooseRandomPieceType();

#endif
