#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;


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
    deque<Vector2> inputQueue;

    void Reset()
    {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = Vector2{1, 0};
        inputQueue.clear();
    }

    void handleInput()
    {
        Vector2 lastDir = inputQueue.empty() ? direction : inputQueue.back();

        if (inputQueue.size() >= 2) return;

        if (IsKeyPressed(KEY_LEFT) && lastDir.x != 1)
        {
            inputQueue.push_back({-1, 0});
        }
        else if (IsKeyPressed(KEY_RIGHT) && lastDir.x != -1)
        {
            inputQueue.push_back({1, 0});
        }
        else if (IsKeyPressed(KEY_UP) && lastDir.y != 1)
        {
            inputQueue.push_back({0, -1});
        }
        else if (IsKeyPressed(KEY_DOWN) && lastDir.y != -1)
        {
            inputQueue.push_back({0, 1});
        }
    }

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset+x*cellSize, offset+y*cellSize, cellSize, cellSize};
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

        if (!inputQueue.empty())
        {
            direction = inputQueue.front();
            inputQueue.pop_front();
        }

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
            offset + position.x * cellSize + food_offset, 
            offset + position.y * cellSize + food_offset, 
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
    int score = 0;
    bool gameOver = false;

    void Draw()
    {
        food.Draw();
        snake.Draw();
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount + 10, (float)cellSize*cellCount+10}, 5, darkgreen);
        DrawText("Snake Game", offset-5, 20, 40, darkgreen);
        DrawText(TextFormat("Score: %i", score), offset-5, offset+cellSize*cellCount + 10, 40, darkgreen);
    }

    void game_over_reset()
    {
        snake.Reset();
        food.generateRandomPos(snake.body);
        gameOver = false;
        score = 0;
    }

    void Update()
    {
        if (gameOver) 
        {
            if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
            {
                game_over_reset();
            }
            return;
        }

        snake.handleInput();

        if (eventTriggered(0.2))
        {
            bool grow = CheckFoodCollision();
            snake.Update(grow);

            if (snake.CheckCollisionWithSelf())
            {
                gameOver = true;
            }
        }
    }


    bool CheckFoodCollision()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.generateRandomPos(snake.body);
            score++;
            return true;
        }

        return false;
    }
};

int main () {

    InitWindow(2*offset + cellCount * cellSize, 2*offset + cellCount * cellSize, "Retro Snake");
    SetTargetFPS(60);

    Game game;

    while (WindowShouldClose() == false) 
    {
        game.Update();

        BeginDrawing();

        ClearBackground(green);
        game.Draw();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}