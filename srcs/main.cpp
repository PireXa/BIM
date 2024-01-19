#define GLEW_STATIC
#include <iostream>
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.hpp"
#include "Matrix.hpp"
#include <fstream>
#include <sstream>
#include <vector>

const char* vertexShaderSource = R"(
	#version 330 core

	uniform mat4 view;  // Uniform view matrix
	uniform mat4 model;  // Uniform model matrix
	uniform mat4 projection;  // Uniform projection matrix
    uniform mat4 vp;  // Uniform view-projection matrix

	layout (location = 0) in vec3 position;  // Input: Vertex position
	layout (location = 1) in vec3 color;     // Input: Vertex color

	out vec3 FragColor;  // Output: Fragment color

	void main() {
	    // Transform vertex position to camera space
		gl_Position = vp * vec4(position, 1.0);

	    // Pass color to the fragment shader
	    FragColor = color;
	}
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    in vec3 FragColor;  // Input: Vertex color

    out vec4 FinalColor;  // Output: Final fragment color

    void main() {
        // Output the color directly
        FinalColor = vec4(FragColor, 1.0);
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

struct Vertex {
    float x, y, z;
    float u, v; // Texture coordinates
    float nx, ny, nz; // Normals
};

std::vector<Vertex> readObjFile(const std::string& filename) {
    std::vector<Vertex> vertices;

    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vertex vertex{};
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
    }
    return vertices;
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

	GLuint shaderProgram = ShaderSetups();

	Camera camera;

    std::vector <Vertex>verticesFromFile = readObjFile("./Models/Lowpoly_tree.obj");
    float vertices[verticesFromFile.size() * 6];
    int vertexCount = verticesFromFile.size();
    int i = 0;
    for (Vertex vertex : verticesFromFile) {
        std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
        vertices[i++] = vertex.x;
        vertices[i++] = vertex.y;
        vertices[i++] = vertex.z;
        vertices[i++] = 1.0f;
        vertices[i++] = 1.0f;
        vertices[i++] = 1.0f;
    }

	// Vertex data for a simple triangle
//	float vertices[] = {
//			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//			0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
//
//            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
//            0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
//            0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
//	};

//    glDisable(GL_CULL_FACE);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify vertex attribute pointers and enable them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {

		camera.rotate(0.0005f, 0.0f, 1.0f, 0.0f);

		// Render
		glClear(GL_COLOR_BUFFER_BIT);

		// Update view matrix
        glm::mat4 viewMatrix;
        viewMatrix = camera.getViewMatrix();

        // Set up the projection matrix (you might do this once in your initialization)
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), // FOV
                                                      800.0f / 600.0f,          // Aspect ratio
                                                      0.1f, 100.0f);        // Near and far planes

        // Combine the view matrix and projection matrix to get the final MVP matrix
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix;

        // Pass the MVP matrix to the shader
        GLint mvpMatrixLoc = glGetUniformLocation(shaderProgram, "vp");
        glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		// Use the shader program and VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, vertexCount * 6);

		// Unbind VAO
		glBindVertexArray(0);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();

		//slow framerate
		glfwSwapInterval(10000);
	}

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



