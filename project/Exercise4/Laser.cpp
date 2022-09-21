#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Laser.hpp"
#include "sre/Renderer.hpp"
#include <iostream>

Laser::Laser(const sre::Sprite &sprite, float rotation, glm::vec2 position) : GameObject(sprite)
{
  this->rotation = rotation;
  velocity = glm::vec2(-glm::sin(glm::radians(rotation)) * 600, glm::cos(glm::radians(rotation)) * 600);
  this->position = position + (velocity * 0.1f);
  winSize = sre::Renderer::instance->getDrawableSize();
  radius = 5;
}

void Laser::update(float deltaTime)
{
  lifeTime -= deltaTime;
  position += velocity * deltaTime;
  // wrap around
  if (position.x < 0)
    position.x += winSize.x;
  else if (position.x > winSize.x)
    position.x -= winSize.x;
  if (position.y < 0)
    position.y += winSize.y;
  else if (position.y > winSize.y)
    position.y -= winSize.y;
}

void Laser::onCollision(std::shared_ptr<GameObject> other)
{
  lifeTime = 0;
}

bool Laser::shouldDelete() { return lifeTime <= 0; }