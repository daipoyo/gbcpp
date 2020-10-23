#include <SDL/SDL.h>

int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

	SDL_Delay(3000);

	SDL_Quit();

	return;
}