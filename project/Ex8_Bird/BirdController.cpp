//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "BirdController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "DeadlyComponent.hpp"
#include "PhysicsComponent.hpp"
#include "BirdGame.hpp"
#include "SpriteComponent.hpp"
// #include "glm/glm.hpp"

BirdController::BirdController(GameObject *gameObject) : Component(gameObject)
{
    // initiate bird physics
    physics = gameObject->getComponent<PhysicsComponent>();
}

bool BirdController::onKey(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !spaceDown)
    {
        // We clamp the velocity in the y-axis if we are going to flap, for better game feel
        auto vel = physics->getLinearVelocity();
        if (vel.y < -3)
            vel.y = -3;
        if (vel.y > 2)
            vel.y = 2;
        physics->setLinearVelocity(vel);

        // ...then we add the flap impulse
        auto impulse = glm::vec2(0, 0.15);
        physics->addImpulse(impulse);

        // Ensures repeated fires from held key don't trigger flap
        spaceDown = true;
    }
    else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
    {
        // Reset whether the space bar is held down
        spaceDown = false;
    }
    return false;
}

void BirdController::onCollisionStart(PhysicsComponent *comp)
{
    auto deadlyComp = comp->getGameObject()->getComponent<DeadlyComponent>();
    if (deadlyComp != nullptr)
    {
        // End the game (as provided from BirdGame::init()) if collided object is Deadly
        endGame();
    }
}

void BirdController::onCollisionEnd(PhysicsComponent *comp)
{
}
