#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player{
    public:
    SDL_Rect object;
    void size(int w, int h);
    void move(int x, int y);
    void setPosition(int x, int y);
    void draw(SDL_Renderer *renderer);
};

void Player::size(int w, int h)
{
    object.w = w;
    object.h = h;
}

void Player::move(int x, int y)
{
    object.x += x;
    object.y += y;
}

void Player::setPosition(int x, int y)
{
    object.x = x - object.w/2;
    object.y = y - object.h/2;
}

void Player::draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer, &object);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

const int WIDTH = 800, HEIGHT = 600;

int main( int argc, char* argv[] )
{
    /*if (argc == 2 && strcmp(argv[1], "aktiviraj") == 0)
        puts("pravi heker");
    */

    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "Could not innit: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow( "GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (nullptr == window)
    {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == renderer)
    {
        std::cout << "Cold not create renderer: " << SDL_GetError() << std::endl;
    }

    SDL_Event event;

    Player player;
    player.size(10,10);
    player.setPosition(WIDTH/2, HEIGHT/2);

    Player enemy;
    enemy.size(10,10);
    enemy.setPosition(WIDTH/2, HEIGHT/3);

    SDL_Texture *playerImage = IMG_LoadTexture(renderer, "Player.png");
    SDL_QueryTexture(playerImage, NULL, NULL, &player.object.w, &player.object.h);

    SDL_Texture *enemyImage = IMG_LoadTexture(renderer,"enemy.png");
    SDL_QueryTexture(enemyImage, NULL, NULL, &enemy.object.w, &enemy.object.h);

    int moveX, moveY, moveAmount = 10;
    int enemyMoveAmount = 5;
    int enemyFollowX = enemyMoveAmount, enemyFollowY = enemyMoveAmount;

    bool running = true;
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            { 
                running = false;
            }
            if(event.type == SDL_KEYDOWN)
            {
                if(*SDL_GetKeyName(event.key.keysym.sym) == 'W')
                {
                    moveY = -moveAmount;
                }
                else if(*SDL_GetKeyName(event.key.keysym.sym) == 'S')
                {
                    moveY = moveAmount;
                }
                if(*SDL_GetKeyName(event.key.keysym.sym) == 'A')
                {
                    moveX = -moveAmount;
                }
                else if(*SDL_GetKeyName(event.key.keysym.sym) == 'D')
                {
                    moveX = moveAmount;
                }
            }

        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, enemyImage, NULL, &enemy.object);
        SDL_RenderCopy(renderer, playerImage, NULL, &player.object);
        SDL_RenderPresent(renderer);
        //player.draw(renderer);
        SDL_Delay(100);
        player.move(moveX,moveY);
        if(enemy.object.x < WIDTH/3)
        {
            enemyFollowX=enemyMoveAmount;
        }
        else if(enemy.object.x > (WIDTH/3) * 2)
        {
            enemyFollowX=-enemyMoveAmount;
        }
        else if(abs(enemy.object.x-player.object.x) <= 5 || abs(enemy.object.y-player.object.y) <= 5)
        {
                if(player.object.x-enemy.object.x < 0)
                    enemyFollowX = -enemyMoveAmount;
                else
                    enemyFollowX = enemyMoveAmount;
                if(player.object.y-enemy.object.y < 0)
                    enemyFollowY = -enemyMoveAmount;
                else
                    enemyFollowY = enemyMoveAmount;
        }
        else
        {
            enemyFollowY = 0;
        }
        enemy.move(enemyFollowX,enemyFollowY);
        moveX = 0;
        moveY = 0;
    }

    SDL_DestroyWindow( window );
    SDL_Quit( );

    return EXIT_SUCCESS;
}