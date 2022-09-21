
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Asteroid.hpp"
#include "sre/Renderer.hpp"
#include <iostream>

Asteroid::Asteroid(const sre::Sprite &sprite, Size size, float rotationSpeed, glm::vec2 position, glm::vec2 velocity, float scale)
    : GameObject(sprite), size(size)
{
  this->rotationSpeed = rotationSpeed; // Do conversion once on construction
  this->position = position;
  this->velocity = velocity;
  this->rotation = 0;
  winSize = sre::Renderer::instance->getDrawableSize();
  this->scale = glm::vec2(scale, scale);
  radius = sprite.getSpriteSize().x * scale / 2; // Gives a tight circle around somehow
}

void Asteroid::update(float deltaTime)
{
  position += velocity * deltaTime;
  rotation += deltaTime * rotationSpeed; // Is already radians, converted from input degrees

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

void Asteroid::onCollision(std::shared_ptr<GameObject> other)
{
  dead = true;
}

bool Asteroid::shouldDelete() { return dead; }

Size Asteroid::getSize() { return size; }
