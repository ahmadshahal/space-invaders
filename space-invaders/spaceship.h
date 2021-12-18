#define _CRT_SECURE_NO_DEPRECATE
#pragma once
#include <windows.h> // Header File For Windows
#include <gl/gl.h>   // Header File For The OpenGL32 Library
#include <gl/glut.h> // Header File For The GLu32 Library
#include <gl/glu.h>
#include <vector>
#include <set>
#include <time.h>
#include <3DTexture.h>
#include <Model_3DS.h>
#include "spaceshot.h"
#include "utility.h"

using namespace std;

class SpaceShip {
public:
	GLdouble x, y;
	GLdouble height, width;
	set<SpaceShot*> shots;
	int type; 
	Model_3DS shipModel;
	GLint health;
	int shotStrength;
	time_t shootingTime;
	GLdouble shootEachMilliSecond;

	SpaceShip(GLdouble x, GLdouble y, GLint health, int shotStrength, int type, GLdouble shootEachMilliSecond = 2000) {
		this->x = x;
		this->y = y;
		this->height = 0.05;
		this->width = 0.4;
		this->health = health;
		this->shootingTime = time(0); // Time which the object is created at.
		this->shootEachMilliSecond = shootEachMilliSecond;
		this->shipModel = Model_3DS();
		this->type = type;
		this->shotStrength = shotStrength;
	}

	virtual void shoot() {
		if ((time(0) * 1000) - (shootingTime * 1000) == shootEachMilliSecond) {
			shootingTime = time(0);
			shots.insert(new SpaceShot(x, y, this->shotStrength, 1, 1, 1));
		}
	}
	virtual void draw() {
		glPushMatrix();
		glTranslated(x, y, 0);
		drawRectangle(0, 0, height, width, 1, 1, 1);
		glPopMatrix();
	}
};


class DeltaSpaceShip : public SpaceShip {
public:
	DeltaSpaceShip(GLdouble x, GLdouble y) : SpaceShip(x, y, 3, 1, 1) {
		this->shipModel.Load("C:/Users/DIGI TECH/Documents/Visual Studio 2012/Projects/space-invaders/space_ship.3ds");
	}
	void shoot() {
		// Shoots manually
		shots.insert(new SpaceShot(x, y, this->shotStrength, 0.822, 0.826, 0.829));
	}
	void draw() {
		glPushMatrix();
		glColor3d(0.822, 0.826, 0.829);
		glTranslated(x, y, 0);
		shipModel.pos.x = 0;
		shipModel.pos.y = 0;
		shipModel.pos.z = 0;
		shipModel.scale = 0.06;
		shipModel.Draw();
		// drawRectangle(0, 0, height, width, 1, 1, 1);
		glPopMatrix();
	}
};

class AlphaSpaceShip : public SpaceShip {
public:
	AlphaSpaceShip(GLdouble x, GLdouble y) : SpaceShip(x, y, 1, 1, 2, 2000) {
		shots.insert(new SpaceShot(x, y, this->shotStrength, 0.828, 0.061, 0.088));
		this->shipModel.Load("C:/Users/DIGI TECH/Documents/Visual Studio 2012/Projects/space-invaders/space_ship.3ds");
	}
	void shoot() {
		if ((time(0) * 1000) - (shootingTime * 1000) == shootEachMilliSecond) {
			shootingTime = time(0);
			shots.insert(new SpaceShot(x, y, this->shotStrength, 0.828, 0.061, 0.088));
		}
	}
	void draw() {
		glPushMatrix();
		glColor3d(0.828, 0.061, 0.088);
		glTranslated(x, y, 0);
		glRotated(180, 0, 0, 1);
		shipModel.pos.x = 0;
		shipModel.pos.y = 0;
		shipModel.pos.z = 0;
		shipModel.scale = 0.06;
		shipModel.Draw();
		// drawRectangle(0, 0, height, width, 1, 0, 0);
		glPopMatrix();
	}
};

class BetaSpaceShip : public SpaceShip {
public:
	BetaSpaceShip(GLdouble x, GLdouble y) : SpaceShip(x, y, 2, 2, 3, 2000) {
		shots.insert(new SpaceShot(x, y, this->shotStrength, 0.097, 0.162, 0.831));
		this->shipModel.Load("C:/Users/DIGI TECH/Documents/Visual Studio 2012/Projects/space-invaders/space_ship.3ds");
	}
	void shoot() {
		if ((time(0) * 1000) - (shootingTime * 1000) == shootEachMilliSecond) {
			shootingTime = time(0);
			shots.insert(new SpaceShot(x, y, this->shotStrength, 0.097, 0.162, 0.831));
		}
	}
	void draw() {
		glPushMatrix();
		glColor3d(0.097, 0.162, 0.831);
		glTranslated(x, y, 0);
		glRotated(180, 0, 0, 1);
		shipModel.pos.x = 0;
		shipModel.pos.y = 0;
		shipModel.pos.z = 0;
		shipModel.scale = 0.06;
		shipModel.Draw();
		// drawRectangle(0, 0, height, width, 0, 0, 1);
		glPopMatrix();
	}
};

class GamaSpaceShip : public SpaceShip {
public:
	GamaSpaceShip(GLdouble x, GLdouble y) : SpaceShip(x, y, 3, 3, 4, 1000) {
		shots.insert(new SpaceShot(x, y, this->shotStrength, 0.031, 0.758, 0.103));
		this->shipModel.Load("C:/Users/DIGI TECH/Documents/Visual Studio 2012/Projects/space-invaders/space_ship.3ds");
	}
	void shoot() {
		if ((time(0) * 1000) - (shootingTime * 1000) == shootEachMilliSecond) {
			shootingTime = time(0);
			shots.insert(new SpaceShot(x, y, this->shotStrength, 0.031, 0.758, 0.103));
		}
	}
	void draw() {
		glPushMatrix();
		glColor3d(0.031, 0.758, 0.103);
		glTranslated(x, y, 0);
		glRotated(180, 0, 0, 1);
		shipModel.pos.x = 0;
		shipModel.pos.y = 0;
		shipModel.pos.z = 0;
		shipModel.scale = 0.06;
		shipModel.Draw();
		// drawRectangle(0, 0, height, width, 0, 1, 0);
		glPopMatrix();
	}
};