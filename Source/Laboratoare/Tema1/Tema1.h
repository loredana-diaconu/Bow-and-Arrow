#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

#define	SHURIKEN_SIZE 50
#define	BOW_RADIUS 70
#define	ARROW_LENGTH 150
#define	BALLOON_RADIUS 50
#define	MAX_LIVES 3
#define	RED_SCORE 1
#define	YELLOW_SCORE -0.5f
#define SHURIKEN_SCORE 2
#define ARROW_HEIGHT BOW_RADIUS
#define ARROW_WIDTH ARROW_LENGTH / 32
#define SPEED_BAR_LENGTH 150
#define SPEED_BAR_HEIGHT
#define LIFE_RADIUS 15
#define SPEED_BAR_WIDTH ARROW_LENGTH / 32
#define BALLOON_SPEED 200
#define BOW_SPEED 350

#define WHITE	glm::vec3(1.f, 1.f, 1.f)
#define GREY	glm::vec3(0.3f, 0.3f, 0.3f)
#define RED		glm::vec3(1.f, 0.f, 0.f)
#define GREEN	glm::vec3(0.f, 1.f, 0.f)
#define YELLOW	glm::vec3(1.f, 1.f, 0.f)
#define BROWN	glm::vec3(0.7f, 0.6f, 0.4f)

class Arrow {
public:
	float angle;
	float tipX;
	float tipY;
	float initialTipX;
	float initialTipY;
	float lastTranslate;
	float speed;

	Arrow(float angle, float lastTranslate, float tipX, float tipY) {
		this->angle = angle;
		this->lastTranslate = lastTranslate;
		this->tipX = tipX;
		this->tipY = tipY;
		lastTranslate = 0;
		initialTipX = 0;
		initialTipY = 0;
		speed = 1;
	}

	~Arrow() {}
};

class Balloon {
public:
	glm::vec3 color;
	float translateX;
	float translateY;
	float startX;
	float startY;
	bool pop;
	float score;
	float scale;

	Balloon(glm::vec3 color, float startX, float startY) {
		this->color = color;
		this->startX = startX;
		this->startY = startY;
		translateX = 0;
		translateY = 0;
		pop = false;
		scale = 1;
		if (color == RED) {
			score = RED_SCORE;
		}
		else {
			score = YELLOW_SCORE;
		}
	}

	~Balloon() {}
};

class Shuriken {
public:
	float translateX;
	float translateY;
	float startX;
	float startY;
	float angle;
	bool dead;
	float score;
	float scale;

	Shuriken(float startX, float startY) {
		this->startX = startX;
		this->startY = startY;
		translateX = 0;
		translateY = 0;
		angle = 0;
		scale = 1;
		dead = false;
		score = SHURIKEN_SCORE;
	}

	~Shuriken() {}
};

class Tema1 : public SimpleScene {
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void drawBow();
	void drawSpeedBar();
	void drawArrows(float deltaTimeSeconds);
	void drawShuriken(float deltaTimeSeconds);
	void drawBalloons(float deltaTimeSeconds);
	void computeArrowAngle();
	void generateBalloon();
	void generateShuriken();
	void checkArrowTargetCollisions();
	void checkBowShurikenCollisions();
	void printScore();
	void Tema1::shootArrow();
	void drawLives();

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;

	float translateBowY = 0;
	float translateArrowX = 0;
	float translateArrowY = 0;
	float arrowAngle = 0;
	float shurikenAngle = 0;
	float balloonScale = 1;
	int lives = MAX_LIVES;
	float score = 0;
	bool gameOver = false;
	float arrowSpeed = 1;
	float speedScale = 0;
	bool ableToShoot = true;

	std::vector<Arrow> arrows;
	std::vector<Balloon> balloons;
	std::vector<Shuriken> shurikens;
};

