#define _CRT_SECURE_NO_DEPRECATE
#pragma once
#include <windows.h> // Header File For Windows
#include <gl/gl.h>   // Header File For The OpenGL32 Library
#include <gl/glut.h> // Header File For The GLu32 Library
#include <gl/glu.h>
#include <cmath>

void drawSphere(double r, int lats, int longs) {
    // The parameters lat defines how many horizontal lines you want to have in
    // your sphere and lon how many vertical lines.
    // r is the radius of your sphere.
    float M_PI = 3.14;
    int i, j;
    for (i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
}

void drawCube() {
	glBegin(GL_QUADS);

	glColor3f(0.0f, 1.0f, 0.0f); // Green

	// Top face
	glNormal3f(0, 1, 0);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(1.0f, 0.0f, 0.0f); // Red

	// Bottom face
	glNormal3f(0, -1, 0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f); // Blue

	// Front face
	glNormal3f(0, 0, -1);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 0.0f, 0.0f); // Red

	// Back face
	glNormal3f(0, 0, 1);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor3f(0.0f, 1.0f, 0.0f); // Green

	// Left face
	glNormal3f(-1, 0, 0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor3f(0.0f, 0.0f, 1.0f); // Blue

	// Right face
	glNormal3f(1, 0, 0);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();
}

void drawCircle(GLdouble centerX, GLdouble centerY, GLdouble radius, GLdouble r, GLdouble g, GLdouble b) {
	glBegin(GL_LINES);
	glColor3f(r, g, b);
	for (GLdouble angle = 0.0; angle <= 2 * (3.14); angle += 0.001) {
		GLdouble x = radius * cos(angle);
		GLdouble y = radius * sin(angle);
		glVertex3d(centerX, centerY, 0);
		glVertex3d(x, y, 0);
	}
	glEnd();
}

void drawRectangle(GLdouble centerX, GLdouble centerY, GLdouble height, GLdouble width, GLdouble r, GLdouble g, GLdouble b) {
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex3d(centerX + (width / 2), centerY + (height / 2), 0);
	glVertex3d(centerX + (width / 2), centerY - (height / 2), 0);
	glVertex3d(centerX - (width / 2), centerY - (height / 2), 0);
	glVertex3d(centerX - (width / 2), centerY + (height / 2), 0);
	glEnd();
}
