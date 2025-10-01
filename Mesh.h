#pragma once
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};