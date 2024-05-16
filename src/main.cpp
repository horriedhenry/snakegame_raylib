#include <iostream>
#include <raylib.h>

#define w_height 750
#define w_width 750

int main (int argc, char *argv[])
{
    std::cout << "Retro SnakeGame" << std::endl;
    InitWindow(w_width, w_height, "Snake");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
