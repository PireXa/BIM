//
// Created by pirexa on 23/01/24.
//

//#include "Input.hpp"
#include "BIM.hpp"

bool Input::keys[1024] = {false};
double Input::lastX = 400.0f;
double Input::lastY = 300.0f;
int Input::dragType = 0; // 0 = None, 1 = TopRight, 2 = TopLeft, 3 = BottomRight, 4 = BottomLeft, 5 = Center
glm::vec2 Input::beginDrag = glm::vec2(0.0f, 0.0f);
glm::vec2 Input::currentDrag = glm::vec2(0.0f, 0.0f);
float Input::camera_yaw = 0.0f;
float Input::camera_pitch = 0.0f;
float Input::model_yaw = 0.0f;
float Input::model_pitch = 0.0f;
float Input::fov = 45.0f;
float Input::moveSpeed = 0.2f;
int Input::animationState = 1;
int Input::TextureMode = 1; // 1 = Texture, 0 = Use Normal
int Input::WireframeMode = 0; // 1 = Wireframe, 0 = Fill
bool Input::firstMouse[3] = {false, false, false}; // Button Array: 0 = Texture, 1 = Wireframe
int Input::mouseMode = 0; // Toggle Mouse Mode: 0 = Control Cursor, 1 = Control Camera
float Input::xoffset = 0.0f;
float Input::yoffset = 0.0f;
std::vector<std::string> Input::filePaths;
glm::vec2 Input::dropPosition = glm::vec2(0.0f, 0.0f);

void    Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key >= 0 && key < 1024) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_M && action == GLFW_PRESS)
        {
            if (Input::WireframeMode == 0)
                Input::WireframeMode = 1;
            else
                Input::WireframeMode = 0;
        }
        if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            if (Input::TextureMode == 1)
                Input::TextureMode = 0;
            else
                Input::TextureMode = 1;
        }
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		{
			if (Input::mouseMode == 0)
				Input::mouseMode = 1;
			else if (Input::mouseMode == 1)
				Input::mouseMode = 0;
		}
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
		Input::beginDrag.x = xpos;
        Input::beginDrag.y = ypos;
        Input::currentDrag.x = xpos;
        Input::currentDrag.y = ypos;
		Input::keys[GLFW_MOUSE_BUTTON_LEFT] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		Input::keys[GLFW_MOUSE_BUTTON_LEFT] = false;
        Input::dragType = 0;
        std::fill(Input::firstMouse, Input::firstMouse + 3, false);
	}
}

void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - Input::lastX;
	float yoffset = Input::lastY - ypos;
	Input::lastX = xpos;
	Input::lastY = ypos;

	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	if (!Input::keys[GLFW_KEY_LEFT_ALT] && Input::mouseMode == 1)
	{
		Input::camera_yaw += xoffset;
		Input::camera_pitch += yoffset;
	}
	else if (Input::keys[GLFW_KEY_LEFT_ALT] && Input::mouseMode == 1)
	{
		Input::model_yaw += xoffset;
		Input::model_pitch += yoffset;
	}
	else if (!Input::keys[GLFW_KEY_LEFT_ALT] && Input::mouseMode == 0 && Input::keys[GLFW_MOUSE_BUTTON_LEFT])
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Input::currentDrag.x = xpos;
		Input::currentDrag.y = ypos;
        Input::xoffset = xoffset;
        Input::yoffset = yoffset;
	}

	if (Input::camera_pitch > 89.0f)
		Input::camera_pitch = 89.0f;
	else if (Input::camera_pitch < -89.0f)
		Input::camera_pitch = -89.0f;
	if (Input::camera_yaw > 360.0f)
		Input::camera_yaw -= 360.0f;
	else if (Input::camera_yaw < -360.0f)
		Input::camera_yaw += 360.0f;
    if (Input::model_pitch > 89.0f)
        Input::model_pitch = 89.0f;
    else if (Input::model_pitch < -89.0f)
        Input::model_pitch = -89.0f;
    if (Input::model_yaw > 360.0f)
        Input::model_yaw -= 360.0f;
    else if (Input::model_yaw < -360.0f)
        Input::model_yaw += 360.0f;
    Input::animationState = 0;
}

void    Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Input::moveSpeed += yoffset * 0.05f;
	if (Input::moveSpeed <= 0.001f)
		Input::moveSpeed = 0.02f;
    Input::animationState = 0;
}

void    Input::dropCallback(GLFWwindow* window, int count, const char** paths) {
//    for (int i = 0; i < count; i++)
//        std::cout << paths[i] << std::endl;
    Input::filePaths = std::vector<std::string>(paths, paths + count);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    Input::dropPosition = glm::vec2(xpos, ypos);
}
