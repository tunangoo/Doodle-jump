
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
const SDL_Color BLACK_COLOR = {0, 0, 0};

static SDL_Texture* background_menu = NULL;
static SDL_Texture* background = NULL;
static SDL_Texture* green_platform = NULL;
static SDL_Texture* blue_platform = NULL;
static SDL_Texture* doodle_left = NULL;
static SDL_Texture* doodle_right = NULL;
static SDL_Event e;

static Mix_Chunk* jump_effect = NULL;
static Mix_Chunk* mouse_click = NULL;
static Mix_Chunk* sound1 = NULL;
static Mix_Chunk* sound2 = NULL;
static Mix_Chunk* sound3 = NULL;
static Mix_Chunk* lose = NULL;

TTF_Font* TTF_OpenFont(const char *file, int ptsize);
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;
const int SCREEN_FPS = 60;
const int DELAY_TIME = 1000 / SCREEN_FPS;
const int PLAT_WIDTH = 68;
const int PLAT_HEIGHT = 14;

struct icon{
    SDL_Rect rect;
    SDL_Texture* red;
    SDL_Texture* yellow;
    bool is_select, is_click;
    int type;
};

struct platform{
    double x, y;
    int type, touch;
    string color;
    double movepf;
};

const int max_plat_move = 4;
const int n_plat = 10, h = 200;
const double speed = 1, distance_move = 4;
static double x, y, acc_y;
static bool quit = false;
static platform plat[n_plat];
static int high_score = 0, score, direction;
static int frame_start, frame_time;

#endif // COMMON_FUNCTION_H_
