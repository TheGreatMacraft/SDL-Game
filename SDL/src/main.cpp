#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int WIDTH = 1000, HEIGHT = 800;
float cameraX = 100, cameraY = 100;

class Player{
    public:
    SDL_Rect object;
    SDL_Texture *objectImage;
    int x,y = 100;
    Player();
    void size(int w, int h);
    void setPosition(int x, int y);
    void centered();
    void draw(SDL_Renderer *renderer);
    void drawFlipped(SDL_Renderer * renderer);
    void addForce(std::string direction, int amount);
};

Player::Player()
{
    object.x = cameraX;
    object.y = cameraY;
}

void Player::size(int w, int h)
{
    object.w = w;
    object.h = h;
}

void Player::setPosition(int newX, int newY)
{
    x = newX - object.w/2;
    y = newY - object.h/2;
}

void Player::draw(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, objectImage, NULL, &object);
}

void Player::drawFlipped(SDL_Renderer * renderer)
{
    SDL_RenderCopyEx(renderer,objectImage,NULL,&object,0,NULL,SDL_FLIP_HORIZONTAL);
}

void Player::centered()
{
    object.x = WIDTH/2 - object.w/2;
    object.y = HEIGHT/2 - object.y/2;
}

void Player::addForce(std::string direction, int amount)
{
    if(direction =="y")
    {
        object.y += amount;
    }
}

class Entity{
    public:
    SDL_Rect object;
    SDL_Texture *objectImage;
    void size(int w, int h);
    void setPosition(int x, int y);
    void setCameraPosition(int x, int y);
    void draw(SDL_Renderer *renderer);
    void setDestination(int x, int y, int speed, float deltaTime);
};

void Entity::size(int w, int h)
{
    object.w = w;
    object.h = h;
}

void Entity::setPosition(int x, int y)
{
    object.x = x - object.w/2;
    object.y = y - object.h/2;
}

void Entity::draw(SDL_Renderer *renderer)
{
    SDL_Rect rect = object;
    rect.x -= cameraX;
    rect.y -= cameraY;
    SDL_RenderCopy(renderer, objectImage, NULL, &rect);
}

void Entity::setDestination(int x, int y, int speed, float deltaTime)
{
    if(object.x < x-2 || object.x > x + 2)
    {
        if(x-object.x > 0)
        {
            object.x += speed * deltaTime;
        }
        else
        {
            object.x -= speed * deltaTime;
        }
    }
    else
    if(object.y < y-2 || object.y > y + 2)
    {
        if(y-object.y > 0)
        {
            object.y += speed * deltaTime;
        }
        else
        {
            object.y -= speed * deltaTime;
        }
    }
}

class Ground{
    public:
    SDL_Rect object;
    SDL_Texture *objectImage;
    bool isLabEntrance;
    bool isHole;
    Ground(){isLabEntrance = false; isHole = false;};
    void draw(SDL_Renderer *renderer);
    void setPosition(int x, int y);
    void size(int w, int h);
};

void Ground::draw(SDL_Renderer *renderer)
{
    SDL_Rect rect = object;
    rect.x -= cameraX;
    rect.y -= cameraY;
    SDL_RenderCopy(renderer, objectImage, NULL, &rect);
}

void Ground::setPosition(int x, int y)
{
    object.x = x;
    object.y = y;
}

void Ground::size(int w, int h)
{
    object.w = w;
    object.h = h;
}

SDL_Rect backgroundSetup()
{
    SDL_Rect background;
    background.h = HEIGHT;
    background.w = WIDTH;
    background.x = 0;
    background.y = 0;
    return background;
}

class Environment{
    public:
    SDL_Rect object;
    SDL_Texture *objectImage;
    void draw(SDL_Renderer *renderer);
    void drawStatic(SDL_Renderer *renderer);
    void setPosition(int x, int y);
    void size(int w, int h);
};

void Environment::draw(SDL_Renderer *renderer)
{
    SDL_Rect rect = object;
    rect.x -= cameraX;
    rect.y -= cameraY;
    SDL_RenderCopy(renderer, objectImage, NULL, &rect);
}

void Environment::setPosition(int x, int y)
{
    object.x = x;
    object.y = y;
}

void Environment::size(int w, int h)
{
    object.w = w;
    object.h = h;
}

void Environment::drawStatic(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, objectImage, NULL, &object);
}

void background(SDL_Renderer *renderer, SDL_Rect background)
{
    SDL_SetRenderDrawColor(renderer, 0, 153, 153, 255);
    SDL_RenderFillRect(renderer, &background);
}

class Vector2{
    public:
    int x;
    int y;
};

int main( int argc, char* argv[] )
{
    srand(time(0));
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

    SDL_Rect BG = backgroundSetup();

    int labCount = 1;

    Player player;
    player.objectImage = IMG_LoadTexture(renderer, "assets/player/player.png");
    player.size(200,200);
    player.centered();
    player.setPosition(350, player.object.x);

    Entity enemy;
    enemy.objectImage = IMG_LoadTexture(renderer,"assets/enemy/enemy.png");
    enemy.size(200,200);
    enemy.setPosition(WIDTH/2, 550);

    int groundSize = 200;
    int groundSquareSide = 20;
    Ground grassy[groundSquareSide][groundSquareSide];
    for(int i=0;i<groundSquareSide;i++)
    {
        for(int j=0;j<groundSquareSide;j++)
        {
            grassy[i][j].objectImage = IMG_LoadTexture(renderer,"assets/environment/grass.png");
            grassy[i][j].size(groundSize,groundSize);
            grassy[i][j].setPosition(i*groundSize+75, j*groundSize+75);
        }
    }
    
    Vector2 labCoordinates[labCount];

    for(int i=0;i<labCount;i++)
    {
        int x = rand()%(groundSquareSide-1);
        int y = rand()%(groundSquareSide-1);
        labCoordinates[i].x = x;
        labCoordinates[i].y = y;
        if(!grassy[x][y].isLabEntrance)
            {
                grassy[x][y].isLabEntrance = true;
            }
    }

    int bushCount = 10;
    Environment bush[bushCount];
    for(int i=0;i<bushCount;i++)
    {
        bush[i].objectImage = IMG_LoadTexture(renderer,"assets/environment/bushes/bush1.png");
        bush[i].size(groundSize,groundSize);
        int x = rand()%(groundSquareSide * groundSize);
        int y = rand()%(groundSquareSide * groundSize);
        bush[i].setPosition(x,y);
    }

    int grassCount = 20;
    Environment grass[grassCount];
    for(int i=0;i<grassCount;i++)
    {
        grass[i].objectImage = IMG_LoadTexture(renderer,"assets/environment/grasses/short_grass#2.png");
        grass[i].size(groundSize,groundSize);
        int x = rand()%(groundSquareSide * groundSize);
        int y = rand()%(groundSquareSide * groundSize);
        grass[i].setPosition(x,y);
    }

    int treeCount = 5;
    Environment tree[treeCount];
    for(int i=0;i<treeCount;i++)
    {
        tree[i].objectImage = IMG_LoadTexture(renderer,"assets/environment/tree.png");
        tree[i].size(groundSize*2,groundSize*2);
        int x = rand()%(groundSquareSide * groundSize);
        int y = rand()%(groundSquareSide * groundSize);
        tree[i].setPosition(x,y);
    }

    Environment radar;
    radar.size(groundSize,groundSize);
    radar.setPosition(WIDTH/10,HEIGHT/3 * 2);
    radar.objectImage = IMG_LoadTexture(renderer,"assets/radar/radar.png");

    Environment radarDot;
    radarDot.size(groundSize/8,groundSize/8);
    radarDot.setPosition(radar.object.x + groundSize/2 - radarDot.object.w/2,radar.object.y + groundSize/2 - radarDot.object.h/2);
    radarDot.objectImage = IMG_LoadTexture(renderer,"assets/radar/dot.png");
    
    std::vector<Environment> hole;

    std::vector<Environment> lab;

    int speed = 1000;
    int enemySpeed = 1500;

    int currentTileX, currentTileY;

    bool playerFlipped = false;
    
    float delay = 2;
    float time = 0;

    uint64_t lastFrameTime = SDL_GetPerformanceCounter();

    int scena = 0;

    //Every frame:

    bool running = true;
    while (running)
    {
        uint64_t currentFrameTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentFrameTime - lastFrameTime) / (float)SDL_GetPerformanceFrequency();
        lastFrameTime = currentFrameTime;
        time += deltaTime;
        while(SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            { 
                running = false;
            }
            if(SDL_KEYDOWN == event.type && event.key.keysym.sym == SDLK_q)
            {
                scena++;
            }
        }
        //std::cout << abs(player.x/groundSize - labCoordinates[0].x) << ": " << abs(player.y/groundSize - labCoordinates[0].y) << "\n";

        const uint8_t* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_D])
        {
            playerFlipped = false;
            cameraX += speed * deltaTime;
        }
        if (keys[SDL_SCANCODE_A])
        {
            playerFlipped = true;
            cameraX -= speed * deltaTime;
            SDL_RenderCopyEx(renderer,player.objectImage,NULL,&player.object,15,NULL,SDL_FLIP_HORIZONTAL);
        }
        if (keys[SDL_SCANCODE_W])
        {
            cameraY -= speed * deltaTime;
        }
        if (keys[SDL_SCANCODE_S])
        {
            cameraY += speed * deltaTime;
        }
        
        if(keys[SDL_SCANCODE_E] && delay + 2 <= time)
        {
            delay = time;
            currentTileX = player.x/groundSize;
            currentTileY = player.y/groundSize;
            if(grassy[currentTileX][currentTileY].isLabEntrance)
                {
                    Environment newLab;
                    newLab.setPosition(currentTileX*groundSize + groundSize/2,currentTileY*groundSize + groundSize/2);
                    newLab.size(groundSize,groundSize);
                    newLab.objectImage = IMG_LoadTexture(renderer, "assets/environment/lab_entrance.png");
                    lab.push_back(newLab);
                    scena++;
                }
            else if(!grassy[currentTileX][currentTileY].isHole)
                {
                    Environment newHole;
                    newHole.setPosition(currentTileX*groundSize + groundSize/2,currentTileY*groundSize + groundSize/2);
                    newHole.size(groundSize,groundSize);
                    int i=rand()%(3);
                    int j=rand()%(4);
                    if(i==0)
                        newHole.objectImage = IMG_LoadTexture(renderer,"assets/environment/hole.png");
                    else if(i==1)
                        newHole.objectImage = IMG_LoadTexture(renderer,"assets/environment/hole1.png");
                    else if(i==2)
                        newHole.objectImage = IMG_LoadTexture(renderer,"assets/environment/hole2.png");
                    SDL_RenderCopyEx(renderer, newHole.objectImage,NULL,& newHole.object,j*90,NULL,SDL_FLIP_NONE);
                    hole.push_back(newHole);
                    grassy[currentTileX][currentTileY].isHole = true;
                }
        }

        player.x = cameraX + WIDTH/2 - player.object.w/2;
        player.y = cameraY + HEIGHT/2 - player.object.h/4;

        SDL_RenderClear(renderer);

        if(scena %2 == 0)
        {
            background(renderer, BG);
            
            for(int i=0;i<groundSquareSide;i++)
            {
                for(int j=0;j<groundSquareSide;j++)
                    grassy[i][j].draw(renderer);
            }

            for(int i=0;i<hole.size();i++)
            {
                hole[i].draw(renderer);
            }

            for(int i=0;i<lab.size();i++)
            {
                lab[i].draw(renderer);
            }

            for(int i=0;i<grassCount;i++)
            {
                grass[i].draw(renderer);
            }

            enemy.draw(renderer);

            if(!playerFlipped)
                player.draw(renderer);
            else if(playerFlipped)
                player.drawFlipped(renderer);

            for(int i=0;i<bushCount;i++)
            {
                bush[i].draw(renderer);
            }

            for(int i=0;i<treeCount;i++)
            {
                tree[i].draw(renderer);
            }

            radar.drawStatic(renderer);
            
            for(int i=0;i<labCount;i++)
            {
                if(abs(player.y/groundSize - labCoordinates[i].y) <= 8 && abs(player.x/groundSize - labCoordinates[i].x) <= 8)
                    {
                        int offX = 0, offY = 0;
                        offX = (radar.object.x + groundSize - radar.object.w/2 - radarDot.object.w/2) + 8*(labCoordinates[0].x - player.x/groundSize);
                        offY = (radar.object.y + groundSize - radar.object.h/2 - radarDot.object.h/2) + 8*(labCoordinates[0].y - player.y/groundSize);
                        radarDot.setPosition(offX,offY);
                        radarDot.drawStatic(renderer);
                    }
            }
        }
        else if(scena==1)
        {
            
        }

        //std::cout << labCoordinates[0].x - player.x/groundSize << ", " << labCoordinates[0].y - player.y/groundSize << std::endl;
        //std::cout << player.x/groundSize << ", " << player.y/groundSize << std::endl;

        SDL_RenderPresent(renderer);

        if(abs(player.x-enemy.object.x)<400 || abs(player.y-enemy.object.y)<400)
            enemy.setDestination(player.x, player.y, enemySpeed, deltaTime);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}