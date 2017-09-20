#include <stdlib.h>
#include <stdbool.h>
#include "gameconstants.h"
#include "physics.h"

void regulateDir(double *dir) {

  while (*dir < 0.0) {

    *dir += TAU;

  }

}

void reflectVertical(double *dir) {

  *dir *= -1;

}

#define PADDLE_HOFFSET 6

void reflectHorizontal(game_t *game) {


  int x = game->ball->xPos;
  int y = game->ball->yPos;

  double *dir = &(game->ball->dir);

  double yDiffOne = game->paddleOneHeight - y;
  double yDiffTwo = game->paddleTwoHeight - y;

  bool xCheckLeft  = (x - BALL_RADIUS) <= PADDLE_X_SIZE + PADDLE_HOFFSET;
  bool xCheckRight = (x + BALL_RADIUS) >= game->gameWidth - PADDLE_X_SIZE
                                          - PADDLE_HOFFSET;

  bool yCheckLeft  = abs(yDiffOne) < BALL_RADIUS + PADDLE_Y_SIZE / 2;
  bool yCheckRight = abs(yDiffTwo) < BALL_RADIUS + PADDLE_Y_SIZE / 2;

  if (xCheckLeft && yCheckLeft) {

    *dir = (PI / 2) * (-yDiffOne / PADDLE_Y_SIZE);

  } else if (xCheckRight && yCheckRight) {

    *dir = PI + (PI / 2) * (yDiffTwo / PADDLE_Y_SIZE);

  }

}

void checkReflects(game_t *game) {

  ball_t *ball = game->ball;
  double *dir = &(ball->dir);

  //printf("%f...%f\n", game->ball->dir, *dir);

  if (game->gameHeight - BORDER_SIZE - ball->yPos <= BALL_RADIUS || ball->yPos - BORDER_SIZE <= BALL_RADIUS) {

    reflectVertical(dir);

  }

  reflectHorizontal(game);

  regulateDir(dir);

}
