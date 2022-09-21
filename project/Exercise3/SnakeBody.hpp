#pragma once

#include "SnakeGUI.hpp"
#include <memory>

class SnakeBody
{
public:
    SnakeBody(int x, int y);
    ~SnakeBody();
    void moveTo(int x, int y);
    std::shared_ptr<SnakeBody> next = std::shared_ptr<SnakeBody>();

private:
    int x;
    int y;
    static int totalSnakeBodies;
    friend class Snake;
    friend class SnakeGUI;
};
