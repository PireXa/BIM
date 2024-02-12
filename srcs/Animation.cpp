//
// Created by fde-albe on 2/12/24.
//

#include <iostream>
#include "Animation.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> Animation::start = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::high_resolution_clock> Animation::end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(2);

bool    Animation::InitialAnimation(Camera *camera, glm::vec3 modelCenter)
{
    auto now = std::chrono::high_resolution_clock::now();
    if (now < end)
    {
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - start).count();
        camera->translate(0.0f, 0.02f * deltaTime, 0.05f * deltaTime);
        camera->lookAt(modelCenter);
        return true;
    }
    return false;
}
