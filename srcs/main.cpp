//
// Created by pirexa on 18-01-2024.
//

#define STB_IMAGE_IMPLEMENTATION
#include "BIM.hpp"
#include <GL/glx.h>
#include <GL/glu.h>

void	printColoredText(const char *text, int r, int g, int b) {
	std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << text << "\033[0m";
}

void	printLog(int logLevel, const char *message) {
	if (logLevel == 0) {
		printColoredText("######### Fatal Error #########\n", 255, 30, 30);
		printColoredText(message, 255, 30, 30);
		std::cout << std::endl;
	}
	else if (logLevel == 1) {
		printColoredText("######### Warning #########\n", 255, 100, 100);
		printColoredText(message, 255, 100, 100);
		std::cout << std::endl;
	}
	else if (logLevel == 2) {
		printColoredText("     -> ", 255, 255, 255);
		printColoredText(message, 255, 255, 255);
		std::cout << std::endl;
	}
	else if (logLevel == 3) {
		printColoredText("    -> ", 0, 255, 255);
		printColoredText(message, 0, 255, 255);
		std::cout << std::endl;
	}
	else if (logLevel == 4) {
		printColoredText("   -> ", 0, 140, 255);
		printColoredText(message, 0, 140, 255);
		std::cout << std::endl;
	}
	else if (logLevel == 5) {
		printColoredText(" -< ", 0, 80, 255);
		printColoredText(message, 0, 80, 255);
		printColoredText(" >-\n", 0, 80, 255);
	}
	else if (logLevel == 6) {
		printColoredText(" <#> ", 0, 200, 100);
		printColoredText(message, 0, 200, 100);
		printColoredText(" <#>\n", 0, 200, 100);
	}
}

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

GLuint   ShaderSetups(const char *shaderLabel, const char *vertexShaderPath, const char *fragmentShaderPath) {
	// Compile Vertex Shader
	std::stringstream ss;
	ss << "Compiling " << shaderLabel << " Shader Program";
	printLog(5, ss.str().c_str());

	ss.str("");
	ss << "Compiling " << shaderLabel << " Vertex Shader";
	printLog(4, ss.str().c_str());

	const char * vertexShaderSource = loadShaderFromFile(vertexShaderPath);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
    GLint compileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        // Compilation failed, retrieve error log
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(vertexShader, logLength, NULL, errorLog.data());

        // Print error log
	    std::stringstream ss;
		ss << shaderLabel << " Vertex Shader compilation failed\n" << errorLog.data();
		printLog(0, ss.str().c_str());

		// Clean up and return
		glDeleteShader(vertexShader);
		return 0;
    }
    else
    {
		std::stringstream ss;
		ss << shaderLabel << " Vertex Shader compiled successfully";
		printLog(3, ss.str().c_str());
    }


    // Compile Fragment Shader
	ss.str("");
	ss << "Compiling " << shaderLabel << " Fragment Shader";
	printLog(4, ss.str().c_str());
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fragmentShaderSource = loadShaderFromFile(fragmentShaderPath);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
    GLint compileStatus2;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus2);
    if (compileStatus2 != GL_TRUE) {
        // Compilation failed, retrieve error log
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(fragmentShader, logLength, NULL, errorLog.data());

        // Print error log
	    std::stringstream ss;
		ss << shaderLabel << " Fragment Shader compilation failed\n" << errorLog.data();
		printLog(0, ss.str().c_str());

		// Clean up and return
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
    }
    else
    {
	    std::stringstream ss;
	    ss << shaderLabel << " Fragment Shader compiled successfully";
		printLog(3, ss.str().c_str());
    }

	// Link Shaders into a Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Clean up individual shaders (they are no longer needed after linking)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
    delete vertexShaderSource;
	delete fragmentShaderSource;
	return shaderProgram;
}

GLFWwindow *InitialSetup() {
	printLog(6, "Initializing OpenGL Setup");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "BIM", nullptr, nullptr);
//    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "BIM", glfwGetPrimaryMonitor(), nullptr);
	if (window == nullptr) {
		printLog(0, "Failed to create GLFW window");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		printLog(0, "Failed to initialize GLEW");
		glfwTerminate();
		return nullptr;
	}

	//Enable rendering of only front-facing triangles
	glEnable(GL_DEPTH_TEST);

	//Vsync 1-on, 0-off
	glfwSwapInterval(1);

	//Enable culing of back-facing triangles
//	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.32f, 0.5f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hide the cursor and lock it to the window
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	printLog(5, "GLFW initialized successfully");
	printLog(5, "GLEW initialized successfully");
	printLog(6, "OpenGL initialized successfully");
	return window;
}

int main(int argc, char** argv) {
	GLFWwindow *window;
	if ((window = InitialSetup()) == nullptr)
		return -1;

	printLog(6, "Compiling Shaders");
	GLuint modelShaderProgram = ShaderSetups("Model", "./shaders/modelVertexShader.glsl", "./shaders/modelFragmentShader.glsl");
	if (modelShaderProgram == 0) {
		glfwTerminate();
		return -1;
	}

	GLuint skyboxShaderProgram = ShaderSetups("Skybox", "./shaders/skyboxVertexShader.glsl", "./shaders/skyboxFragmentShader.glsl");
	if (skyboxShaderProgram == 0) {
		glfwTerminate();
		return -1;
	}

	GLuint GUIshaderProgram = ShaderSetups("GUI", "./shaders/GUIVertexShader.glsl", "./shaders/GUIFragmentShader.glsl");
	if (GUIshaderProgram == 0) {
		glfwTerminate();
		return -1;
	}

	GLuint DefaultPlaneShaderProgram = ShaderSetups("DefaultPlane", "./shaders/DefaultPlaneVertexShader.glsl", "./shaders/DefaultPlaneFragmentShader.glsl");
	if (DefaultPlaneShaderProgram == 0) {
		glfwTerminate();
		return -1;
	}

	GLuint LightSimulationShaderProgram = ShaderSetups("LightSimulation", "./shaders/LightSimulationVertexShader.glsl", "./shaders/LightSimulationFragmentShader.glsl");
	if (LightSimulationShaderProgram == 0) {
		glfwTerminate();
		return -1;
	}
	printLog(6, "Shaders compiled successfully");

	printColoredText("Monitor refresh rate: ", 155, 80, 255);
	printColoredText(std::to_string(glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate).c_str(), 0, 185, 185);
	printColoredText(" Hz\n", 0, 185, 185);

	Camera camera;

    const char * windows_filename = "..\\Models\\Porsche_911_GT2.obj";
    const char * debian_filename = "./Resources/Models/cube.obj";
    if (argc == 2)
    {
        windows_filename = argv[1];
        debian_filename = argv[1];
    }
    Model model("./Resources/Textures/zebra.bmp", debian_filename, 0.3f);

	DefaultPlane defaultPlane("./Resources/Textures/vaporwavegrid.bmp");

	// Set up the projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(75.0f), // FOV
												  WIN_WIDTH / WIN_HEIGHT,          // Aspect ratio
												  0.1f, 4000.0f);        // Near and far planes

	TextFont font("./Fonts/Font3White.png");

    GUI gui("./Resources/Textures/Backwall.bmp");

    ProgressBar progressBar(glm::vec2(WIN_WIDTH / 2 - 100, 60.0f), glm::vec2(200.0f, 20.0f), 100.0f, 100.0f, "./Resources/Textures/white.bmp");

	Skybox skybox;

	auto lastFPSTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
	double fps = 0;

    bool animation_end;
    float blendFactor = 1.0f;
	printColoredText("Starting BIM...\n", 0, 140, 255);
	while (!glfwWindowShouldClose(window)) {

        animation_end = Animation::InitialAnimation(&camera, model.getCenter(), model.getScale(), &model);
        Animation::TransitionAnimation(blendFactor);
        Animation::buildAnimation(&model);

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

		GLuint blendFactorLoc;
		if (Input::LightSimulation)
		{
			// Use the Light Simulation Shader Program
			glUseProgram(LightSimulationShaderProgram);
			// Set the uniform in the shader to transitionBlendFactor to switch between the texture and normal map
			blendFactorLoc = glGetUniformLocation(LightSimulationShaderProgram, "transitionBlendFactor");
			// Set the uniform in the shader to the MVP matrix
			glUniformMatrix4fv(glGetUniformLocation(LightSimulationShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
			// Set the uniform in the shader to the model matrix
			glUniformMatrix4fv(glGetUniformLocation(LightSimulationShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			// Set the uniform in the shader to the normal matrix
			glUniformMatrix4fv(glGetUniformLocation(LightSimulationShaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose((glm::inverse(modelMatrix)))));
		}
		else
		{
			// Use the Default Model Shader Program
			glUseProgram(modelShaderProgram);
			// Set the uniform in the shader to transitionBlendFactor to switch between the texture and normal map
			blendFactorLoc = glGetUniformLocation(modelShaderProgram, "transitionBlendFactor");
			// Set the uniform in the shader to the MVP matrix
			glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
			// Set the uniform in the shader to the model matrix
			glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			// Set the uniform in the shader to the normal matrix
			glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose((glm::inverse(modelMatrix)))));
		}

        if (Input::WireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (Input::TextureMode)
			glUniform1f(blendFactorLoc, blendFactor);
		else
			glUniform1f(blendFactorLoc, 1.0f - blendFactor);

		model.draw();

        glUniform1f(blendFactorLoc, 1.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// FPS calculation
        fpsCounter(lastFPSTime, frameCount, fps);

		// Draw the XZ plane
		glUseProgram(DefaultPlaneShaderProgram);

		// Set the uniform in the shader to the MVP matrix for the XZ plane
        glUniformMatrix4fv(glGetUniformLocation(DefaultPlaneShaderProgram, "planeMatrix"), 1, GL_FALSE, glm::value_ptr(planeMatrix));

        defaultPlane.draw();

		// Draw the skybox
		glUseProgram(skyboxShaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "pv"), 1, GL_FALSE, glm::value_ptr(planeMatrix));
		skybox.draw();

		glUseProgram(GUIshaderProgram);

		glDisable(GL_DEPTH_TEST);

		// Set the uniform in the shader to the text matrix
		glUniformMatrix4fv(glGetUniformLocation(GUIshaderProgram, "textMatrix"), 1, GL_FALSE, glm::value_ptr(textMatrix));

		// Draw text
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << fps;
		std::string fpsString = ss.str();
		font.renderText("FPS: " + fpsString, WIN_WIDTH / 25, WIN_HEIGHT - WIN_HEIGHT / 20, 1.00f);

        //GUI Transparency based on Mouse Mode (0 = Control Cursor, 1 = Control Camera) CTRL to toggle
        if (Input::mouseMode == 0)
            glUniform1i(glGetUniformLocation(GUIshaderProgram, "GUITransparent"), 0); //Non-Transparent
        else
            glUniform1i(glGetUniformLocation(GUIshaderProgram, "GUITransparent"), 1); //Transparent

        // Draw GUI
        gui.draw();

        glUniform1i(glGetUniformLocation(GUIshaderProgram, "GUITransparent"), 0);

        // Draw Progress Bar
        if (blendFactor == 1.0f)
            progressBar.setProgress(0.0f);
        else
            progressBar.setProgress(blendFactor * 100.0f);
        progressBar.draw();

        glEnable(GL_DEPTH_TEST);

        // Poll events
        glfwPollEvents();
        if (!animation_end)
        {
            glfwSetKeyCallback(window, Input::keyCallback);
			glfwSetMouseButtonCallback(window, Input::Input::mouseButtonCallback);
            glfwSetCursorPosCallback(window, Input::mouseCallback);
            glfwSetScrollCallback(window, Input::scrollCallback);
            glfwSetDropCallback(window, Input::dropCallback);
            camera.setMoveSpeed(Input::moveSpeed);
            updateStates(window, camera, model, gui);
            if (mouseIntersectModel(window, model, mvpMatrix))
                updateModel(window, model);
            else if (mouseIntersectPlane(window, defaultPlane, mvpMatrix))
				updateDefaultPlane(window, defaultPlane);
            else
                Input::dropPosition = glm::vec2(-500.0f, -500.0f);
        }

        // Swap buffers
        glfwSwapBuffers(window);

	}

	// Clean up
	glDeleteProgram(modelShaderProgram);
	glDeleteProgram(skyboxShaderProgram);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}



