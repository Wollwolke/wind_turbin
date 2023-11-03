#include <SDL.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>

#include "renderer.hpp"

/// @brief Reads the first entry of a text file and validates wether it is an integer between 1 and 10
/// @return The read number or 1 on failure
uint8_t readSpeedSetting()
{
    int number = 0;

    std::ifstream file("speed.txt");
    if (!file.is_open())
    {
        std::cerr << "Unable to open speed.txt file" << std::endl;
        return 1;
    }

    file >> number;
    file.close();

    if (number < 1 || number > 10)
    {
        std::cerr << "Invalid number. Please enter a number between 1 and 10." << std::endl;
        return 1;
    }

    return number;
}

int main(int argc, char *args[])
{
    using namespace std::chrono;

    std::unique_ptr<Renderer> renderer;
    try
    {
        renderer = std::make_unique<Renderer>();
        renderer->loadTextures();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    SDL_Event e;
    int degrees = 0;
    uint8_t speedSetting = readSpeedSetting();
    auto start = steady_clock::now();

    while (true)
    {
        // Listen for SDL Events to gracefully quit the application
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                return 0;
            }
        }

        // update speed setting if more than 5 seconds have passed since the last update
        auto current_time = std::chrono::steady_clock::now();
        if (current_time - start >= std::chrono::seconds(5))
        {
            speedSetting = readSpeedSetting();
            start = current_time;
        }

        // update the canvas
        renderer->paint(degrees);

        // calculate the new rotation of the blades
        degrees += speedSetting;
        if (degrees > 360)
        {
            degrees -= 360;
        }

        // limit updates to ~60 fps
        SDL_Delay(16);
    }
}
