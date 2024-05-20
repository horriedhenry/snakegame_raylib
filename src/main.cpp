#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include <deque>
#include <string>

Color Green{173, 204, 96, 255};
Color DarkGreen{43, 51, 24, 255};

int CellSize{30};
int CellCount{25};
int offset{75};

double last_update_time{0};

// wait for certain amount of time before doing something
bool wait(double interval)
{
    // Get elapsed time in seconds since InitWindow()
    double current_time{GetTime()};
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

class Snake {
  public:
    std::deque<Vector2> body{
        Vector2{6, 9},
        Vector2{5, 9},
        Vector2{4, 9},
    };

    Vector2 right{1, 0};
    Vector2 left{-1, 0};
    Vector2 up{0, -1};
    Vector2 down{0, 1};
    Vector2 direction = right;

    Vector2 head = body.front();
    Vector2 tail = body.back();

    void Draw()
    {
        for (auto it = body.begin(); it != body.end(); ++it) {
            float x = it->x;
            float y = it->y;
            Rectangle segment =
                Rectangle{offset + (x * CellSize), offset + (y * CellSize),
                          (float)CellSize, (float)CellSize};
            DrawRectangleRounded(segment, 0.5, 6, DarkGreen);
        }
    }

    void Update()
    {
        body.pop_back();
        body.push_front(Vector2Add(body.front(), direction));
        head = body.front();
    }

    void Grow() { body.push_front(Vector2Add(body.front(), direction)); }

    void reset()
    {
        body = {
            Vector2{6, 9},
            Vector2{5, 9},
            Vector2{4, 9},
        };
        direction = right;
        head = body.front();
        tail = body.back();
    }
};

class Food {
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
        DrawTexture(food_texture, offset + position.x * CellSize,
                    offset + position.y * CellSize, WHITE);
    }

    void Update(std::deque<Vector2> snake_body)
    {
        position = GenerateRandomPosition(snake_body);
    }
};

class Game {
  public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound EatSound;
    Sound WallSound;

    Game()
    {
        InitAudioDevice();
        EatSound = LoadSound("../assets/eat.mp3");
        WallSound = LoadSound("../assets/wall.mp3");
    }

    void EndGame()
    {
        UnloadSound(EatSound);
        UnloadSound(WallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        snake.Draw();
        food.Draw();
    }

    void GameOver()
    {
        snake.reset();
        food.position = food.GenerateRandomPosition(snake.body);
        running = false;
        score = 0;
    }

    void CheckCollisionWithBody()
    {
        std::deque<Vector2> head_less_body(snake.body.begin() + 1,
                                           snake.body.end());
        if (position_in_deque(snake.body[0], head_less_body)) {
            GameOver();
        }
    }

    void CheckCollisionWithWalls()
    {
        // Detect Collision for wall on right and bottom
        if (snake.head.x * CellSize >= CellSize * CellCount ||
            snake.head.y * CellSize >= CellSize * CellCount ||
            snake.head.x < 0 || snake.head.y < 0) {
            PlaySound(WallSound);
            GameOver();
        }
    }

    void CheckCollisionWithFood()
    {
        // Detect Collision with food and Grow the snake if so
        if (Vector2Equals(snake.head, food.position)) {
            PlaySound(EatSound);
            score++;
            food.Update(snake.body);
            food.Draw();
            snake.Grow();
        }
    }
};

int main()
{
    InitWindow((2 * offset) + CellSize * CellCount,
               (2 * offset) + CellSize * CellCount, "Snake");
    SetTargetFPS(60);
    Rectangle Border{(float)offset - 5, (float)offset - 5,
                     (float)CellSize * CellCount + 10,
                     (float)CellSize * CellCount + 10};

    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (wait(0.2)) {
            if (game.running == true) {
                game.snake.Update();
                game.CheckCollisionWithBody();
                game.CheckCollisionWithWalls();
                game.CheckCollisionWithFood();
            }
        }
        if (IsKeyPressed(KEY_K) && game.snake.direction.y != 1) { // UP
            game.running = true;
            game.snake.direction = game.snake.up;
        }
        if (IsKeyPressed(KEY_J) && game.snake.direction.y != -1) { // DOWN
            game.running = true;
            game.snake.direction = game.snake.down;
        }
        if (IsKeyPressed(KEY_L) && game.snake.direction.x != -1) { // RIGHT
            game.running = true;
            game.snake.direction = game.snake.right;
        }
        if (IsKeyPressed(KEY_H) && game.snake.direction.x != 1) { // LEFT
            game.running = true;
            game.snake.direction = game.snake.left;
        }
        if (IsKeyPressed(KEY_Q)) {
            game.EndGame();
            CloseWindow();
            exit(0);
        }
        ClearBackground(Green);
        DrawRectangleLinesEx(Border, 5, DarkGreen);
        DrawText("Snake Raylib", offset - 5, 20, 40, DarkGreen);
        DrawText(TextFormat("Score : %i", game.score), offset - 5,
                 offset + CellSize * CellCount + 10, 30, DarkGreen);
        game.Draw();
        EndDrawing();
    }

    game.EndGame();
    CloseWindow();
    return 0;
}
