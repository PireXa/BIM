//
// Created by pirexa on 16-02-2024.
//

#ifndef BIM_BIM_HPP
#define BIM_BIM_HPP

//#define WIN_WIDTH 1920.0f
//#define WIN_HEIGHT 1080.0f
#define WIN_WIDTH 1280.0f
#define WIN_HEIGHT 720.0f

#define GLEW_STATIC
#include <iostream>
#include <iomanip>
#include "GLEW/glew.h"
#include "glMath.hpp"
#include "Camera.hpp"
#include "Matrix.hpp"
#include "Input.hpp"
#include "readOBJ.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <thread>
#include <algorithm>
#include "Texture.hpp"
#include "Animation.hpp"
#include "DefaultPlane.hpp"
#include "Font.hpp"
#include "GUI.hpp"
#include "Button.hpp"

void    fpsCounter(std::chrono::time_point<std::chrono::high_resolution_clock> &lastFPSTime, int &frameCount, double &fps);

#endif //BIM_BIM_HPP
