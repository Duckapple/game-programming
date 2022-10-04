#pragma once

#include "glm/glm.hpp"
#include "Box.hpp"
#include "Ball.hpp"
#include "sre/SDLRenderer.hpp"

class Pong
{
public:
    Pong();

    void update(float deltaTime);
    void render();

private:
    int width;
    int height;
    const int paddleHeight = 50;
    const int lineWidth = 10;
    const int paddleOffsetX = 50;
    const int barOffsetY = lineWidth * 2;
    bool wPressed;
    bool pPressed;
    bool sPressed;
    bool lPressed;

    int leftScore = 0;
    int rightScore = 0;
    void movePaddle(Box *paddle, float yDelta);
    void handleKeyInput(SDL_Event &event);
    bool hasCollision(Edge2D edge);
    bool handleCollision(Box *paddle);
    void resetBall(bool right);
    Ball ball;
    Box leftPaddle;
    Box rightPaddle;
    Box topBar;
    Box bottomBar;
    sre::Color backgroundColor;

    sre::Camera cam;
    void handleOutOfBounds();
    sre::SDLRenderer r;
};
