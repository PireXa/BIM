#define GLEW_STATIC
#include <iostream>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 position;
    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Orange color
    }
)";

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

	// Vertex data for a simple triangle
	float vertices[] = {
			-0.5f, -0.5f,
			0.0f,  0.5f,
			0.5f, -0.5f
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

	// Specify vertex attribute pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		// Render
		glClear(GL_COLOR_BUFFER_BIT);

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
	}

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



