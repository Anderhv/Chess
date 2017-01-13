#include "Chess.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

Chess::Chess() {


	
	tileArray = new Tile*[8] {};
	gameOver = false;
	movesCount = 0;
	indicator_row = -1;
	indicator_col = -1;

	Tile empty_tile;
	empty_tile.color = EMPTY;

	Tile white_pawn;
	white_pawn.color = WHITE;
	white_pawn.piece = PAWN;

	Tile black_pawn;
	black_pawn.color = BLACK;
	black_pawn.piece = PAWN;

	Tile white_rook;
	white_rook.color = WHITE;
	white_rook.piece = ROOK;

	Tile black_rook;
	black_rook.color = BLACK;
	black_rook.piece = ROOK;

	Tile white_knight;
	white_knight.color = WHITE;
	white_knight.piece = KNIGHT;

	Tile black_knight;
	black_knight.color = BLACK;
	black_knight.piece = KNIGHT;

	Tile white_bishop;
	white_bishop.color = WHITE;
	white_bishop.piece = BISHOP;

	Tile black_bishop;
	black_bishop.color = BLACK;
	black_bishop.piece = BISHOP;

	Tile white_queen;
	white_queen.color = WHITE;
	white_queen.piece = QUEEN;

	Tile black_queen;
	black_queen.color = BLACK;
	black_queen.piece = QUEEN;

	Tile white_king;
	white_king.color = WHITE;
	white_king.piece = KING;

	Tile black_king;
	black_king.color = BLACK;
	black_king.piece = KING;


	for (int row = 0; row < 8; row++) {
		tileArray[row] = new Tile[8] {};
		for (int col = 0; col < 8; col++) {
			if (row == 6) {
				tileArray[row][col] = white_pawn;
			}
			else if (row == 1) {
				tileArray[row][col] = black_pawn;
			}
			else {
				tileArray[row][col] = empty_tile;
			}
		}
	}

	// Rooks
	tileArray[0][0] = black_rook;
	tileArray[0][7] = black_rook;
	tileArray[7][0] = white_rook;
	tileArray[7][7] = white_rook;
	
	// Knights
	tileArray[0][1] = black_knight;
	tileArray[0][6] = black_knight;
	tileArray[7][1] = white_knight;
	tileArray[7][6] = white_knight;

	// Bishops
	tileArray[0][2] = black_bishop;
	tileArray[0][5] = black_bishop;
	tileArray[7][2] = white_bishop;
	tileArray[7][5] = white_bishop;

	// Kings 
	tileArray[0][4] = black_king;
	tileArray[7][4] = white_king;

	// Queens 
	tileArray[0][3] = black_queen;
	tileArray[7][3] = white_queen;
	

}

Chess::~Chess()
{
	for (int i = 0; i < 8; i++) {
		delete[] tileArray[i];
	}
	delete[] tileArray;
}

bool Chess::isValidMove(PIECE piece, int from_row, int from_col, int to_row, int to_col, bool ignoreCheck)
{

	COLOR movingColor = tileArray[from_row][from_col].color;
	
	// Attempting to capture own piece -> Invalid
	if (movingColor == tileArray[to_row][to_col].color) {
		return false;
	}

	// White pawn
	if (tileArray[from_row][from_col].piece == PAWN && movingColor == WHITE) {

		bool enPassent = false;
		if (movesCount > 1) {
			ChessMove latest = moveVector[moveVector.size() - 1];

			if (latest.movingPiece == PAWN && latest.movingColor == BLACK && latest.fromRow == 1 && latest.toRow == 3
				&& from_row == 3 && latest.toCol == 'a' + to_col ) {
				enPassent = true;
			}
		}

		// Moving down or more than 2 up -> Invalid 
		if (from_row <= to_row || from_row - to_row > 2 || (from_row != 6 && from_row - to_row > 1)) {
			// Invalid move
			return false;
		}
		// Moving sideways into empty destiantion -> Invalid
		else if (abs(from_col - to_col) > 1 || 
			(abs(from_col - to_col) == 1 
				&& ((tileArray[to_row][to_col].color == EMPTY && !enPassent) || abs(from_row -to_row) == 2))) {
			return false;
		}
		// Moving straight into non-empty destination -> Invalid
		else if (from_col == to_col && tileArray[to_row][to_col].color != EMPTY) {
			return false;
		}

		// Moving straight from first row over non-empty tile -> Invalid
		else if (from_row == 6 && tileArray[from_row - 1][from_col].color != EMPTY) {
			return false;
		}

	}

	// Black pawn
	else if (tileArray[from_row][from_col].piece == PAWN && movingColor == BLACK) {

		bool enPassent = false;

		if (movesCount > 1) {
			ChessMove latest = moveVector[moveVector.size() - 1];

			if (latest.movingPiece == PAWN && latest.movingColor == WHITE && latest.fromRow == 6 && latest.toRow == 4
				&& from_row == 4 && latest.toCol == 'a' + to_col) {
				enPassent = true;
			}
		}

		// Moving up or more than 2 down -> Invalid 
		if (from_row >= to_row || to_row - from_row > 2 || (from_row != 1 && to_row - from_row > 1)) {
			// Invalid move
			return false;
		}

		// Moving sideways into empty destiantion -> Invalid
		else if (abs(from_col - to_col) > 1 || 
			(abs(from_col - to_col) == 1 
				&& ((tileArray[to_row][to_col].color == EMPTY && !enPassent) || abs(from_row - to_row) == 2))) {
			return false;
		}
		// Moving straight into non-empty destination -> Invalid
		else if (from_col == to_col && tileArray[to_row][to_col].color != EMPTY) {
			return false;
		}

		// Moving straight from first row over non-empty tile -> Invalid
		else if (from_row == 1 && tileArray[from_row + 1][from_col].color != EMPTY) {
			return false;
		}
	}

	// Rooks
	else if (tileArray[from_row][from_col].piece == ROOK) {

		// Diagonal move -> Invalid
		if (from_row != to_row && from_col != to_col) {
			return false;
		}

		if (from_row > to_row) {
			// Moving up through non-empty tile -> Invalid 
			for (int i = from_row - 1; i > to_row; i--) {
				if (tileArray[i][from_col].color != EMPTY) {
					return false;
				}
			}
		}
		else if (from_row < to_row) {
			// Moving down through non-empty tile -> Invalid 
			for (int i = from_row + 1; i < to_row; i++) {
				if (tileArray[i][from_col].color != EMPTY) {
					return false;
				}
			}
		}
		else if (from_col > to_col) {
			// Moving left through non-empty tile -> Invalid
			for (int i = from_col - 1; i > to_col; i--) {
				if (tileArray[from_row][i].color != EMPTY) {
					return false;
				}
			}
		}
		else {
			// Moving right through non-empty tile -> Invalid
			for (int i = from_col + 1; i < to_col; i++) {
				if (tileArray[from_row][i].color != EMPTY) {
					return false;
				}
			}
		}

	}

	// Knights
	else if (tileArray[from_row][from_col].piece == KNIGHT) {
		double dist_row = abs(from_row - to_row);
		double dist_col = abs(from_col - to_col);

		if (dist_row + dist_col != 3 || dist_row > 2 || dist_col > 2 ) {
			return false;
		}

	}

	// Bishops
	else if (tileArray[from_row][from_col].piece == BISHOP) {

		// Horizontal/Vertical move -> Invalid 
		if (from_row == to_row || from_col == to_col) {
			return false;
		}

		// Not diagonal 
		if (abs(from_row - to_row) != abs(from_col - to_col)) {
			return false;
		}


		if (from_row > to_row && from_col > to_col) {
			// Moving up-left through non-empty tile -> Invalid 
			for (int i = 1; i < (from_row - to_row); i++) {
				if (tileArray[from_row - i][from_col - i].color != EMPTY) {
					return false;
				}
			}
		}
		else if (from_row > to_row && from_col < to_col) {
			// Moving up-right through non-empty tile -> Invalid 
			for (int i = 1; i < (from_row - to_row); i++) {
				if (tileArray[from_row - i][from_col + i].color != EMPTY) {
					return false;
				}
			}
		}
		else if (from_row < to_row && from_col > to_col) {
			// Moving down-left through non-empty tile -> Invalid 
			for (int i = 1; i < (to_row - from_row); i++) {
				if (tileArray[from_row + i][from_col - i].color != EMPTY) {
					return false;
				}
			}
		}
		else if (from_row < to_row && from_col < to_col) {
			// Moving down-right through non-empty tile -> Invalid 
			for (int i = 1; i < (to_row - from_row); i++) {
				if (tileArray[from_row + i][from_col + i].color != EMPTY) {
					return false;
				}
			}
		}

	}

	// Queens
	else if (tileArray[from_row][from_col].piece == QUEEN) {

		// Diagonal move 
		if (abs(from_row - to_row) == abs(from_col - to_col)) {
			// Check valid moves for bishop
			if (from_row > to_row && from_col > to_col) {
				// Moving up-left through non-empty tile -> Invalid 
				for (int i = 1; i < (from_row - to_row); i++) {
					if (tileArray[from_row - i][from_col - i].color != EMPTY) {
						return false;
					}
				}
			}
			else if (from_row > to_row && from_col < to_col) {
				// Moving up-right through non-empty tile -> Invalid 
				for (int i = 1; i < (from_row - to_row); i++) {
					if (tileArray[from_row - i][from_col + i].color != EMPTY) {
						return false;
					}
				}
			}
			else if (from_row < to_row && from_col > to_col) {
				// Moving down-left through non-empty tile -> Invalid 
				for (int i = 1; i < (to_row - from_row); i++) {
					if (tileArray[from_row + i][from_col - i].color != EMPTY) {
						return false;
					}
				}
			}
			else if (from_row < to_row && from_col < to_col) {
				// Moving down-right through non-empty tile -> Invalid 
				for (int i = 1; i < (to_row - from_row); i++) {
					if (tileArray[from_row + i][from_col + i].color != EMPTY) {
						return false;
					}
				}
			}
		}

		// Horizontal / Vertical move 
		else if (from_row == to_row || from_col == to_col) {
			// Check valid moves for rook
			if (from_row > to_row) {
				// Moving up through non-empty tile -> Invalid 
				for (int i = from_row - 1; i > to_row; i--) {
					if (tileArray[i][from_col].color != EMPTY) {
						return false;
					}
				}
			}
			else if (from_row < to_row) {
				// Moving down through non-empty tile -> Invalid 
				for (int i = from_row + 1; i < to_row; i++) {
					if (tileArray[i][from_col].color != EMPTY) {
						return false;
					}
				}
			}
			else if (from_col > to_col) {
				// Moving left through non-empty tile -> Invalid
				for (int i = from_col - 1; i > to_col; i--) {
					if (tileArray[from_row][i].color != EMPTY) {
						return false;
					}
				}
			}
			else {
				// Moving right through non-empty tile -> Invalid
				for (int i = from_col + 1; i < to_col; i++) {
					if (tileArray[from_row][i].color != EMPTY) {
						return false;
					}
				}
			}
		}

		// Not moving in a straight line -> Invalid
		else {
			return false;
		}

	}

	// Kings
	else if (tileArray[from_row][from_col].piece == KING) {

		Tile empty_tile;
		empty_tile.color = EMPTY;

		bool castling = false;

		// White queenside castling
		if (from_row == 7 && from_col == 4 && to_row == 7 && to_col == 2 // White King castling queenside
			&& tileArray[7][3].color == tileArray[7][2].color == tileArray[7][1].color == EMPTY // Empty tiles
			) {
			castling = true;

			tileArray[7][3] = tileArray[7][4];

			tileArray[7][4] = empty_tile;

			if (kingInCheck(WHITE)) {
				tileArray[7][4] = tileArray[7][3];
				tileArray[7][3] = empty_tile;
				return false;
			}
			tileArray[7][4] = tileArray[7][3];
			tileArray[7][3] = empty_tile;

			// See if either rook or king has moved earlier
			for (int i = 0; i < moveVector.size(); i++) {
				if ( (moveVector[i].movingColor == tileArray[from_row][from_col].color) 
					&& ((moveVector[i].fromCol == 'a' && moveVector[i].movingPiece == ROOK)
					|| (moveVector[i].movingPiece == KING)) ) {
					castling = false;
				}
			}
		}
		// White kingside castling
		else if (from_row == 7 && from_col == 4 && to_row == 7 && to_col == 6 // White King castling queenside
			&& tileArray[7][5].color == EMPTY && tileArray[7][6].color == EMPTY// Empty tile
			) {
			castling = true;

			tileArray[7][5] = tileArray[7][4];
			tileArray[7][4] = empty_tile;

			if (kingInCheck(WHITE)) {
				tileArray[7][4] = tileArray[7][5];
				tileArray[7][5] = empty_tile;
				return false;
			}
			tileArray[7][4] = tileArray[7][5];
			tileArray[7][5] = empty_tile;

			// See if either rook or king has moved earlier
			for (int i = 0; i < moveVector.size(); i++) {
				if ((moveVector[i].movingColor == tileArray[from_row][from_col].color)
					&& ((moveVector[i].fromCol == 'h' && moveVector[i].movingPiece == ROOK)
						|| (moveVector[i].movingPiece == KING))) {
					castling = false;
				}
			}

		}
		// Black queenside castling
		else if (from_row == 0 && from_col == 4 && to_row == 0 && to_col == 2 // White King castling queenside
			&& tileArray[0][3].color == tileArray[0][2].color == tileArray[0][1].color == EMPTY // Empty tiles
			) {
			castling = true;

			tileArray[0][3] = tileArray[0][4];
			tileArray[0][4] = empty_tile;

			if (kingInCheck(WHITE)) {
				tileArray[0][4] = tileArray[0][3];
				tileArray[0][3] = empty_tile;
				return false;
			}
			tileArray[0][4] = tileArray[0][3];
			tileArray[0][3] = empty_tile;

			// See if either rook or king has moved earlier
			for (int i = 0; i < moveVector.size(); i++) {
				if ((moveVector[i].movingColor == tileArray[from_row][from_col].color)
					&& ((moveVector[i].fromCol == 'a' && moveVector[i].movingPiece == ROOK)
						|| (moveVector[i].movingPiece == KING))) {
					castling = false;
				}
			}
		}
		// Black kingside castling
		else if (from_row == 0 && from_col == 4 && to_row == 0 && to_col == 6 // White King castling queenside
			&& tileArray[0][5].color == EMPTY && tileArray[0][6].color == EMPTY // Empty tiles
			) {
			castling = true;

			tileArray[0][5] = tileArray[0][4];
			tileArray[0][4] = empty_tile;

			if (kingInCheck(WHITE)) {
				tileArray[0][4] = tileArray[0][5];
				tileArray[0][5] = empty_tile;
				return false;
			}
			tileArray[0][4] = tileArray[0][5];
			tileArray[0][5] = empty_tile;

			// See if either rook or king has moved earlier
			for (int i = 0; i < moveVector.size(); i++) {
				if ((moveVector[i].movingColor == tileArray[from_row][from_col].color)
					&& ((moveVector[i].fromCol == 'h' && moveVector[i].movingPiece == ROOK)
						|| (moveVector[i].movingPiece == KING))) {
					castling = false;
				}
			}
		}

		// Attempting to move further than 1 tile -> Invalid, unless Castling
		if (abs(from_row - to_row) > 1 || abs(from_col - to_col) > 1 && !castling) {
			return false;
		}
	}

	Tile fromTile = tileArray[from_row][from_col];
	Tile toTile = tileArray[to_row][to_col];

	if (!ignoreCheck) {
		// See if own King gets checked

		tileArray[to_row][to_col] = tileArray[from_row][from_col];
		Tile empty_tile;
		empty_tile.color = EMPTY;
		tileArray[from_row][from_col] = empty_tile;

		if (kingInCheck(movingColor)) {
			tileArray[from_row][from_col] = fromTile;
			tileArray[to_row][to_col] = toTile;

			return false;
		}
	}
	

	tileArray[from_row][from_col] = fromTile;
	tileArray[to_row][to_col] = toTile;

	return true;
}

bool Chess::movePiece(int from_row, int from_col, int to_row, int to_col)
{

	if (!isValidMove(tileArray[from_row][from_col].piece, from_row, from_col, to_row, to_col,0)) {
		return false;
	}

	Tile empty_tile;
	empty_tile.color = EMPTY;

	// Move is valid, register move
	ChessMove newMove;

	newMove.capture = false;
	newMove.check = false;
	newMove.mate = false;

	movesCount++;
	newMove.movingColor = WHITE;
	if (movesCount % 2 == 0) {
		newMove.movingColor = BLACK;
	}

	// Check for castling
	if (tileArray[from_row][from_col].piece == KING && abs(from_col - to_col) > 1) {

		if (from_col > to_col) {
			newMove.castleType = QUEENSIDE;
			// Execute queenside castling
			if (tileArray[from_row][from_col].color == WHITE) {
				tileArray[to_row][to_col] = tileArray[from_row][from_col];
				tileArray[from_row][from_col] = empty_tile;
				tileArray[7][0] = empty_tile;
				tileArray[7][3].color = WHITE;
				tileArray[7][3].piece = ROOK;
			}
			else {
				tileArray[to_row][to_col] = tileArray[from_row][from_col];
				tileArray[from_row][from_col] = empty_tile;
				tileArray[0][0] = empty_tile;
				tileArray[0][3].color = BLACK;
				tileArray[0][3].piece = ROOK;
			}
		}
		else {
			newMove.castleType = KINGSIDE;
			// Execute queenside castling
			if (tileArray[from_row][from_col].color == WHITE) {
				tileArray[to_row][to_col] = tileArray[from_row][from_col];
				tileArray[from_row][from_col] = empty_tile;
				tileArray[7][7] = empty_tile;
				tileArray[7][5].color = WHITE;
				tileArray[7][5].piece = ROOK;
			}
			else {
				tileArray[to_row][to_col] = tileArray[from_row][from_col];
				tileArray[from_row][from_col] = empty_tile;
				tileArray[0][7] = empty_tile;
				tileArray[0][5].color = BLACK;
				tileArray[0][5].piece = ROOK;
			}
		}

		this->moveVector.push_back(newMove);

		return true;
	}

	// Check for en passent
	else if (tileArray[from_row][from_col].piece == PAWN && abs(from_col - to_col) == 1 && tileArray[to_row][to_col].color == EMPTY) {
		tileArray[from_row][to_col] = empty_tile;
		newMove.capture = true;
	}

	newMove.movingPiece = tileArray[from_row][from_col].piece;
	newMove.toCol = 'a' + to_col;
	newMove.fromCol = 'a' + from_col;
	newMove.toRow = to_row;
	newMove.fromRow = from_row;

	

	if (tileArray[to_row][to_col].color != EMPTY) {
		newMove.capture = true;
	}

	// Execute move 

	tileArray[to_row][to_col] = tileArray[from_row][from_col];
	tileArray[from_row][from_col] = empty_tile;

	// Check for promotion
	if (tileArray[from_row][from_col].piece == PAWN && to_row == 0) {
		Tile promotionTile;
		promotionTile.color = tileArray[from_row][from_col].color;
		int choice = 0;
		std::cout << "Choose a promotion for your pawn. 1 = Queen, 2 = Knight, 3 = Bishop, 4 = Rook. \n: ";
		std::cin >> choice;
		switch (choice) {
		case 1:
			promotionTile.piece = QUEEN;
			break;
		case 2:
			promotionTile.piece = KNIGHT;
			break;
		case 3:
			promotionTile.piece = BISHOP;
			break;
		case 4:
			promotionTile.piece = ROOK;
			break;
		}

		tileArray[to_row][to_col] = promotionTile;
	}

	if (movesCount % 2 != 0 && kingInCheck(BLACK)) {
		newMove.check = true;
	}

	if (movesCount % 2 == 0 && kingInCheck(WHITE)) {
		newMove.check = true;
	}

	this->moveVector.push_back(newMove);

	return true;

}


bool Chess::kingInCheck(COLOR color)
{
	//std::cout << "Checking to see if " << color << " king is in check" << std::endl;
	COLOR opposingColor = BLACK;
	if (color == BLACK) {
		opposingColor = WHITE;
	}

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (tileArray[row][col].color == opposingColor) {

				for (int row2 = 0; row2 < 8; row2++) {
					for (int col2 = 0; col2 < 8; col2++) {
						if (tileArray[row2][col2].piece == KING && tileArray[row2][col2].color == color && isValidMove(tileArray[row][col].piece, row, col, row2, col2,1)) {
							//std::cout << color << " king is checked" << std::endl;
							return true;
						}

					}
				}

			}

		}
	}

	return false;
}

void Chess::printLatestMove() {
	ChessMove latestMove = this->moveVector[this->moveVector.size()-1];
	if (latestMove.movingColor == WHITE) {
		std::cout << ceil(movesCount/2)+1 << ". ";

	}
	else {
		std::cout << ",   \t";
	}

	if (latestMove.castleType == KINGSIDE) {
		std::cout << "O-O";
		if (latestMove.movingColor == BLACK) {
			std::cout << "\n";
		}
		return;
	}
	else if (latestMove.castleType == QUEENSIDE) {
		std::cout << "O-O-O";
		if (latestMove.movingColor == BLACK) {
			std::cout << "\n";
		}
		return;
	}

	switch (latestMove.movingPiece) {
	case 0:
		// Pawn
		break;
	case 1:
		// Knight
		std::cout << "N";
		break;
	case 2:
		// Bishop
		std::cout << "B";
		break;
	case 3:
		// Rook
		std::cout << "R";
		break;
	case 4: 
		// Queen
		std::cout << "Q";
		break;
	case 5:
		// King
		std::cout << "K";
		break;
	}

	if (latestMove.capture) {
		std::cout << "x";
	}

	int a = latestMove.toRow;
	std::string s(1, char(a));

	std::cout << latestMove.toCol << (8-latestMove.toRow);

	if (latestMove.check) {
		std::cout << "+";
	}
	else if (latestMove.mate) {
		std::cout << "#";
	}
	if (latestMove.movingColor == BLACK) {
		std::cout << "\n";
	}

}
