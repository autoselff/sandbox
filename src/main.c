#include "raylib.h"

typedef struct {
    int x;
    int y;
} pixel;

pixel create_pixel(int pos_x, int pos_y) {
    pixel p;
    p.x = pos_x;
    p.y = pos_y;

    return p;
}

void draw_pixel(pixel* p) {
    DrawPixel(p->x, p->y, RED);
}

int main(void) {
    InitWindow(800, 600, "sandbox");
    SetTargetFPS(60);

    pixel p = create_pixel(0, 0);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_pixel(&p);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}