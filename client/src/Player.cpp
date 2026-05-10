#include "Player.h"


Player::Player(SDL2pp::Renderer& renderer) :
    renderer(renderer),
    texture(SDL2pp::Texture(renderer, SDL2pp::Surface("textures/M484SpaceSoldier.png").SetColorKey(true, 0))) {
	    texture.SetBlendMode(SDL_BLENDMODE_BLEND);
    }


void Player::updatePosition(unsigned int frameDelta, unsigned int frameTicks) {

    if (isRunning) {
        if (isRunningToRight) {
            position += frameDelta * 0.2;
        } else {
            position -= frameDelta * 0.2;
        }
        iterationStep = (frameTicks / 100) % 8;

    } else {
        iterationStep = 0;
    }

}

void Player::render() {

    if (isRunning) {
        textureX = 8 + 51 * iterationStep;

        if (isRunningToRight) 
            textureY = 67;
        else 
            textureY = 123;

    } else {
        if (isRunningToRight) 
            textureX = 8;
        else 
            textureX = 59;
        
        textureY = 11;
    }

    if (position > renderer.GetOutputWidth())
        position = -50;

    texture.SetAlphaMod(255);
    renderer.Copy(
        texture,
        SDL2pp::Rect(textureX, textureY, 50, 50),
        SDL2pp::Rect((int)position, 245, 50, 50)
    );

    // Reflejo
    texture.SetAlphaMod(127); 
    renderer.Copy(
        texture,
        SDL2pp::Rect(textureX, textureY, 50, 50),
        SDL2pp::Rect((int)position, 295, 50, 50),
        0.0,              
        SDL2pp::NullOpt,          
        SDL_FLIP_VERTICAL 
    );
}


void Player::handleEvent(const SDL_Event& event) {

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT: 
                isRunning = true;
                isRunningToRight = true;
                break;
            case SDLK_LEFT:
                isRunning = true;
                isRunningToRight = false;
                break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
            case SDLK_LEFT:
                isRunning = false;
        }
    }

}

bool Player::getRunningStatus()
{
    return isRunning;
}

int Player::getPosition()
{
    return (int)position;
}
