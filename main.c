#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_WIDTH 50
#define GRID_HEIGHT 50
#define CELL_SIZE 20

#define SCREEN_WIDTH (GRID_WIDTH * CELL_SIZE)
#define SCREEN_HEIGHT (GRID_HEIGHT * CELL_SIZE)

#define BG_COLOR BLACK
#define SNAKE_COLOR GREEN
#define APPLE_COLOR RED

#define TPS 10

// :DDDDD
#define APPLE (GRID_WIDTH * GRID_HEIGHT + 1)

char alive = 1;

typedef struct {
  int cells[GRID_HEIGHT][GRID_WIDTH];
} Grid;

typedef enum {NORTH, EAST, SOUTH, WEST} Dir;

typedef struct {
  int x;
  int y;
  int score;
  Dir dir;
} Snake;

void decrement_grid(Grid *grid) {
  for (int y = 0; y < GRID_HEIGHT; y++)
    for (int x = 0; x < GRID_WIDTH; x++)
      if (grid->cells[y][x] > 0 && grid->cells[y][x] <= GRID_WIDTH * GRID_HEIGHT)
        grid->cells[y][x]--;
}

void render_grid(Grid *grid) {
  // less than maximum snake cell value (grid_width * height) should be treated as
  // snake cells. i'm assigning width * height + 1 for apples... this might be stupid
  for (int y = 0; y < GRID_HEIGHT; y++)
    for (int x = 0; x < GRID_WIDTH; x++) {
      if (grid->cells[y][x] == 0)
        continue;
      if (grid->cells[y][x] == APPLE)
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, APPLE_COLOR);
      else
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, SNAKE_COLOR);
    }
}

void die(void) {
  puts("died");
  alive = 0;
}

void spawn_apple(Grid *grid) {
  char spot_found = 0;
  while (!spot_found) {
    int x = rand() % GRID_WIDTH;
    int y = rand() % GRID_HEIGHT;
    if (grid->cells[y][x] == 0)
      spot_found = 1;
    grid->cells[y][x] = APPLE;
  }
}

void move(Snake *snake, Grid *grid) {
  switch (snake->dir) {
    case NORTH:
      snake->y--;
      break;
    case EAST:
      snake->x++;
      break;
    case SOUTH:
      snake->y++;
      break;
    case WEST:
      snake->x--;
      break;
    default:
      break;
  }

  if (grid->cells[snake->y][snake->x] == APPLE) {
    snake->score++;
    spawn_apple(grid);
    grid->cells[snake->y][snake->x] = 0;
  } else if (grid->cells[snake->y][snake->x] != 0 || snake->x < 0 || snake->x > GRID_WIDTH
      || snake->y < 0 || snake->y > GRID_HEIGHT)
    die();

  grid->cells[snake->y][snake->x] += snake->score;
  printf("x: %d, y: %d\n", snake->x, snake->y);
}

void update_direction(Snake *snake) {
  switch (GetKeyPressed()) {
    case KEY_W:
      snake->dir = NORTH;
      break;
    case KEY_A:
      snake->dir = WEST;
      break;
    case KEY_S:
      snake->dir = SOUTH;
      break;
    case KEY_D:
      snake->dir = EAST;
      break;
    default:
      break;
  }
}

int main(void) {
  srand(time(NULL));

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snake");
  SetTargetFPS(60);

  printf("SCREEN WIDTH: %d SCREEN HEIGHT %d \n", SCREEN_WIDTH, SCREEN_HEIGHT);

  Grid grid = {0};

  Snake snake = {0};
  snake.dir = EAST;
  snake.score = 4;

  float accumulator = 0;

  for (int i = 0; i < 20; i++)
    spawn_apple(&grid);

  while (!WindowShouldClose() && alive) {
    accumulator += GetFrameTime();
    if (accumulator > (float)1/TPS) {
      accumulator = 0;

      move(&snake, &grid);

      decrement_grid(&grid);
    }
    update_direction(&snake);
  
    /// RENDERING
    BeginDrawing();
    {
      ClearBackground(BG_COLOR);
      render_grid(&grid);
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
