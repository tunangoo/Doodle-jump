
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

const int n_menu = 2, sizez = 60;

struct icon{
    SDL_Rect rect;
    SDL_Texture* red;
    SDL_Texture* yellow;
    bool is_choose, is_click;
} a[n_menu];

void InitIcon( icon& a, int x, int y, string title, int sizez )
{
    a.is_choose = a.is_click = false;
    a.red = loadTexture( DrawText( title, RED_COLOR, "Roboto-Bold.ttf", sizez ), renderer );
    a.yellow = loadTexture( DrawText( title, YELLOW_COLOR, "Roboto-Bold.ttf", sizez), renderer );
    getRect( a.yellow, a.rect, x, y );
}

bool CheckMouse( int x, int y, SDL_Rect t )
{
    if(x >= t.x && y >= t.y && x <= t.x + t.w && y <= t.y + t.h) return 1;
    return 0;
}

void MouseMotion()
{
    while( SDL_PollEvent( &e ) != 0 )
    {
        switch( e.type )
        {
        case SDL_QUIT:
            quit = true;
            return;
        break;
        case SDL_MOUSEMOTION:
            {
                int x = e.motion.x;
                int y = e.motion.y;
                for( int i = 0; i < n_menu; i++ )
                if( CheckMouse( x, y, a[i].rect ) )
                    a[i].is_choose = true;
                else a[i].is_choose = false;
            }
        break;
        case SDL_MOUSEBUTTONDOWN:
            {
                int x = e.motion.x;
                int y = e.motion.y;
                for( int i = 0; i < n_menu; i++ )
                if( CheckMouse( x, y, a[i].rect ) )
                    a[i].is_click = true;
                else a[i].is_click = false;
            }
        break;
        default:
        break;
        }
    }
}

bool SetMenu()
{
    SDL_Texture* background_menu = loadTexture( loadIMG( "images/background_menu.jpg" ), renderer );
    InitIcon( a[0], 40, 200, "Play Game", sizez );
    InitIcon( a[1], 40, 300, "Exit", sizez );
    while(true)
    {
        MouseMotion();
        Render( background_menu, 0, 0, renderer);
        for( int i = 0; i < n_menu; i++ )
        if( a[i].is_choose == true )
        {
            Render( a[i].red, a[i].rect.x, a[i].rect.y, renderer );
            if( a[i].is_click == true )
            {
                Mix_PlayChannel( -1, mouse_click, 0 );
                SDL_Delay(100);
                return i;
            }
        }
        else Render( a[i].yellow, a[i].rect.x, a[i].rect.y, renderer );
        SDL_RenderPresent( renderer );
    }
    return 1;
}

int nplat = 10, size_menu = 70, h = 200, direction;
int max_plat_move = 4, high_score = 0, score, current_plat;
int frame_start, frame_time;
double speed = 0.5, distance_move = 8, acc_y;
int x, y;
string key;

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

void HandleInputAction()
{
    if(e.type == SDL_KEYDOWN)
    {
        if ( e.key.keysym.sym == SDLK_RIGHT )
        {
            key = "right";
        }
        else if ( e.key.keysym.sym == SDLK_LEFT )
        {
            key = "left";
        }
    }
    else if(e.type == SDL_KEYUP)
    {
        if ( e.key.keysym.sym == SDLK_RIGHT )
        {
            key = "none";
        }
        else if ( e.key.keysym.sym == SDLK_LEFT )
        {
            key = "none";
        }
    }
    if(key == "left")
    {
        x -= distance_move;
        if( direction == 1 )
        {
            x -= 8;
        }
        direction = 0;
    }
    else if(key == "right")
    {
        x += distance_move;
        if( direction == 0 )
        {
            x += 8;
        }
        direction = 1;
    }
}

void PlayGame()
{
    Mix_PlayChannel( 0, music2, -1 );

    acc_y = 0; score = 0; current_plat = -1;
    quit = false;
    point plat[nplat];
    for ( int i = 0; i < nplat; i++ )
    {
        plat[i].gen( i * SCREEN_HEIGHT / nplat, ( i + 1 ) * SCREEN_HEIGHT / nplat );
    }
    plat[nplat / 2].x = SCREEN_WIDTH / 2;
    plat[nplat / 2].type = 0;
    x = plat[nplat / 2].x;
    y = plat[nplat / 2].y - 100;
    direction = 1;
    while (!quit)
    {
        frame_start = SDL_GetTicks();
        while( SDL_PollEvent( &e ) != 0 )
        {
            if ( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else HandleInputAction();
        }
        int left_foot, right_foot;
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
            left_foot = x + 27;;
            right_foot = left_foot + 34;
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
            left_foot = x + 19;
            right_foot = left_foot + 34;
        }

        for( int i = 0; i < nplat; i++ )
            plat[i].y += speed;
        y += speed;

        acc_y += 0.2;
        y += acc_y;
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
        {
            for ( int i = 0; i < nplat; i++ )
            {
            y = h;
                plat[i].y = plat[i].y - acc_y;
                if ( plat[i].y > SCREEN_HEIGHT )
                {
                    plat[i].gen( -20, 0 );
                }
            }
        }

        for ( int i = 0; i < nplat; i++ )
        if( plat[i].color == "green" || plat[i].touch == 0 )
            if ( ( right_foot > plat[i].x ) && ( left_foot < plat[i].x + PLAT_WIDTH )
                    && ( y + 70 > plat[i].y ) && ( y + 70 < plat[i].y + PLAT_HEIGHT ) && ( acc_y > 0 ) )
                    {
                        acc_y = -10;
                        if( current_plat != i )
                        {
                            if( plat[i].color == "blue" ) plat[i].touch = 1;
                            score++;
                            current_plat = i;
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
        ss << score;
        string textpoint = "Score: " + ss.str();
        Render( loadTexture( DrawText( textpoint, YELLOW_COLOR, "Roboto-Bold.ttf", 30 ), renderer ), 190, 0, renderer );

		if( y > SCREEN_HEIGHT - 70 )
        {
            high_score = max( high_score, score );
            Mix_HaltChannel( 0 );
            return;
        }

        SDL_RenderPresent( renderer );
        frame_time = SDL_GetTicks() - frame_start;
        //cout << frame_time << " " << key << '\n';
		if ( frame_time < DELAY_TIME )
		{
			SDL_Delay( DELAY_TIME - frame_time );
		}
    }
}

void GameOver()
{

}

int main( int argc, char* argv[] )
{
    initData();

    background = loadTexture( loadIMG( "images/background.jpg" ), renderer );
    green_platform = loadTexture( loadIMG( "images/greenplatform.png" ), renderer );
    blue_platform = loadTexture( loadIMG( "images/blueplatform.png" ), renderer );
    doodle_left = loadTexture( loadIMG( "images/doodle_left.png" ), renderer );
    doodle_right = loadTexture( loadIMG( "images/doodle_right.png" ), renderer );

    jump_effect = Mix_LoadWAV( "sounds/jump.wav" );
    lofi = Mix_LoadWAV( "sounds/lofi.wav" );
    mouse_click = Mix_LoadWAV( "sounds/mouse_click.wav" );
    music = Mix_LoadWAV( "sounds/music.wav" );
    music2 = Mix_LoadWAV( "sounds/music2.wav" );

    Mix_PlayChannel( 0, music, -1 );
    quit = SetMenu();
    Mix_HaltChannel( 0 );

    while(!quit)
    {
        PlayGame();
        GameOver();
    }

	Mix_CloseAudio();
	//Destroy a window.
	SDL_DestroyWindow(window);

	//Destroy a renderer
	SDL_DestroyRenderer(renderer);

	//cleans up all initialized subsystems
	SDL_Quit();

    //waitUntilKeyPressed();

    return 0;
}
