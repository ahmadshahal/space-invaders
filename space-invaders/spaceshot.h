#define _CRT_SECURE_NO_DEPRECATE
#pragma once
#include <windows.h> // Header File For Windows
#include <gl/gl.h>   // Header File For The OpenGL32 Library
#include <gl/glut.h> // Header File For The GLu32 Library
#include <gl/glu.h>
#include <cmath>
#include "utility.h"

using namespace std;

class SpaceShot {
public:
	GLdouble x, y;
	GLdouble strength;
	GLdouble r, g, b;
	SpaceShot(GLdouble x, GLdouble y, GLdouble strength, GLdouble r, GLdouble g, GLdouble b) {
		this->x = x;
		this->y = y;
		this->strength = strength;
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void draw() {
		glPushMatrix();
		glTranslated(x, y, 0);
		drawCircle(0, 0, 0.015, r, g, b);
		glPopMatrix();
	}
};
