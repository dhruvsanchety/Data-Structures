#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <string>

enum SpecialControls {QUIT, SPACEBAR, DOWN, NORMAL, NOTHING};

class Game
{
  public:
    Game(int width, int height);
    void play();

	
  private:
	

    Well    m_well;
    Screen  m_screen;
    int     m_level;
	int		m_score;
	int		m_rowsLeft;

	
	bool playOneLevel();
	void displayPrompt(std::string s);
	void displayStatus(PieceType nextPiece = NONE);
	void updateStats(int rowsVaporized);
	void printNextPiece(PieceType nextPiece);
	void printRightJustified(int stat, int x, int y);
	bool pieceInPlay(PieceType current);

	SpecialControls controls(char move, PieceType current);
};

#endif 
