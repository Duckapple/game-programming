#include "Box.hpp"
#include "sre/Shader.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace sre;

Box::Box()
{
    position = glm::vec2(0, 0);
    scale = glm::vec2(1, 1);
}

void Box::draw(sre::RenderPass &renderPass)
{
    renderPass.draw(mesh, getTransform(), material);
}

glm::mat4 Box::getTransform()
{
    auto t = glm::mat4(1);
    t = glm::translate(t, glm::vec3(position, 0));
    t = glm::scale(t, glm::vec3(scale, 0.1));
    return t;
}

Edge2D Box::getEdge(int index)
{
    Edge2D res;
    glm::vec4 vertices[] = {
        glm::vec4(-1, -1, 0, 1),
        glm::vec4(1, -1, 0, 1),
        glm::vec4(1, 1, 0, 1),
        glm::vec4(-1, 1, 0, 1),
    };
    res.from = vec2(getTransform() * vertices[index]);
    res.to = vec2(getTransform() * vertices[(index + 1) % 4]);
    glm::vec2 norm = glm::normalize(res.to - res.from);
    res.normal = glm::vec2(norm.y, -norm.x);

    return res;
}

void Box::init()
{
    mesh = Mesh::create().withCube().build();
    auto shader = Shader::getUnlit();
    material = shader->createMaterial();
    material->setColor(sre::Color(1, 1, 1));
}
