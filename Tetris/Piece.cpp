#include "Piece.h"
#include <vector>
using std::vector;

///////////////////////////////////////////////////////////////
// Piece orientations
///////////////////////////////////////////////////////////////

const char i02[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ '#', '#', '#', '#' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char i13[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' }
};
const char l0[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ '#', '#', '#', ' ' },
{ '#', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char l1[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', '#', ' ' },
{ ' ', ' ', '#', ' ' },
{ ' ', ' ', '#', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char l2[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', '#', ' ' },
{ '#', '#', '#', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char l3[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', '#', ' ' }
};
const char jj0[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', '#', '#', '#' },
{ ' ', ' ', ' ', '#' },
{ ' ', ' ', ' ', ' ' }
};
const char jj1[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', '#', ' ' },
{ ' ', ' ', '#', ' ' },
{ ' ', '#', '#', ' ' }
};
const char j2[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', '#', '#' },
{ ' ', ' ', ' ', ' ' }
};
const char j3[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', '#', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char t0[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', ' ', ' ' },
{ '#', '#', '#', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char t1[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', ' ', ' ' },
{ ' ', '#', '#', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char t2[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ '#', '#', '#', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char t3[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', ' ', ' ' },
{ '#', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char o0[BOX_LEN][BOX_LEN] = {
	{ '#', '#', ' ', ' ' },
{ '#', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char s02[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', '#', '#', ' ' },
{ '#', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char s13[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', ' ', ' ' },
{ ' ', '#', '#', ' ' },
{ ' ', ' ', '#', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char z02[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ '#', '#', ' ', ' ' },
{ ' ', '#', '#', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char z13[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', '#', ' ' },
{ ' ', '#', '#', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char v0[BOX_LEN][BOX_LEN] = {
	{ ' ', '#', '#', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char f0[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char c0[BOX_LEN][BOX_LEN] = {
	{ '#', ' ', ' ', '#' },
{ ' ', '#', '#', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' }
};
const char c1[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', '#' },
{ ' ', ' ', '#', ' ' },
{ ' ', ' ', '#', ' ' },
{ ' ', ' ', ' ', '#' }
};
const char c2[BOX_LEN][BOX_LEN] = {
	{ ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ' },
{ ' ', '#', '#', ' ' },
{ '#', ' ', ' ', '#' }
};
const char c3[BOX_LEN][BOX_LEN] = {
	{ '#', ' ', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ ' ', '#', ' ', ' ' },
{ '#', ' ', ' ', ' ' }
};

///////////////////////////////////////////////////////////////
// Interface functions
///////////////////////////////////////////////////////////////

Piece::Piece(PieceType piece)
	:m_x(4), m_y(0), m_orientation(0)
{
	switch (piece)
	{
	case PIECE_I:
		createBoxes(i02, i13);
		break;
	case PIECE_L:
		createBoxes(l0, l1, l2, l3);
		break;
	case PIECE_J:
		createBoxes(jj0, jj1, j2, j3);
		break;
	case PIECE_T:
		createBoxes(t0, t1, t2, t3);
		break;
	case PIECE_O:
		createBoxes(o0);
		break;
	case PIECE_S:
		createBoxes(s02, s13);
		break;
	case PIECE_Z:
		createBoxes(z02, z13);
		break;
	case PIECE_VAPOR:
		createBoxes(v0);
		break;
	case PIECE_FOAM:
		createBoxes(f0);
		break;
	case PIECE_CRAZY:
		createBoxes(c0, c1, c2, c3);
		break;
	}
}

bool VaporBomb::performSpecialAction(vector<vector<char>>& board) const
{
	// Iterate over the 2x5 box that the vapor bomb clears out
	int startX = getX() + 1, endX = startX + 2;
	int startY = getY() - 2, endY = startY + 5;
	for (int row = startY; row < endY; row++)
	{
		if (row < 0)	// Check if we will go past the top of the well
			break;
		if (board[row][startX] == '@')	// Check if we hit the bottom of the well
			break;

		for (int col = startX; col < endX; col++)
		{
			if (board[startY][col] == '@')	// Check if we hit the wall of the well
				break;

			// If there is something in the well here, vaporize it
			if (board[row][col] == '$' || board[row][col] == '*')
				board[row][col] = ' ';
		}	
	}
	return true;
}

bool FoamBomb::performSpecialAction(vector<vector<char>>& board) const
{
	// Pass in the position of the foam bomb, not the bouding box
	foamBombHelper(board, getY() + 1, getX() + 1);
	return true;
}

void FoamBomb::foamBombHelper(vector<vector<char>>& board, int row, int col) const
{
	// Iterate over the 5x5 box that the foam bomb covers
	const int minCol = getX() - 1, maxCol = getX() + 3;
	const int minRow = getY() - 1, maxRow = getY() + 3;

	// Base cases
	if (row < minRow || row > maxRow || col < minCol || col > maxCol) // check that space is within 5x5 box
		return;
	if (row < 0)	// Don't search past the top of the well
		return;
	if (board[row][col] == '@')	// Don't search past the well's walls
		return;

	// Space is valid, leave a *
	board[row][col] = '*';

	// Check if any adjacent spaces are open
	if (board[row + 1][col] == ' ')
		foamBombHelper(board, row + 1, col);
	if (row != 0 && board[row - 1][col] == ' ')
		foamBombHelper(board, row - 1, col);
	if (board[row][col + 1] == ' ')
		foamBombHelper(board, row, col + 1);
	if (board[row][col - 1] == ' ')
		foamBombHelper(board, row, col - 1);
}

///////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////

void Piece::createBoxes(const char box0[BOX_LEN][BOX_LEN], const  char box1[BOX_LEN][BOX_LEN], const  char box2[BOX_LEN][BOX_LEN], const  char box3[BOX_LEN][BOX_LEN])
{
	for (int i = 0; i < BOX_LEN; i++)
	{
		for (int j = 0; j < BOX_LEN; j++)
		{
			m_boxOrientations[0][i][j] = box0[i][j];
			if (box1 == nullptr)
			{
				m_boxOrientations[1][i][j] = box0[i][j];
				m_boxOrientations[2][i][j] = box0[i][j];
				m_boxOrientations[3][i][j] = box0[i][j];
			}
			else if (box2 == nullptr)
			{
				m_boxOrientations[1][i][j] = box1[i][j];
				m_boxOrientations[2][i][j] = box0[i][j];
				m_boxOrientations[3][i][j] = box1[i][j];
			}
			else
			{
				m_boxOrientations[1][i][j] = box1[i][j];
				m_boxOrientations[2][i][j] = box2[i][j];
				m_boxOrientations[3][i][j] = box3[i][j];
			}
		}
	}
}