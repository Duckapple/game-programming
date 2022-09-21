//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include <ctime>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "Asteroid.hpp"
#include "Collidable.hpp"
#include "Laser.hpp"
#include <random>
#include <algorithm>

using namespace sre;

AsteroidsGame::AsteroidsGame()
{
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING).withSdlWindowFlags(SDL_WINDOW_OPENGL);

    time_t t; // random seed based on time
    srand((unsigned)time(&t));

    atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");

    // I declare the asteroid sprites on a field, for easier access when making new asteroids
    asteroids = {
        atlas->get("meteorBrown_big1.png"),
        atlas->get("meteorBrown_med1.png"),
        atlas->get("meteorBrown_small1.png"),
    };

    // I use reset to just set up the same way every time
    reset();

    camera.setWindowCoordinates();

    r.frameUpdate = [&](float deltaTime)
    {
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event &event)
    {
        keyEvent(event);
    };

    r.frameRender = [&]()
    {
        render();
    };

    r.startEventLoop();
}

void AsteroidsGame::reset()
{
    // Clean-up, in case this is a reset after death
    players.clear();
    gameObjects.clear();
    playing = true;
    score = 0;
    timer = 0;

    auto spaceshipSprite = atlas->get("playerShip1_blue.png");
    auto spaceShip = std::make_shared<SpaceShip>(spaceshipSprite);
    gameObjects.push_back(spaceShip);
    players.push_back(spaceShip);
    for (size_t i = 0; i < 5; i++)
        gameObjects.push_back(makeAsteroid());
}

std::shared_ptr<Asteroid> AsteroidsGame::makeAsteroid(Size size)
{
    auto bounds = sre::Renderer::instance->getDrawableSize();
    return std::make_shared<Asteroid>(
        asteroids[size],
        size,
        ((rand() % 300) - 150) / 2.0f,
        glm::vec2(rand() % bounds.x, rand() % bounds.y),
        glm::vec2(((rand() % 20) - 10) * 10, ((rand() % 20) - 10) * 10),
        0.9 + (((float)rand()) / RAND_MAX / 3));
}

#define SHOT_SPEED 0.25

void AsteroidsGame::update(float deltaTime)
{
    timer += deltaTime;
    // Do not make game updates when game isn't playing
    if (!playing)
        return;
    // We use the timer to spawn lasers only on the interval defined
    // I use the loop to be safe in terms of insane slowdowns
    int shotCount = 0;
    while (timer >= SHOT_SPEED)
    {
        shotCount++;
        timer -= SHOT_SPEED;
    }

    // Collision check
    for (size_t i = 0; i < gameObjects.size(); i++)
    {
        auto i_o = gameObjects[i];
        auto i_c = std::dynamic_pointer_cast<Collidable>(i_o);
        for (size_t j = i + 1; j < gameObjects.size(); j++)
        {
            if (i == j)
                continue;
            auto j_o = gameObjects[j];
            auto j_c = std::dynamic_pointer_cast<Collidable>(j_o);
            if (glm::distance(i_o->position, j_o->position) > j_c->getRadius() + i_c->getRadius())
                continue;

            std::shared_ptr<Asteroid> ast = std::dynamic_pointer_cast<Asteroid>(i_o);
            if (!ast)
                ast = std::dynamic_pointer_cast<Asteroid>(j_o);
            std::shared_ptr<SpaceShip> ship = std::dynamic_pointer_cast<SpaceShip>(i_o);
            if (!ship)
                ship = std::dynamic_pointer_cast<SpaceShip>(j_o);
            std::shared_ptr<Laser> laser = std::dynamic_pointer_cast<Laser>(i_o);
            if (!laser)
                laser = std::dynamic_pointer_cast<Laser>(j_o);

            // If an asteroid and a laser collides, remove both and spawn two smaller asteroids if applicable.
            if (ast && laser)
            {
                ast->onCollision(laser);
                laser->onCollision(ast);
                if (ast->getSize() != SMALL)
                { // we just make two new asteroids of one size smaller (yes, that means a higher int behind the scenes)
                    Size newSize = Size(ast->getSize() + 1);
                    // I scale up the velocity to make smaller asteroids harder to hit
                    auto velocity = glm::vec2(ast->velocity.x * 1.3, ast->velocity.y * 1.3);

                    auto ast1 = makeAsteroid(newSize);
                    ast1->position = ast->position;
                    // It spits out a bit to the side
                    ast1->velocity = glm::rotateZ(glm::vec3(velocity.x, velocity.y, 0), glm::radians(55.0f));
                    gameObjects.push_back(ast1);

                    auto ast2 = makeAsteroid(newSize);
                    ast2->position = ast->position;
                    // It spits out a bit to the other side
                    ast2->velocity = glm::rotateZ(glm::vec3(velocity.x, velocity.y, 0), glm::radians(-55.0f));
                    gameObjects.push_back(ast2);
                }
                score++;
            }
            // If the ship crashes into an asteroid
            else if (ast && ship)
            {
                // We don't run onCollision on `ast`, to keep the visual state on game over
                ship->onCollision(ast);
                // We just update the sprite of the ship to "bang" -- didn't want to make an entire implementation for this
                ship->sprite = atlas->get("bang.png");
                ship->rotation = 0;
                ship->scale = glm::vec2(3, 3);
                playing = false;
            }
        }
    }

    // Clean up
    for (int i = 0; i < gameObjects.size(); i++)
        gameObjects[i]->update(deltaTime);

    // remove_if from <algorithm> moves elements to the end of the vector...
    auto it = std::remove_if(
        gameObjects.begin(),
        gameObjects.end(),
        [](const std::shared_ptr<GameObject> &obj)
        { return obj->shouldDelete(); });

    // ...so that we may remove them in O(n)
    gameObjects.erase(it, gameObjects.end());

    for (auto &ship : players)
    {
        // Handle shooting: make a laser if shooting
        if (ship->isShooting())
        {
            for (size_t i = 0; i < shotCount; i++)
            {
                auto laser = std::make_shared<Laser>(atlas->get("laserBlue01.png"), ship->rotation, ship->position);
                gameObjects.push_back(laser);
            }
        }
    }
}

void drawCircle(std::vector<glm::vec3> &lines, glm::vec2 position, float radius)
{
    float quaterPi = glm::quarter_pi<float>();
    for (float f = 0; f < glm::two_pi<float>(); f += quaterPi)
    {
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f) * radius,
                                  position.y + sinf(f) * radius,
                                  0});
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f + quaterPi) * radius,
                                  position.y + sinf(f + quaterPi) * radius,
                                  0});
    }
}

void AsteroidsGame::render()
{
    auto renderPass = RenderPass::create()
                          .withCamera(camera)
                          .withClearColor(true, {.20, .60, .86, 1})
                          .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles)
    {
        std::vector<glm::vec3> lines;
        for (auto &go : gameObjects)
        {
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr)
            {
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i", score);
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event &event)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->onKey(event);
    }
    if (event.type != SDL_KEYDOWN)
        return;
    if (event.key.keysym.sym == SDLK_d)
        debugCollisionCircles = !debugCollisionCircles;
    else if (!playing && timer > 1.0f && event.key.keysym.sym == SDLK_SPACE)
    {
        // Let "space" be a reset button if game is over, and at least a second has passed
        reset();
    }
    else if (event.key.keysym.sym == SDLK_r)
    {
        // Reset when "r" is pressed -- since I don't have a win screen
        reset();
    }
}

int main()
{
    new AsteroidsGame();
    return 0;
}