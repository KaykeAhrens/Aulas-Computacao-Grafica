#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h> // GLEW deve estar antes do glfw3.h para evitar conflitos de tipos e definições
#include <GLFW\glfw3.h>
#include <time.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

const GLint WIDTH = 1024, HEIGHT = 768; // Definindo a largura e altura da janela
GLuint VAO, VBO, IBO, shaderProgram;

float toRadians = 3.1415f / 180.0f;

bool direction = false, directionSize = false;
float triOffset = 0.0f, triOffsetMax = 0.7f, triOffsetMin = -0.7f, triIncrement = 0.01f;
float triOffsetSize = 0.2f, triOffsetSizeMax = 1.2f, triOffsetSizeMin = 0.2, triOffsetSizeIncrement = 0.01f;
float triCurrentAngle = 0.0f, triIncrementAngle = 1.0f;

// Plota o X,Y
static const char *vertexShader = "								\n\
#version 330													\n\
																\n\
layout(location=0) in vec3 pos;									\n\
uniform mat4 model;												\n\
uniform mat4 projection;										\n\
out vec4 vCol;													\n\
																\n\
void main() {													\n\
	gl_Position =  projection * model * vec4(pos, 1.0);			\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}																\n\
";

// Troca a Cor
static const char *fragmentShader = "							\n\
#version 330													\n\
																\n\
in vec4 vCol;													\n\
uniform vec3 triColor;											\n\
out vec4 color;													\n\
																\n\
void main() {													\n\
	color = vCol;												\n\
}																\n\
";

void criaTriangulo() {

	unsigned int indices[] = {
		0,1,2,	// Base
		0,1,3,	// Esquerda
		0,2,4,	// Direita
		1,2,3,	// Frente
	};

	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f,   // Vértice 1
		-1.0f, -1.0f, 0.0f, // Vértice 2
		1.0, -1.0f, 0.0f,    // Vértice 3
		0.0f, 0.0f, 1.0f	// Vértice 4
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	  
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		  
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

	criaTriangulo();
	adicionarPrograma();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.0f);

	// Altera cor do triângulo
	srand(time(NULL));

	GLint uniformColor = glGetUniformLocation(shaderProgram, "triColor");
	float color1 = rand() % 100 / 100.0f;
	float color2 = rand() % 100 / 100.0f;
	float color3 = rand() % 100 / 100.0f;

	// Loop principal
	while (!glfwWindowShouldClose(window)) {
		// Limpar a tela
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform3f(uniformColor, color1, color2, color3);

		// Movimenta o triângulo
		if (!direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (triOffset > triOffsetMax || triOffset < triOffsetMin) {
			direction = !direction;
		}

		triCurrentAngle += triIncrementAngle;

		if (triCurrentAngle >= 360) {
			triCurrentAngle = 0;
		}

		if (!directionSize) {
			triOffsetSize += triOffsetSizeIncrement;
		}
		else {
			triOffsetSize -= triOffsetSizeIncrement;
		}

		if (triOffsetSize > triOffsetSizeMax || triOffsetSize < triOffsetSizeMin) {
			directionSize = !directionSize;
		}

		GLint uniformModel = glGetUniformLocation(shaderProgram, "model");
		GLint uniformProjection = glGetUniformLocation(shaderProgram, "projection");
		glm::mat4 model(1.0f); // matriz 4x4 completa com 1.0 em todas posições
		
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, triCurrentAngle * toRadians, glm::vec3(0.7f, 0.5f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Desenhando o triangulo
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

			//glDrawArrays(GL_TRIANGLES, 0, 3); // Triângulo começando na posição 0, Número de pontos 3
		

		// Trocar os buffers
		glfwSwapBuffers(window);
	}
	// Encerrar o GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
