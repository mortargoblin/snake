#include <raylib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// #define SCREEN_SIZE 400
enum { SCREEN_SIZE = 400 };

typedef struct {
  int x;
  int y;
  unsigned char dir[2];
  int score;
} Snake;

int main(void) {

  InitWindow(SCREEN_SIZE, SCREEN_SIZE, "snake");

  if (!IsWindowReady()) {
    CloseWindow();
    return -1;
  }

  // this is disgusting
  int step_size = 20;
  int width = (int) (SCREEN_SIZE / step_size);
  int height = (int) (SCREEN_SIZE / step_size);
  printf("width: %d, height: %d\n", width, height);
  int map[width][height];
  memset(map, 0, sizeof(map));

  Snake snake = {
    5, // pos
    5,
    {1, 0}, // dir vec2
    2 // score
  };

  SetTargetFPS(4);

  while (!WindowShouldClose()) {
    /// logic ///
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        if (map[i][j] > 0) {
          map[i][j] -= 1;
        }
      }
    }

    snake.x += snake.dir[0];
    snake.y += snake.dir[1];
    map[snake.x][snake.y] += snake.score;

    /// draw ///
    BeginDrawing();
    ClearBackground(BLACK);
    printf("drawing\n");
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        printf("%d ", map[i][j]);
        if (map[i][j] > 0) {
          DrawRectangle(
              i * step_size, j * step_size, 
              step_size, 
              step_size, 
              WHITE
              );
        }
      }
      printf("\n");
    }
    // sleep(1);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
