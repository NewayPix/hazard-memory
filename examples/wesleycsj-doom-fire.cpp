/*
 *	Author: WesleyCSJ
 *  Description: Doom Fire PSX Implementation
 *
 *
 */
#include <vector>
#include <SDL2/SDL.h>
#include "GameLoop.hpp"
#include "InputHandler.hpp"

#include <cstdlib>
#include <ctime>
#include <math.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define TILESIZE      4
std::map<const char*, SDL_Keycode> input_config = {
    {"quit", SDLK_ESCAPE},
};

class Game: public GameLoop {
	using GameLoop::GameLoop;
	InputHandler input_handler = InputHandler(input_config);
	// Game variables
	int widthQuantity  = SCREEN_WIDTH  / TILESIZE;
  int heightQuantity = 96;
	int offsetX        = 0;
	int offsetY        = SCREEN_HEIGHT - (heightQuantity * TILESIZE) + 5;
	//int tiles[widthQuantity][heightQuantity];
	int colors[36][3] = {
		{0,0,0},
		{31,7,7},                                                                                                                                
		{47,15,7},                                                                                                                               
		{71,15,7},                                                                                                                               
		{87,23,7},
		{103,31,7},
		{119,31,7},
		{143,39,7},
		{159,47,7},
		{175,63,7},
		{191,71,7},
		{199,71,7},
		{223,79,7},
		{223,87,7},
		{223,87,7},
		{215,95,7},
		{215,103,15},
		{207,111,15},
		{207,119,15},
		{207,127,15},
		{207,137,23},
		{199,135,23},
		{199,143,23},
		{199,151,31},
		{191,159,31},
		{191,159,31},
		{191,167,39},
		{191,167,39},
		{191,175,47},
		{183,175,47},
		{183,183,47},
		{183,183,55},
		{207,207,111},
		{223,223,159},
		{239,239,199},
		{255,255,255}
	};
	std::vector<int> tiles;
	void start(){
		for (int i = 0; i < widthQuantity; i++){
			for (int j = 0; j < heightQuantity; j++){
				if (j == (heightQuantity - 1)){
						tiles.push_back(35);
				} else {
					tiles.push_back(0);
				}
			}
		}
	}

	void event(){
		static SDL_Event e;
		input_handler.process(e);
		//This is obligatory, if not, window will run forever and be blocked by OS when tried to close.
		running = !(input_handler.read("quit") || input_handler.read(SDL_QUIT));

	}

	void update(float dt) {
		for (int i = 1; i < (widthQuantity - 1); i++){
    	for (int j = 1; j < (heightQuantity - 1); j++){		
					int value = getTileValue(i, (j + 1));
					double randNumber = sortNumber();
					int coeficient    = value - floor(randNumber * 3);
					value =  (coeficient > 0) ? coeficient : 0;
//					std::cout << value << std::endl;
					if (randNumber < 0.4){
						setTileValue((i - 1), j,  value);
					} else if (randNumber > 0.4 && randNumber <= 0.8){
						setTileValue((i + 1), j , value);
					} else {
						setTileValue(i, (j - 1) , value);
					}
			}
		}
	}
	
	int getTileValue(int x, int y){
		return tiles.at( (x * heightQuantity) + y );
	}

	double sortNumber(){
		return (std::rand() / (double)RAND_MAX);
	}

	int setTileValue(int x, int y, int value){
		tiles.at((x * heightQuantity) + y) = value;
	}

	void render(){
		//Sets a color in a renderer and render on screen
		SDL_RenderClear(renderer);
		for (int i = 0; i < widthQuantity; i++){
			for (int j = 0; j < heightQuantity; j++){		
			  int tileEl = getTileValue(i, j);	
				SDL_SetRenderDrawColor(renderer, colors[tileEl][0] , colors[tileEl][1], colors[tileEl][2], 255);
				SDL_Rect rectangle;

				rectangle.x = offsetX + (TILESIZE * i);
				rectangle.y = offsetY + (TILESIZE * j);
				rectangle.w = TILESIZE;
				rectangle.h = TILESIZE;
				SDL_RenderFillRect(renderer, &rectangle);
			}                                        		
		}
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderPresent(renderer);
	}
};

int main(int argc, char** argv){
	std::srand(std::time(nullptr));
	Game game("Doom Fire", SCREEN_WIDTH, SCREEN_HEIGHT);
	return game.run();
}
