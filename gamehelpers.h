#include "game.h"

#define PIXEL_HEIGHT 480
#define PIXEL_WIDTH 640

void restoreBall(ball_t *ball);

void make_move(game_t* game);

int check_if_scored(game_t* game);

void record_score(game_t *game);
