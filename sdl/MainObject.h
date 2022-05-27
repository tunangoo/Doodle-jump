
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_
#include "CommonFunc.h"
#include "BaseObject.h"
void InitData( SDL_Window*& window, SDL_Renderer*& renderer );
void ClearData( SDL_Window*& window, SDL_Renderer*& renderer);
long long Rand( long long l, long long r );
string IntToString( int x );
bool MouseSelect( int x, int y, SDL_Rect t );
void InitIcon( icon& a, int x, int y, string title, int sizez, int type, SDL_Renderer* renderer );
void HandleInputAction( SDL_Event& e, string& key, bool& quit );
void MouseMotion( icon* a, int n, bool& quit );
void GenPlatform( platform& a, int l, int r );

#endif // MAIN_OBJECT_H_
