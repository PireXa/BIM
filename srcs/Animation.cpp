//
// Created by fde-albe on 2/12/24.
//

#include <iostream>
#include "Animation.hpp"
#include "Input.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> Animation::start = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::high_resolution_clock> Animation::end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(2);
int Animation::animationState = 0;

bool    Animation::InitialAnimation(Camera *camera, glm::vec3 modelCenter, float Scale, Model *model)
{
    if (animationState == 2)
        return false;
    glm::vec3 cameraPosition = modelCenter + glm::vec3(0.5f * Scale, 0.5f * Scale, 0.5f * Scale);
    float distance = glm::length(cameraPosition - modelCenter);
    float distance2 = glm::length(camera->getPosition() - modelCenter);
    auto now = std::chrono::high_resolution_clock::now();
    if (distance > distance2 && animationState == 0)
    {
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - start).count();
        camera->translate(0.02f * deltaTime, 0.003f * deltaTime, 0.01f * deltaTime);
        camera->lookAt(modelCenter);
        return true;
    }
    else if (animationState == 1 && Input::animationState == 1)
    {
        model->rotate(0.007f, 0.0f, 1.0f, 0.0f);
        return false;
    }
    if (animationState == 0)
        animationState = 1;
    else if (animationState == 1)
        animationState = 2;
    return false;
}

bool   Animation::TransitionAnimation(float &blendFactor)
{
    if (!Input::beginTransition || animationState == 4 || Input::animationState == 2)
        return false;
    else if (animationState == 2 || animationState == 1)
    {
        animationState = 3;
        start = std::chrono::high_resolution_clock::now();
    }
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - start).count();
    if (deltaTime < 2.0f)
    {
        blendFactor = deltaTime / 2.0f;
        if (blendFactor > 1.0f)
            blendFactor = 1.0f;
        return true;
    }
    animationState = 2;
    Input::beginTransition = false;
    blendFactor = 1.0f;
    return false;
}

bool Animation::buildAnimation(Model *model)
{
    if (!Input::beginTransition || animationState == 3 || Input::animationState == 1)
        return false;
    else if (animationState == 2 || animationState == 1)
    {
        animationState = 4;
        start = std::chrono::high_resolution_clock::now();
    }
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - start).count();
    if (deltaTime < 3.0f)
    {
        float buildVertexCount = deltaTime * (model->getObj().getVertexCount() + 1) / 3.0f;
        model->setBuildAnimationVertexCount(buildVertexCount);
        return true;
    }
    animationState = 2;
    Input::beginTransition = false;
    model->changeTextureScale(0.0f);
    return true;
}
