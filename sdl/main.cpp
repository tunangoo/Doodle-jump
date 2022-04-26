#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>

using namespace std;

SDL_Window* window = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* background = NULL;
SDL_Renderer* renderer;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 533;
const int SCREEN_FPS = 60;

SDL_Texture* loadTexture( string path, SDL_Renderer* renderer )
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == nullptr )
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == nullptr )
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    background = IMG_Load("images/background.jpg");
    window = SDL_CreateWindow( "Doodle game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    ScreenSurface = SDL_GetWindowSurface(window);
    SDL_BlitSurface(background, NULL, ScreenSurface, NULL);
    SDL_UpdateWindowSurface(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* t1 = loadTexture("images/background.jpg", renderer);
    SDL_Texture* t2 = loadTexture("images/platform.png", renderer);
    SDL_Texture* t3 = loadTexture("images/doodle.png", renderer);

    waitUntilKeyPressed();

    return 0;
}
