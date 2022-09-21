#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

enum Size
{
  BIG,
  MEDIUM,
  SMALL
};

class Asteroid : public GameObject,
                 public Collidable
{
public:
  Asteroid(const sre::Sprite &sprite, Size size, float rotationSpeed, glm::vec2 position, glm::vec2 velocity, float scale);

  void update(float deltaTime) override;

  void onCollision(std::shared_ptr<GameObject> other) override;

  bool shouldDelete() override;

  Size getSize();

  glm::vec2 velocity;

private:
  float rotationSpeed;
  glm::vec2 winSize;
  Size size;
  bool dead = false;
};
