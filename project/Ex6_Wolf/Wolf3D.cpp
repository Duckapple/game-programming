//
// Created by Morten Nobel-Jørgensen on 22/09/2017.
//

#include "Wolf3D.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace std;
using namespace sre;
using namespace glm;

Wolf3D::Wolf3D()
    : fpsController(&camera)
{
    r.init();
    init();

    // Enable mouse lock
    SDL_SetWindowGrab(r.getSDLWindow(), SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_SetWindowFullscreen(r.getSDLWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);

    r.frameUpdate = [&](float deltaTime)
    {
        update(deltaTime);
    };
    r.frameRender = [&]()
    {
        render();
    };
    r.keyEvent = [&](SDL_Event &e)
    {
        fpsController.onKey(e);
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        {
            // Convenience for closing the program
            r.stopEventLoop();
        }
    };
    r.mouseEvent = [&](SDL_Event &e)
    {
        if (!lockRotation)
        {
            fpsController.onMouse(e);
        }
    };
    r.startEventLoop();
}

void Wolf3D::update(float deltaTime)
{
    fpsController.update(deltaTime);
}

void Wolf3D::render()
{
    auto renderPass = RenderPass::create()
                          .withCamera(camera)
                          .build();

    renderPass.draw(walls, glm::mat4(1), wallMaterial);
    renderPass.draw(floor, floorPosition, floorColor);
    renderPass.draw(floor, ceilPosition, ceilColor);

    if (debugBricks)
    {
        renderDebugBricks(renderPass);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::DragFloat("Rot", &fpsController.rotation);
    ImGui::DragFloat3("Pos", &(fpsController.position.x), 0.1f);
    ImGui::Checkbox("DebugBricks", &debugBricks);
    ImGui::Checkbox("LockRotation", &lockRotation);
    ImGui::End();
}

// Creates a textured cube with a side length of 1 (e.g. from -0.5 to 0.5).
// The cube is centered at (x,0,z)
// The texturing
void Wolf3D::addCube(std::vector<glm::vec3> &vertexPositions, std::vector<glm::vec4> &textureCoordinates, int x, int z, int type)
{
    float mX = x - 0.5, pX = x + 0.5, mZ = z - 0.5, pZ = z + 0.5;
    vertexPositions.insert(
        vertexPositions.end(),
        {// -x x
         glm::vec3(mX, -0.5, pZ), glm::vec3(pX, -0.5, pZ), glm::vec3(mX, 0.5, pZ),
         glm::vec3(pX, 0.5, pZ), glm::vec3(mX, 0.5, pZ), glm::vec3(pX, -0.5, pZ),
         // z -z
         glm::vec3(pX, -0.5, pZ), glm::vec3(pX, -0.5, mZ), glm::vec3(pX, 0.5, pZ),
         glm::vec3(pX, 0.5, mZ), glm::vec3(pX, 0.5, pZ), glm::vec3(pX, -0.5, mZ),
         // x -x
         glm::vec3(pX, -0.5, mZ), glm::vec3(mX, -0.5, mZ), glm::vec3(pX, 0.5, mZ),
         glm::vec3(mX, 0.5, mZ), glm::vec3(pX, 0.5, mZ), glm::vec3(mX, -0.5, mZ),
         // -z z
         glm::vec3(mX, -0.5, mZ), glm::vec3(mX, -0.5, pZ), glm::vec3(mX, 0.5, mZ),
         glm::vec3(mX, 0.5, pZ), glm::vec3(mX, 0.5, mZ), glm::vec3(mX, -0.5, pZ)});

    glm::vec2 textureSize(2048, 4096);
    glm::vec2 tileSize(64, 64);
    glm::vec2 tileSizeWithBorder(65, 65);
    int typeX = type % 8, typeY = type / 8;

    glm::vec2 minLight = vec2(typeX * 2 * tileSizeWithBorder.x, (42 - typeY) * tileSizeWithBorder.y) / textureSize;
    glm::vec2 maxLight = minLight + tileSize / textureSize;
    glm::vec2 minDark = minLight + (vec2(tileSizeWithBorder.x, 0) / textureSize);
    glm::vec2 maxDark = minDark + tileSize / textureSize;
    textureCoordinates.insert(
        textureCoordinates.end(),
        {glm::vec4(minLight.x, minLight.y, 0, 0), glm::vec4(maxLight.x, minLight.y, 0, 0), glm::vec4(minLight.x, maxLight.y, 0, 0),
         glm::vec4(maxLight.x, maxLight.y, 0, 0), glm::vec4(minLight.x, maxLight.y, 0, 0), glm::vec4(maxLight.x, minLight.y, 0, 0),
         glm::vec4(minDark.x, minDark.y, 0, 0), glm::vec4(maxDark.x, minDark.y, 0, 0), glm::vec4(minDark.x, maxDark.y, 0, 0),
         glm::vec4(maxDark.x, maxDark.y, 0, 0), glm::vec4(minDark.x, maxDark.y, 0, 0), glm::vec4(maxDark.x, minDark.y, 0, 0),
         glm::vec4(minLight.x, minLight.y, 0, 0), glm::vec4(maxLight.x, minLight.y, 0, 0), glm::vec4(minLight.x, maxLight.y, 0, 0),
         glm::vec4(maxLight.x, maxLight.y, 0, 0), glm::vec4(minLight.x, maxLight.y, 0, 0), glm::vec4(maxLight.x, minLight.y, 0, 0),
         glm::vec4(minDark.x, minDark.y, 0, 0), glm::vec4(maxDark.x, minDark.y, 0, 0), glm::vec4(minDark.x, maxDark.y, 0, 0),
         glm::vec4(maxDark.x, maxDark.y, 0, 0), glm::vec4(minDark.x, maxDark.y, 0, 0), glm::vec4(maxDark.x, minDark.y, 0, 0)});
}

void Wolf3D::init()
{
    wallMaterial = Shader::getUnlit()->createMaterial();
    auto texture = Texture::create().withFile("level0.png").withGenerateMipmaps(false).withFilterSampling(false).build();
    wallMaterial->setTexture(texture);

    map.loadMap("level0.json");

    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec4> textureCoordinates;

    for (int x = 0; x < map.getWidth(); x++)
    {
        for (int y = 0; y < map.getHeight(); y++)
        {
            int field = map.getTile(x, y);
            if (field != -1)
            {
                addCube(vertexPositions, textureCoordinates, x, y, field);
            }
        }
    }

    fpsController.setInitialPosition(map.getStartingPosition(), map.getStartingRotation());

    walls = Mesh::create()
                .withPositions(vertexPositions)
                .withUVs(textureCoordinates)
                .build();

    std::vector<glm::vec4> colors;

    auto size = map.getWidth() > map.getHeight() ? map.getWidth() : map.getHeight();

    floorColor = Shader::getUnlit()->createMaterial();
    floorColor->setColor(Color(map.getFloorColor()));
    ceilColor = Shader::getUnlit()->createMaterial();
    ceilColor->setColor(Color(map.getCeilColor()));

    floor = Mesh::create().withQuad(size / 2).build();

    floorPosition = glm::mat4(1);
    floorPosition = glm::translate(floorPosition, glm::vec3(size / 2, 0.5, size / 2));
    floorPosition = glm::rotate(floorPosition, glm::pi<float>() / 2, glm::vec3(1, 0, 0));

    ceilPosition = glm::translate(floorPosition, glm::vec3(0, 0, 1));
    ceilPosition = glm::rotate(ceilPosition, glm::pi<float>(), glm::vec3(1, 0, 0));
}

void Wolf3D::renderDebugBricks(RenderPass &renderPass)
{
    static auto cube = Mesh::create().withCube(0.5f).build();
    static vector<shared_ptr<Material>> materials = {
        Shader::getUnlit()->createMaterial(),
        Shader::getUnlit()->createMaterial(),
        Shader::getUnlit()->createMaterial()};

    std::vector<vec3> positions = {
        {-1, 0, -2},
        {0, 0, -3},
        {1, 0, -4}};
    std::vector<sre::Color> colors = {
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 1},
    };
    for (int i = 0; i < positions.size(); i++)
    {
        materials[i]->setColor(colors[i]);
        renderPass.draw(cube, glm::translate(positions[i]), materials[i]);
    }
}

int main()
{
    new Wolf3D();
    return 0;
}