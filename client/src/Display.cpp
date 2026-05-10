#include "Display.h"


Display::Display(SDL2pp::Renderer &renderer, Player &player) 
    : renderer(renderer),
    player(player),
    font(SDL2pp::Font("fonts/Vera.ttf", 12)) { }


void Display::render()
{
    std::string text =
        "Position: "
        + std::to_string((int)player.getPosition())
        + ", running: "
        + (player.getRunningStatus() ? "true" : "false");

    SDL2pp::Texture sprite(
        renderer,
        font.RenderText_Blended(text, SDL_Color{255, 255, 255, 255})
    );
    renderer.Copy(sprite, SDL2pp::NullOpt, SDL2pp::Rect(0, 0, sprite.GetWidth(), sprite.GetHeight()));
}
