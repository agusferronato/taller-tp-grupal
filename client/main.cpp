#include "common/foo.h"

#include <iostream>
#include <exception>

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

#include "Player.h"
#include "Background.h"
#include "Display.h"


int main() try {

	SDL2pp::SDL sdl(SDL_INIT_VIDEO);
	SDL2pp::SDLTTF ttf;

	SDL2pp::Window window("Example",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			720, 410,
			SDL_WINDOW_MINIMIZED);

	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	Player player(renderer);
	Background background(renderer);
	Display display(renderer, player);
	
	unsigned int prev_ticks = SDL_GetTicks();

	while (1) {
		
		unsigned int frame_ticks = SDL_GetTicks();
		unsigned int frame_delta = frame_ticks - prev_ticks;
		prev_ticks = frame_ticks;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) 
				return 0;
			player.handleEvent(event);
		}

		player.updatePosition(frame_delta, frame_ticks);

		renderer.Clear();

		background.render();		
		player.render();
		display.render();

		renderer.Present();

		// Frame limiter: sleep for a little bit to not eat 100% of CPU
		SDL_Delay(1);
	}

	return 0;
} catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return 1;
}
