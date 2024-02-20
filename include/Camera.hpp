//
// Created by kikop on 16/01/2024.
//

#ifndef BIM_CAMERA_HPP
#define BIM_CAMERA_HPP

#include <cmath>
#include "glMath.hpp"
#include "Quaternion.hpp"
#define SENSITIVITY 0.0005f

class Camera {
    private:
        glm::quat   orientation;
        glm::vec3   position;
		float       moveSpeed;
    public:
        Camera() : orientation(), position() {
            position = glm::vec3(0.0f, 1.0f, 1.5f);
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
            moveSpeed = 0.5f;
        }

		float getMoveSpeed() {
			return moveSpeed;
		}

        glm::quat getOrientation() {
            return orientation;
        }

        glm::vec3 getPosition() {
            return position;
        }

        void    setPosition(glm::vec3 position) {
            this->position = position;
        }

		void setMoveSpeed(float speed) {
			moveSpeed = speed;
		}

        void    rotate(float angle, float x, float y, float z) {
            glm::vec3 axis = glm::normalize(glm::vec3(x, y, z));
            glm::quat rotation = glm::angleAxis(angle, axis);
            orientation = rotation * orientation;
        }

		void    rotate_pitch(float pitch) {
			glm::vec3 axis = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
			glm::quat rotation = glm::angleAxis(pitch, axis);
			orientation = glm::normalize(rotation * orientation);
		}

		void rotate_yaw(float yaw) {
			// Specify the rotation axis in world coordinates
			glm::vec3 worldYawAxis = glm::vec3(0.0f, 1.0f, 0.0f);

			// Apply the rotation to the orientation quaternion
			orientation = glm::rotate(orientation, yaw, worldYawAxis);

			// Ensure the quaternion is normalized to prevent unwanted drift
			orientation = glm::normalize(orientation);
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

        void    lookAt(glm::vec3 target) {
            glm::vec3 direction = target - position;
            direction = glm::normalize(direction);
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 right = glm::cross(direction, up);
            glm::vec3 newUp = glm::cross(right, direction);

            glm::mat3 rotationMatrix = glm::mat3(right, newUp, -direction);
            orientation = glm::quat_cast(rotationMatrix);
            orientation = glm::normalize(orientation);
            orientation = glm::conjugate(orientation);
        }

};

#endif //BIM_CAMERA_HPP
