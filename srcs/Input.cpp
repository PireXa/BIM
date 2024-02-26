//
// Created by pirexa on 23/01/24.
//

#include "Input.hpp"

bool Input::keys[1024] = {false};
double Input::lastX = 400.0f;
double Input::lastY = 300.0f;
double Input::lastDragX = 0.0f;
double Input::lastDragY = 0.0f;
float Input::camera_yaw = 0.0f;
float Input::camera_pitch = 0.0f;
float Input::model_yaw = 0.0f;
float Input::model_pitch = 0.0f;
float Input::fov = 45.0f;
float Input::moveSpeed = 0.2f;
int Input::animationState = 1;

void    Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key >= 0 && key < 1024) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
        if (action == GLFW_PRESS)
        {
            Input::keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
			Input::keys[key] = false;
        }
    }
    Input::animationState = 0;
}

void	Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "x: " << xpos << " y: " << ypos << std::endl;
		Input::lastDragX = xpos;
		Input::lastDragY = ypos;
		Input::keys[GLFW_MOUSE_BUTTON_LEFT] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "x: " << xpos << " y: " << ypos << std::endl;
		Input::lastDragX = xpos;
		Input::lastDragY = ypos;
		Input::keys[GLFW_MOUSE_BUTTON_LEFT] = false;
	}

}

void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - Input::lastX;
	float yoffset = Input::lastY - ypos;
	Input::lastX = xpos;
	Input::lastY = ypos;

	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

    if (!Input::keys[GLFW_KEY_LEFT_ALT] && !Input::keys[GLFW_KEY_LEFT_CONTROL])
    {
        Input::camera_yaw += xoffset;
        Input::camera_pitch += yoffset;
    }
    else if (Input::keys[GLFW_KEY_LEFT_ALT] && !Input::keys[GLFW_KEY_LEFT_CONTROL])
    {
        Input::model_yaw += xoffset;
        Input::model_pitch += yoffset;
    }
//	else if (!Input::keys[GLFW_KEY_LEFT_ALT] && Input::keys[GLFW_KEY_LEFT_CONTROL] && Input::keys[GLFW_MOUSE_BUTTON_LEFT])
//	{
//		std::cout << "x: " << xpos << " y: " << ypos << std::endl;
//	}

	if (Input::camera_pitch > 89.0f)
		Input::camera_pitch = 89.0f;
	if (Input::camera_pitch < -89.0f)
		Input::camera_pitch = -89.0f;
	if (Input::camera_yaw > 360.0f)
		Input::camera_yaw -= 360.0f;
	if (Input::camera_yaw < -360.0f)
		Input::camera_yaw += 360.0f;
    if (Input::model_pitch > 89.0f)
        Input::model_pitch = 89.0f;
    if (Input::model_pitch < -89.0f)
        Input::model_pitch = -89.0f;
    if (Input::model_yaw > 360.0f)
        Input::model_yaw -= 360.0f;
    if (Input::model_yaw < -360.0f)
        Input::model_yaw += 360.0f;
    Input::animationState = 0;
}

void    Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Input::moveSpeed += yoffset * 0.05f;
	if (Input::moveSpeed <= 0.001f)
		Input::moveSpeed = 0.02f;
    Input::animationState = 0;
}

void    Input::doMovement(GLFWwindow* window, Camera &camera, Model &model, GUI &gui) {
	if (Input::keys[GLFW_KEY_LEFT_CONTROL]) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (Input::keys[GLFW_KEY_Q]) {
		camera.translate(0.0f, 0.0f, -camera.getMoveSpeed());
	}
	if (Input::keys[GLFW_KEY_E]) {
		camera.translate(0.0f, 0.0f, camera.getMoveSpeed());
	}
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
    if (Input::keys[GLFW_KEY_R]) {
		camera.lookAt(model.getCenter());
    }
    if (Input::keys[GLFW_KEY_T]) {
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    }
    if (Input::keys[GLFW_KEY_LEFT]) {
        gui.translate(glm::vec2(-1.0f, 0.0f));
    }
    if (Input::keys[GLFW_KEY_RIGHT]) {
        gui.translate(glm::vec2(1.0f, 0.0f));
    }
    if (Input::keys[GLFW_KEY_UP]) {
        gui.translate(glm::vec2(0.0f, 1.0f));
    }
    if (Input::keys[GLFW_KEY_DOWN]) {
        gui.translate(glm::vec2(0.0f, -1.0f));
    }
    if (Input::keys[GLFW_KEY_Y]) {
        gui.addWidth(10);
    }
    if (Input::keys[GLFW_KEY_H]) {
        gui.addWidth(-10);
    }
    if (Input::keys[GLFW_KEY_F]) {
        model.reset();
    }
    if (Input::keys[GLFW_KEY_I]) {
        model.translate(0.0f, 0.0f, 0.2f);
    }
    if (Input::keys[GLFW_KEY_K]) {
        model.translate(0.0f, 0.0f, -0.2f);
    }
	if (Input::camera_pitch != 0.0f)
	{
		camera.rotate_pitch(Input::camera_pitch);
		Input::camera_pitch = 0.0f;
	}
	if (Input::camera_yaw != 0.0f)
	{
		camera.rotate_yaw(Input::camera_yaw);
		Input::camera_yaw = 0.0f;
	}
    if (Input::model_pitch != 0.0f) {
        glm::vec3 right = camera.getRightVector();
        model.rotate(-Input::model_pitch, right.x, right.y, right.z);
        Input::model_pitch = 0.0f;
    }
    if (Input::model_yaw != 0.0f) {
        glm::vec3 up = glm::cross(camera.getRightVector(), camera.getDirectionVector());
        model.rotate(Input::model_yaw, up.x, up.y, up.z);
        Input::model_yaw = 0.0f;
    }
}
