#define GLEW_STATIC
#include <iostream>
#include "GLEW/glew.h"
#include "glMath.hpp"
#include "Camera.hpp"
#include "Matrix.hpp"
#include "Input.hpp"
#include "readOBJ.hpp"
#include "Texture.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <algorithm>
#include "Texture.hpp"

const char* vertexShaderSource = R"(
	#version 330 core

//	uniform mat4 view;  // Uniform view matrix
//	uniform mat4 model;  // Uniform model matrix
//	uniform mat4 projection;  // Uniform projection matrix
    uniform mat4 vp;  // Uniform view-projection matrix

	layout (location = 0) in vec3 position;  // Input: Vertex position
	layout (location = 1) in vec3 uv;     // Input: Vertex color

	out vec2 FragColor;  // Output: TextCoord for fragment shader

	void main() {
	    // Transform vertex position to camera space
		gl_Position = vp * vec4(position, 1.0);

		vec2 texCoord = uv.xy;
	    // Pass color to the fragment shader
	    FragColor = texCoord;
		//FragColor = uv; // Pass rgb color to the fragment shader (for classic rainbow effect)
	}
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    in vec2 FragColor;  // Input: Vertex color (vec3 if using classic rainbow effect)

	uniform sampler2D textureSampler;  // Texture sampler

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {
		// Sample the texture using the texture coordinates
        vec4 texColor = texture(textureSampler, FragColor);

        // Output the color directly
        FinalColor = texColor;
    }
)";

GLuint   ShaderSetups() {
	// Compile Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Compile Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Link Shaders into a Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Clean up individual shaders (they are no longer needed after linking)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "BIM", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//Enable rendering of only front-facing triangles
	glEnable(GL_DEPTH_TEST);

	//Vsync 1-on, 0-off
	glfwSwapInterval(1);

	//Enable culing of back-facing triangles
//	glEnable(GL_CULL_FACE);

	GLuint shaderProgram = ShaderSetups();

	Camera camera;

	const char * windows_filename = "..\\Models\\Porsche_911_GT2.obj";
    const char * debian_filename = "./Models/42.obj";

	readOBJ obj(debian_filename);
    if (obj.getuvCount() == 0)
        obj.PlanarMapping();
	std::cout << "Face count: " << obj.getFaceCount() << std::endl;
	std::cout << "vt count: " << obj.getuvCount() << std::endl;

	Texture texture("./Models/pattern5.bmp");
    glGenTextures(1, texture.getTextureID());
	glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getPixels().data());

	// Vertex Buffer Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO and copy vertex data to it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * obj.getVertexCount() , obj.getVerticesArray(), GL_STATIC_DRAW);

	// Specify vertex attribute pointers and enable them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//    const double targetFrameTime = 1.0f * 1e6 / 60.0f;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto lastFPSTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
//
//	glfwGetCursorPos(window, &Input::lastX, &Input::lastY);
//	std::cout << Input::lastX << " " << Input::lastY << std::endl;

	while (!glfwWindowShouldClose(window)) {

//        lastFrameTime = std::chrono::high_resolution_clock::now();

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update view matrix
        glm::mat4 viewMatrix;
        viewMatrix = camera.getViewMatrix();

        // Set up the projection matrix (you might do this once in your initialization)
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), // FOV
                                                      800.0f / 600.0f,          // Aspect ratio
                                                      0.1f, 600.0f);        // Near and far planes

        // Combine the view matrix and projection matrix to get the final MVP matrix
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix;

        // Pass the MVP matrix to the shader
        GLint mvpMatrixLoc = glGetUniformLocation(shaderProgram, "vp");
        glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		// Use the shader program and VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, obj.getVertexCount());

		// FPS calculation
		auto currentTime = std::chrono::high_resolution_clock::now();
		double deltaFPSMeasureTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFPSTime).count();
		frameCount++;
//        double deltaFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime).count();
//        std::this_thread::sleep_for(std::chrono::microseconds (static_cast<long long>((targetFrameTime - deltaFrameTime) * 0.7)));
		if (deltaFPSMeasureTime >= 1e6) {
			// Calculate FPS
			double fps = frameCount / deltaFPSMeasureTime * 1e6;

			// Print FPS
			std::cout << "FPS: " << fps << std::endl;

			// Reset counters
			frameCount = 0;
			lastFPSTime = currentTime;
		}

		// Unbind VAO
		glBindVertexArray(0);

		// Poll events
        glfwPollEvents();
		glfwSetKeyCallback(window, Input::keyCallback);
		glfwSetCursorPosCallback(window, Input::mouseCallback);
		glfwSetScrollCallback(window, Input::scrollCallback);
		camera.setMoveSpeed(Input::moveSpeed);
        Input::doMovement(camera);

        // Swap buffers and poll events
        glfwSwapBuffers(window);

		//slow framerate
//		glfwSwapInterval(100);
	}

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



