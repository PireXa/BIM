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

void    updateModel(GLFWwindow *window, Model &model, RenderBatch &modelBatch)
{
    if (Input::filePaths.size() > 0)
    {
        for (int i = 0; i < Input::filePaths.size(); i++)
        {
            if (Input::filePaths[i].find(".obj") != std::string::npos)
            {
                model.setOBJ(Input::filePaths[i]);
                model.setCenter(model.getObj().getCenter());
                model.setBoundingBox(model.getObj().getBoundingBox());
                model.setScale(model.getObj().getBoundingBox().max.x - model.getObj().getBoundingBox().min.x);
                modelBatch.setVertices(model.getObj().getVerticesArray(), model.getObj().getVertexCount());
            }
            else if (Input::filePaths[i].find(".bmp") != std::string::npos)
            {
//                std::cout << "Texture: " << Input::filePaths[i] << std::endl;
                model.setTexture(Input::filePaths[i].c_str());
                modelBatch.setTexture(*model.getTexture().getTextureID());
                glGenTextures(1, model.getTexture().getTextureID());
                glBindTexture(GL_TEXTURE_2D, *model.getTexture().getTextureID());
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, model.getTexture().getWidth(), model.getTexture().getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, model.getTexture().getPixels().data());
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        Input::filePaths.clear();
        Input::dropPosition = glm::vec2(-500.0f, -500.0f);
    }
}

bool mouseIntersectModel(GLFWwindow *window, Model &model, glm::mat4 &mvpMatrix) {
    if (Input::dropPosition.x == -500.0f && Input::dropPosition.y == -500.0f)
        return false;
    //Calculate the mvpMatrix Inverse
    glm::mat4 invMVP = glm::inverse(mvpMatrix);
    //Calculate the ray direction
    // Normalize mouse coordinates to range [-1, 1]
    float x = (2.0f * Input::dropPosition.x) / WIN_WIDTH - 1.0f;
    float y = 1.0f - (2.0f * Input::dropPosition.y) / WIN_HEIGHT;

    // Construct near and far points in screen space
    glm::vec4 nearPoint = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 farPoint = glm::vec4(x, y, 1.0f, 1.0f);

    // Transform points to world space
    glm::vec4 nearWorld = invMVP * nearPoint;
    glm::vec4 farWorld = invMVP * farPoint;

    // Convert to homogeneous coordinates
    nearWorld /= nearWorld.w;
    farWorld /= farWorld.w;

    // Construct ray
    glm::vec3 origin = glm::vec3(nearWorld);
    glm::vec3 direction = glm::normalize(glm::vec3(farWorld - nearWorld));

    //Check if the ray intersects the model
    return (model.intersectRay(origin, direction));
}
