#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include "game.h"
#include "physics.h"
#include "gameconstants.h"
#include "pictures.h"
#include "engine.h"
#include "graphics.h"
#include "gamehelpers.h"

extern volatile unsigned int* gpio;
extern rpi_sys_timer_t* systemTimer;

#define CHECK_FPS(N) if(counter % N == 0) {\
  printf("FPS: %d\r\n", counter/((systemTimer->counter_lo - timer_start) / 1000000));}
  /*uart_puts("FPS: ");\
  uart_puti(counter/((systemTimer->counter_lo - timer_start) / 1000000));\
  uart_puts("\r\n");} */


void cfg_gpio(void) {
  CFG_GPIO_INP(24);
  CFG_GPIO_INP(23);
  CFG_GPIO_INP(17);
  CFG_GPIO_INP(18);
}

void init_ball_pixels(uint16_t pixels[BALL_DIM][BALL_DIM]) {
  for(int i = 0; i < BALL_DIM; i++) {
    for(int j = 0; j < BALL_DIM; j++) {
      if(j >= (BALL_DIM - LINE_WIDTH) / 2 && j < (BALL_DIM + LINE_WIDTH) / 2 ) {
        pixels[i][j] = GREY_LINE;
      } else {
        pixels[i][j] = 0;
      }
    }
  }
}



void game_loop(uint16_t* mptr) {
  cfg_gpio();
  unsigned int counter = 0;
  volatile uint32_t timer_start = systemTimer->counter_lo;
  volatile uint32_t start_time;
  uint16_t ball_pixels[BALL_DIM][BALL_DIM];
  game_t* game;
  draw_bad_karma_logo(mptr);
  while(!(GET_GPIO(23) || GET_GPIO(24) || GET_GPIO(17) || GET_GPIO(18))) {
    //Do Nothing
  }
  fade_out(mptr);
  fade_in(mptr, &pim_logo);
  waitMicroSeconds(1*US_TO_S);
  fade_out(mptr);
  waitMicroSeconds(1*US_TO_S);
  while(1) {
    game = initialiseGame();
    init_board(game, mptr, ball_pixels);
    counter = 0;
    timer_start = systemTimer->counter_lo;
    while(!IS_GAMEOVER(game)) {
      start_time = systemTimer->counter_lo;
      counter++;
      CHECK_FPS(2000);
      if(GET_GPIO(23)) {
        move_paddle_l(PADDLE_SPEED, mptr, game);
      }
      if(GET_GPIO(24)) {
        move_paddle_l(-PADDLE_SPEED, mptr, game);
      }
      if(GET_GPIO(18)) {
        move_paddle_r(PADDLE_SPEED, mptr, game);
      }
      if(GET_GPIO(17)) {
        move_paddle_r(-PADDLE_SPEED, mptr, game);
      }
      move_ball(mptr, game, ball_pixels);
      if(check_if_scored(game)) {
        record_score(game);
        draw_scores(game, mptr);
        draw_ball(mptr, game, BLACK);
        draw_border(mptr);
        restoreBall(game->ball);
        init_ball_pixels(ball_pixels);
      }
      checkReflects(game);
      while(systemTimer->counter_lo - start_time < MSPF) {
        //Do Nothing
      }
    }
    if(game->scoreOne > 9) {
      draw_end_images(mptr, LEFT);
    } else {
      draw_end_images(mptr, RIGHT);
    }
    game->scoreOne = 10;
    game->scoreTwo = 10;
    draw_scores(game, mptr);
    while(!(GET_GPIO(23) || GET_GPIO(24) || GET_GPIO(17) || GET_GPIO(18))) {
      //Do Nothing
    }
    free_game(game);
  }
}
