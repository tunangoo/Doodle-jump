
#include "MainObject.h"

void InitData( SDL_Window*& window, SDL_Renderer*& renderer )
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

void ClearData(SDL_Window*& window, SDL_Renderer*& renderer)
{
    Mix_CloseAudio();

	//Destroy a window.
	SDL_DestroyWindow( window );

	//Destroy a renderer
	SDL_DestroyRenderer( renderer );

	//cleans up all initialized subsystems
	SDL_Quit();

    //waitUntilKeyPressed();
}

long long Rand( long long l, long long r )
{
    return l + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) +
                rand())%(r-l+1);
}

string IntToString( int x )
{
    stringstream s;
    s << x;
    return s.str();
}

bool MouseSelect( int x, int y, SDL_Rect t )
{
    if(x >= t.x && y >= t.y && x <= t.x + t.w && y <= t.y + t.h) return 1;
    return 0;
}

void InitIcon( icon& a, int x, int y, string title, int sizez, int type, SDL_Renderer* renderer )
{
    a.is_select = a.is_click = false;
    a.type = type;
    a.red = LoadTexture( DrawText( title, RED_COLOR, "Roboto-Bold.ttf", sizez ), renderer );
    a.yellow = LoadTexture( DrawText( title, YELLOW_COLOR, "Roboto-Bold.ttf", sizez), renderer );
    GetRect( a.yellow, a.rect, x, y );
}

void HandleInputAction( SDL_Event& e, string& key, bool& quit )
{
    while( SDL_PollEvent( &e ) != 0 )
    {
        switch( e.type )
        {
        case SDL_QUIT:
            quit = true;
        break;
        case SDL_KEYDOWN:
        {
            switch( e.key.keysym.sym )
            {
            case SDLK_RIGHT:
                key = "right";
            break;
            case SDLK_LEFT:
                key = "left";
            break;
            default:
            break;
            }
        }
        break;
        case SDL_KEYUP:
        {
            if(e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_LEFT)
                key = "none";
        }
        break;
        default:
        break;
        }
    }
}

void MouseMotion( icon* a, int n, bool& quit )
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
                for( int i = 0; i < n; i++ )
                if( MouseSelect( x, y, a[i].rect ) )
                    a[i].is_select = true;
                else a[i].is_select = false;
            }
        break;
        case SDL_MOUSEBUTTONDOWN:
            {
                int x = e.motion.x;
                int y = e.motion.y;
                for( int i = 0; i < n; i++ )
                if( MouseSelect( x, y, a[i].rect ) )
                    a[i].is_click = true;
                else a[i].is_click = false;
            }
        break;
        default:
        break;
        }
    }
}

void GenPlatform( platform& a, int l, int r )
{
    a.x = Rand(0, SCREEN_WIDTH - PLAT_WIDTH);
    a.y = Rand(l, r);
    a.type = Rand(0, 1);
    a.type = (a.type == 0) ? Rand(-1, 1) : 0;
    int cl = Rand(0, 2);
    if(cl == 0) a.color = "blue"; else a.color = "green";
    a.movepf = (double)Rand(1, max_plat_move * 10) / 10;
    a.touch = 0;
}

