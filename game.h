#define BALL_RADIUS 5
#define PADDLE_X_SIZE 10
#define PADDLE_Y_SIZE 40
#define IS_GAMEOVER(game) (10 <= game->scoreOne || 10 <= game->scoreTwo)
#define PI 3.1415926535897932
#define TAU PI*2

#ifndef DEFINITIONFLAG
#define DEFINITIONFLAG

enum side_t {LEFT, RIGHT};

typedef struct ball ball_t;

typedef struct game {
  int paddleOneHeight;
  int paddleTwoHeight;
  ball_t *ball;
  int scoreOne;
  int scoreTwo;
  int gameHeight;
  int gameWidth;
} game_t;

struct ball {
  double xPos;
  double yPos;
  double dir;
};
#endif

void free_game(game_t* game);

game_t *initialiseGame(void);
