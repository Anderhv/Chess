#pragma once
#include <vector>

typedef enum PIECE { PAWN = 0, KNIGHT, BISHOP, ROOK, QUEEN, KING };
typedef enum COLOR { EMPTY = 0, WHITE, BLACK };
typedef enum CASTLE { NONE = 0, QUEENSIDE, KINGSIDE };

class Chess {
private:
	// Legg til de medlemsvariablene og hjelpefunksjonene du trenger

public:
	Chess();
	~Chess();

	int movesCount = 0;

	int indicator_row;
	int indicator_col;

	bool isValidMove(PIECE piece, int from_row, int from_col, int to_row, int to_col, bool ignoreCheck);

	bool movePiece(int chosen_row, int chosen_col, int rol, int col); 

	bool gameOver;

	bool kingInCheck(COLOR color);

	void printLatestMove();

	struct Tile {
		PIECE piece;
		COLOR color;
	};

	struct ChessMove {
		PIECE movingPiece;
		COLOR movingColor;
		char toCol;
		char fromCol;
		int toRow;
		int fromRow;
		CASTLE castleType;
		bool capture;
		bool check;
		bool mate;
	};

	Tile **tileArray;
	std::vector<ChessMove> moveVector;

	// Vi slår av autogenerert kopikonstruktør og tilordningsoperator for å unngå feil
	Chess(const Chess &) = delete;
	Chess &operator=(const Chess &) = delete;
};
