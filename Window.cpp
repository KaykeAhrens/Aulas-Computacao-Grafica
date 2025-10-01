#include "Window.h"

Window::Window() {
	width = 800;
	height = 600;
	for (int i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

	for (int i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
	xChange = 0.0f;
	yChange = 0.0f;
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

int Window::initialize() {
	//INICIALIZAR O GLFW
	if (!glfwInit()) {
		printf("GLFW Nao foi inicializado");
		glfwTerminate();
		return 1;
	};

	//GLFW OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Forward Functions
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Nova Janela", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW nao criou a janela");
		glfwTerminate();
		return 1;
	}

	//Pegar o buffer size da largura e altura
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Configurando a janela principal
	glfwMakeContextCurrent(mainWindow);

	//Associa as funções de callback de teclado e mouse
	createCallbacks();

	//Desabilitar o cursor do mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Glew nao foi iniciado");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	};

	glEnable(GL_DEPTH_TEST); //Habilitar o Depth Test

	//Configurando viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	/**
	* Define no GLFW que a classe associada a essa window é essa (this),
	* para utilizar as funções estaticas de teclado
	*/
	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

void Window::swapBuffers() {
	glfwSwapBuffers(mainWindow);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	//Recebe o valor da classe transformando e convertendo para Window que foi gravada no glfwSetWindowUserPointer
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	//Se a tecla que tem a action "Apertada" e for o valor "ESQ"
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		//Enviar o comando de fechar a janela
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	//Garatnir que somente as teclas até 1024 foram apertadas
	if (key >= 0 && key <= 1024) {
		if (action == GLFW_PRESS){
			theWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}else if(action == GLFW_RELEASE){
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (theWindow->mouseFirstMove) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMove = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("x:%.6f y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

void Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0;
	return theChange;
}

GLfloat Window::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0;
	return theChange;
}