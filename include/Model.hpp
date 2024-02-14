//
// Created by fde-albe on 2/8/24.
//

#ifndef BIM_MODEL_HPP
#define BIM_MODEL_HPP

#include "GLM/glm.hpp"
#include "readOBJ.hpp"

class Model {
    private:
        glm::quat   orientation;
        glm::vec3   position;
        glm::vec3   center;
        BoundingBox boundingBox;
        float       scale;
    public:
        Model() : orientation(), position() {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        glm::vec3 getCenter() {
            return center;
        }

        BoundingBox getBoundingBox() {
            return boundingBox;
        }

        float  getScale() {
            return scale;
        }

        void    setCenter(glm::vec3 center) {
            this->center = center;
        }

        void    setBoundingBox(BoundingBox boundingBox) {
            this->boundingBox = boundingBox;
            scale = glm::length(boundingBox.max - boundingBox.min);
        }

        void    rotate(float angle, float x, float y, float z) {
            glm::quat rotation = glm::angleAxis(angle, glm::vec3(x, y, z));
            orientation = rotation * orientation;
            orientation = glm::normalize(orientation);
        }

        void    rotate_pitch(float pitch, glm::vec3 right) {
            glm::quat rotation = glm::angleAxis(-pitch, right);
            orientation = glm::normalize(rotation * orientation);
        }

        void    rotate_yaw(float yaw) {
            glm::quat rotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
            orientation = glm::normalize(rotation * orientation);
        }

        void    translate(float x, float y, float z) {
            position.x += x;
            position.y += y;
            position.z += z;
            center.x += x;
            center.y += y;
            center.z += z;
        }

        glm::mat4 getModelMatrix() {
            glm::mat4 model = glm::mat4(1.0f);

            // Translate model to align centroid with origin
            model = glm::translate(model, center);

            // Apply rotation (using quaternion)
            glm::mat4 rotationMatrix = glm::mat4_cast(orientation);
            model = model * rotationMatrix;

            // Translate back to original position
            model = glm::translate(model, -center);

            // Apply model translation
            model = glm::translate(model, position);

            return model;
        }
};

#endif //BIM_MODEL_HPP
