//#ifdef _WIN32
//#include <windows.h>

//int main();

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	//return main();
//}
//#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include <vector>

#include <glm/mat4x4.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <WindowManager.h>
#include <InputHandler.h>
#include <Camera.h>
#include <Shader.h>
#include <Texture.h>
#include <Mesh.h>

#include <filesystem>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "3D Game Engine"

int main()
{
	WindowManager windowManager(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	Camera camera;
	InputHandler inputHandler(camera);

	glfwSetWindowUserPointer(windowManager.getWindow(), &inputHandler);
	glfwSetCursorPosCallback(windowManager.getWindow(), InputHandler::mouse_callback);
	glfwSetScrollCallback(windowManager.getWindow(), InputHandler::scroll_callback);

	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(windowManager.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	Shader shader("default");
	Shader lightShader("light");

	Texture texture0("highqualitybrick.jpg");
	Texture texture1("stone03d.jpg");

	Mesh cubeMesh = Mesh::generateBlock();
	Mesh lightCubeMesh = Mesh::generateLightBlock();

	//Draw a chunk of cubes 
	int chunkSize = 16; //16x16
	int chunkHeight = 12;
	glm::vec3 chunkPos(0.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> cubePositions;

	for (int x = 0; x < chunkSize; x++) {
		for (int y = -chunkHeight; y < 0; y++) {
			for (int z = -chunkSize; z < 0; z++) {
				glm::vec3 cubePos = glm::vec3(x, y, z) + chunkPos;
				cubePositions.push_back(cubePos);
			}
		}
	}

	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, cubePositions.size() * sizeof(glm::vec3), &cubePositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(cubeMesh.getVAO());
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1); 
	glBindVertexArray(0);

	bool opened = true;
	const char* items[] = {"chunk", "cube", "light"};
	int current = 0;

	glm::vec3 cubePos(2.0f, 0.0f, -2.0f);
	glm::vec3 lightPos(5.0f, 3.0f, -5.0f);

	float lastFrame = 0.0f;

	//Adding Shadows
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(windowManager.getWindow())) {
		float currentFrame = glfwGetTime();
		inputHandler.setDeltaTime(currentFrame - lastFrame);
		lastFrame = currentFrame;

		inputHandler.processInput(windowManager.getWindow());

		//Render scene to depth map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		
		float near_plane = 1.0f, far_plane = 20.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;





		//Render scene as normal
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//MVP
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::translate(model, chunkPos);
		view = camera.getViewMat();
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

		shader.use();
		shader.setMat4(model, "model");
		shader.setMat4(view, "view");
		shader.setMat4(projection, "projection");

		shader.setVec3(camera.getPos(), "viewPos");
		shader.setVec3(lightPos, "lightPos");

		texture0.bind();
		shader.setBool(1, "isInstanced");
		cubeMesh.renderInstanced(cubePositions.size());

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePos);

		shader.setMat4(model, "model");
		texture1.bind();
		shader.setBool(0, "isInstanced");
		cubeMesh.render();

		lightShader.use();
		model = glm::mat4(1.0);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));

		lightShader.setMat4(model, "model");
		lightShader.setMat4(view, "view");
		lightShader.setMat4(projection, "projection");

		lightCubeMesh.render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(300, 400));
		if (ImGui::Begin("Test", &opened, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
			
			if (ImGui::Button("Hello, world!")) {
				std::cout << "Hello, world!" << std::endl;
			}

			float f = 5.0; 
			ImGui::Combo("Objects", &current, items, IM_ARRAYSIZE(items));
			if (current == 0) {
				ImGui::DragFloat3("ChunkPos", &chunkPos[0], 0.05f);
			}
			else if(current == 1) {
				ImGui::DragFloat3("CubePos", &cubePos[0], 0.05f);
			}
			else if (current == 2) {
				ImGui::DragFloat3("lightPos", &lightPos[0], 0.05f);
			}
			ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)",
				camera.getPos().x, camera.getPos().y, camera.getPos().z);
			ImGui::Text("Camera Rotation: (%.1f, %.1f)",
				camera.getYaw(), camera.getPitch());
			ImGui::Text("Camera Front: (%.1f, %.1f, %.1f)",
				camera.getFront().x, camera.getFront().y, camera.getFront().z);
		}

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		windowManager.swapBuffers();
		windowManager.pullEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
