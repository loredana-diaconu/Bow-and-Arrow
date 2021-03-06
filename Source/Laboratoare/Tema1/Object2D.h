#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

#define WHITE glm::vec3(1.f, 1.f, 1.f)

namespace Object2D {

	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateBow(std::string name, float length, glm::vec3 color);
	Mesh* CreateRectangle(std::string name, float length, float width, float height, glm::vec3 color);
	Mesh* CreateArrowTip(std::string name, float length, float height, glm::vec3 color);
	Mesh* CreateBalloonTip(std::string name, float radius, glm::vec3 color);
	Mesh* CreateShuriken(std::string name, float length, glm::vec3 color);
	Mesh* CreateCircle(std::string name, float radius, glm::vec3 color, glm::vec3 centerColor);

}

