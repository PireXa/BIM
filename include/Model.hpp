//
// Created by fde-albe on 2/8/24.
//

#ifndef BIM_MODEL_HPP
#define BIM_MODEL_HPP

#include "GLM/glm.hpp"

class Model {
    private:
        glm::quat   orientation;
        glm::vec3   position;
    public:
        Model() : orientation(), position() {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        void    rotate(float angle, float x, float y, float z) {
            orientation = glm::rotate(orientation, angle, glm::vec3(x, y, z));
            orientation = glm::normalize(orientation);
        }

        void    translate(float x, float y, float z) {
            position.x += x;
            position.y += y;
            position.z += z;
        }

        glm::mat4   getModelMatrix() {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = model * glm::mat4_cast(orientation);
            return model;
        }
};

#endif //BIM_MODEL_HPP
