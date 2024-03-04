//
// Created by fde-albe on 3/4/24.
//

#include "BIM.hpp"

void    updateStates(GLFWwindow* window, Camera &camera, Model &model, GUI &gui)
{
    int corner = 0;
    if ((corner = gui.isClicked(Input::beginDrag)) > 0 && corner <= 5 && Input::keys[GLFW_MOUSE_BUTTON_LEFT]) {
        if (Input::dragType == 0)
        {
            Input::dragType = corner;
        }
        gui.dragResize(Input::beginDrag, Input::currentDrag, corner, Input::dragType);
        Input::beginDrag = Input::currentDrag;
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
    else if (Input::keys[GLFW_MOUSE_BUTTON_LEFT])
    {
        Input::model_yaw += Input::xoffset * 10.0f;
        Input::model_pitch += Input::yoffset * 10.0f;
        Input::xoffset = 0.0f;
        Input::yoffset = 0.0f;
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
