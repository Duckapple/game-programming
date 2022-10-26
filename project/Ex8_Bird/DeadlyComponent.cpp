#include "DeadlyComponent.hpp"
#include "GameObject.hpp"

/** A dummy component that, if present, will result in Bird dying */
DeadlyComponent::DeadlyComponent(GameObject *gameObject) : Component(gameObject) {}
