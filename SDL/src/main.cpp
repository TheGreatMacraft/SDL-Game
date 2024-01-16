#include <iostream>
#include <SDL2/SDL.h>

void rect()
{
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = 32;
    rectangle.h = 32;
}

const int WIDTH = 800, HEIGHT = 600;

int main( int argc, char* argv[] )
{
    /*if (argc == 2 && strcmp(argv[1], "aktiviraj") == 0)
        puts("pravi heker");
    */

    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "sdl init rahlo ne dela " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow( "GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (window == nullptr)
    {
        std::cout << "Napaka pri ustvarjanju okna: " << SDL_GetError( ) << std::endl;
        return 1;
    }

    SDL_Event windowEvent;

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }
    }

    SDL_RenderDrawRect()

    SDL_DestroyWindow(window);
    SDL_Quit( );

    return EXIT_SUCCESS;
}