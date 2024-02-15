#define GLEW_STATIC
#include <iostream>
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
#include <chrono>
#include <thread>
#include <algorithm>
#include "Texture.hpp"
#include "Animation.hpp"

const char * loadShaderFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string str = buffer.str();
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}

GLuint   ShaderSetups() {
	// Compile Vertex Shader
    const char * vertexShaderSource = loadShaderFromFile("./srcs/vertexShader.glsl");
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Compile Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char * fragmentShaderSource = loadShaderFromFile("./srcs/fragmentShader.glsl");
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
    free ((void *)vertexShaderSource);
    free ((void *)fragmentShaderSource);
	return shaderProgram;
}

int main(int argc, char** argv) {

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
    Model model;

    const char * windows_filename = "..\\Models\\Porsche_911_GT2.obj";
    const char * debian_filename = "./Models/cube.obj";
    if (argc == 2)
    {
        windows_filename = argv[1];
        debian_filename = argv[1];
    }

	readOBJ obj(debian_filename);
    model.setCenter(obj.getCenter());
    model.setBoundingBox(obj.getBoundingBox());
//    std::cout << "Bounding box: " << model.getBoundingBox().min.x << " " << model.getBoundingBox().min.y << " " << model.getBoundingBox().min.z << std::endl;
//    std::cout << "Bounding box: " << model.getBoundingBox().max.x << " " << model.getBoundingBox().max.y << " " << model.getBoundingBox().max.z << std::endl;
//    std::cout << "Scale: " << model.getScale() << std::endl;
    if (obj.getuvCount() == 0)
    {
//        std::cout << "No uv coordinates found, planar mapping will be used" << std::endl;
        obj.PlanarMapping();
    }
//	std::cout << "Face count: " << obj.getFaceCount() << std::endl;

	Texture texture("./Models/zebra.bmp");
    glGenTextures(1, texture.getTextureID());
	glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.getWidth(), texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getPixels().data());
    glBindTexture(GL_TEXTURE_2D, 0);

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

    float planeVertices[] = {
            // Position            // Texture coordinates
            -100.0f, -1.0f, -100.0f,    0.0f, 0.0f, 0.0f,  // Bottom-left
             100.0f, -1.0f, -100.0f,     1.0f, 0.0f, 0.0f, // Bottom-right
            -100.0f, -1.0f, 100.0f,     0.0f, 1.0f, 0.0f,  // Top-left
            100.0f, -1.0f, -100.0f,     1.0f, 0.0f, 0.0f,  // Bottom-right
            100.0f, -1.0f, 100.0f,      1.0f, 1.0f, 0.0f,  // Top-right
            -100.0f, -1.0f, 100.0f,     0.0f, 1.0f, 0.0f,  // Top-left
    };

    int vertexCount = 6;

    // Generate VBO and VAO for the XZ plane
    GLuint planeVBO, planeVAO;
    glGenBuffers(1, &planeVBO);
    glGenVertexArrays(1, &planeVAO);

    // Bind VAO for the XZ plane
    glBindVertexArray(planeVAO);

    // Bind VBO for the XZ plane and copy vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexCount, planeVertices, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * obj.getVertexCount() , obj.getVerticesArray(), GL_STATIC_DRAW);

    // Specify vertex attribute pointers and enable them for the XZ plane
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO and VBO for the XZ plane
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load texture for the XZ plane
    Texture planeTexture("./Models/grid2.bmp"); // Replace with the actual path to your texture file

    // Generate texture ID for the XZ plane
    glGenTextures(1, planeTexture.getTextureID());


    // Bind the texture and set its parameters
    glBindTexture(GL_TEXTURE_2D, *planeTexture.getTextureID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data into the texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, planeTexture.getWidth(), planeTexture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, planeTexture.getPixels().data());

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

//    const double targetFrameTime = 1.0f * 1e6 / 60.0f;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto lastFPSTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
//
//	glfwGetCursorPos(window, &Input::lastX, &Input::lastY);
//	std::cout << Input::lastX << " " << Input::lastY << std::endl;

    bool animation_end;
    std::cout << "Monitor refresh rate: " << glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate << " Hz" << std::endl;
	while (!glfwWindowShouldClose(window)) {

//        lastFrameTime = std::chrono::high_resolution_clock::now();
        animation_end = Animation::InitialAnimation(&camera, model.getCenter(), model.getScale(), &model);

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update view matrix
        glm::mat4 viewMatrix;
        viewMatrix = camera.getViewMatrix();

        // Set up the projection matrix (you might do this once in your initialization)
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), // FOV
                                                      800.0f / 600.0f,          // Aspect ratio
                                                      0.1f, 1000.0f);        // Near and far planes

        glm::mat4 modelMatrix = model.getModelMatrix();

        // Combine the view matrix and projection matrix to get the final MVP matrix
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

        // Pass the MVP matrix to the shader
        GLint mvpMatrixLoc = glGetUniformLocation(shaderProgram, "vp");
        glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		// Use the shader program and VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

        glBindTexture(GL_TEXTURE_2D, *texture.getTextureID());
		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, obj.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);

        // Unbind VAO
        glBindVertexArray(0);

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

        glBindTexture(GL_TEXTURE_2D, *planeTexture.getTextureID());
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount); // Replace numberOfVertices with the actual number of vertices for the XZ plane
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (!animation_end)
        {
            // Poll events
            glfwPollEvents();
            glfwSetKeyCallback(window, Input::keyCallback);
            glfwSetCursorPosCallback(window, Input::mouseCallback);
            glfwSetScrollCallback(window, Input::scrollCallback);
            camera.setMoveSpeed(Input::moveSpeed);
            Input::doMovement(camera, model);
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);

		//slow framerate
//		glfwSwapInterval(100);
	}

	// Clean up
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
	glDeleteProgram(shaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



