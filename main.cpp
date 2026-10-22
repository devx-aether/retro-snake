#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

bool GAME_OVER = false;
int SCORE = 0;

Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};
Color headColor= {43, 80, 24, 255};
Color softRed = {230, 80, 80, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;
int food_offset = 7;


double lastUpdateTime = 0;

bool elementInDeque(Vector2 element, deque<Vector2> deq)
{
    for (unsigned int i = 0; i < deq.size(); i++)
    {
        if(Vector2Equals(deq[i], element))
        {
            return true;
        } 

    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    } else 
    {
        return false;
    }
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = Vector2{1, 0};
    bool canChangeDirection = true;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x*cellSize, y*cellSize, cellSize, cellSize};
            Color color = i == 0 ? headColor : darkgreen; 
            DrawRectangleRounded(
                segment,
                0.5,
                6,
                color
            );
        }
    }

    bool CheckCollisionWithSelf()
    {
        for (int i = 1; i < body.size(); i++)
        {
            if (Vector2Equals(body[0], body[i]))
            {
                return true;
            }
        }
        return false;
    }

    void Update(bool grow)
    {
        Vector2 nextHead = Vector2Add(body[0], direction);

        // x axis wrapping
        if (nextHead.x >= cellCount)
        {
            nextHead.x = 0;
        } 
        else if (nextHead.x < 0)
        {
            nextHead.x = cellCount - 1;
        }

        // y axis wrapping
        if (nextHead.y >= cellCount)
        {
            nextHead.y = 0;
        } 
        else if (nextHead.y < 0)
        {
            nextHead.y = cellCount - 1;
        }

        body.push_front(nextHead);
        if (!grow)
        {
            body.pop_back();
        }
        canChangeDirection = true;
    }
};

class Food
{

public:

    Vector2 position;

    Food(deque<Vector2> snakeBody)
    {
        position = generateRandomPos(snakeBody);
    }

    void Draw() {
        DrawRectangle(
            position.x * cellSize + food_offset, 
            position.y * cellSize + food_offset, 
            cellSize - 2 * food_offset, 
            cellSize - 2 * food_offset, 
            softRed
        );
    }

    Vector2 generateRandomPos(deque<Vector2> snakeBody)
    {
        do
        {
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            position = {x, y};
        } while (elementInDeque(position, snakeBody));
        
        return position;
    }

};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        bool grow= CheckFoodCollision();
        if (!GAME_OVER)
        {
            snake.Update(grow);
        }
        
        if (snake.CheckCollisionWithSelf())
        {
            // cout << "game over" << endl;
            GAME_OVER = true;
        }
    }


    bool CheckFoodCollision()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.generateRandomPos(snake.body);
            SCORE++;
            return true;
        }

        return false;
    }
};

int main () {

    InitWindow(cellCount * cellSize, cellCount * cellSize, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false) 
    {
        if (eventTriggered(0.2))
        {
            game.Update();
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && game.snake.canChangeDirection) 
        {
            game.snake.direction = {-1, 0};
            game.snake.canChangeDirection = false;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && game.snake.canChangeDirection) 
        {
            game.snake.direction = {1, 0};
            game.snake.canChangeDirection = false;
        }
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && game.snake.canChangeDirection) 
        {
            game.snake.direction = {0, -1};
            game.snake.canChangeDirection = false;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && game.snake.canChangeDirection) 
        {
            game.snake.direction = {0, 1};
            game.snake.canChangeDirection = false;
        }

        BeginDrawing();

        ClearBackground(green);
        game.Draw();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}