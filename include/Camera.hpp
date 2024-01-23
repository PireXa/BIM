//
// Created by kikop on 16/01/2024.
//

#ifndef BIM_CAMERA_HPP
#define BIM_CAMERA_HPP

#include <cmath>
#include "glMath.hpp"
#include "Quaternion.hpp"
#define STEP 4.0f
#define SENSITIVITY 0.002f

class Camera {
    private:
        glm::quat   orientation;
        glm::vec3   position;
    public:
        Camera() : orientation(), position() {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            position = glm::vec3(-150.0f, 150.0f, 250.0f);
//			position = glm::vec3(-2.0f, 1.5f, 5.0f);
        }
        void    rotate(float angle, float x, float y, float z) {
            glm::vec3 axis = glm::normalize(glm::vec3(x, y, z));
            glm::quat rotation = glm::angleAxis(angle, axis);
            orientation = rotation * orientation;
        }

		void    rotate_pitch(float pitch) {
			glm::vec3 axis = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
			glm::quat rotation = glm::angleAxis(pitch, axis);
			orientation = rotation * orientation;
		}

		void	rotate_yaw(float yaw) {
			glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat rotation = glm::angleAxis(yaw, axis);
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

        //function that returns the direction vector of the camera
        glm::vec3 getDirectionVector() {
            glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
            direction = glm::normalize(direction * orientation);
            return direction;
        }

		glm::vec3 getRightVector() {
			glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
			direction = glm::normalize(direction * orientation);
			return direction;
		}

};

#endif //BIM_CAMERA_HPP
