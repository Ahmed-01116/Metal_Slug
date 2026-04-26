#include "Game.h"

int main()
{
	Game game;
	game.run();
	return 0;
}

// functions

void draw_player(RenderWindow &window, Sprite &playerSprite, float player_x, float player_y)
{
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}

void display_level(RenderWindow &window, const int height, const int width, char **lvl, Sprite &wallSprite1, const int cell_size)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			if (lvl[i][j] == 'g')
			{
				wallSprite1.setPosition(j * cell_size, i * cell_size);
				window.draw(wallSprite1);
			}
		}
	}
}