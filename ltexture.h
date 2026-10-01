#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL.h>

#include <string>
class LTexture
{
public:
    LTexture(SDL_Renderer *renderer);

    int width() const;

    int height() const;

    // Dealocate the memory???
    void free();

    void render(int x, int y);

    bool loadFromFile(std::string path);

    ~LTexture();

private:
    SDL_Texture *mTexture = nullptr;
    SDL_Renderer *mRenderer = nullptr;
    int mWidth = 0;
    int mHeight = 0;
};

#endif // LTEXTURE_H
