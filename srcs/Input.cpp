//
// Created by pirexa on 23/01/24.
//

#include "Input.hpp"

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
bool Input::firstMouse[3] = {false, false, false};
int Input::mouseMode = 0; // Toggle Mouse Mode: 0 = Control Cursor, 1 = Control Camera

void    Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key >= 0 && key < 1024) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
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
//		std::cout << "x: " << xpos << " y: " << ypos << std::endl;
		Input::beginDrag.x = xpos;
        Input::beginDrag.y = ypos;
        Input::currentDrag.x = xpos;
        Input::currentDrag.y = ypos;
		Input::keys[GLFW_MOUSE_BUTTON_LEFT] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
//		std::cout << "x: " << xpos << " y: " << ypos << std::endl;
		Input::keys[GLFW_MOUSE_BUTTON_LEFT] = false;
        Input::dragType = 0;
        std::fill(Input::firstMouse, Input::firstMouse + 3, false);
//        for (int i = 0; i < 3; i++)
//            Input::firstMouse[i] = false;
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
	}

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
    int corner = 0;
    if ((corner = gui.isClicked(Input::beginDrag)) > 0 && corner <= 5 && Input::keys[GLFW_MOUSE_BUTTON_LEFT]) {
        if (Input::dragType == 0)
        {
            Input::dragType = corner;
        }
        gui.dragResize(Input::beginDrag, Input::currentDrag, corner, Input::dragType);
        beginDrag = currentDrag;
    }
    else if (corner > 5 && Input::keys[GLFW_MOUSE_BUTTON_LEFT])
    {
        int buttonPressed = corner - 6;
		if (Input::dragType == 0)
		{
			if (buttonPressed == 0 && !Input::firstMouse[0])
			{
				Input::firstMouse[0] = true;
				if (Input::TextureMode == 1)
					Input::TextureMode = 0;
				else
					Input::TextureMode = 1;
			}
			else if (buttonPressed == 1 && !Input::firstMouse[1])
			{
				Input::firstMouse[1] = true;
				if (Input::WireframeMode == 1)
					Input::WireframeMode = 0;
				else
					Input::WireframeMode = 1;
			}
		}
    }
	if (Input::mouseMode == 1)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
