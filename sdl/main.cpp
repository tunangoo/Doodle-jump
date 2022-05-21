
#include "CommonFunc.h"
#include "baseObject.h"

void initData()
{
	srand( time( NULL ) );
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Unable to initialize SDL %s\n", SDL_GetError());
		exit(0);
	}

	//Initialize the truetype font API.
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		exit(0);
	}

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }

    window = SDL_CreateWindow( "Doodle Jump", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
}

long long Rand( long long l, long long r )
{
    return l + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) +
                rand())%(r-l+1);
}


struct point{
    double x, y;
    int type, touch;
    string color;
    double movepf;
    void gen(int l, int r)
    {
        x = Rand(0, SCREEN_WIDTH - PLAT_WIDTH);
        y = Rand(l, r);
        type = Rand(0, 1);
        type = (type == 0) ? Rand(-1, 1) : 0;
        int cl = Rand(0, 2);
        if(cl == 0) color = "blue"; else color = "green";
        movepf = (double)Rand(1, max_plat_move * 10) / 10;
        touch = 0;
    }
};



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



void playgame()
{
    Mix_PlayChannel( 0, lofi, -1 );

    //theo right direction trai cach 19, phai cach 27, do dai chan la 34;
    double dx = 0, dy = 0;
    point plat[nplat];
    int Score = 0, localplat = -1;
    bool quit = 0;
    SDL_Event e;
    for ( int i = 0; i < nplat; i++ )
    {
        plat[i].gen( i * SCREEN_HEIGHT / nplat, ( i + 1 ) * SCREEN_HEIGHT / nplat );
    }
    plat[nplat / 2].x = SCREEN_WIDTH / 2;
    plat[nplat / 2].type = 0;
    int x = plat[nplat / 2].x , y = plat[nplat / 2].y - 100, h = 200, direction = 1;
    while (!quit)
    {
        framestart = SDL_GetTicks();
        while( SDL_PollEvent( &e ) != 0 )
        {
            if ( e.type == SDL_QUIT )
            {
                quit = true;
            }
            if ( e.key.keysym.sym == SDLK_RIGHT )
            {
                x += distance_move;
                if( direction == 0 )
                {
                    x += 8;
                }
                direction = 1;
            }
            else if ( e.key.keysym.sym == SDLK_LEFT )
            {
                x -= distance_move;
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

        for( int i = 0; i < nplat; i++ )
            plat[i].y += speed;
        y += speed;

        dy += 0.2;
        y += dy;
        //if ( y > 670 )  dy = -10;


        for( int i = 0; i < nplat; i++ )
        {
            plat[i].x += plat[i].type * plat[i].movepf;
            if(plat[i].x + PLAT_WIDTH > SCREEN_WIDTH)
            {
                plat[i].x = SCREEN_WIDTH * 2 - (plat[i].x + PLAT_WIDTH) - PLAT_WIDTH;
                plat[i].type *= -1;
            }
            else if(plat[i].x < 0)
            {
                plat[i].x *= -1;
                plat[i].type *= -1;
            }
        }

        if ( y < h )
            for ( int i = 0; i < nplat; i++ )
            {
                y = h;
                plat[i].y = plat[i].y - dy;
                if ( plat[i].y > SCREEN_HEIGHT )
                {
                    plat[i].gen( -20, 0 );
                }
            }

        for ( int i = 0; i < nplat; i++ )
        if( plat[i].color == "green" || plat[i].touch == 0 )
            if ( ( rightfoot > plat[i].x ) && ( leftfoot < plat[i].x + PLAT_WIDTH )
                    && ( y + 70 > plat[i].y ) && ( y + 70 < plat[i].y + PLAT_HEIGHT ) && ( dy > 0 ) )
                    {
                        dy = -10;
                        if( localplat != i )
                        {
                            if( plat[i].color == "blue" ) plat[i].touch = 1;
                            Score++;
                            localplat = i;
                            Mix_PlayChannel( -1, jump_effect, 0 );
                        }
                    }

        Render( background, 0, 0, renderer );

        for ( int i = 0; i < nplat; i++ )
        {
            if( plat[i].color == "blue" && plat[i].touch < 2 ) Render( blue_platform, plat[i].x, plat[i].y, renderer );
            else if(plat[i].color == "green") Render( green_platform, plat[i].x, plat[i].y, renderer );
            if(plat[i].touch == 1) plat[i].touch++;
        }

        if ( !direction )
        {
            Render( doodle_left, x, y, renderer );
        }
        else
        {
            Render( doodle_right, x, y, renderer );
        }
        stringstream ss;
        ss << Score;
        string textpoint = "Score: " + ss.str();
        Render( loadTexture( DrawText( textpoint, YELLOW_COLOR, "RobotoCondensed-BoldItalic.ttf", 30 ), renderer ), 190, 0, renderer );

		if(y > SCREEN_HEIGHT - 70)
        {
            highScore = max(highScore, Score);
            Mix_HaltChannel( 0 );
            //return;
        }

        SDL_RenderPresent( renderer );

        frametime = SDL_GetTicks() - framestart;
		if (frametime < DELAY_TIME)
		{
			SDL_Delay(DELAY_TIME - frametime);
		}
    }
}

int main( int argc, char* argv[] )
{
    initData();

    background = loadTexture( loadIMG( "images/background.jpg" ), renderer );
    green_platform = loadTexture( loadIMG( "images/greenplatform.png" ), renderer );
    blue_platform = loadTexture( loadIMG( "images/blueplatform.png" ), renderer );
    doodle_left = loadTexture( loadIMG( "images/doodle_left.png" ), renderer );
    doodle_right = loadTexture( loadIMG( "images/doodle_right.png" ), renderer );
    play_game_red = loadTexture( DrawText( "Play", RED_COLOR, "RobotoCondensed-BoldItalic.ttf", size_menu), renderer );
    play_game_yellow = loadTexture( DrawText( "Play", YELLOW_COLOR, "RobotoCondensed-BoldItalic.ttf", size_menu), renderer );
    exit_game_red = loadTexture( DrawText( "Exit", RED_COLOR, "RobotoCondensed-BoldItalic.ttf", size_menu), renderer );
    exit_game_yellow = loadTexture( DrawText( "Exit", YELLOW_COLOR, "RobotoCondensed-BoldItalic.ttf", size_menu), renderer );

    jump_effect = Mix_LoadWAV( "sounds/jump.wav" );
    lofi = Mix_LoadWAV( "sounds/lofi.wav" );

    playgame();


	Mix_CloseAudio();
	//Destroy a window.
	SDL_DestroyWindow(window);

	//Destroy a renderer
	SDL_DestroyRenderer(renderer);

	//cleans up all initialized subsystems
	SDL_Quit();

    waitUntilKeyPressed();

    return 0;
}
