
#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"

int SetMenu()
{
    Mix_PlayChannel( 0, sound1, -1 );

    int n = 2;
    icon a[n];
    InitIcon( a[0], 40, 200, "Play Game", 60, 1, renderer );
    InitIcon( a[1], 40, 300, "Exit", 60, 1, renderer );

    while(true)
    {
        MouseMotion( a, 2, quit );
        Render( background_menu, 0, 0, renderer );
        for( int i = 0; i < n; i++ )
        if( a[i].is_select == true )
        {
            Render( a[i].red, a[i].rect.x, a[i].rect.y, renderer );
            if( a[i].is_click == true )
            {
                Mix_PlayChannel( -1, mouse_click, 0 );
                SDL_Delay(100);
                Mix_HaltChannel( 0 );
                return i;
            }
        }
        else Render( a[i].yellow, a[i].rect.x, a[i].rect.y, renderer );
        SDL_RenderPresent( renderer );
    }
    Mix_HaltChannel( 0 );
    return 1;
}

void PlayGame()
{
    Mix_PlayChannel( 0, sound2, -1 );

    string key;

    int current_plat = -1;
    acc_y = 0; score = 0;

    for( int i = 0; i < n_plat; i++ )
    {
        GenPlatform( plat[i], i * SCREEN_HEIGHT / n_plat, ( i + 1 ) * SCREEN_HEIGHT / n_plat );
    }
    plat[n_plat / 2].x = SCREEN_WIDTH / 2;
    plat[n_plat / 2].type = 0;
    x = plat[n_plat / 2].x;
    y = plat[n_plat / 2].y - 100;
    direction = 1;
    while (!quit)
    {
        frame_start = SDL_GetTicks();
        HandleInputAction( e, key, quit );
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

        for( int i = 0; i < n_plat; i++ )
            plat[i].y += speed;
        y += speed;

        acc_y += 0.2;
        y += acc_y;
        //if ( y > 670 )  dy = -10;


        for( int i = 0; i < n_plat; i++ )
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
            for ( int i = 0; i < n_plat; i++ )
            {
            y = h;
                plat[i].y = plat[i].y - acc_y;
                if ( plat[i].y > SCREEN_HEIGHT )
                {
                    GenPlatform(plat[i], -20, 0 );
                }
            }
        }

        for ( int i = 0; i < n_plat; i++ )
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

        for ( int i = 0; i < n_plat; i++ )
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
        Render( LoadTexture( DrawText( "Score: " + IntToString( score ), YELLOW_COLOR, "Roboto-Bold.ttf", 30 ), renderer ), 190, 0, renderer );

		if( y > SCREEN_HEIGHT - 70 )
        {
            high_score = max( high_score, score );
            Mix_HaltChannel( 0 );
            Mix_PlayChannel( 0, lose, 0);
            SDL_Delay(1000);
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

int GameOver()
{
    int n = 2;
    icon a[n];
    InitIcon( a[0], 140, 390, "Play again", 45, 1, renderer );
    InitIcon( a[1], 190, 450, "Home", 45, 1, renderer );

    Mix_PlayChannel( 0, sound3, -1 );

    while(!quit)
    {
        MouseMotion(a, n, quit);

        Render( background, 0, 0, renderer );
        for ( int i = 0; i < n_plat; i++ )
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
        Render( LoadTexture( DrawText( "Score: " + IntToString( score ), YELLOW_COLOR, "Roboto-Bold.ttf", 30 ), renderer ), 190, 0, renderer );

        RenderRectangle( {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 3 * 2, SCREEN_HEIGHT / 2}, 48, 48, 48, renderer );
        RectangleOutline( {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 3 * 2, SCREEN_HEIGHT / 2}, 128, 128, 0, renderer );

        Render( LoadTexture( DrawText( "Game Over!", YELLOW_COLOR, "Roboto-Bold.ttf", 60), renderer), 95, 180, renderer );
        Render( LoadTexture( DrawText( "Your high score: " + IntToString( high_score ), YELLOW_COLOR, "Roboto-BoldItalic.ttf", 30), renderer), 120, 280, renderer );
        Render( LoadTexture( DrawText( "Your score: " + IntToString( score ), YELLOW_COLOR, "Roboto-BoldItalic.ttf", 30), renderer), 150, 320, renderer );

        for( int i = 0; i < n; i++ )
        if( a[i].is_select == true )
        {
            Render( a[i].red, a[i].rect.x, a[i].rect.y, renderer );
            if( a[i].is_click == true )
            {
                Mix_PlayChannel( -1, mouse_click, 0 );
                SDL_Delay(100);
                Mix_HaltChannel( 0 );
                return i;
            }
        }
        else Render( a[i].yellow, a[i].rect.x, a[i].rect.y, renderer );

        SDL_RenderPresent( renderer );
    }
    Mix_HaltChannel( 0 );
    return 0;
}

int main( int argc, char* argv[] )
{
    InitData( window, renderer );

    background_menu = LoadTexture( LoadIMG( "images/background_menu.jpg" ), renderer );
    background = LoadTexture( LoadIMG( "images/background.jpg" ), renderer );
    green_platform = LoadTexture( LoadIMG( "images/greenplatform.png" ), renderer );
    blue_platform = LoadTexture( LoadIMG( "images/blueplatform.png" ), renderer );
    doodle_left = LoadTexture( LoadIMG( "images/doodle_left.png" ), renderer );
    doodle_right = LoadTexture( LoadIMG( "images/doodle_right.png" ), renderer );

    jump_effect = Mix_LoadWAV( "sounds/jump.wav" );
    mouse_click = Mix_LoadWAV( "sounds/mouse_click.wav" );
    sound1 = Mix_LoadWAV( "sounds/sound1.wav" );
    sound2 = Mix_LoadWAV( "sounds/sound2.wav" );
    sound3 = Mix_LoadWAV( "sounds/sound3.wav" );
    lose = Mix_LoadWAV( "sounds/lose.wav" );

    int status = 1;
    while( !quit )
    {
        if( status == 1 ) quit = SetMenu();
        if( !quit )
        {
            PlayGame();
            status = GameOver();
        }
    }

	ClearData( window, renderer );

    return 0;
}
