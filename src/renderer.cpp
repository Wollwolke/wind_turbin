#include "renderer.hpp"
#include <iostream>

#include <SDL_image.h>
#include <stdexcept>

/// @throws runtime_error
Renderer::Renderer() : textureBlades(), texturePole()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error(std::string("SDL2 init error: ") + SDL_GetError());
    }

    window = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        throw std::runtime_error(std::string("SDL2 window error: ") + SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        throw std::runtime_error(std::string("SDL2 renderer error: ") + SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        throw std::runtime_error(std::string("SDL2_image init error: ") + IMG_GetError());
    }
}

Renderer::~Renderer()
{
    SDL_DestroyTexture(textureBlades);
    SDL_DestroyTexture(texturePole);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

/// @brief Load the required textures from disk and calculate image scaling
/// @throws runtime_error
void Renderer::loadTextures()
{
    textureBlades = loadTexture("rotor.png");
    texturePole = loadTexture("pole.png");
    if (textureBlades == NULL || texturePole == NULL)
    {
        throw std::runtime_error(std::string("Error loading image: ") + IMG_GetError());
    }
    calculateScaling();
}

/// @brief Updates the canvas
/// @param degrees rotation in degrees of the blades texture
void Renderer::paint(int degrees)
{
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texturePole, NULL, &dstRectPole);
    SDL_RenderCopyEx(renderer, textureBlades, NULL, &dstRectBlades, degrees, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}

/// @brief Creates a SDL_Texture from an image file on disk
/// @param path Path to the image file
/// @return The created texture or NULL on failure
SDL_Texture *Renderer::loadTexture(std::string path)
{
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        return NULL;
    }

    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    return newTexture;
}

/// @brief Initializes the scaling rectangles for the textures
void Renderer::calculateScaling()
{
    // load the original image sizes
    SDL_Point sizePole, sizeBlades;
    SDL_QueryTexture(texturePole, NULL, NULL, &sizePole.x, &sizePole.y);
    SDL_QueryTexture(textureBlades, NULL, NULL, &sizeBlades.x, &sizeBlades.y);

    dstRectBlades.x = 0;
    dstRectBlades.y = 0;
    dstRectBlades.w = sizeBlades.x / SCALE_FACTOR;
    dstRectBlades.h = sizeBlades.y / SCALE_FACTOR;

    // move the pole texture in the center of the blades
    dstRectPole.x = (sizeBlades.x / SCALE_FACTOR / 2) - (IMAGE_OFFSET.X / SCALE_FACTOR);
    dstRectPole.y = (sizeBlades.y / SCALE_FACTOR / 2) - (IMAGE_OFFSET.Y / SCALE_FACTOR);
    dstRectPole.w = sizePole.x / SCALE_FACTOR;
    dstRectPole.h = sizePole.y / SCALE_FACTOR;
}
