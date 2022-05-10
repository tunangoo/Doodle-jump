#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_image.h>

using namespace std;

SDL_Window* window = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* background = NULL;
SDL_Renderer* renderer;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;
const int SCREEN_FPS = 60;
const int DELAY_TIME = 1000 / SCREEN_FPS;

struct point{
    int x, y;
    int type;
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

    //theo right direction trai cach 19, phai cach 27, do dai chan la 34;
    int x = 100 , y = 100, h = 200, direction = 1;
    float dx = 0, dy = 0;
    int framestart, frametime;
    int nplat = 14;
    int Point = 0;
    bool quit = 0;
    SDL_Event e;
    for ( int i = 0; i < nplat; i++ )
    {
        plat[i].x = rand() % ( SCREEN_WIDTH - 68 );
        plat[i].y = rand() % SCREEN_HEIGHT / nplat + i * SCREEN_HEIGHT / nplat;
    }
    while (!quit)
    {
        int framestart = SDL_GetTicks();
        while( SDL_PollEvent( &e ) != 0 )
        {
            if ( e.type == SDL_QUIT )
            {
                quit = true;
            }
            if ( e.key.keysym.sym == SDLK_RIGHT )
            {
                x += 5;
                if( direction == 0 )
                {
                    x += 8;
                }
                direction = 1;
            }
            if ( e.key.keysym.sym == SDLK_LEFT )
            {
                x -= 5;
                if( direction == 1 )
                {
                    x -= 8;
                }
                direction = 0;
            }
        }
        int leftfoot, rightfoot;
        if( direction == 0 )
        {
            if( x + 27 < 0 )
            {
                x = -27;
            }
            else if( x + 27 + 34 > SCREEN_WIDTH )
            {
                x = SCREEN_WIDTH - 27 - 34;
            }
            leftfoot = x + 27;;
            rightfoot = leftfoot + 34;
        }
        else
        {
            if( x + 19 < 0 )
            {
                x = -19;
            }
            else if( x + 19 + 34 > SCREEN_WIDTH )
            {
                x = SCREEN_WIDTH - 19 - 34;
            }
            leftfoot = x + 19;
            rightfoot = leftfoot + 34;
        }
        dy += 0.2;
        y += dy;
        //if ( y > 670 )  dy = -10;

        if ( y < h )
            for ( int i = 0; i < nplat; i++ )
            {
                y = h;
                plat[i].y = plat[i].y - dy;
                if (plat[i].y > SCREEN_HEIGHT)
                {
                    plat[i].y = 0;
                    plat[i].x = rand() % ( SCREEN_WIDTH - 68 );
                }
            }

        for ( int i = 0; i < nplat; i++ )
            if ( ( rightfoot > plat[i].x ) && ( leftfoot < plat[i].x + 68 )
                    && ( y + 70 > plat[i].y ) && ( y + 70 < plat[i].y + 14 ) && ( dy > 0 ) )  dy = -10;

        Render( t1, 0, 0 );

        for ( int i = 0; i < nplat; i++ )
        {
            Render( t2, plat[i].x, plat[i].y );
        }

        if ( !direction )
        {
            Render( t3, x, y );
        }
        else
        {
            Render( t4, x, y );
        }
        SDL_RenderPresent( renderer );

        frametime = SDL_GetTicks() - framestart;
		if (frametime < DELAY_TIME)
		{
			SDL_Delay(DELAY_TIME - frametime);
		}
		if(y > SCREEN_HEIGHT)
        {
            cout << "game over!";
            return 0;
        }

    }

    waitUntilKeyPressed();

    return 0;
}
