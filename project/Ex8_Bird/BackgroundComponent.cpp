//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include <sre/SpriteAtlas.hpp>
#include <sre/Texture.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "BackgroundComponent.hpp"
#include "BirdGame.hpp"

using namespace sre;

BackgroundComponent::BackgroundComponent()
{
}

void BackgroundComponent::renderBackground(sre::RenderPass &renderPass, float offset)
{
    renderPass.draw(batch, glm::translate(glm::vec3(offset, 0, 0)));
}

void BackgroundComponent::init(std::string filename)
{
    tex = Texture::create().withFile(filename).withFilterSampling(false).build();

    auto atlas = SpriteAtlas::createSingleSprite(tex, "background", glm::vec2(0, 0));
    auto sprite = atlas->get("background");
    float scale = BirdGame::windowSize.y / tex->getHeight();
    sprite.setScale({scale, scale});
    auto batchBuilder = SpriteBatch::create();
    for (int i = 0; i < 100; i++)
    {
        sprite.setPosition(glm::vec2(tex->getWidth() * (i - 1) * scale, 0));
        batchBuilder.addSprite(sprite);
    }
    batch = batchBuilder.build();
}
