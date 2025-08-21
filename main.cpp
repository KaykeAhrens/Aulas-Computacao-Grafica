#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h> // GLEW deve estar antes do glfw3.h para evitar conflitos de tipos e definições
#include <GLFW\glfw3.h>

using namespace std;

const GLint WIDTH = 1024, HEIGHT = 768; // Definindo a largura e altura da janela
GLuint VAO, VBO, shaderProgram;

// Plota o X,Y
static const char *vertexShader = "				\n\
#version 330									\n\
												\n\
layout(location=0) in vec2 pos;					\n\
												\n\
void main() {									\n\
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); \n\
}												\n\
";

// Troca a Cor
static const char *fragmentShader = "			\n\
#version 330									\n\
												\n\
uniform in vec3 triColor;						\n\
												\n\
void main() {									\n\
	color = vec4(triColor, 1.0);				\n\
}												\n\
";

void criaTriangulo() {
	GLfloat vertices[] = {
		0.0f, 1.0f,   // Vertice 1
		-1.0f, -1.0f, // Vertice 2
		1.0, -1.0f    // Vertice 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	  
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		  
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0); // Location

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
}

void adicionaTriangulo(GLuint program, const char *shaderCode, GLenum type) {
	GLuint _shader = glCreateShader(type);

	// Converte Char pra GL_CHAR
	const GLchar *code[1];
	code[0] = shaderCode;

	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);

	// Tratar os Erros

	glAttachShader(program, _shader);
}

void adicionarPrograma() {
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		printf("Erro ao criar o programa");
		return;
	}

	adicionaTriangulo(shaderProgram, vertexShader, GL_VERTEX_SHADER);
	adicionaTriangulo(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

	glLinkProgram(shaderProgram);
}

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