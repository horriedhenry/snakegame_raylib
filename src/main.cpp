#include <deque>
#include <iostream>
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

bool position_in_deque(Vector2 position, std::deque<Vector2> deque)
{
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        if (Vector2Equals(position, *it)) {
            return true;
        }
    }
    return false;
}

class Snake
{
public:
    std::deque<Vector2> body {
        Vector2{8, 9},
        Vector2{7, 9},
        Vector2{6, 9},
        Vector2{5, 9},
        Vector2{4, 9},
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
            float x = it->x;
            float y = it->y;
            Rectangle segment = Rectangle{x * CellSize, y * CellSize, (float)CellSize, (float)CellSize};
            DrawRectangleRounded(segment, 0.5, 6, DarkGreen);
        }
    }

    void Update()
    {
        body.pop_back();
        body.push_front(Vector2Add(body.front(), direction));
        head = body.front();
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

    Food(std::deque<Vector2> snake_body)
    {
        Image image = LoadImage("../assets/food.png");
        food_texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPosition(snake_body);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, CellCount - 1);
        float y = GetRandomValue(0, CellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPosition(std::deque<Vector2> deque)
    {
        position = GenerateRandomCell();
        while (position_in_deque(position, deque)) {
            position = GenerateRandomCell();
        }
        return position;
    }

    void Draw()
    {
        DrawTexture(food_texture, position.x * CellSize, position.y * CellSize, WHITE);
    }

    void Update(std::deque<Vector2> snake_body)
    {
        position = GenerateRandomPosition(snake_body);
    }

};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    void Draw()
    {
        snake.Draw();
        food.Draw();
    }

};

int main ()
{
    InitWindow(CellSize * CellCount, CellSize * CellCount, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Green);
        if (wait(0.2)) {
            game.snake.Update();
        }
        if (IsKeyPressed(KEY_K) && game.snake.direction.y != 1) { // UP
            game.snake.direction = game.snake.up;
        }
        if (IsKeyPressed(KEY_J) && game.snake.direction.y != -1) { // DOWN
            game.snake.direction = game.snake.down;
        }
        if (IsKeyPressed(KEY_L) && game.snake.direction.x != -1) { // RIGHT
            game.snake.direction = game.snake.right;
        }
        if (IsKeyPressed(KEY_H) && game.snake.direction.x != 1) { // LEFT
            game.snake.direction = game.snake.left;
        }
        if (IsKeyPressed(KEY_Q)) {
            CloseWindow();
            exit(0);
        }

        // Detect Collision with food and Grow the snake if so
        if (Vector2Equals(game.snake.head, game.food.position)) {
            game.food.Update(game.snake.body);
            game.food.Draw();
            game.snake.Grow();
        }

        // Detect Collision for wall on right and bottom
        if (game.snake.head.x * CellSize >= CellSize * CellCount || game.snake.head.y * CellSize >= CellSize * CellCount) {
            CloseWindow();
            exit(0);
        }

        // Detect Collision for wall on left and top
        if (game.snake.head.x < 0 || game.snake.head.y < 0) {
            CloseWindow();
            exit(0);
        }

        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
