//
// Created by pirexa on 23/01/24.
//

#include "Input.hpp"

bool Input::keys[1024] = {false};
bool Input::firstMouse = true;
double Input::lastX = 400.0f;
double Input::lastY = 300.0f;
float Input::yaw = 0.0f;
float Input::pitch = 0.0f;
float Input::fov = 45.0f;

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
//	std::cout << xoffset << " " << yoffset << std::endl;

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

void    Input::doMovement(GLFWwindow *window, Camera &camera) {
    if (Input::keys[GLFW_KEY_W] || Input::keys[GLFW_KEY_UP]) {
        glm::vec3 direction = camera.getDirectionVector();
        camera.translate(direction.x * STEP, direction.y * STEP, direction.z * STEP);
    }
    if (Input::keys[GLFW_KEY_S] || Input::keys[GLFW_KEY_DOWN]) {
        glm::vec3 direction = camera.getDirectionVector();
        camera.translate(-direction.x * STEP, -direction.y * STEP, -direction.z * STEP);
    }
    if (Input::keys[GLFW_KEY_A] || Input::keys[GLFW_KEY_LEFT]) {
		glm::vec3 right = camera.getRightVector();
		camera.translate(-right.x * STEP, -right.y * STEP, -right.z * STEP);
	}
    if (Input::keys[GLFW_KEY_D] || Input::keys[GLFW_KEY_RIGHT]) {
		glm::vec3 right = camera.getRightVector();
		camera.translate(right.x * STEP, right.y * STEP, right.z * STEP);
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
