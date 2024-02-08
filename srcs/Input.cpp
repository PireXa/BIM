//
// Created by pirexa on 23/01/24.
//

#include "Input.hpp"

bool Input::keys[1024] = {false};
double Input::lastX = 400.0f;
double Input::lastY = 300.0f;
float Input::yaw = 0.0f;
float Input::pitch = 0.0f;
float Input::fov = 45.0f;
float Input::moveSpeed = 0.5f;

void    Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key >= 0 && key < 1024) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
        if (action == GLFW_PRESS)
            Input::keys[key] = true;
        else if (action == GLFW_RELEASE) {
			Input::keys[key] = false;
        }
    }
}

void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - Input::lastX;
	float yoffset = Input::lastY - ypos;
	Input::lastX = xpos;
	Input::lastY = ypos;

	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	Input::yaw += xoffset;
	Input::pitch += yoffset;

	if (Input::pitch > 89.0f)
		Input::pitch = 89.0f;
	if (Input::pitch < -89.0f)
		Input::pitch = -89.0f;
	if (Input::yaw > 360.0f)
		Input::yaw -= 360.0f;
	if (Input::yaw < -360.0f)
		Input::yaw += 360.0f;
}

void    Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Input::moveSpeed += yoffset * 0.05f;
	if (Input::moveSpeed < 0)
		Input::moveSpeed = 0.02f;
}

void    Input::doMovement(Camera &camera, Model &model) {
    if (Input::keys[GLFW_KEY_W]) {
        glm::vec3 direction = camera.getDirectionVector();
        camera.translate(direction.x * camera.getMoveSpeed(), direction.y * camera.getMoveSpeed(), direction.z * camera.getMoveSpeed());
    }
    if (Input::keys[GLFW_KEY_S]) {
        glm::vec3 direction = camera.getDirectionVector();
        camera.translate(-direction.x * camera.getMoveSpeed(), -direction.y * camera.getMoveSpeed(), -direction.z * camera.getMoveSpeed());
    }
    if (Input::keys[GLFW_KEY_A]) {
		glm::vec3 right = camera.getRightVector();
		camera.translate(-right.x * camera.getMoveSpeed(), -right.y * camera.getMoveSpeed(), -right.z * camera.getMoveSpeed());
	}
    if (Input::keys[GLFW_KEY_D]) {
		glm::vec3 right = camera.getRightVector();
		camera.translate(right.x * camera.getMoveSpeed(), right.y * camera.getMoveSpeed(), right.z * camera.getMoveSpeed());
	}
    if (Input::keys[GLFW_KEY_SPACE]) {
        camera.translate(0.0f, camera.getMoveSpeed(), 0.0f);
    }
    if (Input::keys[GLFW_KEY_LEFT_SHIFT]) {
        camera.translate(0.0f, -camera.getMoveSpeed(), 0.0f);
    }
    if (Input::keys[GLFW_KEY_LEFT]) {
        model.rotate(0.1f, 0.0f, 1.0f, 0.0f);
    }
    if (Input::keys[GLFW_KEY_RIGHT]) {
        model.rotate(-0.1f, 0.0f, 1.0f, 0.0f);
    }
    if (Input::keys[GLFW_KEY_UP]) {
        model.rotate(0.1f, 1.0f, 0.0f, 0.0f);
    }
    if (Input::keys[GLFW_KEY_DOWN]) {
        model.rotate(-0.1f, 1.0f, 0.0f, 0.0f);
    }
    if (Input::keys[GLFW_KEY_I]) {
        model.translate(0.0f, 0.0f, 1.0f);
    }
    if (Input::keys[GLFW_KEY_K]) {
        model.translate(0.0f, 0.0f, -1.0f);
    }
	if (Input::pitch != 0.0f)
	{
		camera.rotate_pitch(Input::pitch);
		Input::pitch = 0.0f;
	}
	if (Input::yaw != 0.0f)
	{
		camera.rotate_yaw(Input::yaw);
		Input::yaw = 0.0f;
	}
}
