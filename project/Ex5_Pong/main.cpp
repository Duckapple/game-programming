#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

#include "Pong.hpp"

using namespace std;

int main(int argc, char** argv) {
    new Pong();

    return 0;
}