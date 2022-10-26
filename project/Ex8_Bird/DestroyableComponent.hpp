#pragma once

#include "Component.hpp"
#include "PhysicsComponent.hpp"

/** Component responsible */
class DestroyableComponent : public Component
{
public:
    explicit DestroyableComponent(GameObject *gameObject);
    void setOnDestroy(std::function<void(GameObject *)> onDestroy);
    void destroy();
    bool remove = false;
    void onCollisionStart(PhysicsComponent *comp);

private:
    std::function<void(GameObject *)> onDestroy;
};
