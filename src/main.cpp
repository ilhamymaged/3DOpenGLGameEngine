#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <iostream>

#include <vector>

#include <stb_image.h>

#include <glm/mat4x4.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <WindowManager.h>
#include <InputHandler.h>
#include <Camera.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "3D Game Engine"

const char* readConfigFile(const char* filename);

int main()
{
	WindowManager windowManager(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	Camera camera;
	InputHandler inputHandler(camera);

	glfwSetWindowUserPointer(windowManager.getWindow(), &inputHandler);
	glfwSetCursorPosCallback(windowManager.getWindow(), InputHandler::mouse_callback);
	glfwSetScrollCallback(windowManager.getWindow(), InputHandler::scroll_callback);;

	glEnable(GL_DEPTH_TEST);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(windowManager.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	//Load an image
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/Textures/highqualitybrick.jpg", &width, &height, &nrChannels, 0);
	if(data != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "DEBUG::IMAGE HAS BEEN LOADED" << std::endl;
	} else {
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//Make the Shader program
	const char* vertexShaderSource = readConfigFile("shaders/default.v");
	const char* fragmentShaderSource = readConfigFile("shaders/default.f");

	//Create Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	//Debugging Vertex Shader Compilation
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	//Debugging Fragment Shader Compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Debugging Shader Program Linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Deleting the shaders as they are linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Load The triangle Vertices to the GPU
	float triangle_vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool opened = true;
	const char* items[] = {"sphere", "monkey"};

	glm::vec3 pos = glm::vec3(0.0, 0.0, -3.0);

	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(windowManager.getWindow())) {
		float currentFrame = glfwGetTime();
		//deltaTime = currentFrame - lastFrame;
		inputHandler.setDeltaTime(currentFrame - lastFrame);
		lastFrame = currentFrame;

		//processInput(window);
		inputHandler.processInput(windowManager.getWindow());
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//MVP
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::translate(model, pos);
		view = camera.getViewMat();
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

		//Send MVP to GPU
		glUseProgram(shaderProgram);

		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Rendering ImGui Here
		ImGui::SetNextWindowSize(ImVec2(300, 400));
		if (ImGui::Begin("Test", &opened, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse)) {
			
			if (ImGui::Button("Hello, world!")) {
				std::cout << "Hello, world!" << std::endl;
			}

			float f = 5.0; 
			int current = 0;
			ImGui::Checkbox("Enable Shadows", &opened);
			ImGui::DragFloat3("Test Float", &pos[0], 0.05f);
			ImGui::Combo("Objects", &current, items, IM_ARRAYSIZE(items));

		}ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		windowManager.swapBuffers();
		windowManager.pullEvents();
	}

	//Shutdowning ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

const char* readConfigFile(const char* filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open config file: " << filename << std::endl;
		return nullptr;
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	char* buffer = new char[content.size() + 1];
	std::copy(content.begin(), content.end(), buffer);
	buffer[content.size()] = '\0';
	return buffer;
}

