#pragma once

#include <vector>
#include <queue>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "SpaceShip.hpp"
#include "Asteroid.hpp"
#include "Laser.hpp"

class GameObject;

class AsteroidsGame
{
public:
    AsteroidsGame();

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    void reset();
    std::shared_ptr<Asteroid> makeAsteroid(Size size = BIG);

    sre::Camera camera;
    sre::SDLRenderer r;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<SpaceShip>> players;
    bool debugCollisionCircles = false;
    float timer = 0;
    int score = 0;
    bool playing = true;
    std::vector<sre::Sprite> asteroids;
};
