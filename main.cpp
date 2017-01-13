#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "Chess.h"

using namespace std;

const sf::Color white_tile_color(255,220,190), black_tile_color(150, 75, 25);
const sf::Color number_colors[9] = {
    sf::Color::White,
    sf::Color::Blue,
    sf::Color(0, 128, 0),
    sf::Color::Red,
    sf::Color(0, 0, 128),
    sf::Color(128, 0, 0),
    sf::Color(0, 128, 128),
    sf::Color::Black,
    sf::Color(128, 128, 128)
};
const sf::Color mine_color = sf::Color::Red;

const int tile_size = 45;
const int border_size = 2;

int main() {

	srand(time(nullptr));

	typedef enum GAME_STATE {DRAW_BOARD, WAIT_FOR_PLAYER, GG};
	typedef enum TURN_ORDER {WHITE_TURN, BLACK_TURN};

	GAME_STATE game_state = DRAW_BOARD;
	TURN_ORDER turn_order = WHITE_TURN;


    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")){
        cout << "Fant ikke skrifttypen 'sansation.ttf'" << endl;
        exit(0);
    }

	Chess* game = new Chess();

    sf::RenderWindow window(sf::VideoMode(9 * tile_size, 9 * tile_size), "Sjakk yoyo", sf::Style::Close);
    window.setFramerateLimit(60);	

    while (window.isOpen()) {

		switch (game_state) {
		case DRAW_BOARD:
			window.clear();
			for (int row = 0; row < 8; ++row) {
				for (int col = 0; col < 8; ++col) {
					const int tile_x = col * tile_size, tile_y = row * tile_size;
					sf::RectangleShape tile;
					tile.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
					if (row % 2) {
						tile.setFillColor(col % 2 ? white_tile_color : black_tile_color);
					}
					else {
						tile.setFillColor(col % 2 ? black_tile_color : white_tile_color);
					}
					tile.setPosition(tile_x + border_size / 2.0, tile_y + border_size / 2.0);

					window.draw(tile);

					if (col == 0) {
						sf::Text text;
						text.setStyle(sf::Text::Bold);
						text.setCharacterSize(tile_size / 3.0);

						text.setString(to_string(8 - row));
						text.setColor(mine_color);

						text.setFont(font);

						sf::FloatRect text_rect = text.getLocalBounds();
						text.setOrigin(text_rect.left,
							text_rect.top - (tile_size / 2) -5);
						text.setPosition(8*tile_size+5, tile_y - (tile_size / 4.0));

						window.draw(text);
					}

					if (row == 7) {
						sf::Text text;
						text.setStyle(sf::Text::Bold);
						text.setCharacterSize(tile_size / 3.0);

						char c = 'a' + col;
						text.setString(c);
						text.setColor(mine_color);

						text.setFont(font);

						text.setPosition(tile_x + (tile_size / 2) - (tile_size / 8) , 8 * tile_size);

						window.draw(text);
					}
					if (game->indicator_col != -1 && game->isValidMove(game->tileArray[game->indicator_row][game->indicator_col].piece,game->indicator_row,game->indicator_col, row, col,0)) {

						sf::Texture valid_texture;
						valid_texture.loadFromFile("choice_indicator.png");

						sf::RectangleShape valid;
						valid.setTexture(&valid_texture);

						valid.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
						valid.setPosition(tile_x + border_size / 2.0, tile_y + border_size / 2.0);

						window.draw(valid);
					}

					if (game->indicator_row == row && game->indicator_col == col) {
						
						sf::Texture indicator_texture;
						indicator_texture.loadFromFile("valid_indicator2.png");

						sf::RectangleShape indicator;
						indicator.setTexture(&indicator_texture);

						indicator.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
						indicator.setPosition(tile_x + border_size / 2.0, tile_y + border_size / 2.0);

						window.draw(indicator);


					}

					sf::Texture texture;

					if (game->tileArray[row][col].piece == PAWN && game->tileArray[row][col].color == WHITE) {

						texture.loadFromFile("white_pawn.png");

					}
					else if (game->tileArray[row][col].piece == PAWN && game->tileArray[row][col].color == BLACK) {

						texture.loadFromFile("black_pawn.png");

					}
					else if (game->tileArray[row][col].piece == ROOK && game->tileArray[row][col].color == WHITE) {

						texture.loadFromFile("white_rook.png");

					}
					else if (game->tileArray[row][col].piece == ROOK && game->tileArray[row][col].color == BLACK) {

						texture.loadFromFile("black_rook.png");

					}
					else if (game->tileArray[row][col].piece == KNIGHT && game->tileArray[row][col].color == WHITE) {

						texture.loadFromFile("white_knight.png");

					}
					else if (game->tileArray[row][col].piece == KNIGHT && game->tileArray[row][col].color == BLACK) {

						texture.loadFromFile("black_knight.png");

					}
					else if (game->tileArray[row][col].piece == BISHOP && game->tileArray[row][col].color == WHITE) {

						texture.loadFromFile("white_bishop.png");

					}
					else if (game->tileArray[row][col].piece == BISHOP && game->tileArray[row][col].color == BLACK) {

						texture.loadFromFile("black_bishop.png");

					}
					else if (game->tileArray[row][col].piece == QUEEN && game->tileArray[row][col].color == WHITE) {

						texture.loadFromFile("white_queen.png");

					}
					else if (game->tileArray[row][col].piece == QUEEN && game->tileArray[row][col].color == BLACK) {

						texture.loadFromFile("black_queen.png");

					}
					else if (game->tileArray[row][col].piece == KING && game->tileArray[row][col].color == WHITE) {

						texture.loadFromFile("white_king.png");

					}
					else if (game->tileArray[row][col].piece == KING && game->tileArray[row][col].color == BLACK) {

						texture.loadFromFile("black_king.png");

					}
					else {
						continue;
					}

					sf::RectangleShape piece;
					piece.setTexture(&texture);

					piece.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
					piece.setPosition(tile_x + border_size / 2.0, tile_y + border_size / 2.0);

					window.draw(piece);

				}
			}
			window.display();
			game_state = WAIT_FOR_PLAYER;

			break;

		case WAIT_FOR_PLAYER:

			sf::Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left) {
						int row = event.mouseButton.y / tile_size;
						int col = event.mouseButton.x / tile_size;

						if (game->indicator_col != -1) {
							if (game->movePiece(game->indicator_row, game->indicator_col, row, col)) {

								if (turn_order == WHITE_TURN) {
									turn_order = BLACK_TURN;
								}
								else {
									turn_order = WHITE_TURN;
								}
								game->printLatestMove();
							}
							game->indicator_row = -1;
							game->indicator_col = -1;

							game_state = DRAW_BOARD;
						}

						else if ((game->tileArray[row][col].color == WHITE && turn_order == WHITE_TURN) || 
								 (game->tileArray[row][col].color == BLACK && turn_order == BLACK_TURN)) {

							game->indicator_row = row;
							game->indicator_col = col;
							
							game_state = DRAW_BOARD;
						}

					}
					break;
				}
			}

			break;
		}
		
		/*
        for(int row = 0; row < 8; ++row) {
            for(int col = 0; col < 8; ++col) {
                const int tile_x = col * tile_size, tile_y = row * tile_size;

                sf::RectangleShape tile;
                tile.setSize(sf::Vector2f(tile_size - border_size, tile_size - border_size));
                tile.setFillColor(open_fill_color);
                tile.setPosition(tile_x + border_size / 2.0, tile_y + border_size / 2.0);

                window.draw(tile);
				
                if (game->isTileOpen(row, col) || (game->isGameOver() && game->isTileMine(row, col))) {
                    sf::Text text;
                    text.setStyle(sf::Text::Bold);
                    text.setCharacterSize(tile_size / 2.0);

                    if (game->isTileMine(row, col)) {
                        text.setString("X");
                        text.setColor(mine_color);
                    }
                    else {
                        int num_adjacent_mines = game->numAdjacentMines(row, col);
                        if(num_adjacent_mines == 0) continue; // Ikke tegn nuller
                        text.setString(to_string(num_adjacent_mines));
                        text.setColor(number_colors[num_adjacent_mines]);
                    }

                    text.setFont(font);

                    sf::FloatRect text_rect = text.getLocalBounds();
                    text.setOrigin(text_rect.left + text_rect.width  / 2.0,
                                   text_rect.top  + text_rect.height / 2.0);
                    text.setPosition(tile_x + tile_size / 2.0, tile_y + tile_size / 2.0);

                    window.draw(text);
                }
				
            }
        }
		*/
        
    }

    delete game;

    return 0;
}
