#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include "gamehelpers.h"
#include "game.h"
#include "physics.h"
#include "gameconstants.h"
#include "pictures.h"
#include "utils.h"
#include "mailbox.h"
#include "graphics.h"
#include "engine.h"

//#define DEBUG


#define CHECK_FRAME_BOUNDS if(j < 0 || i < 0 || j >= 480 || i >= 640) continue
#define CHECK_PADDLE_BOUNDS if((i < FWIDTH - PADDLE_HOFFSET && i >= FWIDTH - PADDLE_HOFFSET\
                            - PADDLE_WIDTH && j >= game->paddleTwoHeight - PADDLE_HEIGHT / 2\
                            && j < game->paddleTwoHeight + PADDLE_HEIGHT / 2)\
                            || (i >= PADDLE_HOFFSET && i < PADDLE_HOFFSET + PADDLE_WIDTH\
                            && j >= game->paddleOneHeight - PADDLE_HEIGHT / 2\
                            && j < game->paddleOneHeight + PADDLE_HEIGHT / 2)) continue

extern rpi_sys_timer_t* systemTimer;

void draw_rectangle(int x, int y, int w, int h, uint16_t col, uint16_t* ptr) {
  uint16_t* cptr;
  for(int j = y; j < y + h; j++) {
    cptr = ptr + j * FWIDTH;
    for(int i = x; i < x + w; i++) {
      CHECK_FRAME_BOUNDS;
      *(cptr + i) = col;
    }
  }
}

void undraw_rectangle(int x, int y, int w, int h, uint16_t* ptr) {
  draw_rectangle(x, y, w, h, BLACK, ptr);
}



void draw_lines(uint16_t* ptr) {
  for(int i = 0; i < LINE_NUM; i++) {
    draw_rectangle((FWIDTH - LINE_WIDTH) / 2, i * (LINE_HEIGHT + SPACER_HEIGHT),
        LINE_WIDTH, LINE_HEIGHT, GREY_LINE, ptr);
  }
}




void draw_ball(uint16_t* ptr, game_t* game, uint16_t color){
  int xpos = (int)round(game->ball->xPos);
  int ypos = (int)round(game->ball->yPos);
  draw_rectangle(xpos - BALL_DIM / 2, ypos - BALL_DIM / 2, BALL_DIM,
                 BALL_DIM, color, ptr);
}



extern int __bss_start__;
extern int __bss_end__;

void move_ball(uint16_t* ptr, game_t* game, uint16_t pixels[BALL_DIM][BALL_DIM]){
  uint16_t* mptr = ptr;
  #ifdef DEBUG
    uart_puth((uint32_t)&__bss_start__);
    uart_puth((uint32_t)&__bss_end__);
  #endif
  int ballx = (int)round(game->ball->xPos);
  int bally = (int)round(game->ball->yPos);

  #ifdef DEBUG
    uart_puti(ballx);
    uart_puts("\r\n");
    uart_puti(bally);
    uart_puts("\r\n");
  #endif
  
  for(int j = bally - BALL_DIM / 2; j < (bally - BALL_DIM / 2) + BALL_DIM; j++) {
    mptr = ptr + j * FWIDTH;
    for(int i = ballx - BALL_DIM / 2; i < (ballx - BALL_DIM / 2) + BALL_DIM; i++) {
      CHECK_FRAME_BOUNDS;
      CHECK_PADDLE_BOUNDS;
      *(mptr + i) = pixels[j - (bally - BALL_DIM / 2)][i - (ballx - BALL_DIM / 2)];
    }
  }

  make_move(game);
  
  ballx = (int)round(game->ball->xPos);
  bally = (int)round(game->ball->yPos);
  if(check_if_scored(game)) {
    init_ball_pixels(pixels);
  }
  for(int j = bally - BALL_DIM / 2; j < (bally - BALL_DIM / 2) + BALL_DIM; j++) {
    mptr = ptr + j * FWIDTH;
    for(int i = ballx - BALL_DIM / 2; i < (ballx - BALL_DIM / 2) + BALL_DIM; i++) {
      CHECK_FRAME_BOUNDS;
      CHECK_PADDLE_BOUNDS;
      pixels[j - (bally - BALL_DIM / 2)][i - (ballx - BALL_DIM / 2)] = *(mptr + i);
    }
  }
  draw_ball(ptr, game, WHITE);
}

void draw_paddle_l(int y, uint16_t* ptr) {
  draw_rectangle(PADDLE_HOFFSET, y - PADDLE_HEIGHT / 2,
      PADDLE_WIDTH, PADDLE_HEIGHT, WHITE, ptr);
}

void draw_paddle_r(int y, uint16_t* ptr) {
  draw_rectangle(FWIDTH - PADDLE_WIDTH - PADDLE_HOFFSET,
      y - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE, ptr);
}

void move_paddle_l(int dy, uint16_t* ptr, game_t* game) {
  if(game->paddleOneHeight + dy < PADDLE_SPEED + PADDLE_HEIGHT / 2
     || game->paddleOneHeight + dy > FHEIGHT - PADDLE_SPEED - PADDLE_HEIGHT / 2) {
    return;
  }

  game->paddleOneHeight += dy;
  int cur_l = game->paddleOneHeight;
  int color1 = dy < 0 ? WHITE : BLACK;
  int color2 = dy < 0 ? BLACK : WHITE;
  int coef   = dy < 0 ? 0 : 1;
  dy = dy > 0 ? dy : -dy;
  draw_rectangle(PADDLE_HOFFSET, cur_l - (PADDLE_HEIGHT / 2) - coef * dy,
      PADDLE_WIDTH, dy, color1, ptr);

  draw_rectangle(PADDLE_HOFFSET, cur_l + (PADDLE_HEIGHT / 2) - coef * dy,
      PADDLE_WIDTH, dy, color2, ptr);
}

void move_paddle_r(int dy, uint16_t* ptr, game_t* game) {
  if(game->paddleTwoHeight + dy < PADDLE_SPEED + PADDLE_HEIGHT / 2
     || game->paddleTwoHeight + dy > FHEIGHT - PADDLE_SPEED - PADDLE_HEIGHT / 2) {
    return;
  }
  game->paddleTwoHeight += dy;
  int cur_r = game->paddleTwoHeight;
  int color1 = dy < 0 ? WHITE : BLACK;
  int color2 = dy < 0 ? BLACK : WHITE;
  int coef   = dy < 0 ? 0 : 1;
  dy = dy > 0 ? dy : -dy;
  draw_rectangle(FWIDTH - PADDLE_WIDTH - PADDLE_HOFFSET,
      cur_r - (PADDLE_HEIGHT / 2) - coef * dy,
      PADDLE_WIDTH, dy, color1, ptr);

  draw_rectangle(FWIDTH - PADDLE_WIDTH - PADDLE_HOFFSET,
      cur_r + (PADDLE_HEIGHT / 2) - coef * dy,
      PADDLE_WIDTH, dy, color2, ptr);
}



#define DRAW_HLINE_NUM(X, Y, W, P) draw_rectangle(X, 10 + Y, W, 8, GREY_TEXT, P)
#define DRAW_VLINE_NUM(X, Y, H, P) draw_rectangle(X, 10 + Y, 8, H, GREY_TEXT, P)

#define DRAW_TOP(L, D, P) DRAW_HLINE_NUM(L + D, 0, NUM_BOX_W, P)
#define DRAW_MID(L, D, P) DRAW_HLINE_NUM(L + D, (NUM_BOX_H / 2 - 4), NUM_BOX_W, P)
#define DRAW_BOTTOM(L, D, P) DRAW_HLINE_NUM(L + D, NUM_BOX_H - 8, NUM_BOX_W, P)
#define DRAW_LT(L, D, P) DRAW_VLINE_NUM(L + D, 0, NUM_BOX_H / 2, P)
#define DRAW_LB(L, D, P) DRAW_VLINE_NUM(L + D, NUM_BOX_H / 2, NUM_BOX_H / 2, P)
#define DRAW_RT(L, D, P) DRAW_VLINE_NUM(L + D + 22, 0, NUM_BOX_H / 2, P)
#define DRAW_RB(L, D, P) DRAW_VLINE_NUM(L + D + 22, NUM_BOX_H / 2, NUM_BOX_H / 2, P)

#define UNDRAW_NUM(L, D, P) draw_rectangle(L + D, 10, NUM_BOX_W, NUM_BOX_H, BLACK, P)


void draw_number(unsigned int n, uint16_t* ptr, enum side_t side) {
  int lr_offst;
  if(side == LEFT) {
    lr_offst = NUM_BOX_XOFFST_L;
  } else {
    lr_offst = NUM_BOX_XOFFST_R;
  }

  UNDRAW_NUM(lr_offst, D2_OFFST, ptr);
  switch(n) {
    case  0:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_LT(lr_offst, D2_OFFST, ptr);
      DRAW_LB(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      break;
    case  1:
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      break;
    case  2:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_LB(lr_offst, D2_OFFST, ptr);
      break;
    case  3:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      break;
    case  4:
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_LT(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      break;
    case  5:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_LT(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      break;
    case  6:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      DRAW_LT(lr_offst, D2_OFFST, ptr);
      DRAW_LB(lr_offst, D2_OFFST, ptr);
      break;
    case  7:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      break;
    case  8:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_LT(lr_offst, D2_OFFST, ptr);
      DRAW_LB(lr_offst, D2_OFFST, ptr);
      break;
    case  9:
      DRAW_TOP(lr_offst, D2_OFFST, ptr);
      DRAW_MID(lr_offst, D2_OFFST, ptr);
      DRAW_BOTTOM(lr_offst, D2_OFFST, ptr);
      DRAW_RB(lr_offst, D2_OFFST, ptr);
      DRAW_RT(lr_offst, D2_OFFST, ptr);
      DRAW_LT(lr_offst, D2_OFFST, ptr);
      break;
    case 10: break;
    case 11: break;
  }
}


void draw_scores(game_t* game, uint16_t* ptr) {
   draw_number(game->scoreOne, ptr, LEFT);
   draw_number(game->scoreTwo, ptr, RIGHT);
}

void draw_border(uint16_t* ptr) {
  draw_rectangle(0, 0, FWIDTH, BORDER_SIZE, GREY_LINE, ptr);
  draw_rectangle(0, FHEIGHT - BORDER_SIZE, FWIDTH, BORDER_SIZE, GREY_LINE, ptr);
  draw_rectangle(0, 0, BORDER_SIZE, FHEIGHT, GREY_LINE, ptr);
  draw_rectangle(FWIDTH - BORDER_SIZE, 0, BORDER_SIZE, FHEIGHT, GREY_LINE, ptr);
}




void draw_end_images(uint16_t* mptr, enum side_t winner) {
  int xstart;
  int ystart = 215;
  if(winner == LEFT) {
    xstart = 70;
  } else {
    xstart = 410;
  }
  uint16_t* im_ptr = (uint16_t*) winner_image.pixel_data;
  for(int i = ystart; i < ystart + winner_image.height; i++) {
    for(int j = xstart; j < xstart + winner_image.width; j++) {
      *(mptr + i * FWIDTH + j) = *im_ptr;
      im_ptr++;
    }
  }
  im_ptr = (uint16_t*)loser_image.pixel_data;
  xstart = (xstart + 340) % 680;
  for(int i = ystart; i < ystart + loser_image.height; i++) {
    for(int j = xstart; j < xstart + loser_image.width; j++) {
      *(mptr + i * FWIDTH + j) = *im_ptr;
      im_ptr++;
    }
  }

}

void draw_bad_karma_logo(uint16_t* mptr) {
  int ystart = (FHEIGHT - bad_karma_logo.height) / 2;
  uint16_t* im_ptr = (uint16_t*) bad_karma_logo.pixel_data;
  for(int j = ystart; j < ystart + bad_karma_logo.height; j++) {
    for(int i = 0; i < bad_karma_logo.width; i++) {
      *(mptr + j * FWIDTH + i) = *im_ptr;
      im_ptr++;
    }
  }
}

int get_max_pxl(uint16_t* mptr) {
  int max_pxl = 0;
  for(int i = 0; i < FHEIGHT * FWIDTH; i++) {
    if(*(mptr + i) > max_pxl) {
      max_pxl = *(mptr + i);
    }
  }
  return max_pxl;
}

void decrease_pxl_val(uint16_t* mptr) {
  uint16_t new_pxl = 0;
  uint16_t pxl_val = *mptr;
  uint16_t chan;
  chan = pxl_val >> 11;
  if(chan >= 1) {
    chan--;
  }
  new_pxl |= (chan << 11);
  chan = pxl_val << 5;
  chan >>= 10;
  if(chan >= 2) {
    chan -= 2;
  } else if(chan == 1) {
    chan = 0;
  }
  new_pxl |= (chan << 5);
  chan = pxl_val << 11;
  chan >>= 11;
  if(chan >= 1) {
    chan--;
  }
  new_pxl |= chan;
  *mptr = new_pxl;
}

void increase_pxl_val(uint16_t* mptr, uint16_t target) {
  uint16_t new_pxl = 0;
  uint16_t pxl_val = *mptr;
  uint16_t chan;
  uint16_t target_green;
  chan = pxl_val >> 11; 
  if(chan <= 30) {
    chan++;
  }
  new_pxl |= (chan << 11);
  target_green = target << 5;
  target_green >>= 10;
  chan = pxl_val << 5;
  chan >>= 10; 
  if(chan <= 61) {
    chan += 2;
  } else if(chan - target_green == 1 || chan - target_green == -1) {
    chan = target_green;
  }
  new_pxl |= (chan << 5); 
  chan = pxl_val << 11; 
  chan >>= 11; 
  if(chan <= 30) {
    chan++;
  }
  new_pxl |= chan;
  *mptr = new_pxl;
}


void fade_out(uint16_t* mptr) {
  int max_pxl = get_max_pxl(mptr);
  uint32_t start_time;
  while(max_pxl > 0) {
    start_time = systemTimer->counter_lo;
    for(int i = 0; i < FHEIGHT * FWIDTH; i++) {
      if(*(mptr + i) > 0) {
        decrease_pxl_val(mptr + i);
      }
    }
    max_pxl = get_max_pxl(mptr);
    while(systemTimer->counter_lo - start_time < MSPF * 3) {
      //Do nothing
    }
  }
}

void fade_in(uint16_t* mptr, const struct gimp_image* img) {
  bool change = true;
  int cur;
  int ystart = (FHEIGHT - img->height) / 2;
  uint16_t* im_ptr = (uint16_t*) img->pixel_data;
  while(change) {
    cur = 0;
    change = false;
    uint32_t start_time = systemTimer->counter_lo;
    for(int j = ystart; j < ystart + img->height; j++) {
      for(int i = 0; i < img->width; i++) {
        if(*(mptr + j * FWIDTH + i) < *(im_ptr + cur)) {
          increase_pxl_val(mptr + j * FWIDTH + i, *(im_ptr + cur)); 
          change = true;
        }
        cur++;
      }
    }
    while(systemTimer->counter_lo - start_time < MSPF * 4) {
      //Do nothing
    }
  }
}

void init_board(game_t* game, uint16_t* mptr, uint16_t ball_pixels[BALL_DIM][BALL_DIM]) {
  undraw_rectangle(0, 0, FWIDTH, FHEIGHT, mptr);
  init_ball_pixels(ball_pixels);
  draw_lines(mptr);
  draw_border(mptr);
  draw_paddle_l(game->paddleOneHeight, mptr);
  draw_paddle_r(game->paddleTwoHeight, mptr);
  draw_scores(game, mptr);
}

uint16_t* init_frame_buffer(void) {
  PUT32(FB_PWIDTH, 640);
  PUT32(FB_PHEIGHT, 480);
  PUT32(FB_VWIDTH, 640);
  PUT32(FB_VHEIGHT, 480);
  PUT32(FB_PITCH, 0);
  PUT32(FB_BDEPTH, 16);
  PUT32(FB_XOFFST, 0);
  PUT32(FB_YOFFST, 0);
  PUT32(FB_PNTR, 0);
  PUT32(FB_SIZE, 0);

  mailbox_write(0x40040000, 1);
  mailbox_read(1);


  unsigned int ptr = GET32(FB_PNTR);
  return (uint16_t*) ptr;
}
