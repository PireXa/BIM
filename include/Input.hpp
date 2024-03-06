//
// Created by pirexa on 23/01/24.
//

#ifndef BIM_INPUT_HPP
#define BIM_INPUT_HPP

#include <iostream>
#include "glMath.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include <vector>

class Input {
    public:
        static bool keys[1024];
        static double lastX;
        static double lastY;
        static glm::vec2 beginDrag;
        static glm::vec2 currentDrag;
        static bool dragging;
        static float camera_yaw;
        static float camera_pitch;
        static float model_yaw;
        static float model_pitch;
        static float fov;
		static float moveSpeed;
        static int animationState;
        static int TextureMode;
        static int WireframeMode;
        static int dragType;
        static bool firstMouse[3];
		static int mouseMode;
        static float xoffset;
        static float yoffset;
        static glm::vec2 dropPosition;
        static std::vector<std::string> filePaths;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void dropCallback(GLFWwindow* window, int count, const char** paths);
};

#endif //BIM_INPUT_HPP
