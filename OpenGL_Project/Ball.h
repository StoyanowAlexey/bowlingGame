#pragma once
#include <GL/glut.h>

class Ball {
public:
    float x, y, z;
    float speed;
    bool active;

    Ball() {
        reset();
    }

    void reset() {
        x = 0; y = 0; z = 0;
        speed = 0;
        active = false;
    }

    void move() {
        if (active) {
            z -= speed;
        }
    }

    void draw() {
        GLUquadric* q = gluNewQuadric();
        glPushMatrix();
        glTranslatef(x, y, z);
        gluSphere(q, 15, 30, 30);
        glPopMatrix();
    }
};