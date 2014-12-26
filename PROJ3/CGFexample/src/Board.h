#ifndef BOARD_H
#define BOARD_H
#include "PieceData.h"
#include "Piece.h"
#include <vector>

using namespace std;

class Board{
private:
	vector<vector<PieceData> > board;
	//guardar historico de jogadas aqui tambem
	Piece piece; //instanciada uma pe�a, que ser� usada para desenhar todo o tabuleiro
public:
	Board(vector<vector<PieceData> > b);
	bool fazerJogada(PieceData jogada);
	void draw(int selectedX, int selectedY);
	vector<vector<PieceData> > getBoard();
	void setBoard(vector<vector<PieceData> > board);
	void addPiece(PieceData newPiece);
	PieceData getPiece(int x, int y);
};

#endif
