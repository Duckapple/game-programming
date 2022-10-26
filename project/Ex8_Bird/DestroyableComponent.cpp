#include "DestroyableComponent.hpp"
#include "GameObject.hpp"

/** A dummy component that, if present, will result in Bird dying */
DestroyableComponent::DestroyableComponent(GameObject *gameObject) : Component(gameObject) {}

void DestroyableComponent::setOnDestroy(std::function<void(GameObject *)> onDestroy)
{
    this->onDestroy = onDestroy;
}

void DestroyableComponent::destroy()
{
    remove = true;
    onDestroy(this->getGameObject());
}

void DestroyableComponent::onCollisionStart(PhysicsComponent *comp)
{
    destroy();
}
