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
	printColoredText("Compiling Vertex Shader\n", 0, 140, 255);
    const char * vertexShaderSource = loadShaderFromFile("./srcs/vertexShader.glsl");
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
//        std::cerr << "Vertex Shader compilation failed:\n" << errorLog.data() << std::endl;
		printColoredText("Vertex Shader compilation failed\n", 255, 30, 30);
		printColoredText(errorLog.data(), 255, 30, 30);

		// Clean up and return
		glDeleteShader(vertexShader);
		return 0;
    }
    else
    {
		printColoredText(" -Vertex Shader compiled successfully\n", 0, 255, 255);
    }


    // Compile Fragment Shader
	printColoredText("Compiling Fragment Shader\n", 0, 140, 255);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char * fragmentShaderSource = loadShaderFromFile("./srcs/fragmentShader.glsl");
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
//        std::cerr << "Fragment Shader compilation failed:\n" << errorLog.data() << std::endl;
		printColoredText("Fragment Shader compilation failed\n", 255, 30, 30);
		printColoredText(errorLog.data(), 255, 30, 30);\

		// Clean up and return
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
    }
    else
    {
		printColoredText(" -Fragment Shader compiled successfully\n", 0, 255, 255);
    }


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
	printColoredText("Initializing GLFW\n", 0, 140, 255);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "BIM", nullptr, nullptr);
//    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "BIM", glfwGetPrimaryMonitor(), nullptr);
	if (window == nullptr) {
		printColoredText("Failed to create GLFW window\n", 255, 30, 30);
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		printColoredText("Failed to initialize GLEW\n", 255, 30, 30);
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

	printColoredText(" -GLFW initialized successfully\n", 0, 255, 255);
	printColoredText(" -GLEW initialized successfully\n", 0, 255, 255);
	return window;
}

int main(int argc, char** argv) {
	GLFWwindow *window;
	if ((window = InitalSetup()) == nullptr)
		return -1;

	GLuint shaderProgram = ShaderSetups();
	if (shaderProgram == 0) {
		glfwTerminate();
		return -1;
	}

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
    model.vertexBufferSetup();

	DefaultPlane defaultPlane("./Resources/Textures/vaporwavegrid.bmp");

	// Set up the projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), // FOV
												  WIN_WIDTH / WIN_HEIGHT,          // Aspect ratio
												  0.1f, 1000.0f);        // Near and far planes

	TextFont font("./Fonts/Font3.png");

    GUI gui("./Resources/Textures/Backwall.bmp");

	auto lastFPSTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
	double fps = 0;

    RenderBatch modelBatch(model.getObj().getVerticesArray(), model.getObj().getVertexCount(), *model.getTexture().getTextureID());

    bool animation_end;
	printColoredText("Starting BIM...\n", 0, 140, 255);
	while (!glfwWindowShouldClose(window)) {

        animation_end = Animation::InitialAnimation(&camera, model.getCenter(), model.getScale(), &model);

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render with texture or normal
        int renderTexture = Input::TextureMode;

        // Pass the render mode to the shader
        GLint renderTextureLoc = glGetUniformLocation(shaderProgram, "renderTexture");

        //Set the uniform in the shader to the render mode
        glUniform1i(renderTextureLoc, renderTexture);

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

        if (Input::WireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        modelBatch.draw();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        renderTexture = 1;

        // Pass the render mode to the shader
        glUniform1i(renderTextureLoc, renderTexture);

		// FPS calculation
        fpsCounter(lastFPSTime, frameCount, fps);

		// Draw the XZ plane
		// Set the uniform in the shader to the MVP matrix for the XZ plane
        glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(planeMatrix));

        defaultPlane.draw();

        glDisable(GL_DEPTH_TEST);

        // Set the uniform in the shader to the MVP matrix for the text
		glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(textMatrix));
		// Draw text
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << fps;
		std::string fpsString = ss.str();
		font.renderText("FPS: " + fpsString, WIN_WIDTH / 25, WIN_HEIGHT - WIN_HEIGHT / 20, 1.00f);

        //GUI Transparency based on Mouse Mode (0 = Control Cursor, 1 = Control Camera) CTRL to toggle
        if (Input::mouseMode == 0)
            glUniform1i(glGetUniformLocation(shaderProgram, "GUITransparent"), 0); //Non-Transparent
        else
            glUniform1i(glGetUniformLocation(shaderProgram, "GUITransparent"), 1); //Transparent

        // Draw GUI
        gui.draw();

        glUniform1i(glGetUniformLocation(shaderProgram, "GUITransparent"), 0);

        glEnable(GL_DEPTH_TEST);

        if (!animation_end)
        {
            // Poll events
            glfwPollEvents();
            glfwSetKeyCallback(window, Input::keyCallback);
			glfwSetMouseButtonCallback(window, Input::Input::mouseButtonCallback);
            glfwSetCursorPosCallback(window, Input::mouseCallback);
            glfwSetScrollCallback(window, Input::scrollCallback);
            glfwSetDropCallback(window, Input::dropCallback);
            camera.setMoveSpeed(Input::moveSpeed);
            updateStates(window, camera, model, gui);
            if (mouseIntersectModel(window, model, mvpMatrix))
                updateModel(window, model, modelBatch);
            else if (mouseIntersectPlane(window, defaultPlane, mvpMatrix))
				updateDefaultPlane(window, defaultPlane);
            else
                Input::dropPosition = glm::vec2(-500.0f, -500.0f);
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



