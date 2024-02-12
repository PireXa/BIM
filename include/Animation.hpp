//
// Created by fde-albe on 2/12/24.
//

#ifndef BIM_ANIMATION_HPP
#define BIM_ANIMATION_HPP

#include "Camera.hpp"
#include <chrono>

class Animation {
    private:
        static std::chrono::time_point<std::chrono::high_resolution_clock> start;
        static std::chrono::time_point<std::chrono::high_resolution_clock> end;
    public:
        static bool InitialAnimation(Camera *camera, glm::vec3 modelCenter);
};

#endif //BIM_ANIMATION_HPP
