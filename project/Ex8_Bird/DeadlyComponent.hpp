#pragma once

#include "Component.hpp"

/** A dummy component that, if present, will result in Bird dying */
class DeadlyComponent : public Component
{
public:
    explicit DeadlyComponent(GameObject *gameObject);
};
