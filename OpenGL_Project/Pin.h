#pragma once
#include <GL/glut.h>
#include "Ball.h"

class Pin {
public:
    float x, y, z;
    bool knocked;

    Pin(float px, float py, float pz) {
        x = px; y = py; z = pz;
        knocked = false;
    }

    void draw() {
        if (knocked) return;

        GLUquadric* q = gluNewQuadric();
        glPushMatrix();
        glTranslatef(x, y, z);
        gluCylinder(q, 10, 10, 40, 20, 20);
        glPopMatrix();
    }

    void checkHit(Ball& ball) {
        if (knocked) return;

        float dx = ball.x - x;
        float dz = ball.z - z;

        if (dx*dx + dz*dz < 400) {
            knocked = true;
        }
    }
};