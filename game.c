#include <stdlib.h>
#include <stdint.h>
#include "uart.h"
#include "game.h"
#include "gameconstants.h"
#include "gamehelpers.h"

void free_game(game_t* game) {
  free(game->ball);
  free(game);
}

game_t *initialiseGame(void) {
  game_t *game = malloc(sizeof(game_t));
  if (game == NULL) {
    uart_puts("Cannot malloc game\r\n");
  }
  game->gameHeight = FHEIGHT;
  game->gameWidth  = FWIDTH;

  game->paddleOneHeight = FHEIGHT / 2;
  game->paddleTwoHeight = FHEIGHT / 2;

  ball_t *ball = malloc(sizeof(ball_t));
  if (ball == NULL) {
    uart_puts("Cannot malloc ball");
  }
  uart_puth((uint32_t) ball);
  restoreBall(ball);

  game->ball = ball;

  game->scoreOne = 0;
  game->scoreTwo = 0;
  return game;
}
