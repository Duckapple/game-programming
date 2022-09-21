#include "Snake.hpp"
#include "SnakeBody.hpp"

Snake::Snake()
{
}

void Snake::move()
{ // This is called every time the snake should move.
    int currentHeadPosX = snakeHead->x;
    int currentHeadPosY = snakeHead->y;

    snakeHead->moveTo(snakeHead->x + moveX, snakeHead->y + moveY);
}

void Snake::setMoveDirection(int moveX, int moveY)
{ // This is called whenever one arrow key is pressed. Values are [-1,0,1] for x and y.
    if (moveX && moveX == -this->moveX)
        return;
    if (moveY && moveY == -this->moveY)
        return;
    this->moveX = moveX;
    this->moveY = moveY;
}

void Snake::init(int initialLength, int x, int y, int moveX, int moveY)
{ // This is called every time the game starts or restarts. Make sure not to leak resources.
    length = initialLength;
    this->moveX = moveX;
    this->moveY = moveY;
    snakeHead = std::make_shared<SnakeBody>(x, y);
    auto current = snakeHead;
    for (int i = 1; i < initialLength; i++)
    {
        auto next = std::make_shared<SnakeBody>(current->x - moveX, current->y - moveY);
        current->next = next;
        current = next;
    }
}

void Snake::grow()
{ // Should increment the length and append a SnakeBody
    auto last = snakeHead;
    while (last->next)
    {
        last = last->next;
    }
    auto newHead = std::make_shared<SnakeBody>(snakeHead->x, snakeHead->y);
    last->next = newHead;
}

bool Snake::collide(int x, int y)
{ // Returns true if one of the SnakeBody objects has the position (x,y)
    auto current = snakeHead;
    while (current)
    {
        if (current->x == x && current->y == y)
            return true;
        current = current->next;
    }
    return false;
}
