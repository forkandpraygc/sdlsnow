#include "snowflake.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int snowFlakesCount = 200;

SnowFlake Snow[snowFlakesCount];

SDL_Surface* screen_surface = nullptr;
SDL_Window* window = nullptr;
SDL_Renderer *renderer = nullptr;


void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

SDL_Texture* loadImage(std::string file) {
    SDL_Texture *texture = nullptr;
    texture = IMG_LoadTexture(renderer, file.c_str());

    if (texture == nullptr) {
        std::cout << SDL_GetError() << std::endl;
    }

    return texture;
}

void applySurface(int x, int y, SDL_Texture *texture, SDL_Renderer *renderer) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &pos.w, &pos.h);
    SDL_RenderCopy(renderer, texture, nullptr, &pos);

}

void cleanUp() {
    SDL_RenderClear(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SnowFlake makeSnowFlake() {
    int maxSpeed = 5;
    SnowFlake snowFlake;
    int bounds = 30;
    int maxSize = 15;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution x_gen(-bounds, SCREEN_WIDTH);

    snowFlake.X = x_gen(gen);
    // snowFlake.Y = rand() % ((this->width() - 0) + 1) + 0;
    snowFlake.Y = -maxSize;

    std::uniform_real_distribution<double> dis(0.0, 1.0);
    double randomDouble = dis(gen);
    snowFlake.speed = 0.5  + randomDouble * maxSpeed;

    std::uniform_int_distribution<int> size_dis(5, maxSize);
    snowFlake.size = size_dis(gen);
    snowFlake.time = dis(gen) * M_PI;

    std::uniform_real_distribution<double> dis_time_delta(0.0, 0.0015);
    snowFlake.timeDelta = dis_time_delta(gen);
    return snowFlake;
}

void makeSnow() {
    for(int i = 0; i < snowFlakesCount; i++) {
        Snow[i] = makeSnowFlake();
    }
}

void paintSnowFlakes() {
    int X, Y, size;
    double deltaX;
    for(int i = 0; i < snowFlakesCount; i++) {
        auto time = Snow[i].time;
        deltaX = sin(time * 27) + sin(time * 21.3) + 3 * sin(time * 18.75) + 7 * sin(time * 7.6) + 10 * sin(time * 5.23);
        deltaX *= 10;

        X = Snow[i].X + deltaX;
        Y = Snow[i].Y;

        DrawCircle(X, Y, 10);
    }
}

int main (int argc, char **args) {

    makeSnow();

    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }

    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        return -1;
    }

    window = SDL_CreateWindow("Hello, Snow",SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return -1;
    }


    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Event e;

    bool quit = false;

    while (!quit)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        for(int i = 0; i < snowFlakesCount; i++) {
            Snow[i].Y += Snow[i].speed;
            if(Snow[i].Y > SCREEN_HEIGHT) {
                Snow[i] = makeSnowFlake();
            }
            Snow[i].time += Snow[i].timeDelta;
        }
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        paintSnowFlakes();

        SDL_RenderPresent(renderer);
    }

    cleanUp();

    return 0;
};
