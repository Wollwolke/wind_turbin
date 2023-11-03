#pragma once
#include <SDL.h>
#include <string>

class Renderer
{
private:
    const std::string TITLE = "Windrad";
    const int SCREEN_WIDTH = 300;
    const int SCREEN_HEIGHT = 500;
    const int SCALE_FACTOR = 4;
    const struct
    {
        int X = 93;
        int Y = 44;
    } IMAGE_OFFSET;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *textureBlades;
    SDL_Texture *texturePole;
    SDL_Rect dstRectBlades;
    SDL_Rect dstRectPole;

    SDL_Texture *loadTexture(std::string path);
    void calculateScaling();

public:
    Renderer();
    ~Renderer();

    void loadTextures();
    void paint(int degrees);
};
