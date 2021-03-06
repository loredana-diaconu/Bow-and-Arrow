#include "Tema1.h"

#include <vector>
#include <iostream>
#include <time.h>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Tema1::Tema1() {
}

Tema1::~Tema1() {
}

void Tema1::Init() {
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	srand(time(NULL));

	Mesh* bow = Object2D::CreateBow("bow", BOW_RADIUS, BROWN);
	AddMeshToList(bow);
	Mesh* shuriken = Object2D::CreateShuriken("shuriken", SHURIKEN_SIZE, GREY);
	AddMeshToList(shuriken);
	Mesh* redBalloon = Object2D::CreateCircle("redBalloon", BALLOON_RADIUS, RED, WHITE);
	AddMeshToList(redBalloon);
	Mesh* redBalloonTip = Object2D::CreateBalloonTip("redBalloonTip", BALLOON_RADIUS, RED);
	AddMeshToList(redBalloonTip);
	Mesh* yellowBalloon = Object2D::CreateCircle("yellowBalloon", BALLOON_RADIUS, YELLOW, WHITE);
	AddMeshToList(yellowBalloon);
	Mesh* yellowBalloonTip = Object2D::CreateBalloonTip("yellowBalloonTip", BALLOON_RADIUS, YELLOW);
	AddMeshToList(yellowBalloonTip);
	Mesh* arrow = Object2D::CreateRectangle("arrow", ARROW_LENGTH, ARROW_WIDTH, ARROW_HEIGHT, BROWN);
	AddMeshToList(arrow);
	Mesh* arrowTip = Object2D::CreateArrowTip("arrowTip", ARROW_LENGTH, ARROW_HEIGHT, GREY);
	AddMeshToList(arrowTip);
	Mesh* life = Object2D::CreateCircle("life", LIFE_RADIUS, RED, RED);
	AddMeshToList(life);
	Mesh* speed = Object2D::CreateRectangle("speed", ARROW_LENGTH, ARROW_WIDTH, ARROW_HEIGHT, GREEN);
	AddMeshToList(speed);

	cout << "LIVES        SCORE\n";
	cout << "---------------------\n";
}

void Tema1::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0.15f, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

// Deseneaza bulinele indicatoare de viata
void Tema1::drawLives() {
	for (int i = 0; i < lives; i++) {
		glm::ivec2 resolution = window->GetResolution();
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(LIFE_RADIUS * (2 * i + 1), resolution.y - LIFE_RADIUS);
		RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
	}
}

// Deseneaza arcul
void Tema1::drawBow() {
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, translateBowY);
	modelMatrix *= Transform2D::Rotate(3 * M_PI / 2);
	glLineWidth(5);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
	glLineWidth(1);
}

// Deseneaza bara ce indica viteza sagetii
void Tema1::drawSpeedBar() {
	modelMatrix = Transform2D::Translate(translateArrowX, translateArrowY - ARROW_LENGTH / 16 - BOW_RADIUS);
	modelMatrix *= Transform2D::Scale(speedScale, 1);
	RenderMesh2D(meshes["speed"], shaders["VertexColor"], modelMatrix);
}

// Deseneaza sagetile
void Tema1::drawArrows(float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();

	// Sageata principala
	modelMatrix = glm::mat3(1);
	modelMatrix = Transform2D::Translate(translateArrowX, translateArrowY);
	modelMatrix *= Transform2D::Translate(translateArrowX, BOW_RADIUS);
	modelMatrix *= Transform2D::Rotate(arrowAngle);
	modelMatrix *= Transform2D::Translate(translateArrowX, -BOW_RADIUS);
	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
	RenderMesh2D(meshes["arrowTip"], shaders["VertexColor"], modelMatrix);

	// Deseneaza sagetile din aer
	for (int i = 0; i < arrows.size(); i++) {
		arrows[i].tipX += arrows[i].initialTipX * deltaTimeSeconds * arrows[i].speed;
		arrows[i].tipY += arrows[i].initialTipY * deltaTimeSeconds * arrows[i].speed;
		modelMatrix = glm::mat3(1);
		modelMatrix = Transform2D::Translate(0, arrows[i].lastTranslate);
		modelMatrix *= Transform2D::Translate(arrows[i].tipX, arrows[i].tipY);
		modelMatrix *= Transform2D::Translate(translateArrowX, BOW_RADIUS);
		modelMatrix *= Transform2D::Rotate(arrows[i].angle);
		modelMatrix *= Transform2D::Translate(translateArrowX, -BOW_RADIUS);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["arrowTip"], shaders["VertexColor"], modelMatrix);
		// Sterge sagetile cand ies din cadru
		if (arrows[i].tipX >= resolution.x || arrows[i].tipY >= resolution.y) {
			arrows.erase(arrows.begin() + i);
			i--;
		}
	}
}

// Deseneaza shurikenurile
void Tema1::drawShuriken(float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	for (int i = 0; i < shurikens.size(); i++) {
		float centerX = SHURIKEN_SIZE / 2;
		float centerY = SHURIKEN_SIZE / 2;
		shurikens[i].angle -= deltaTimeSeconds * 5;
		shurikens[i].translateX -= deltaTimeSeconds * 400;
		modelMatrix = glm::mat3(1);
		modelMatrix = Transform2D::Translate(shurikens[i].startX, shurikens[i].startY);
		modelMatrix *= Transform2D::Translate(shurikens[i].translateX, shurikens[i].translateY);
		modelMatrix *= Transform2D::Translate(centerX, centerY);
		modelMatrix *= Transform2D::Rotate(shurikens[i].angle);
		modelMatrix *= Transform2D::Translate(-centerX, -centerY);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
		// Sterge shurikenurile cand ies din cadru sau cand sunt doborate
		if (shurikens[i].startX + shurikens[i].translateX + SHURIKEN_SIZE<= 0 || shurikens[i].dead) {
			shurikens.erase(shurikens.begin() + i);
			i--;
		}
	}
}

// Deseneaza baloanele
void Tema1::drawBalloons(float deltaTimeSeconds) {
	glm::ivec2 resolution = window->GetResolution();
	for (int i = 0; i < balloons.size(); i++) {
		balloons[i].translateY += BALLOON_SPEED * deltaTimeSeconds;
		modelMatrix = glm::mat3(1);
		modelMatrix = Transform2D::Translate(balloons[i].startX, balloons[i].startY);

		if (balloons[i].pop) {
			// Daca balonul a fost intepat, se dezumfla
			balloons[i].scale -= deltaTimeSeconds;
			modelMatrix *= Transform2D::Scale(balloons[i].scale, balloons[i].scale);
		}
		modelMatrix *= Transform2D::Translate(balloons[i].translateX, balloons[i].translateY);
		modelMatrix *= Transform2D::Scale(0.75f, 1.f);

		if (balloons[i].color == RED) {
			RenderMesh2D(meshes["redBalloon"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["redBalloonTip"], shaders["VertexColor"], modelMatrix);
		} else {
			RenderMesh2D(meshes["yellowBalloon"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["yellowBalloonTip"], shaders["VertexColor"], modelMatrix);
		}
		// Daca balonul e dezumflat sau iese in cadru, sterge
		if (balloons[i].translateY >= resolution.y + BALLOON_RADIUS * 2 || balloons[i].scale <= 0.1 * deltaTimeSeconds) {
			balloons.erase(balloons.begin() + i);
			i--;
		}
	}
}

// Genereaza un balon random
void Tema1::generateBalloon() {
	glm::ivec2 resolution = window->GetResolution();
	// Exista o sansa de 2% sa se genereze un balon la un moment dat
	int chance = rand() % 100 + 1;
	if (chance <= 2) {
		glm::vec3 color;
		float posX = resolution.x / 2 + (rand() % (resolution.x / 2 - BALLOON_RADIUS + 1));
		float posY = -BALLOON_RADIUS;
		// Exista o sansa de 75% ca balonul generat sa fie rosu (25% galben)
		int colorChance = rand() % 100 + 1;
		if (colorChance <= 75) {
			color = RED;
		} else {
			color = YELLOW;
		}
		Balloon b(color, posX, posY);
		balloons.push_back(b);
	}
}

// Genereaza shuriken random
void Tema1::generateShuriken() {
	glm::ivec2 resolution = window->GetResolution();
	// Exista sansa de 0.8% sa se genereze un shuriken la un moment dat
	int chance = rand() % 1000 + 1;
	if (chance <= 8) {
		float posX = resolution.x + SHURIKEN_SIZE;
		float posY = rand() % (resolution.y - SHURIKEN_SIZE + 1);
		Shuriken s(posX, posY);
		shurikens.push_back(s);
	}
}

// Gaseste coliziunile dintre sageata si tinte
// Coliziunea este de tip punct-cerc
void Tema1::checkArrowTargetCollisions() {
	for (int i = 0; i < arrows.size(); i++) {
		// Pentru fiecare sageata, gaseste varful
		float alpha = arrows[i].angle;
		float ax = arrows[i].tipX + ARROW_LENGTH * cos(alpha); //varf
		float ay = arrows[i].lastTranslate + arrows[i].tipY + BOW_RADIUS + ARROW_LENGTH * sin(alpha);

		// Coliziunea cu baloanele
		for (int j = 0; j < balloons.size(); j++) {
			// Gaseste centrul cercului circumscrus balonului
			float bx = balloons[j].startX; 
			float by = balloons[j].translateY - BALLOON_RADIUS;      // centru balon
			float r = BALLOON_RADIUS;
			float distance = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
			// Daca varful sagetii se afla in balon
			if (r >= distance && !balloons[j].pop) {
				balloons[j].pop = true;
				score += balloons[j].score;
				// Daca scorul ajunge sub 0, pierde o viata;
				if (score < 0) {
					lives--;
					score = 0;
				}
				printScore();
			}
		}

		// Asemanator pentru coliziunea cu shuriken
		for (int j = 0; j < shurikens.size(); j++) {
			float rs = SHURIKEN_SIZE / 2;
			float sx = shurikens[j].startX + shurikens[j].translateX + rs;
			float sy = shurikens[j].startY + rs;
			float distance = sqrt((ax - sx) * (ax - sx) + (ay - sy) * (ay - sy));
			if (rs >= distance && !shurikens[j].dead) {
				shurikens[j].dead = true;
				score += shurikens[j].score;
				printScore();
			}
		}
	}
}

// Gaseste coliziunile dintre shuriken si arc (personaj)
// Coliziunea este de tip cerc-cerc
void Tema1::checkBowShurikenCollisions() {
	// Calculeaza centrul cercului circumscris arcului
	// Sageta nu este luata in calcul
	float bx = 0;
	float by = translateBowY + BOW_RADIUS;
	float rb = BOW_RADIUS;

	for (int i = 0; i < shurikens.size(); i++) {
		// Calculeaza centrul cercului circumscris shurikenului
		float rs = SHURIKEN_SIZE / 2;
		float sx = shurikens[i].startX + shurikens[i].translateX + rs;
		float sy = shurikens[i].startY + rs;
		// Daca cercurile se intersecteaza, jucatorul pierde o viata
		// Shurikenul este sters
		float distance = sqrt((bx - sx) * (bx - sx) + (by - sy) * (by - sy));
		if (rb + rs >= distance) {
			lives--;
			shurikens.erase(shurikens.begin() + i);
			i--;
			printScore();
		}
	}

}

// Afiseaza scorul jucatorului.
void Tema1::printScore() {
	for (int j = 0; j < lives; j++) {
		cout << "*";
	}
	for (int j = 0; j < 13 - lives; j++) {
		cout << " ";
	}
	cout << score << endl;
}

void Tema1::Update(float deltaTimeSeconds) {
	if (gameOver) {
		// Se schimba culoarea ecranului
		glClearColor(0.2f, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	drawLives();
	drawBow();
	drawSpeedBar();

	if (!gameOver) {
		generateBalloon();
	}
	drawBalloons(deltaTimeSeconds);

	if (!gameOver) {
		generateShuriken();
	}
	drawShuriken(deltaTimeSeconds);
	drawArrows(deltaTimeSeconds);
	checkArrowTargetCollisions();
	checkBowShurikenCollisions();

	if (!gameOver && lives <= 0) {
		shurikens.clear();
		balloons.clear();
		cout << "\n\n======= GAME OVER ========\n\n";
		gameOver = true;
	}
}

void Tema1::FrameEnd() {
}

// Calculeaza unghiul sub care se afla sageata astfel incat
// aceasta sa urmeze directia cursorului
void Tema1::computeArrowAngle() {
	glm::ivec2 resolution = window->GetResolution();
	glm::ivec2 cursorPos = window->GetCursorPosition();
	float mouseX = cursorPos.x;
	float mouseY = resolution.y - cursorPos.y;
	float c = mouseY - (translateArrowY + BOW_RADIUS);
	float i = sqrt(mouseX * mouseX + c * c);
	arrowAngle = asin(c / i);
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {
	glm::ivec2 resolution = window->GetResolution();

	// Misca arcul si sageata in sus si in jos folosind WS
	if (window->KeyHold(GLFW_KEY_W)) {
		if (translateBowY + BOW_RADIUS * 2 <= resolution.y) {
			translateBowY += BOW_SPEED * deltaTime;
			translateArrowY += BOW_SPEED * deltaTime;
		}
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		if (translateBowY >= SPEED_BAR_WIDTH * 3) {
			translateBowY -= BOW_SPEED * deltaTime;
			translateArrowY -= BOW_SPEED * deltaTime;
		}
	}

	computeArrowAngle();

	// Jucatorul poate sa traga cu sageata doar daca bara care
	// indica viteza revine la 0
	if (speedScale <= 0) {
		ableToShoot = true;
	}

	// Viteza sageata creste cat timp jucatorul tine apasat pe mouse
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (arrowSpeed <= 20 && speedScale <= 1) {
			arrowSpeed += 0.2;
			speedScale += deltaTime;
		}
	} else {
		// altfel, viteza scade
		arrowSpeed = 1;
		if (speedScale > 0) {
			speedScale -= 1 * deltaTime;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods) {
}

void Tema1::OnKeyRelease(int key, int mods) {
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
}

// Lanseaza o sageata
void Tema1::shootArrow() {
	// Calculeaza pozitia din care trebuie lansata sageata
	float height = BOW_RADIUS;
	float tY = sin(arrowAngle) * ARROW_LENGTH;
	float tX = cos(arrowAngle) * ARROW_LENGTH;
	// Creeaza o sageata noua si o adauga in lista de sageti vizibile
	Arrow a(arrowAngle, translateArrowY, 0, 0);
	a.initialTipX = tX;
	a.initialTipY = tY;
	a.speed = arrowSpeed;
	arrows.push_back(a);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		// Cand jucatorul elibereaza butonul mouseului, lanseaza o sageata
		if (ableToShoot) {
			shootArrow();
		}
		// Imediat dupa ce a fost lansata o sageata, jucatorul trebuie sa astepte 
		// pana ce bara de viteza ajunge la 0
		ableToShoot = false;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Tema1::OnWindowResize(int width, int height) {
}
