//
// Created by pirexa on 23/01/24.
//

#ifndef BIM_INPUT_HPP
#define BIM_INPUT_HPP

#include <iostream>
#include "glMath.hpp"
#include "Camera.hpp"

class Input {
    public:
        static bool keys[1024];
        static double lastX;
        static double lastY;
        static float yaw;
        static float pitch;
        static float fov;
		static float moveSpeed;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void doMovement(Camera &camera);
};

#endif //BIM_INPUT_HPP
