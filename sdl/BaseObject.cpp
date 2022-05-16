
#include "baseObject.h"

SDL_Surface* loadIMG( string path )
{
    return IMG_Load( path.c_str() );
}

SDL_Surface* DrawText( string text, SDL_Color color, const char *font, int textsize )
{
    TTF_Font* textFont = NULL;
    textFont = TTF_OpenFont( font, textsize );
    return TTF_RenderText_Solid( textFont, text.c_str(), color );
}

SDL_Texture* loadTexture( SDL_Surface* loadedSurface, SDL_Renderer* renderer )
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

void Render( SDL_Texture* t, int x, int y, SDL_Renderer* renderer )
{
    SDL_Rect tRect;
    SDL_QueryTexture( t, NULL, NULL, &tRect.w, &tRect.h );
    tRect.x = x;
    tRect.y = y;
    SDL_RenderCopy( renderer, t, NULL, &tRect );
}
