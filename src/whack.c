#include <raylib.h>
#include <stdio.h>


typedef enum{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  CENTER,
  NONE
} PlayerPos;

typedef struct{
  double duration;
  double start_time;
  bool active;
  bool now;
} Timer;

typedef struct{
  int direction;
  int old_direction;
  short life;
  Vector2 pos;
  float radius;
  Color color;
} Player;

typedef struct{
  Vector2 startPos;
  Vector2 endPos;
  float thick;
  Color color;
} Line;

// Local Variables
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

Player player1 = {0};
Timer player1_move_timer = {0};
Timer player1_attack_timer = {0};


Player player2 = {0};
Timer player2_move_timer = {0};
bool p2ismoving = false;
bool p2afk = false;
double p2now = 1000;

Line player1line = {0};


// Functions Declaration
void init_game(void);

void all_draw(void);
void draw_player1_line(void);
void draw_life(void);

void input(void);
bool player1_attack(void);
void attack(void);

void all_timers(void);

void update_player1_move_timer(void);
void activate_player1_move_timer(void);
void deactivate_player1_move_timer(void);

void update_player2_move_timer(void);
void activate_player2_move_timer(void);
void deactivate_player2_move_timer(void);
void change_player2(void);
void check_player2_afk(void);






void init_game(){
  player1.direction = DOWN;
  player1.old_direction = DOWN;
  player1.pos = (Vector2){WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2};
  player1.radius = 20;
  player1.color = BLUE;
  player1.life = 16;

  player1_attack_timer.active = false;
  player1_attack_timer.duration = 0.2;
  player1_attack_timer.start_time = 0;
  player1_move_timer.active = false;
  player1_move_timer.duration = 0.2;
  player1_move_timer.start_time = 0;
  

  player2.direction = NONE;
  player2.old_direction = DOWN;
  player2.pos = (Vector2){(WINDOW_WIDTH / 2),(WINDOW_HEIGHT / 2) - (player1.radius * 4)};
  player2.radius = 20;
  player2.color = RED;
  player2.life = 16;

  player2_move_timer.active = false;
  player2_move_timer.duration = 0.2;
  player2_move_timer.start_time = 0;

  player1line.color = WHITE;
  player1line.thick = 3;

}

int main(){
  InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Whack");
  init_game();
  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);
    all_draw();
    EndDrawing();
    input();
    all_timers();
  }
}


void all_draw(){
  if(player2.direction != NONE){
    DrawCircleV(player2.pos,player2.radius,player2.color);
  }
  DrawCircleV(player1.pos,player1.radius,player1.color);
  DrawLineEx(player1line.startPos,player1line.endPos,player1line.thick,player1line.color);
  draw_life();
}

void draw_life(){
  for(int i = 0; i < player1.life; i++){
    DrawRectangleV((Vector2){20 + (20 * i),20},(Vector2){10,20},player1.color);
  }
  for(int i = 0; i < player2.life; i++){
    DrawRectangleV((Vector2){(WINDOW_WIDTH - 20) - (20 * i),20},(Vector2){10,20},player2.color);
  }

}


void draw_player1_line(){
  switch(player1.direction){
    case UP:
      player1line.startPos = (Vector2){player1.pos.x,player1.pos.y - player1.radius};
      player1line.endPos = (Vector2){player1.pos.x,player1.pos.y - (player1.radius * 3)};
      break;
    case DOWN:
      player1line.startPos = (Vector2){player1.pos.x,player1.pos.y + player1.radius};
      player1line.endPos = (Vector2){player1.pos.x,player1.pos.y + player1.radius * 3};
      break;
    case RIGHT:
      player1line.startPos = (Vector2){player1.pos.x + player1.radius,player1.pos.y};
      player1line.endPos = (Vector2){player1.pos.x + player1.radius * 3,player1.pos.y};
      break;
    case LEFT:
      player1line.startPos = (Vector2){player1.pos.x - player1.radius,player1.pos.y};
      player1line.endPos = (Vector2){player1.pos.x - player1.radius * 3,player1.pos.y};
      break;

  }
}

void change_player2(){
  printf("entrou no change e player direction = %d\n", player2.direction);
  switch(player2.old_direction){
    case UP:
      player2.pos = (Vector2){(WINDOW_WIDTH / 2),(WINDOW_HEIGHT / 2) - (player1.radius * 4)};
      player2.direction = UP;
      printf("UP\n");
      break;
    case DOWN:
      player2.pos = (Vector2){(WINDOW_WIDTH / 2),(WINDOW_HEIGHT / 2) + (player1.radius * 4)};
      player2.direction = DOWN;
      printf("DOWN\n");
      break;
    case RIGHT:
      player2.pos = (Vector2){(WINDOW_WIDTH / 2) + player1.radius * 4,WINDOW_HEIGHT / 2};
      break;
      player2.direction = RIGHT;
      printf("RIGHT\n");
    case LEFT:
      player2.pos = (Vector2){(WINDOW_WIDTH / 2) - player1.radius * 4, WINDOW_HEIGHT / 2};
      player2.direction = LEFT;
      printf("LEFT\n");
      break;
    case NONE:

  }
}

void input(){

  // player2 input;
  if(IsKeyDown(KEY_UP)){
    p2ismoving = true;
    player2.old_direction = UP;
    p2now = GetTime();
    activate_player2_move_timer();
    
  }
  else if(IsKeyDown(KEY_DOWN)){
    p2ismoving = true;
    player2.old_direction = DOWN;
    p2now = GetTime();
    activate_player2_move_timer();
    
  }
  else if(IsKeyDown(KEY_RIGHT)){
    p2ismoving = true;
    player2.old_direction = RIGHT;
    p2now = GetTime();
    activate_player2_move_timer();
    
  }
  else if(IsKeyDown(KEY_LEFT)){
    p2ismoving = true;
    player2.old_direction = LEFT;
    p2now = GetTime();
    activate_player2_move_timer();
    
  }
  else{


  }

  //player1 input
  if(IsKeyPressed('W')){
    player1.direction = UP;
    activate_player1_move_timer();
  }
  else if(IsKeyPressed('S')){
    player1.direction = DOWN;
    activate_player1_move_timer();
  }
  else if(IsKeyPressed('D')){
    player1.direction = RIGHT;
    activate_player1_move_timer();

  }
  else if(IsKeyPressed('A')){
    player1.direction = LEFT;
    activate_player1_move_timer();
  }

  attack();

}


bool player1_attack(){
  if(IsKeyPressed(KEY_SPACE)){
    return true;
  }
  return false;
  
}

void attack(){
  if(player1.direction == player2.direction && player1_attack()){
    player2.life--;
  }
}

void all_timers(){
  update_player1_move_timer();
  update_player2_move_timer();
  check_player2_afk();
}


void activate_player1_move_timer(){
  player1_move_timer.active = true;
  player1_move_timer.start_time = GetTime();
}

void deactivate_player1_move_timer(){
  player1_move_timer.active = false;
  player1_move_timer.start_time = 0;
}

void update_player1_move_timer(){
  if(player1_move_timer.active){
    if(GetTime() - player1_move_timer.start_time >= player1_move_timer.duration){
      printf("CARAMBOLAS\n");
      draw_player1_line();
      deactivate_player1_move_timer();
    
  }
  }
  
}


void activate_player2_move_timer(){
  player2_move_timer.active = true;
  player2_move_timer.start_time = GetTime();
}

void deactivate_player2_move_timer(){
  player2_move_timer.active = false;
  player2_move_timer.start_time = 0;
}

void update_player2_move_timer(){
  if(player2_move_timer.active){
    if(GetTime() - player2_move_timer.start_time >= player2_move_timer.duration){
      printf("CATAPIMBAS\n");
      change_player2();
      deactivate_player2_move_timer();
    
  }
  }
  
}

void check_player2_afk(){
  if(GetTime() - p2now >= 2){
    player2.direction = NONE;
  }
}