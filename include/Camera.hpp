//
// Created by kikop on 16/01/2024.
//

#ifndef BIM_CAMERA_HPP
#define BIM_CAMERA_HPP

#include <cmath>
#include "glMath.hpp"
#include "Quaternion.hpp"

class Camera {
    private:
        glm::quat   orientation;
        glm::vec3   position;
    public:
        Camera() : orientation(), position() {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(0.0f, 10.0f, 20.0f);
        }
        void    rotate(float angle, float x, float y, float z) {
            glm::vec3 axis = glm::normalize(glm::vec3(x, y, z));
            glm::quat rotation = glm::angleAxis(angle, axis);
            orientation = rotation * orientation;
        }

        void    translate(float x, float y, float z) {
            position.x += x;
            position.y += y;
            position.z += z;
        }

        glm::mat4   getViewMatrix()
        {
            glm::mat3 rotationMatrix = glm::mat3_cast(orientation);
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);
            glm::mat4 viewMatrix = glm::mat4(rotationMatrix) * translationMatrix;
            return viewMatrix;
        }
};

#endif //BIM_CAMERA_HPP
