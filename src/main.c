#include "raylib.h"

#define WINDOW_X 800
#define WINDOW_Y 600
#define TARGET_FPS 60

#define CELL_SIZE 4
#define CELLS_X WINDOW_X / CELL_SIZE
#define CELLS_Y WINDOW_Y / CELL_SIZE

#define PHYSICS_FPS 50
#define PHYSICS_DELAY (1.0f / PHYSICS_FPS)

void place_pixels(int cells[CELLS_X][CELLS_Y], int mouse_x, int mouse_y, int size, int type) {
    for (int x = -size / 2; x < size / 2; x++) {
        for (int y = -size / 2; y < size / 2; y++) {
            int target_x = mouse_x + x;
            int target_y = mouse_y + y;
            
            if (target_x >= 0 && target_x < CELLS_X && 
                target_y >= 0 && target_y < CELLS_Y) {
                cells[target_x][target_y] = type;
            }
        }
    }
}

int main(void) {
    InitWindow(WINDOW_X, WINDOW_Y, "sandbox");
    SetTargetFPS(TARGET_FPS);

    int cells[CELLS_X][CELLS_Y] = { 0 };
    float physicsTimer = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        physicsTimer += deltaTime;

        Vector2 temp_mouse_position = GetMousePosition();
        int mouse_cell_x = (int)(temp_mouse_position.x) / CELL_SIZE;
        int mouse_cell_y = (int)(temp_mouse_position.y) / CELL_SIZE;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            place_pixels(cells, mouse_cell_x, mouse_cell_y, 4, 1);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            place_pixels(cells, mouse_cell_x, mouse_cell_y, 4, 2);
        }

        if (physicsTimer >= PHYSICS_DELAY) {
            physicsTimer -= PHYSICS_DELAY;

            for (int y = CELLS_Y - 1; y >= 0; y--) {
                for (int x = 0; x < CELLS_X; x++) {
                    if (cells[x][y] == 1) {
                        if (y + 1 < CELLS_Y) {
                            if (cells[x][y + 1] == 0) {
                                cells[x][y + 1] = 1;
                                cells[x][y] = 0;
                            }
                            else if (cells[x][y + 1] == 2) {
                                cells[x][y + 1] = 1;
                                cells[x][y] = 2;
                            }
                            else {
                                int left = (x - 1 >= 0) && (cells[x - 1][y + 1] == 0);
                                int right = (x + 1 < CELLS_X) && (cells[x + 1][y + 1] == 0);

                                if (left && right) {
                                    int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
                                    cells[x + direction][y + 1] = 1;
                                    cells[x][y] = 0;
                                }
                                else if (left) {
                                    cells[x - 1][y + 1] = 1;
                                    cells[x][y] = 0;
                                }
                                else if (right) {
                                    cells[x + 1][y + 1] = 1;
                                    cells[x][y] = 0;
                                }
                            }
                        }
                    }

                    if (cells[x][y] == 2) {
                        if (y + 1 < CELLS_Y && cells[x][y + 1] == 0) {
                            cells[x][y + 1] = 2;
                            cells[x][y] = 0;
                        }
                        else {
                            int left = (x - 1 >= 0) && (y + 1 < CELLS_Y) && (cells[x - 1][y + 1] == 0);
                            int right = (x + 1 < CELLS_X) && (y + 1 < CELLS_Y) && (cells[x + 1][y + 1] == 0);

                            if (left && right) {
                                int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
                                cells[x + direction][y + 1] = 2;
                                cells[x][y] = 0;
                            }
                            else if (left) {
                                cells[x - 1][y + 1] = 2;
                                cells[x][y] = 0;
                            }
                            else if (right) {
                                cells[x + 1][y + 1] = 2;
                                cells[x][y] = 0;
                            }
                            else {
                                int left_h = (x - 1 >= 0) && (cells[x - 1][y] == 0);
                                int right_h = (x + 1 < CELLS_X) && (cells[x + 1][y] == 0);

                                if (left_h && right_h) {
                                    int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
                                    cells[x + direction][y] = 2;
                                    cells[x][y] = 0;
                                }
                                else if (left_h) {
                                    cells[x - 1][y] = 2;
                                    cells[x][y] = 0;
                                }
                                else if (right_h) {
                                    cells[x + 1][y] = 2;
                                    cells[x][y] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        BeginDrawing();
        ClearBackground((Color){ 50, 50, 64, 255 });
        
        for (int x = 0; x < CELLS_X; x++) {
            for (int y = 0; y < CELLS_Y; y++) {
                switch (cells[x][y]) {
                    case 1:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, ORANGE);
                        break;

                    case 2:
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
                        break;

                    default:
                        break;
                }
            }
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}