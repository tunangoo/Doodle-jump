
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer;

const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};

static SDL_Texture* background = NULL;
static SDL_Texture* green_platform = NULL;
static SDL_Texture* blue_platform = NULL;
static SDL_Texture* doodle_left = NULL;
static SDL_Texture* doodle_right = NULL;
static SDL_Texture* play_game_red;
static SDL_Texture* play_game_yellow;
static SDL_Texture* exit_game_red;
static SDL_Texture* exit_game_yellow;

static Mix_Chunk* jump_effect = NULL;
static Mix_Chunk* lofi = NULL;

TTF_Font* TTF_OpenFont(const char *file, int ptsize);
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;
const int SCREEN_FPS = 60;
const int DELAY_TIME = 1000 / SCREEN_FPS;
const int PLAT_WIDTH = 68;
const int PLAT_HEIGHT = 14;

static int nplat = 10, size_menu = 70;
static int max_plat_move = 4, highScore = 0;
static double speed = 0.5, distance_move = 6;
static int framestart, frametime;

#endif // COMMON_FUNCTION_H_
