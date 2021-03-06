#include "Object2D.h"

#include <Core/Engine.h>

#include <iostream>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	} else {
		indices.push_back(0);
		indices.push_back(2);
	}
	square->InitFromData(vertices, indices);
	return square;
}

// Creeaza arcul
Mesh* Object2D::CreateBow(std::string name, float radius, glm::vec3 color) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	for (float angle = 0.0f; angle <= 180; angle += 0.005f) {
		float rad = M_PI * angle / 180;
		int x = radius * cos(rad);
		int y = radius * sin(rad);
		vertices.push_back(VertexFormat(glm::vec3(x - radius, y, 0), color));
	}

	for (unsigned short i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* bow = new Mesh(name);
	bow->SetDrawMode(GL_LINE_LOOP);
	bow->InitFromData(vertices, indices);
	return bow;
}

// Creeaza un shuriken
Mesh* Object2D::CreateShuriken(std::string name, float length, glm::vec3 color) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(length / 2, length / 2, 0), color),
		VertexFormat(glm::vec3(0,          length,	   0), WHITE),
		VertexFormat(glm::vec3(length / 2, length,	   0), color),
		VertexFormat(glm::vec3(length,	   length,	   0), WHITE),
		VertexFormat(glm::vec3(length,	   length / 2, 0), color),
		VertexFormat(glm::vec3(length,	   0,	       0), WHITE),
		VertexFormat(glm::vec3(length / 2, 0,          0), color),
		VertexFormat(glm::vec3(0,		   0,	       0), WHITE),
		VertexFormat(glm::vec3(0,		   length / 2, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 2, 1, 
											0, 4, 3,
											0, 6, 5,
											0, 8, 7 };

	Mesh* shuriken = new Mesh(name);
	shuriken->SetDrawMode(GL_TRIANGLES);
	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

// Creeaza motul balonului
Mesh* Object2D::CreateBalloonTip(std::string name, float radius, glm::vec3 color) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, -radius, 0), color),
		VertexFormat(glm::vec3(-radius / 4, -radius - radius / 4, 0), color),
		VertexFormat(glm::vec3(radius / 4, -radius - radius / 4, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2 };

	Mesh* balloonTip = new Mesh(name);
	balloonTip->SetDrawMode(GL_TRIANGLES);
	balloonTip->InitFromData(vertices, indices);
	return balloonTip;
}

// Creeaza un dreptunghi
Mesh* Object2D::CreateRectangle(std::string name, float length, float width, float height, glm::vec3 color) {
	glm::vec3 corner = glm::vec3(0, height, 0);

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner,                               color),
		VertexFormat(corner + glm::vec3(length, 0, 0),     color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0),      color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	Mesh* rectangle = new Mesh(name);
	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

// Creeaza varful sagetii
Mesh* Object2D::CreateArrowTip(std::string name, float length, float height, glm::vec3 color) {
	glm::vec3 corner = glm::vec3(0, height, 0);
	float width = length / 32;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner + glm::vec3(length, -width * 2 + width / 2, 0), color),
		VertexFormat(corner + glm::vec3(length + 6 * width, 0, 0), glm::vec3(1.f, 1.f, 1.f)),
		VertexFormat(corner + glm::vec3(length, width * 2 + width / 2, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2 };

	Mesh* arrowTip = new Mesh(name);
	arrowTip->SetDrawMode(GL_TRIANGLES);
	arrowTip->InitFromData(vertices, indices);
	return arrowTip;
}

// Creeaza un cerc. 
// Utilizata la reprezentarea baloanelor si bulinelor indicatoare de viata.
Mesh* Object2D::CreateCircle(std::string name, float radius, glm::vec3 color, glm::vec3 centerColor) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	glm::vec3 center(0.f, 0.f, 0.f);

	vertices.push_back(VertexFormat(center, centerColor));
	for (float angle = 0.0f; angle < 360; angle += 0.5) {
		float rad = M_PI * angle / 180;
		int x = radius * cos(rad);
		int y = radius * sin(rad);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}

	for (unsigned short i = 1; i < vertices.size() - 1; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices.push_back(0);
	indices.push_back(vertices.size() - 1);
	indices.push_back(1);

	Mesh* circle = new Mesh(name);
	circle->SetDrawMode(GL_TRIANGLES);
	circle->InitFromData(vertices, indices);
	return circle;
}