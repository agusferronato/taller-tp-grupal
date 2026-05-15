#include <iostream>
#include <exception>

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>


int main(int argc, char* argv[]) try {

	SDL2pp::SDL sdl(SDL_INIT_VIDEO);

	SDL2pp::Window window("Example",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			720, 410,
			SDL_WINDOW_MINIMIZED);

	SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	unsigned int prev_ticks = SDL_GetTicks();

	while (1) {
		
        
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) 
				return 0;
		}

		renderer.Clear();


		renderer.Present();

	}

	return 0;

} catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return 1;
}