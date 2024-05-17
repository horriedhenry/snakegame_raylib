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

    Vector2 head = body.back();
    Vector2 tail = body.front();

    void Draw()
    {
        for (auto it = body.begin(); it != body.end(); ++it) {
            DrawRectangle(it->x * CellSize, it->y * CellSize, CellSize, CellSize, DarkGreen);
        }
    }

    void Update()
    {
        body.pop_front();
        body.push_back(Vector2Add(body.back(), direction));
        head = body.back();
    }

    void Grow()
    {
        body.push_front(Vector2Add(body.front(), direction));
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

    void Update()
    {
        position = GenerateRandomPosition();
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
        ClearBackground(Green);
        if (wait(0.2)) {
            snake.Update();
        }
        if (IsKeyPressed(KEY_K) && snake.direction.y != 1) { // UP
            snake.direction = snake.up;
        }
        if (IsKeyPressed(KEY_J) && snake.direction.y != -1) { // DOWN
            snake.direction = snake.down;
        }
        if (IsKeyPressed(KEY_L) && snake.direction.x != -1) { // RIGHT
            snake.direction = snake.right;
        }
        if (IsKeyPressed(KEY_H) && snake.direction.x != 1) { // LEFT
            snake.direction = snake.left;
        }
        if (IsKeyPressed(KEY_Q)) {
            CloseWindow();
            exit(0);
        }

        // Detect Collision with food and Grow the snake if so
        if (Vector2Equals(snake.head, food.position)) {
            food.Update();
            food.Draw();
            snake.Grow();
        }

        // Detect Collision for wall on right and bottom
        if (snake.head.x * CellSize >= CellSize * CellCount || snake.head.y * CellSize >= CellSize * CellCount) {
            CloseWindow();
            exit(0);
        }

        // Detect Collision for wall on left and top
        if (snake.head.x < 0 || snake.head.y < 0) {
            CloseWindow();
            exit(0);
        }

        food.Draw();
        snake.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
