//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <iostream>
#include "FirstPersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

enum Direction
{
    NONE = 0,
    FORWARD = 0b0001,
    LEFT = 0b0010,
    BACK = 0b0100,
    RIGHT = 0b1000,
    UP = 0b010000,
    DOWN = 0b100000,
    ALL = 0b111111,
};

FirstPersonController::FirstPersonController(sre::Camera *camera)
    : camera(camera)
{
    camera->setPerspectiveProjection(45, 0.1f, 1000);
    position = glm::vec3(0, 0, 0);
}

void FirstPersonController::update(float deltaTime)
{
    auto direction = glm::rotateY(glm::vec3(1, 0, 0), rotation);
    float fb, lr, ud;
    if (keys & FORWARD)
        fb += deltaTime * speed;
    if (keys & BACK)
        fb -= deltaTime * speed;
    if (keys & RIGHT)
        lr += deltaTime * speed;
    if (keys & LEFT)
        lr -= deltaTime * speed;
    if (keys & UP)
        ud += deltaTime * speed;
    if (keys & DOWN)
        ud -= deltaTime * speed;
    position += glm::rotateY(glm::vec3(fb, ud, lr), rotation);
    auto at = position + direction;
    camera->lookAt(position, at, glm::vec3{0, 1, 0});
}

void FirstPersonController::onKey(SDL_Event &event)
{
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
        return;

    int isDown = event.type == SDL_KEYDOWN ? ALL : NONE;
    switch (event.key.keysym.sym)
    {
    case SDLK_w:
        keys = (keys & (ALL ^ FORWARD)) | (FORWARD & isDown);
        break;
    case SDLK_a:
        keys = (keys & (ALL ^ LEFT)) | (LEFT & isDown);
        break;
    case SDLK_s:
        keys = (keys & (ALL ^ BACK)) | (BACK & isDown);
        break;
    case SDLK_d:
        keys = (keys & (ALL ^ RIGHT)) | (RIGHT & isDown);
        break;
    case SDLK_SPACE:
        keys = (keys & (ALL ^ UP)) | (UP & isDown);
        break;
    case SDLK_LSHIFT:
        keys = (keys & (ALL ^ DOWN)) | (DOWN & isDown);
        break;
    default:
        break;
    }
}

void FirstPersonController::onMouse(SDL_Event &event)
{
    if (event.type != SDL_MOUSEMOTION)
        return;

    this->rotation -= event.motion.xrel / sensitivity;
}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation)
{
    this->position = glm::vec3(position.x, 0, position.y);
    this->rotation = glm::radians(rotation);
}
