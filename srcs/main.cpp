#define GLEW_STATIC
#include <iostream>
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.hpp"
#include "Matrix.hpp"

const char* vertexShaderSource = R"(
	#version 330 core

	uniform mat4 view;  // Uniform view matrix
	uniform mat4 model;  // Uniform model matrix
	uniform mat4 projection;  // Uniform projection matrix

	layout (location = 0) in vec3 position;  // Input: Vertex position
	layout (location = 1) in vec3 color;     // Input: Vertex color

	out vec3 FragColor;  // Output: Fragment color

	void main() {
	    // Transform vertex position to camera space
		gl_Position = view * vec4(position, 1.0);

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

#include <cmath>

void invertMatrix(const float src[16], float dest[16]) {
	float tmp[12];
	float srcCopy[16];

	// Copy the source matrix
	for (int i = 0; i < 16; i++) {
		srcCopy[i] = src[i];
	}

	// Calculate pairs for first 8 elements (cofactors)
	tmp[0] = srcCopy[10] * srcCopy[15];
	tmp[1] = srcCopy[11] * srcCopy[14];
	tmp[2] = srcCopy[9] * srcCopy[15];
	tmp[3] = srcCopy[11] * srcCopy[13];
	tmp[4] = srcCopy[9] * srcCopy[14];
	tmp[5] = srcCopy[10] * srcCopy[13];
	tmp[6] = srcCopy[8] * srcCopy[15];
	tmp[7] = srcCopy[11] * srcCopy[12];
	tmp[8] = srcCopy[8] * srcCopy[14];
	tmp[9] = srcCopy[10] * srcCopy[12];
	tmp[10] = srcCopy[8] * srcCopy[13];
	tmp[11] = srcCopy[9] * srcCopy[12];

	// Calculate first 8 elements (cofactors)
	dest[0] = tmp[0] * srcCopy[5] + tmp[3] * srcCopy[6] + tmp[4] * srcCopy[7];
	dest[0] -= tmp[1] * srcCopy[5] + tmp[2] * srcCopy[6] + tmp[5] * srcCopy[7];
	dest[1] = tmp[1] * srcCopy[4] + tmp[6] * srcCopy[6] + tmp[9] * srcCopy[7];
	dest[1] -= tmp[0] * srcCopy[4] + tmp[7] * srcCopy[6] + tmp[8] * srcCopy[7];
	dest[2] = tmp[2] * srcCopy[4] + tmp[7] * srcCopy[5] + tmp[10] * srcCopy[7];
	dest[2] -= tmp[3] * srcCopy[4] + tmp[6] * srcCopy[5] + tmp[11] * srcCopy[7];
	dest[3] = tmp[5] * srcCopy[4] + tmp[8] * srcCopy[5] + tmp[11] * srcCopy[6];
	dest[3] -= tmp[4] * srcCopy[4] + tmp[9] * srcCopy[5] + tmp[10] * srcCopy[6];
	dest[4] = tmp[1] * srcCopy[1] + tmp[2] * srcCopy[2] + tmp[5] * srcCopy[3];
	dest[4] -= tmp[0] * srcCopy[1] + tmp[3] * srcCopy[2] + tmp[4] * srcCopy[3];
	dest[5] = tmp[0] * srcCopy[0] + tmp[7] * srcCopy[2] + tmp[8] * srcCopy[3];
	dest[5] -= tmp[1] * srcCopy[0] + tmp[6] * srcCopy[2] + tmp[9] * srcCopy[3];
	dest[6] = tmp[3] * srcCopy[0] + tmp[6] * srcCopy[1] + tmp[11] * srcCopy[3];
	dest[6] -= tmp[2] * srcCopy[0] + tmp[7] * srcCopy[1] + tmp[10] * srcCopy[3];
	dest[7] = tmp[4] * srcCopy[0] + tmp[9] * srcCopy[1] + tmp[10] * srcCopy[2];
	dest[7] -= tmp[5] * srcCopy[0] + tmp[8] * srcCopy[1] + tmp[11] * srcCopy[2];

	// Calculate pairs for second 8 elements (cofactors)
	tmp[0] = srcCopy[2] * srcCopy[7];
	tmp[1] = srcCopy[3] * srcCopy[6];
	tmp[2] = srcCopy[1] * srcCopy[7];
	tmp[3] = srcCopy[3] * srcCopy[5];
	tmp[4] = srcCopy[1] * srcCopy[6];
	tmp[5] = srcCopy[2] * srcCopy[5];
	tmp[6] = srcCopy[0] * srcCopy[7];
	tmp[7] = srcCopy[3] * srcCopy[4];
	tmp[8] = srcCopy[0] * srcCopy[6];
	tmp[9] = srcCopy[2] * srcCopy[4];
	tmp[10] = srcCopy[0] * srcCopy[5];
	tmp[11] = srcCopy[1] * srcCopy[4];

	// Calculate second 8 elements (cofactors)
	dest[8] = tmp[0] * srcCopy[13] + tmp[3] * srcCopy[14] + tmp[4] * srcCopy[15];
	dest[8] -= tmp[1] * srcCopy[13] + tmp[2] * srcCopy[14] + tmp[5] * srcCopy[15];
	dest[9] = tmp[1] * srcCopy[12] + tmp[6] * srcCopy[14] + tmp[9] * srcCopy[15];
	dest[9] -= tmp[0] * srcCopy[12] + tmp[7] * srcCopy[14] + tmp[8] * srcCopy[15];
	dest[10] = tmp[2] * srcCopy[12] + tmp[7] * srcCopy[13] + tmp[10] * srcCopy[15];
	dest[10] -= tmp[3] * srcCopy[12] + tmp[6] * srcCopy[13] + tmp[11] * srcCopy[15];
	dest[11] = tmp[5] * srcCopy[12] + tmp[8] * srcCopy[13] + tmp[11] * srcCopy[14];
	dest[11] -= tmp[4] * srcCopy[12] + tmp[9] * srcCopy[13] + tmp[10] * srcCopy[14];
	dest[12] = tmp[2] * srcCopy[10] + tmp[5] * srcCopy[11] + tmp[1] * srcCopy[9];
	dest[12] -= tmp[4] * srcCopy[11] + tmp[0] * srcCopy[9] + tmp[3] * srcCopy[10];
	dest[13] = tmp[8] * srcCopy[11] + tmp[0] * srcCopy[8] + tmp[7] * srcCopy[10];
	dest[13] -= tmp[6] * srcCopy[10] + tmp[9] * srcCopy[11] + tmp[1] * srcCopy[8];
	dest[14] = tmp[6] * srcCopy[9] + tmp[11] * srcCopy[11] + tmp[3] * srcCopy[8];
	dest[14] -= tmp[10] * srcCopy[11] + tmp[2] * srcCopy[8] + tmp[7] * srcCopy[9];
	dest[15] = tmp[10] * srcCopy[10] + tmp[4] * srcCopy[8] + tmp[9] * srcCopy[9];
	dest[15] -= tmp[8] * srcCopy[9] + tmp[11] * srcCopy[10] + tmp[5] * srcCopy[8];

	// Calculate determinant
	float det = srcCopy[0] * dest[0] + srcCopy[1] * dest[1] + srcCopy[2] * dest[2] + srcCopy[3] * dest[3];

	// Multiply by inverse of determinant
	det = 1.0f / det;

	// Multiply each element by the inverse of the determinant
	for (int i = 0; i < 16; i++) {
		dest[i] *= det;
	}
}

void updateViewMatrix(const Camera& camera, float viewMatrix[16]) {
	float matrix[16];
	camera.getRotationMatrix(matrix);
	invertMatrix(matrix, viewMatrix);
	viewMatrix[12] = -camera.position[0];
	viewMatrix[13] = -camera.position[1];
	viewMatrix[14] = -camera.position[2];
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

	// Vertex data for a simple triangle
	float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

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

//	Model model;
	Projection projection;

	while (!glfwWindowShouldClose(window)) {
		//Rotation Animation
//		if (count++ < 100)
//			camera.rotate(0.02f, 0.0f, 1.0f, 0.0f);
//		else
//			camera.rotate(0.02f, 1.0f, 0.0f, 0.0f);
		camera.rotate(0.02f, 0.0f, 1.0f, 0.0f);

		// Render
		glClear(GL_COLOR_BUFFER_BIT);

		// Update view matrix
		float viewMatrix[16];
		updateViewMatrix(camera, viewMatrix);

//		float modelMatrix[16];
//		model.getModelMatrix(modelMatrix);

		float projectionMatrix[16];
		projection.getPerspectiveProjectionMatrix(800, 600, projectionMatrix);

		// Pass the view matrix to the vertex shader
		GLint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix);

		// Pass the model matrix to the vertex shader
//		GLint modelMatrixLoc = glGetUniformLocation(shaderProgram, "model");
//		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix);

		// Pass the projection matrix to the vertex shader
		GLint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix);

		// Use the shader program and VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Unbind VAO
		glBindVertexArray(0);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();

		//slow framerate
//		glfwSwapInterval(5);
	}

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



