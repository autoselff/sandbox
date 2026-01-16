#include "raylib.h"
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60

#define CELL_SIZE 4
#define GRID_WIDTH (WINDOW_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / CELL_SIZE)

#define PHYSICS_FPS 50
#define PHYSICS_TIMESTEP (1.0f / PHYSICS_FPS)

#define BRUSH_SIZE 4

typedef enum {
    CELL_EMPTY,
    CELL_SAND,
    CELL_WATER,
    CELL_STONE
} cell_type;

typedef struct {
    cell_type grid[GRID_WIDTH][GRID_HEIGHT];
    float physics_accumulator;
    cell_type selected_type;
} simulation_state;

static void init_simulation(simulation_state* state);
static void handle_input(simulation_state* state);
static void update_physics(simulation_state* state);
static void render_simulation(const simulation_state* state);
static void place_cells(cell_type grid[GRID_WIDTH][GRID_HEIGHT], int cell_x, int cell_y, int size, cell_type type);
static Color get_cell_color(cell_type type);

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "sandbox");
    SetTargetFPS(TARGET_FPS);
    simulation_state state;
    init_simulation(&state);

    while (!WindowShouldClose()) {
        handle_input(&state);
        update_physics(&state);
        render_simulation(&state);
    }

    CloseWindow();
    return 0;
}

static void init_simulation(simulation_state* state) {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            state->grid[x][y] = CELL_EMPTY;
        }
    }
    state->physics_accumulator = 0.0f;
    state->selected_type = CELL_SAND;
}

static void handle_input(simulation_state* state) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        int cell_x = (int)(mouse_pos.x / CELL_SIZE);
        int cell_y = (int)(mouse_pos.y / CELL_SIZE);
        place_cells(state->grid, cell_x, cell_y, BRUSH_SIZE, state->selected_type);
    }

    if (IsKeyPressed(KEY_ONE)) state->selected_type = CELL_SAND;
    if (IsKeyPressed(KEY_TWO)) state->selected_type = CELL_WATER;
    if (IsKeyPressed(KEY_THREE)) state->selected_type = CELL_STONE;
    if (IsKeyPressed(KEY_ZERO)) state->selected_type = CELL_EMPTY;
}

static void update_physics(simulation_state* state) {
    state->physics_accumulator += GetFrameTime();
    if (state->physics_accumulator >= PHYSICS_TIMESTEP) {
        state->physics_accumulator -= PHYSICS_TIMESTEP;
        for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (state->grid[x][y] == CELL_SAND) {
                    if (y + 1 < GRID_HEIGHT) {
                        if (state->grid[x][y + 1] == CELL_EMPTY) {
                            state->grid[x][y + 1] = CELL_SAND;
                            state->grid[x][y] = CELL_EMPTY;
                        }
                        else if (state->grid[x][y + 1] == CELL_WATER) {
                            state->grid[x][y + 1] = CELL_SAND;
                            state->grid[x][y] = CELL_WATER;
                        }
                        else if (state->grid[x][y + 1] != CELL_STONE) {
                            int left = (x - 1 >= 0) && (state->grid[x - 1][y + 1] == CELL_EMPTY);
                            int right = (x + 1 < GRID_WIDTH) && (state->grid[x + 1][y + 1] == CELL_EMPTY);
                            if (left && right) {
                                int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
                                state->grid[x + direction][y + 1] = CELL_SAND;
                                state->grid[x][y] = CELL_EMPTY;
                            }
                            else if (left) {
                                state->grid[x - 1][y + 1] = CELL_SAND;
                                state->grid[x][y] = CELL_EMPTY;
                            }
                            else if (right) {
                                state->grid[x + 1][y + 1] = CELL_SAND;
                                state->grid[x][y] = CELL_EMPTY;
                            }
                        }
                    }
                }
                if (state->grid[x][y] == CELL_WATER) {
                    if (y + 1 < GRID_HEIGHT && state->grid[x][y + 1] == CELL_EMPTY) {
                        state->grid[x][y + 1] = CELL_WATER;
                        state->grid[x][y] = CELL_EMPTY;
                    }
                    else {
                        int left = (x - 1 >= 0) && (y + 1 < GRID_HEIGHT) && (state->grid[x - 1][y + 1] == CELL_EMPTY);
                        int right = (x + 1 < GRID_WIDTH) && (y + 1 < GRID_HEIGHT) && (state->grid[x + 1][y + 1] == CELL_EMPTY);
                        if (left && right) {
                            int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
                            state->grid[x + direction][y + 1] = CELL_WATER;
                            state->grid[x][y] = CELL_EMPTY;
                        }
                        else if (left) {
                            state->grid[x - 1][y + 1] = CELL_WATER;
                            state->grid[x][y] = CELL_EMPTY;
                        }
                        else if (right) {
                            state->grid[x + 1][y + 1] = CELL_WATER;
                            state->grid[x][y] = CELL_EMPTY;
                        }
                        else {
                            int left_h = (x - 1 >= 0) && (state->grid[x - 1][y] == CELL_EMPTY);
                            int right_h = (x + 1 < GRID_WIDTH) && (state->grid[x + 1][y] == CELL_EMPTY);
                            if (left_h && right_h) {
                                int direction = GetRandomValue(0, 1) == 0 ? -1 : 1;
                                state->grid[x + direction][y] = CELL_WATER;
                                state->grid[x][y] = CELL_EMPTY;
                            }
                            else if (left_h) {
                                state->grid[x - 1][y] = CELL_WATER;
                                state->grid[x][y] = CELL_EMPTY;
                            }
                            else if (right_h) {
                                state->grid[x + 1][y] = CELL_WATER;
                                state->grid[x][y] = CELL_EMPTY;
                            }
                        }
                    }
                }
            }
        }
    }
}

static void render_simulation(const simulation_state* state) {
    BeginDrawing();
    ClearBackground((Color){ 50, 50, 64, 255 });
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            cell_type type = state->grid[x][y];
            if (type != CELL_EMPTY) {
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, get_cell_color(type));
            }
        }
    }
    EndDrawing();
}

static void place_cells(cell_type grid[GRID_WIDTH][GRID_HEIGHT], int cell_x, int cell_y, int size, cell_type type) {
    int half_size = size / 2;
    for (int dx = -half_size; dx < half_size; dx++) {
        for (int dy = -half_size; dy < half_size; dy++) {
            int x = cell_x + dx;
            int y = cell_y + dy;
            if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                grid[x][y] = type;
            }
        }
    }
}

static Color get_cell_color(cell_type type) {
    switch (type) {
        case CELL_SAND:
            return ORANGE;
        case CELL_WATER:
            return BLUE;
        case CELL_STONE:
            return GRAY;
        default:
            return BLANK;
    }
}