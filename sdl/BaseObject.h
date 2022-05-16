
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "CommonFunc.h"

SDL_Surface* loadIMG( string path );
SDL_Surface* DrawText( string text, SDL_Color color, const char *font, int textsize );
SDL_Texture* loadTexture( SDL_Surface* loadedSurface, SDL_Renderer* renderer );
void Render( SDL_Texture* t, int x, int y, SDL_Renderer* renderer );

#endif // BASE_OBJECT_H_
