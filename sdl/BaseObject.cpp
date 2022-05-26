
#include "baseObject.h"

SDL_Surface* LoadIMG( string path )
{
    return IMG_Load( path.c_str() );
}

SDL_Surface* DrawText( string text, SDL_Color color, const char *font, int textsize )
{
    TTF_Font* textFont = NULL;
    textFont = TTF_OpenFont( font, textsize );
    return TTF_RenderText_Solid( textFont, text.c_str(), color );
}

SDL_Texture* LoadTexture( SDL_Surface* loadedSurface, SDL_Renderer* renderer )
{
    SDL_Texture* newTexture = NULL;
    if ( loadedSurface == NULL )
        cout << "Unable to load image; SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
            cout << "Unable to create texture; SDL Error: "
                 << SDL_GetError() << endl;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

void GetRect( SDL_Texture* t, SDL_Rect& tRect, int x, int y)
{
    SDL_QueryTexture( t, NULL, NULL, &tRect.w, &tRect.h );
    tRect.x = x;
    tRect.y = y;
}

void Render( SDL_Texture* t, int x, int y, SDL_Renderer* renderer )
{
    SDL_Rect tRect;
    GetRect( t, tRect, x, y );
    SDL_RenderCopy( renderer, t, NULL, &tRect );
}

void WaitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay( 100 );
    }
}

void RenderRectangle( SDL_Rect rect, int r, int g, int b, SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, r, g, b, 0xFF );
    SDL_RenderFillRect( renderer, &rect );
}

void RectangleOutline( SDL_Rect rect, int r, int g, int b, SDL_Renderer* renderer )
{
    SDL_SetRenderDrawColor( renderer, r, g, b, 0xFF );
    SDL_RenderDrawRect( renderer, &rect );
}
