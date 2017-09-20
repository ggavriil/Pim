#include <stdint.h>
#include "gameconstants.h"
#include "game.h"
#include "pictures.h"

#define FB_PWIDTH  0x40040000
#define FB_PHEIGHT 0x40040004
#define FB_VWIDTH  0x40040008
#define FB_VHEIGHT 0x4004000C
#define FB_PITCH   0x40040010
#define FB_BDEPTH  0x40040014
#define FB_XOFFST  0x40040018
#define FB_YOFFST  0x4004001C
#define FB_PNTR    0x40040020
#define FB_SIZE    0x40040024

void draw_scores(game_t* game, uint16_t* ptr);

void draw_end_images(uint16_t* mptr, enum side_t winner);

void move_paddle_r(int dy, uint16_t* ptr, game_t* game);

void move_paddle_l(int dy, uint16_t* ptr, game_t* game);

void draw_border(uint16_t* ptr);

void draw_ball(uint16_t* ptr, game_t* game, uint16_t color);

void draw_scores(game_t* game, uint16_t* ptr);

void move_ball(uint16_t* ptr, game_t* game, uint16_t pixels[BALL_DIM][BALL_DIM]);

void init_board(game_t* game, uint16_t* mptr, uint16_t ball_pixels[BALL_DIM][BALL_DIM]);

void draw_bad_karma_logo(uint16_t* mptr);

void fade_out(uint16_t* mptr);

void fade_in(uint16_t* mptr, const struct gimp_image* img); 

uint16_t* init_frame_buffer(void);


