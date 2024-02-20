//
// Created by pirexa on 18/01/24.
//

#ifndef BIM_GLMATH_HPP
#define BIM_GLMATH_HPP

#include "GLEW/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/quaternion.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"


struct Vertex {
	float x, y, z;
    float u, v; // Texture coordinates
//    float nx, ny, nz; // Normals
};

struct Pixel {
	unsigned char r, g, b;
};

struct UV {
	float u, v;
};

#endif //BIM_GLMATH_HPP
