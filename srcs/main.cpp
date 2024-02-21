//
// Created by pirexa on 18-01-2024.
//

#define STB_IMAGE_IMPLEMENTATION
#include "BIM.hpp"

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

GLFWwindow *InitalSetup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "BIM", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return nullptr;
	}

	//Enable rendering of only front-facing triangles
	glEnable(GL_DEPTH_TEST);

	//Vsync 1-on, 0-off
	glfwSwapInterval(1);

	//Enable culing of back-facing triangles
//	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.68f, 1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	return window;
}

int main(int argc, char** argv) {
	GLFWwindow *window;
	if ((window = InitalSetup()) == nullptr)
		return -1;

	GLuint shaderProgram = ShaderSetups();

	Camera camera;

    const char * windows_filename = "..\\Models\\Porsche_911_GT2.obj";
    const char * debian_filename = "./Models/cube.obj";
    if (argc == 2)
    {
        windows_filename = argv[1];
        debian_filename = argv[1];
    }
    Model model("./Models/zebra.bmp", debian_filename, 0.3f);
    model.vertexBufferSetup(shaderProgram);

	DefaultPlane defaultPlane("./Models/grid2.bmp");

	// Set up the projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), // FOV
												  800.0f / 600.0f,          // Aspect ratio
												  0.1f, 1000.0f);        // Near and far planes
//	glm::mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), // FOV
//												  1920.0f / 1080.0f,          // Aspect ratio
//												  0.1f, 1000.0f);        // Near and far planes

	Font font("./Fonts/Font3.png");
	font.readFNT("./Fonts/Font3.fnt");

	auto lastFPSTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
	double fps = 0;

    bool animation_end;
    std::cout << "Monitor refresh rate: " << glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate << " Hz" << std::endl;
	while (!glfwWindowShouldClose(window)) {
        animation_end = Animation::InitialAnimation(&camera, model.getCenter(), model.getScale(), &model);

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update Camera View Matrix
        glm::mat4 viewMatrix;
        viewMatrix = camera.getViewMatrix();

        glm::mat4 modelMatrix = model.getModelMatrix();

        // Combine the view matrix and projection matrix to get the final MVP matrix
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        glm::mat4 planeMatrix = projectionMatrix * viewMatrix;
		glm::mat4 textMatrix = font.getMatrix();

        // Pass the MVP matrix to the shader
        GLint mvpMatrixLoc = glGetUniformLocation(shaderProgram, "vp");

        // Set the uniform in the shader to the MVP matrix for Model
        glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		// Use the shader program and VAO
		glUseProgram(shaderProgram);

        // Bind Model VAO
		glBindVertexArray(model.getVAO());

        // Bind texture for the Model
        glBindTexture(GL_TEXTURE_2D, *model.getTexture().getTextureID());

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, model.getObj().getVertexCount());

        // Unbind Texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Unbind VAO
        glBindVertexArray(0);

		// FPS calculation
        fpsCounter(lastFPSTime, frameCount, fps);

		// Draw the XZ plane
		// Set the uniform in the shader to the MVP matrix for the XZ plane
        glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(planeMatrix));

        // Bind the texture for the XZ plane
		glBindTexture(GL_TEXTURE_2D, *defaultPlane.getTexture().getTextureID());

		// Bind the VAO for the XZ plane
        glBindVertexArray(defaultPlane.getVAO());

//        glEnable(GL_CULL_FACE);

		// Draw the XZ plane
        glDrawArrays(GL_TRIANGLES, 0, defaultPlane.getVertexCount());

//        glDisable(GL_CULL_FACE);

        // Unbind the VAO for the XZ plane
		glBindVertexArray(0);

		// Unbind the texture for the XZ plane
        glBindTexture(GL_TEXTURE_2D, 0);

        // Set the uniform in the shader to the MVP matrix for the text
		glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(textMatrix));
		// Draw text
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << fps;
		std::string fpsString = ss.str();
		font.renderText("FPS: " + fpsString, 5.0f, 185.0f, 0.20f, shaderProgram);

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

	}

	// Clean up
	glDeleteProgram(shaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



