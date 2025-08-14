#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h> // GLEW deve estar antes do glfw3.h para evitar conflitos de tipos e definições
#include <GLFW\glfw3.h>

const GLint WIDTH = 1024, HEIGHT = 768; // Definindo a largura e altura da janela

int main() {
	// Iniciar o GLFW
	glfwInit();
	if (!glfwInit()) {
		printf("GLFW não foi iniciado\n");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
	if (!window) {
		printf("Erro ao criar a janela GLFW\n");
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; // Permitir o uso de extensões modernas do OpenGL
	if (glewInit() != GLEW_OK) {
		printf("Erro ao iniciar o GLEW\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	// Configurar a viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Loop principal
	while (!glfwWindowShouldClose(window)) {
		// Limpar a tela
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		// Trocar os buffers
		glfwSwapBuffers(window);
	}
	// Encerrar o GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}