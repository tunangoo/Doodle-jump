
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "CommonFunc.h"

SDL_Surface* LoadIMG( string path );
SDL_Surface* DrawText( string text, SDL_Color color, const char *font, int textsize );
SDL_Texture* LoadTexture( SDL_Surface* loadedSurface, SDL_Renderer* renderer );
void GetRect( SDL_Texture* t, SDL_Rect& tRect, int x, int y );
void Render( SDL_Texture* t, int x, int y, SDL_Renderer* renderer );
void WaitUntilKeyPressed();
void RenderRectangle( SDL_Rect rect, int r, int g, int b, SDL_Renderer* renderer );
void RectangleOutline( SDL_Rect rect, int r, int g, int b, SDL_Renderer* renderer );
#endif // BASE_OBJECT_H_
