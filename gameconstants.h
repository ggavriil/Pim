#define FWIDTH  640
#define FHEIGHT 480

#define BLACK 0
#define WHITE 0xFFFF
#define GREY_TEXT 0xC618
#define GREY_LINE 0x8410

#define BORDER_SIZE 3
#define FPS 120
#define MSPF (1000000 / FPS)
#define US_TO_S 1000000
#define BALL_DIM 10

#define PADDLE_WIDTH   10
#define PADDLE_HEIGHT  40
#define PADDLE_HOFFSET  6

#define D2_OFFST 0
#define NUM_BOX_H 50
#define NUM_BOX_W 30
#define NUM_BOX_XOFFST_L (290 - NUM_BOX_W)
#define NUM_BOX_XOFFST_R 352

#define LINE_WIDTH     6
#define LINE_HEIGHT   30
#define LINE_NUM      11
#define SPACER_HEIGHT 15

#define PADDLE_SPEED 3

#define BALL_RADIUS 5
#define PADDLE_X_SIZE 10
#define PADDLE_Y_SIZE 40
#define IS_GAMEOVER(game) (10 <= game->scoreOne || 10 <= game->scoreTwo)
#define PI 3.1415926535897932
#define TAU PI*2
#define BALLSPEED 3.0
