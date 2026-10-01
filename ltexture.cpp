#include "ltexture.h"
#include <SDL2/SDL_image.h>
#include <iostream>

LTexture::LTexture(SDL_Renderer *renderer) {
    mRenderer = renderer;
}


bool LTexture::loadFromFile(std::string path) {
    free();
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {
        std::cerr << "Error " << IMG_GetError();
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    }
    auto newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (newTexture == NULL) {
        std::cerr << "Error " << IMG_GetError();
    } else {
        mWidth = loadedSurface->w;
        mHeight = loadedSurface->h;
    }

    SDL_FreeSurface(loadedSurface);
    mTexture = newTexture;

    return mTexture != NULL;
}

void LTexture::render(int x, int y) {
    SDL_Rect renderRect = {x, y, mWidth, mHeight};
    SDL_RenderCopy(mRenderer, mTexture, NULL, &renderRect);
}

void LTexture::free() {
    if(mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

int LTexture::width() const
{
    return mWidth;
}

int LTexture::height() const
{
    return mHeight;
}
