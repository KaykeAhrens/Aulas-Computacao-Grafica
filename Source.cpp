#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "texture.h"

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Window mainWindow;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

//Old version of FPS
GLfloat deltaTime = 0.0f, lastime = 0.0f;

static const char* vertexLocation = "./Shaders/VertexShader.glsl";
static const char* fragmentLocation = "./Shaders/FragmentShader.glsl";

void CreateTriangle() {
	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f,	0.5f, 1.0f,  //Vértice 0 (x,y,z, u,v)
		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,  //Vértice 1 (x,y,z, u,v)
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //Vértice 2 (x,y,z, u,v)
		0.0f, -1.0f, 1.0f,	0.5f, 0.0f   //Vértice 3 (x,y,z, u,v)
	};

	unsigned int indices[] = {
		0,1,2, //Frente da pirâmide
		0,1,3, //Parede lateral direita
		0,2,3, //Parede lateral esquerda
		1,2,3  //Base da pirâmide
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
	meshList.push_back(obj2);
}

void CreateFloor() {
	GLfloat vertices[] = {
		// Posições (x, y, z)    // Coordenadas de textura (u, v)
		-10.0f, 0.0f, -10.0f,    0.0f, 0.0f,   // Vértice 0 - canto inferior esquerdo
		 10.0f, 0.0f, -10.0f,    10.0f, 0.0f,  // Vértice 1 - canto inferior direito
		 10.0f, 0.0f,  10.0f,    10.0f, 10.0f, // Vértice 2 - canto superior direito
		-10.0f, 0.0f,  10.0f,    0.0f, 10.0f   // Vértice 3 - canto superior esquerdo
	};

	unsigned int indices[] = {
		0, 1, 2,  // Primeiro triângulo
		2, 3, 0   // Segundo triângulo
	};

	Mesh* floor = new Mesh();
	floor->CreateMesh(vertices, indices, sizeof(vertices), sizeof(indices));
	meshList.push_back(floor);
}

void CreateShader() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vertexLocation, fragmentLocation);
	shaderList.push_back(*shader1);
}

int main() {
	mainWindow = Window(800, 600);
	mainWindow.initialize();

	//Criar o Triangulo
	CreateTriangle(); //Coloca os dados na memória da placa de vídeo
	CreateFloor(); //Cria o chão
	CreateShader(); //Cria os Shaders

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 8.0f);
	
	//Carrega as Textures
	brickTexture = Texture((char*)("Textures/brick.png"));
	brickTexture.loadTexture();
	dirtTexture = Texture((char*)("Textures/dirt.png"));
	dirtTexture.loadTexture();
	
	glm::mat4 projection = glm::perspective(1.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getWindowShouldClose()) {
		//Old version of FPS
		GLfloat now = glfwGetTime();
		deltaTime = now - lastime;
		lastime = now;

		//Ativa inputs e eventos
		glfwPollEvents();

		//Controle do teclado
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), deltaTime);

		/********************************
		* Cor de fundo da tela
		*********************************/
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Limpa a janela, cor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/********************************
		* Piramides
		*********************************/
		shaderList[0].UseProgram(); //Usar o programa
		glUniformMatrix4fv(shaderList[0].getUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection)); //Movimentação da projeção da camera
		glUniformMatrix4fv(shaderList[0].getUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		
			/********************************
			* Piramide 1
			*********************************/
			glm::mat4 model(1.0f); //cria uma matriz 4x4 e coloca os valores 1.0f em todas as posições
			model = glm::translate(model, glm::vec3(0.0f, -0.25f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(model));
			brickTexture.useTexture();
			meshList[0]->RenderMesh();

			/********************************
			* Piramide 2
			*********************************/
			model = glm::mat4(1.0f); //cria uma matriz 4x4 colocando 1.0f em cada uma das posições
			model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f)); //traduz o modelo para movimentar a posição (x,y,z)
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
			glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(model));
			dirtTexture.useTexture();
			meshList[1]->RenderMesh();

		/********************************
		* Chão
		*********************************/
		glm::mat4 modelFloor(1.0f);
		modelFloor = glm::translate(modelFloor, glm::vec3(0.0f, -1.0f, 0.0f)); // Posiciona o chão abaixo das pirâmides
		glUniformMatrix4fv(shaderList[0].getUniformModel(), 1, GL_FALSE, glm::value_ptr(modelFloor));
		dirtTexture.useTexture(); 
		meshList[2]->RenderMesh(); 
		
		glUseProgram(0); //Removo o Programa da memória

		//Atualiza a tela
		mainWindow.swapBuffers();
	}

	return 0;
}