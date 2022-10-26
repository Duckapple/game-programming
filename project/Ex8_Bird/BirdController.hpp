#pragma once

#include "Component.hpp"
#include "PhysicsComponent.hpp"
#include "sre/Sprite.hpp"

class BirdController : public Component
{
public:
    explicit BirdController(GameObject *gameObject);

    bool onKey(SDL_Event &event) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    std::function<void()> endGame;

private:
    std::shared_ptr<PhysicsComponent> physics;
    bool spaceDown = false;
};
