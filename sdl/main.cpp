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
const int DELAY_TIME = 1000 / SCREEN_FPS;

struct point{
    int x, y;
};

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
        SDL_Delay( 100 );
    }
}

void Render( SDL_Texture* t, int x, int y )
{
    SDL_Rect tRect;
    SDL_QueryTexture( t, NULL, NULL, &tRect.w, &tRect.h );
    tRect.x = x;
    tRect.y = y;
    SDL_RenderCopy( renderer, t, NULL, &tRect );
}
int main( int argc, char* argv[] )
{
    srand( time( NULL ) );
    window = SDL_CreateWindow( "Doodle game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC );

    SDL_Texture* t1 = loadTexture( "images/background.jpg", renderer );
    SDL_Texture* t2 = loadTexture( "images/platform.png", renderer );
    SDL_Texture* t3 = loadTexture( "images/doodle_left.png", renderer );
    SDL_Texture* t4 = loadTexture( "images/doodle_right.png", renderer );

    point plat[20];

    for ( int i = 0; i < 10; i++ )
    {
        plat[i].x=rand()%SCREEN_WIDTH;
        plat[i].y=rand()%SCREEN_HEIGHT;
    }

    int x = 100, y = 100, h = 200, direction;
    float dx = 0, dy = 0;

    bool quit = 0;
    SDL_Event e;
    while (!quit)
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if ( e.type == SDL_QUIT )
            {
                quit = true;
            }
            if ( e.key.keysym.sym == SDLK_RIGHT )
            {
                x += 5;
                direction = 1;
            }
            if ( e.key.keysym.sym == SDLK_LEFT )
            {
                x -= 5;
                direction = 0;
            }
        }

        dy += 0.2;
        y += dy;
        if ( y > 500 )  dy = -10;

        if ( y < h )
            for ( int i = 0; i < 10; i++ )
            {
                y = h;
                plat[i].y = plat[i].y - dy;
                if (plat[i].y > SCREEN_HEIGHT)
                {
                    plat[i].y = 0;
                    plat[i].x = rand() % SCREEN_WIDTH;
                }
            }

        for ( int i = 0; i < 10; i++ )
            if ( ( x + 50 > plat[i].x ) && ( x + 20 < plat[i].x + 68 )
                    && ( y + 70 > plat[i].y ) && ( y + 70 < plat[i].y + 14 ) && ( dy > 0 ) )  dy = -10;

        Render( t1, 0, 0 );
        if ( !direction )
        {
            Render( t3, x, y );
        }
        else
        {
            Render( t4, x, y );
        }

        for ( int i = 0; i < 10; i++ )
        {
            Render( t2, plat[i].x, plat[i].y );
        }
        SDL_RenderPresent( renderer );
    }

    waitUntilKeyPressed();

    return 0;
}
