#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdbool.h>
#include "physics.h"
#include "gameconstants.h"
#include "game.h"
#include "uart.h"
#include "timer.h"
#include "physics.h"
#include "gamehelpers.h"

extern rpi_sys_timer_t* systemTimer;

//moves the ball back to the centre and randomises direction
void restoreBall(ball_t *ball) {

  ball->xPos = PIXEL_WIDTH / 2;
  ball->yPos = PIXEL_HEIGHT / 2;
  srand(systemTimer->counter_lo);
  int player = rand() % 2;
  int rrmax = RAND_MAX / 2;
  int random = rand() - rrmax;
  double randomRatio = (random * 0.74)  / rrmax;
  if(player == 0) {
    ball->dir = (PI / 2) * randomRatio;
  } else {
    ball->dir = PI - (PI / 2) * randomRatio;
  }
  regulateDir(&ball->dir);
}

//actually moves the ball
void make_move(game_t* game) {

  game->ball->xPos += (BALLSPEED * cos(game->ball->dir));

  game->ball->yPos += (BALLSPEED * sin(game->ball->dir));

}

int check_if_scored(game_t* game) {
  int x = game->ball->xPos;
  if(x - BALL_RADIUS <= 0) {
    return 1;
  } else if(x + BALL_RADIUS >= game->gameWidth) {
    return 2;
  }
  return 0;
}

//checks if a goal has been scored and resets the ball if so
void record_score(game_t *game) {
  int check = check_if_scored(game);
  if(check == 1) {
    game->scoreTwo++;
  } else if(check == 2) {
    game->scoreOne++;
  }
}
