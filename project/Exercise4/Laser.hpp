#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Laser : public GameObject,
              public Collidable
{
public:
  Laser(const sre::Sprite &sprite, float rotation, glm::vec2 position);

  void update(float deltaTime) override;

  void onCollision(std::shared_ptr<GameObject> other) override;

  bool shouldDelete() override;

private:
  glm::vec2 velocity;
  float lifeTime = 1.0f;
  glm::vec2 winSize;
};