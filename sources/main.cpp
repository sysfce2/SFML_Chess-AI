#include "PieceType.h"
#include "utils.h"
#include "Move.h"
#include "Piece.h"
#include "Board.h"
#include "Computer.h"

// Initialisation de la fenêtre en fonction de l'écran

void init_window(sf::RenderWindow& window, std::string project_name)
{
	uint16_t width;
	uint16_t height;

	if (sf::VideoMode::getDesktopMode().width > (16. / 9.) * sf::VideoMode::getDesktopMode().height)
		height = (sf::VideoMode::getDesktopMode().height * 3) / 4, width = (height * 16) / 9;

	else if (sf::VideoMode::getDesktopMode().width < (16. / 9.) * sf::VideoMode::getDesktopMode().height)
		width = (sf::VideoMode::getDesktopMode().width * 3) / 4, height = (width * 9) / 16;

	else
		width = (sf::VideoMode::getDesktopMode().width * 3) / 4, height = (sf::VideoMode::getDesktopMode().height * 3) / 4;

	screen_width = width;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	settings.depthBits = 16;
	settings.majorVersion = 3;

	window.create(sf::VideoMode(width, height), project_name, sf::Style::Close | sf::Style::Titlebar, settings);

	sf::Image icon;
	icon.loadFromFile("dependencies/resources/icon.png");

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void draw_grid(sf::RenderTexture& tex, float cell_size)
{
	sf::RectangleShape cell;

	cell.setSize({cell_size, cell_size});
	cell.setFillColor(sf::Color(90, 137, 181));
	for (int8_t i = 0; i < 64; i++)
	{
		cell.setPosition({(i % 8) * cell_size, (i / 8) * cell_size });
		tex.draw(cell);
		if (i % 8 != 7)
			cell.setFillColor(cell.getFillColor() == sf::Color(234, 234, 210) ? sf::Color(90, 137, 181) : sf::Color(234, 234, 210));
	}
}

// Main

int main()
{
	sf::RenderWindow window;
	init_window(window, "Chess AI");
	bool end = false;
	sf::RenderTexture grid_tex;
	float cell_size = std::min(window.getSize().x, window.getSize().y) / 8.f;

	bool space_pressed = false;

	grid_tex.create(cell_size * 8, cell_size * 8);
	draw_grid(grid_tex, cell_size);

	sf::Sprite grid_spr(grid_tex.getTexture());
	grid_spr.setPosition({(window.getSize().x - grid_tex.getSize().x) / 2.f, (window.getSize().y - grid_tex.getSize().y) / 2.f});

	Board board;
	Computer white_computer(White);
	Computer black_computer(Black);

	sf::Event event;
	while(!end)
	{
		event_check(event, window, end);
		window.clear();
		window.draw(grid_spr);

		board.draw_pieces(window, cell_size);
		//board.draw_moves(window, cell_size);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!space_pressed)
			{
				if (board.player_turn == White)
					white_computer.move(board, 3);

				else
					black_computer.move(board, 3); 
			}

			space_pressed = true;
		}

		else
			space_pressed = false;

		board.check_click_on_piece(window, cell_size);

		window.display();
	}
}
