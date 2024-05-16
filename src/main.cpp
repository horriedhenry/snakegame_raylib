#include <iostream>
#include <raylib.h>

Color Green { 173, 204, 96, 255 };
Color DarkGreen { 43, 51, 24, 255 };

int CellSize = 30;
int CellCount = 25;

class Food
{
public:
    Vector2 position;
    Texture2D food_texture;

    Food()
    {
        Image image = LoadImage("../assets/food.png");
        food_texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPosition();
    }

    Vector2 GenerateRandomPosition()
    {
        float x = GetRandomValue(0, CellCount - 1);
        float y = GetRandomValue(0, CellCount - 1);
        return Vector2{x, y};
    }

    void Draw()
    {
        DrawTexture(food_texture, position.x * CellSize, position.y * CellSize, WHITE);
    }

};

int main (int argc, char *argv[])
{
    InitWindow(CellSize * CellCount, CellSize * CellCount, "Snake");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Green);
        Food food = Food();
        food.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
