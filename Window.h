#pragma once
#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();
	int initialize();
	void swapBuffers();

	GLfloat getBufferWidth() { return (GLfloat)bufferWidth; }
	GLfloat getBufferHeight() { return (GLfloat)bufferHeight; }
	bool getWindowShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//Configuração de mouse e teclado
	void createCallbacks();

	//Configuração para teclado
	bool keys[1024];
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	//Configuração para mouse
	GLfloat lastX; //ultima posição de X
	GLfloat lastY; //ultima posição de Y
	GLfloat xChange; //alteração de X
	GLfloat yChange; //alteração de Y
	bool mouseFirstMove; //verifica se é o primeiro movimento do mouse
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

