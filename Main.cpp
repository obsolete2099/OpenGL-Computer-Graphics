#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath>




#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


float cx = 10.0f;
float cy = 3.0f;
float cz = 20.0f;

bool wireframeMode = false;
bool showSphere = true;
bool showTetrahedron = true;
bool showCube = true;
bool showOctahedron = true;
bool showIcosahedron = true;
bool showDodecahedron = true;
bool showAllShapes = true;



const float PI = 3.14159265358979323846f;

std::vector<float> CreateSphereVertices(float radius, int stackCount, int sectorCount, int xOffset, int yOffset, int zOffset) {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> colors; // New vector for vertex colors

	float x, y, z, xy; // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);      // r * cos(u)
		z = radius * sinf(stackAngle);       // r * sin(u)

		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep; // starting from 0 to 2pi

			// Vertex position (x, y, z)
			x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
			vertices.push_back(x + xOffset);
			vertices.push_back(y + yOffset);
			vertices.push_back(z + zOffset);

			// Calculate color based on vertex position (you can customize this)
			float r = (x + 1.0f) / 2.0f; // Red component
			float g = (y + 1.0f) / 2.0f; // Green component
			float b = (z + 1.0f) / 2.0f; // Blue component
			vertices.push_back(r);
			vertices.push_back(g);
			vertices.push_back(b);

			// Normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);

			
		}
	}

	

	return vertices;
}



std::vector<unsigned int> CreateSphereIndices(int stacks, int slices) {
	std::vector<unsigned int> indices;

	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			int first = (i * (slices + 1)) + j;
			int second = first + slices + 1;

			// Define the indices for two triangles that make up a quad
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	return indices;
}








void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		wireframeMode = !wireframeMode;
		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
	}
	else if (action == GLFW_PRESS) {

		switch (key) {
		case GLFW_KEY_1:
			showTetrahedron = true;
			showCube = false;
			showOctahedron = false;
			showIcosahedron = false;
			showDodecahedron = false;
			showSphere = false;
			break;
		case GLFW_KEY_2:
			showTetrahedron = false;
			showCube = true;
			showOctahedron = false;
			showIcosahedron = false;
			showDodecahedron = false;
			showSphere = false;
			break;
		case GLFW_KEY_3:
			showTetrahedron = false;
			showCube = false;
			showOctahedron = true;
			showIcosahedron = false;
			showDodecahedron = false;
			showSphere = false;
			break;
		case GLFW_KEY_4:
			showTetrahedron = false;
			showCube = false;
			showOctahedron = false;
			showIcosahedron = true;
			showDodecahedron = false;
			showSphere = false;
			break;
		case GLFW_KEY_5:
			showTetrahedron = false;
			showCube = false;
			showOctahedron = false;
			showIcosahedron = false;
			showDodecahedron = true;
			showSphere = false;
			break;
		case GLFW_KEY_6:
			showTetrahedron = false;
			showCube = false;
			showOctahedron = false;
			showIcosahedron = false;
			showDodecahedron = false;
			showSphere = true;
			break;
		case GLFW_KEY_7:
			showTetrahedron = showCube = showOctahedron = showIcosahedron = showDodecahedron = showSphere = true;
			break;
		}
	}
}

const unsigned int width = 1000;
const unsigned int height = 1000;

GLfloat lightVertices[] =
{
	//Coordinates
	9.0f, 1.0f, 1.0f,
	9.0f, 1.0f, -1.0f,
	11.0f, 1.0f, -1.0f,
	11.0f, 1.0f, 1.0f,
	9.0f, 3.0f, 1.0f,
	9.0f, 3.0f, -1.0f,
	11.0f, 3.0f, -1.0f,
	11.0f, 3.0f, 1.0f

};


GLuint lightIndices[] =
{
	0,1,2,
	0,2,3,
	0,4,7,
	0,7,3,
	3,7,6,
	3,6,2,
	2,6,5,
	2,5,1,
	1,5,4,
	1,4,0,
	4,5,6,
	4,6,7

};



GLfloat vertices[] =
{
	// vertices            color               normals
	11.25, 1.0, 4.0,     0.0f, 0.0f, 0.8f,  0.938927, 0.0834602, 0.333841,
	13.25, 1.0, 4.0,     0.0f, 0.0f, 0.8f,  0.938927, 0.0834602, 0.333841,
	12.25, 1.0, 2.0,     0.0f, 0.0f, 0.8f,  0.938927, 0.0834602, 0.333841,

	12.25, 3.0, 3.0,     0.0f, 0.0f, 0.8f,  0.944932, 0.231412, 0.231412,
	11.25, 1.0, 4.0,     0.0f, 0.0f, 0.8f,  0.944932, 0.231412, 0.231412,
	13.25, 1.0, 4.0,     0.0f, 0.0f, 0.8f,  0.944932, 0.231412, 0.231412,

	12.25, 3.0, 3.0,     0.0f, 0.0f, 0.8f,   -0.944932, -0.231412, 0.231412,
	13.25, 1.0, 4.0,     0.0f, 0.0f, 0.8f,   -0.944932, -0.231412, 0.231412,
	12.25, 1.0, 2.0,     0.0f, 0.0f, 0.8f,   -0.944932, -0.231412, 0.231412,

	12.25, 3.0, 3.0,     0.0f, 0.0f, 0.8f,  -0.944932, -0.231412, -0.231412,
	11.25, 1.0, 4.0,     0.0f, 0.0f, 0.8f,  -0.944932, -0.231412, -0.231412,
	12.25, 1.0, 2.0,     0.0f, 0.0f, 0.8f,  -0.944932, -0.231412, -0.231412,
};




// TetrahedronIndices for vertices order
GLuint indices[] =
{
	1, 0, 2,  //bottom
	3, 4, 5,  //back
	6, 7, 8,  //right
	9, 10, 11, //left


};


//vertices
GLfloat cubeVertices[] = {
	// vertices				color						normals
	 3.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		 0.514496, 0, -0.857493,
	 5.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		0.514496, 0, -0.857493,
	 5.0,  2.0,  5.0,		 0.92f, 0.88f, 0.78f,		0.514496, 0, -0.857493,

	 3.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		0.514496, 0, -0.857493,
	 5.0,  2.0,  5.0,		 0.92f, 0.88f, 0.78f,		0.514496, 0, -0.857493,
	 3.0,  2.0,  5.0,		 0.92f, 0.88f, 0.78f,		0.514496, 0, -0.857493,

	 5.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,	     -0.707107, 0, -0.707107,
	 5.0,  0.0, 3.0,		0.92f, 0.88f, 0.78f,		 -0.707107, 0, -0.707107,
	 5.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		 -0.707107, 0, -0.707107,

	 5.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		 -0.707107, 0, -0.707107,
	 5.0,  2.0,  5.0,		 0.92f, 0.88f, 0.78f,		 -0.707107, 0, -0.707107,
	 5.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		 -0.707107, 0, -0.707107,

	 3.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.639602, -0.426401, -0.639602,
	 5.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.639602, -0.426401, -0.639602,
	 5.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.639602, -0.426401, -0.639602,

	 5.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.639602, -0.426401, -0.639602,
	 3.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.639602, -0.426401, -0.639602,
	 3.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.639602, -0.426401, -0.639602,

	 3.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		0.707107, 0, -0.707107,
	 3.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		0.707107, 0, -0.707107,
	 3.0,  2.0,  5.0,		0.92f, 0.88f, 0.78f,		0.707107, 0, -0.707107,

	 3.0,  2.0,  5.0,		0.92f, 0.88f, 0.78f,		0.707107, 0, -0.707107,
	 3.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		0.707107, 0, -0.707107,
	 3.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		0.707107, 0, -0.707107,

	 3.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		0, 0, 0,
	 5.0,  0.0, 3.0,		0.92f, 0.88f, 0.78f,		0, 0, 0,
	 5.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		0, 0, 0,

	 5.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		0, 0, 0,
	 3.0,  0.0,  5.0,		 0.92f, 0.88f, 0.78f,		0, 0, 0,
	 3.0,  0.0, 3.0,		 0.92f, 0.88f, 0.78f,		0, 0, 0,

	 3.0,  2.0,  5.0,		 0.92f, 0.88f, 0.78f,		-0.486664, -0.324443, -0.811107,
	 5.0,  2.0,  5.0,		0.92f, 0.88f, 0.78f,		-0.486664, -0.324443, -0.811107,
	 5.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.486664, -0.324443, -0.811107,

	 5.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.486664, -0.324443, -0.811107,
	 3.0,  2.0, 3.0,		 0.92f, 0.88f, 0.78f,		-0.486664, -0.324443, -0.811107,
	 3.0,  2.0,  5.0,		 0.92f, 0.88f, 0.78f,		-0.486664, -0.324443, -0.811107,
};
//cube
GLuint cubeIndices[] = {
		//back
		0, 1, 2,
		3, 4, 5,
		// right
		6, 7, 8,
		9, 10, 11,
		// front
		12, 13, 14,
		15, 16, 17,
		// left
		18, 19, 20,
		21, 22, 23,
		// bottom
		24, 25, 26,
		27, 28, 29,
		// top
		30, 31, 32,
		33, 34, 35,
};

GLfloat octoVertices[] = {

		//vertices                color                     normals
		//top-north-east
	   8.0,  2.0,  8.0,		 0.22f, 0.70f, 0.44f,		0.696311, 0.174078, 0.696311,
	   8.0,  1.0,  9.0,		 0.83f, 0.70f, 0.44f,		0.696311, 0.174078, 0.696311,
	   9.0,  1.0,  8.0,		0.22f, 0.70f, 0.44f,		0.696311, 0.174078, 0.696311,

	   //top-north-west
		8.0,  2.0,  8.0,     0.22f, 0.70f, 0.44f,		0.696311, 0.174078, 0.696311,
		7.0,  1.0,  8.0,	 0.83f, 0.70f, 0.44f,		0.696311, 0.174078, 0.696311,
		8.0,  1.0,  9.0,     0.22f, 0.70f, 0.44f,		0.696311, 0.174078, 0.696311,

		//top-south-west
		 8.0,  2.0,  8.0,    0.22f, 0.70f, 0.44f,		-0.696311, 0.174078, -0.696311,
		 8.0,  1.0, 7.0,    0.83f, 0.70f, 0.44f,		-0.696311, 0.174078, -0.696311,
		7.0,  1.0,  8.0,    0.22f, 0.70f, 0.44f,		-0.696311, 0.174078, -0.696311,

		//top-south-east
		 8.0,  2.0,  8.0,    0.22f, 0.70f, 0.44f,		-0.696311, -0.174078, -0.696311,
		 9.0,  1.0,  8.0,    0.83f, 0.70f, 0.44f,		-0.696311, -0.174078, -0.696311,
		 8.0,  1.0, 7.0,    0.22f, 0.70f, 0.44f,		-0.696311, -0.174078, -0.696311,

		 //bottom-north-east
		 8.0, 0.0,  8.0,	0.22f, 0.70f, 0.44f,		0.707107, 0, 0.707107,
		 9.0,  1.0,  8.0,	 0.83f, 0.70f, 0.44f,		0.707107, 0, 0.707107,
		 8.0,  1.0,  9.0,	0.22f, 0.70f, 0.44f,		0.707107, 0, 0.707107,

		 //bottom-north-west
		 8.0, 0.0,  8.0,	 0.22f, 0.70f, 0.44f,		0.0,0.0,0.0,
		 8.0,  1.0,  9.0,	 0.83f, 0.70f, 0.44f,		0.0,0.0,0.0,
		 7.0,  1.0,  8.0,  0.22f, 0.70f, 0.44f,			0.0,0.0,0.0,

		 //bottom-south-west
		  8.0, 0.0,  8.0,	0.22f, 0.70f, 0.44f,		-0.707107, 0, -0.707107,
		 7.0,  1.0,  8.0,  0.83f, 0.70f, 0.44f,			-0.707107, 0, -0.707107,
		  8.0,  1.0, 7.0,  0.22f, 0.70f, 0.44f,			-0.707107, 0, -0.707107,

		  //bottom-south-east
		  8.0, 0.0,  8.0,	  0.22f, 0.70f, 0.44f,		-0.707107, 0, -0.707107,
		  8.0,  1.0, 7.0,	 0.83f, 0.70f, 0.44f,		-0.707107, 0, -0.707107,
		  9.0,  1.0,  8.0,	 0.22f, 0.70f, 0.44f,		-0.707107, 0, -0.707107,

};


GLuint octoIndices[] = {
	// first triangle
	 0, 1, 2,
	 // second triangle
	 3, 4, 5,
	 // third triangle
	 6, 7, 8,
	 // fourth triangle
	 9, 10, 11,
	 // fifth triangle
	 12, 13, 14,
	 // sixth triangle
	 15, 16, 17,
	 // seventh triangle
	 18, 19, 20,
	 // eighth triangle
	 21, 22, 23

};

#define X 0.525731112119133606
#define Z 0.850650808352039932


GLfloat icoVertices[] = {
	// Vertices				color								Normals
	-X, 0.0 + 2.0, Z,      0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, -0.380423,//upper left side
	0.0, Z + 2.0, X,       0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, -0.380423,
	X, 0.0 + 2.0, Z,       0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, -0.380423,

	-X, 0.0 + 2.0, Z,      0.8f, 0.0f, 0.0f,		 0.235114, 0.894427, 0.380423, //back upper right
	-Z, X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 0.235114, 0.894427, 0.380423,
	0.0, Z + 2.0, X,       0.8f, 0.0f, 0.0f,		 0.235114, 0.894427, 0.380423,

	0.0, Z + 2.0, X,       0.8f, 0.0f, 0.0f,		 0, -0.983416, -0.181366, //upper front
	0.0, Z + 2.0, -X,      0.8f, 0.0f, 0.0f,		 0, -0.983416, -0.181366,
	Z, X + 2.0, 0.0,       0.8f, 0.0f, 0.0f,		 0, -0.983416, -0.181366,

	0.0, Z + 2.0, X,       0.8f, 0.0f, 0.0f,		 0, -0.983416, -0.181366, //front upper left
	Z, X + 2.0, 0.0,       0.8f, 0.0f, 0.0f,		0, -0.983416, -0.181366,
	X, 0.0 + 2.0, Z,       0.8f, 0.0f, 0.0f,		 0, -0.983416, -0.181366,

	Z, X + 2.0, 0.0,       0.8f, 0.0f, 0.0f,		-0.319178, -0.947695, 0, //front middle left
	Z, -X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 -0.319178, -0.947695, 0,
	X, 0.0 + 2.0, Z,       0.8f, 0.0f, 0.0f,		 -0.319178, -0.947695, 0,

	Z, X + 2.0, 0.0,       0.8f, 0.0f, 0.0f,		 -0.319178, -0.947695, 0, //front middle right
	X, 0.0 + 2.0, -Z,      0.8f, 0.0f, 0.0f,		 -0.319178, -0.947695, 0,
	Z, -X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 -0.319178, -0.947695, 0,

	0.0, Z + 2.0, -X,      0.8f, 0.0f, 0.0f,		 0, -0.983416, 0.181366, //front upper right
	X, 0.0 + 2.0, -Z,      0.8f, 0.0f, 0.0f,		  0, -0.983416, 0.181366,
	Z, X + 2.0, 0.0,       0.8f, 0.0f, 0.0f,		 0, -0.983416, 0.181366,

	0.0, Z + 2.0, -X,      0.8f, 0.0f, 0.0f,		 0, -0.356822, 0.934172,  //upper middle right
	-X, 0.0 + 2.0, -Z,     0.8f, 0.0f, 0.0f,		 0, -0.356822, 0.934172,
	X, 0.0 + 2.0, -Z,      0.8f, 0.0f, 0.0f,		 0, -0.356822, 0.934172,

	-X, 0.0 + 2.0, -Z,     0.8f, 0.0f, 0.0f,		 0, -0.983416, 0.181366, // right side lower middle right
	0.0, -Z + 2.0, -X,     0.8f, 0.0f, 0.0f,		 0, -0.983416, 0.181366,
	X, 0.0 + 2.0, -Z,      0.8f, 0.0f, 0.0f,		 0, -0.983416, 0.181366,

	0.0, -Z + 2.0, -X,     0.8f, 0.0f, 0.0f,		  0.235114, -0.894427, 0.380423, //front lower right
	Z, -X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, 0.380423,
	X, 0.0 + 2.0, -Z,      0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, 0.380423,

	0.0, -Z + 2.0, -X,     0.8f, 0.0f, 0.0f,		 0, -0.90938, -0.415965,  //lower front
	0.0, -Z + 2.0, X,      0.8f, 0.0f, 0.0f,		 0, -0.90938, -0.415965,
	Z, -X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 0, -0.90938, -0.415965,

	0.0, -Z + 2.0, -X,     0.8f, 0.0f, 0.0f,		0, 0.90938, -0.415965, //lower back
	-Z, -X + 2.0, 0.0,     0.8f, 0.0f, 0.0f,		0, 0.90938, -0.415965,
	0.0, -Z + 2.0, X,      0.8f, 0.0f, 0.0f,		0, 0.90938, -0.415965,

	-Z, -X + 2.0, 0.0,     0.8f, 0.0f, 0.0f,		 0.499771, 0.866157, 0, //back lower right
	-X, 0.0 + 2.0, Z,      0.8f, 0.0f, 0.0f,		 0.499771, 0.866157, 0,
	0.0, -Z + 2.0, X,      0.8f, 0.0f, 0.0f,		 0.499771, 0.866157, 0,

	-X, 0.0 + 2.0, Z,      0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, -0.380423,  //lower middle
	X, 0.0 + 2.0, Z,       0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, -0.380423,
	0.0, -Z + 2.0, X,      0.8f, 0.0f, 0.0f,		 0.235114, -0.894427, -0.380423,

	0.0, -Z + 2.0, X,      0.8f, 0.0f, 0.0f,		 0, -0.90938, -0.415965, //front lower left ???
	X, 0.0 + 2.0, Z,       0.8f, 0.0f, 0.0f,		 0, -0.90938, -0.415965,
	Z, -X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 0, -0.90938, -0.415965,

	-Z, X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		  0.319178, 0.947695, 0, //back middle right
	-X, 0.0 + 2.0, Z,      0.8f, 0.0f, 0.0f,		  0.319178, 0.947695, 0,
	-Z, -X + 2.0, 0.0,     0.8f, 0.0f, 0.0f,		  0.319178, 0.947695, 0,

	-Z, X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 0.319178, 0.947695, 0, //back middle left
	-Z, -X + 2.0, 0.0,     0.8f, 0.0f, 0.0f,		 0.319178, 0.947695, 0,
	-X, 0.0 + 2.0, -Z,     0.8f, 0.0f, 0.0f,		 0.319178, 0.947695, 0,

	-Z, X + 2.0, 0.0,      0.8f, 0.0f, 0.0f,		 0.319178, 0.947695, 2.23194e-17, //back upper left
	-X, 0.0 + 2.0, -Z,     0.8f, 0.0f, 0.0f,		 0.319178, 0.947695, 2.23194e-17,
	0.0, Z + 2.0, -X,      0.8f, 0.0f, 0.0f,		 0.319178, 0.947695, 2.23194e-17,

	0.0, -Z + 2.0, -X,     0.8f, 0.0f, 0.0f,		 0, 0.90938, 0.415965, //back lower left
	-X, 0.0 + 2.0, -Z,     0.8f, 0.0f, 0.0f,		 0, 0.90938, 0.415965,
	-Z, -X + 2.0, 0.0,     0.8f, 0.0f, 0.0f,		 0, 0.90938, 0.415965,

};

// Indices to define the triangles of the icosahedron
GLuint icoIndices[] = {
	// Upper hemisphere
	 // Upper hemisphere //
	0,1,2,
	3,4,5,
	6,7,8,
	9,10,11,
	12,13,14,
	// Lower hemisphere //
	15,16,17,
	18,19,20,
	21,22,23,
	24,25,26,
	27,28,29,

	// Equator
	30,31,32,
	33,34,35,
	36,37,38,
	39,40,41,
	42,43,44,
	45,46,47,
	48,49,50,
	51,52,53,
	54,55,56,
	57,58,59


};

GLfloat dodecaVertices[] = {
	//vertices				colors					normals
	
	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,//middle back left
	21.618, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,
	21, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,

	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,
	21, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,
	20, -0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,

	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,
	20, -0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,
	20, 0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.99774, -0.0475114, 0.0475114,  //middle back left done

	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,//top left
	20, 0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	20.618, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, //top left done

	21.618, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756, //upper back
	21.618, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,
	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,

	21.618, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,
	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,
	20.618, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,
			
	21.618, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,
	20.618, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756,
	21, 1, 1,				0.5f, 0.0f, 0.5f,		-0.999592, 0, 0.0285756, //upper back done??

	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114, //middle back right
	20, 0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,
	20, -0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,

	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,
	20, -0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,
	21, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,

	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,
	21, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114,
	21.618, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, -0.0475114, //middle back right done

	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, //top right
	20.618, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, 1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	20, 0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, //top right done

	21, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, // bottom left
	20.618, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, -1, 1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	20, -0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, //bottom left

	20.618, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0, //lower back
	21, -1, -1,				0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,
	21.618, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,

	20.618, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,
	21.618, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,
	21.618, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,

	20.618, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,
	21.618, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0,
	21, -1, 1,				0.5f, 0.0f, 0.5f,		-0.996935, 0.0782346, 0, //lower back done

	21, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, //bottom right
	20, -0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,

	21, -1, -1,				0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114,
	20.618, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.99774, 0.0475114, 0.0475114, //bottom right done

	19, 1, 1,				0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864, //front right
	20, 0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,
	20, -0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,

	19, 1, 1,				0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,
	20, -0.618, 1.618,		0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,
	19, -1, 1,				0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,

	19, 1, 1,				0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864, //
	19, -1, 1,				0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,
	18.382, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.997241, -0.0524864, 0.0524864,  //front right

	19.382, 1.618, 0,		 0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18, //upper front
	19, 1, -1,				0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,
	18.382, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,

	19.382, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,
	18.382, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,
	18.382, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,

	19.382, 1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,
	18.382, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18,
	19, 1, 1,				0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, -8.03924e-18, //upper front

	19, 1, -1,				0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864, //front left
	18.382, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,
	19, -1, -1,				0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,

	19, 1, -1,				0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,
	19, -1, -1,				0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,
	20, -0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,

	19, 1, -1,				0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,
	20, -0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,
	20, 0.618, -1.618,		0.5f, 0.0f, 0.5f,		-0.997241, 0.0524864, -0.0524864,  // front left

	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18, //lower front
	19, -1, -1,				0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,
	18.382, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,

	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,
	18.382, 0, -0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,
	18.382, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,

	19.382, -1.618, 0,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,
	18.382, 0, 0.618,		0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18,
	19, -1, 1,				0.5f, 0.0f, 0.5f,		-0.996534, -0.0831902, 8.03924e-18, //lower front


};



GLuint dodecaIndices[] = {
	0,1,2,
	3,4,5,
	6,7,8,

	9,10,11,
	12,13,14,
	15,16,17,

	18,19,20,
	21,22,23,
	24,25,26,

	27,28,29,
	30,31,32,
	33,34,35,

	36,37,38,
	39,40,41,
	42,43,44,

	45,46,47,
	48,49,50,
	51,52,53,

	54,55,56,
	57,58,59,
	60,61,62,

	63,64,65,
	66,67,68,
	69,70,71,

	72,73,74,
	75,76,77,
	78,79,80,

	81,82,83,
	84,85,86,
	87,88,89,

	90,91,92,
	93,94,95,
	96,97,98,

	99,100,101,
	102,103,104,
	105,106,107,

	

};








int main()
{
	
	

	


	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 1000 by 1000 pixels
	GLFWwindow* window = glfwCreateWindow(width, height, "AZaragosa", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Set the key callback function
	glfwSetKeyCallback(window, keyCallback);


	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 1000, y = 1000
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");





	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();


	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));



	// Links VBO attributes such as colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();






	//cube
	Shader cubeShader("default.vert", "pearl.frag");

	VAO cubeVAO;
	cubeVAO.Bind();

	VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
	EBO cubeEBO(cubeIndices, sizeof(cubeIndices));

	cubeVAO.LinkAttrib(cubeVBO, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	cubeVAO.LinkAttrib(cubeVBO, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	cubeVAO.LinkAttrib(cubeVBO, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	cubeVAO.Unbind();
	cubeVBO.Unbind();
	cubeEBO.Unbind();

	//octahedron
	Shader octoShader("default.vert", "bronze.frag");

	VAO octoVAO;
	octoVAO.Bind();

	VBO octoVBO(octoVertices, sizeof(octoVertices));
	EBO octoEBO(octoIndices, sizeof(octoIndices));

	octoVAO.LinkAttrib(octoVBO, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	octoVAO.LinkAttrib(octoVBO, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	octoVAO.LinkAttrib(octoVBO, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));


	octoVAO.Unbind();
	octoVBO.Unbind();
	octoEBO.Unbind();

	//icosahedron
	Shader icoShader("default.vert", "ruby.frag");

	VAO icoVAO;
	icoVAO.Bind();

	VBO icoVBO(icoVertices, sizeof(icoVertices));
	EBO icoEBO(icoIndices, sizeof(icoIndices));

	icoVAO.LinkAttrib(icoVBO, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	icoVAO.LinkAttrib(icoVBO, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	icoVAO.LinkAttrib(icoVBO, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	icoVAO.Unbind();
	icoVBO.Unbind();
	icoEBO.Unbind();


	//dodecahedron
	Shader dodecaShader("default.vert", "obsidian.frag");

	VAO dodecaVAO;
	dodecaVAO.Bind();

	VBO dodecaVBO(dodecaVertices, sizeof(dodecaVertices));
	EBO dodecaEBO(dodecaIndices, sizeof(dodecaIndices));

	dodecaVAO.LinkAttrib(dodecaVBO, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	dodecaVAO.LinkAttrib(dodecaVBO, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	dodecaVAO.LinkAttrib(dodecaVBO, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	dodecaVAO.Unbind();
	dodecaVBO.Unbind();
	dodecaEBO.Unbind();


	// Create sphere vertices using the CreateSphereVertices function
	Shader sphereShader("default.vert", "brass.frag");
	std::vector<float> sphereVertices = CreateSphereVertices(1.0f, 10, 10, 0, 0, 0); // Adjust the parameters as needed
	std::vector<unsigned int> sphereIndices = CreateSphereIndices(10, 10);


	// Generate VAO, VBO, and EBO for the sphere
	VAO sphereVAO;
	sphereVAO.Bind();

	VBO sphereVBO(sphereVertices.data(), sizeof(float) * sphereVertices.size());
	EBO sphereEBO(sphereIndices.data(), sizeof(int) * sphereIndices.size()); // There are no indices for a sphere; it's a simple vertex list.

	// Link attributes for the sphere VAO
	sphereVAO.LinkAttrib(sphereVBO, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	sphereVAO.LinkAttrib(sphereVBO, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	sphereVAO.LinkAttrib(sphereVBO, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind the sphere VAO
	sphereVAO.Unbind();
	sphereVBO.Unbind();
	sphereEBO.Unbind();

	
	
	


	// Create sphere vertices using the CreateSphereVertices function
	Shader sphereShader2("default.vert", "bronze.frag");
	std::vector<float> sphereVertices2 = CreateSphereVertices(1.0f, 10, 10, 0, 0, 0); // Adjust the parameters as needed
	std::vector<unsigned int> sphereIndices2 = CreateSphereIndices(10, 10);


	// Generate VAO, VBO, and EBO for the sphere
	VAO sphereVAO2;
	sphereVAO2.Bind();

	VBO sphereVBO2(sphereVertices2.data(), sizeof(float)* sphereVertices2.size());
	EBO sphereEBO2(sphereIndices2.data(), sizeof(int)* sphereIndices2.size()); // There are no indices for a sphere; it's a simple vertex list.

	// Link attributes for the sphere VAO
	sphereVAO2.LinkAttrib(sphereVBO2, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	sphereVAO2.LinkAttrib(sphereVBO2, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	sphereVAO2.LinkAttrib(sphereVBO2, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind the sphere VAO
	sphereVAO2.Unbind();
	sphereVBO2.Unbind();
	sphereEBO2.Unbind();


	
	
	



	//lightShader
	Shader lightShader("cube.vert", "cube.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(3.0f);
	lightModel = glm::translate(lightModel, lightPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);


	//testing for sphere
	glm::vec3 spherePos = glm::vec3(15.0f, 6.0f, 0.0f);
	glm::mat4 sphereModel = glm::mat4(1.0f);
	sphereModel = glm::translate(sphereModel, spherePos);

	glm::vec3 sphere2Pos = glm::vec3(14.0f, 8.0f, 0.0f);
	glm::mat4 sphere2Model = glm::mat4(1.0f);
	sphere2Model = glm::translate(sphere2Model, sphere2Pos);


	glm::vec3 dodecaPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 dodecaModel = glm::mat4(1.0);
	dodecaModel = glm::translate(dodecaModel, dodecaPos);

	glm::vec3 icoPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 icoModel = glm::mat4(1.0);
	icoModel = glm::translate(icoModel, icoPos);

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 cubeModel = glm::mat4(1.0);
	cubeModel = glm::translate(cubeModel, cubePos);

	glm::vec3 octoPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 octoModel = glm::mat4(1.0);
	octoModel = glm::translate(octoModel, octoPos);

	glm::vec3 tetraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 tetraModel = glm::mat4(1.0f);
	tetraModel = glm::translate(tetraModel, tetraPos);



	sphereShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(sphereShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(sphereModel));
	glUniform4f(glGetUniformLocation(sphereShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(sphereShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	sphereShader2.Activate();
	glUniformMatrix4fv(glGetUniformLocation(sphereShader2.ID, "model"), 1, GL_FALSE, glm::value_ptr(sphere2Model));
	glUniform4f(glGetUniformLocation(sphereShader2.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(sphereShader2.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	dodecaShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(dodecaShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(dodecaModel));
	glUniform4f(glGetUniformLocation(dodecaShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(dodecaShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	icoShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(icoShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(icoModel));
	glUniform4f(glGetUniformLocation(icoShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(icoShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	octoShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(octoShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(octoModel));
	glUniform4f(glGetUniformLocation(octoShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(octoShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	cubeShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(cubeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
	glUniform4f(glGetUniformLocation(cubeShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(cubeShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(tetraModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(cx, cy, cz));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		if (showSphere && showAllShapes) {
			sphereShader.Activate();
			camera.Matrix(sphereShader, "camMatrix");
			glUniformMatrix4fv(glGetUniformLocation(sphereShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(sphereModel));

			// Bind the sphere's VAO
			sphereVAO.Bind();

			// Draw the sphere
			glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);


			sphereShader2.Activate();
			camera.Matrix(sphereShader2, "camMatrix");
			glUniformMatrix4fv(glGetUniformLocation(sphereShader2.ID, "model"), 1, GL_FALSE, glm::value_ptr(sphere2Model));

			// Bind the sphere's VAO
			sphereVAO2.Bind();

			// Draw the sphere
			glDrawElements(GL_TRIANGLES, sphereIndices2.size(), GL_UNSIGNED_INT, 0);

			
		}

		if (showTetrahedron && showAllShapes)
		{
			// Tell OpenGL which Shader Program we want to use
			shaderProgram.Activate();
			camera.Matrix(shaderProgram, "camMatrix");
			VAO1.Bind();

			// Draw primitives, number of indices, datatype of indices, index of indices
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		if (showCube && showAllShapes)
		{
			//cube
			cubeShader.Activate();
			camera.Matrix(cubeShader, "camMatrix");
			cubeVAO.Bind();
			glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		if (showOctahedron && showAllShapes)
		{
			//octahedron
			octoShader.Activate();
			camera.Matrix(octoShader, "camMatrix");
			octoVAO.Bind();
			glDrawElements(GL_TRIANGLES, sizeof(octoIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		if (showIcosahedron && showAllShapes)
		{
			//icosahedron
			icoShader.Activate();
			camera.Matrix(icoShader, "camMatrix");
			icoVAO.Bind();
			glDrawElements(GL_TRIANGLES, sizeof(icoIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		if (showDodecahedron && showAllShapes)
		{
			//dodecahedron
			dodecaShader.Activate();
			camera.Matrix(dodecaShader, "camMatrix");
			dodecaVAO.Bind();
			glDrawElements(GL_TRIANGLES, sizeof(dodecaIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	cubeVAO.Delete();
	cubeVBO.Delete();
	cubeEBO.Delete();

	octoVAO.Delete();
	octoVBO.Delete();
	octoEBO.Delete();

	icoVAO.Delete();
	icoVBO.Delete();
	icoEBO.Delete();

	dodecaVAO.Delete();
	dodecaVBO.Delete();
	dodecaEBO.Delete();

	sphereVAO.Delete();
	sphereVBO.Delete();
	sphereEBO.Delete();

	sphereVAO2.Delete();
	sphereVBO2.Delete();
	sphereEBO2.Delete();

	shaderProgram.Delete();
	cubeShader.Delete();
	octoShader.Delete();
	icoShader.Delete();
	dodecaShader.Delete();
	sphereShader.Delete();
	sphereShader2.Delete();
	
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}