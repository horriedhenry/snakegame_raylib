#include <deque>
#include <raylib.h>
#include <raymath.h>

Color Green { 173, 204, 96, 255 };
Color DarkGreen { 43, 51, 24, 255 };

int CellSize = 30;
int CellCount = 25;

double last_update_time {0};

// wait for certain amount of time before doing something
bool wait(double interval)
{
    double current_time { GetTime() }; // Get elapsed time in seconds since InitWindow()
    if (current_time - last_update_time >= interval) {
        last_update_time = current_time;
        return true;
    }
    return false;
}

class Snake
{
public:
    // TODO : Use std::vector instead of deque
    std::deque<Vector2> body {
        Vector2{4, 9},
        Vector2{5, 9},
        Vector2{6, 9},
        Vector2{7, 9},
        Vector2{8, 9}
    };
    Vector2 right {1, 0};
    Vector2 left  {-1, 0};
    Vector2 up    {0, -1};
    Vector2 down  {0, 1};
    Vector2 direction = right;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++) {
            DrawRectangle(body[i].x * CellSize, body[i].y * CellSize, CellSize, CellSize, DarkGreen);
        }
    }

    void Update()
    {
        body.pop_front();
        body.push_back(Vector2Add(body.back(), this->direction));
    }
};

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

int main ()
{
    InitWindow(CellSize * CellCount, CellSize * CellCount, "Snake");
    SetTargetFPS(60);
    Food food = Food();
    Snake snake = Snake();

    while(!WindowShouldClose()) {
        BeginDrawing();
        if (wait(0.2)) {
            snake.Update();
        }
        /*
         * TODO : 
         * Don't allow snake to move backward, when going in any direction :
         * when moving to the right don't allow to move left
         * when moving to the left don't allow to move right
         * when moving to the up don't allow to move down
         * when moving to the down don't allow to move up
        */
        if (IsKeyPressed(KEY_K)) { // UP
            snake.direction = snake.up;
        }
        if (IsKeyPressed(KEY_J)) { // DOWN
            snake.direction = snake.down;
        }
        if (IsKeyPressed(KEY_L)) { // RIGHT
            snake.direction = snake.right;
        }
        if (IsKeyPressed(KEY_H)) { // LEFT
            snake.direction = snake.left;
        }
        ClearBackground(Green);
        food.Draw();
        snake.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
