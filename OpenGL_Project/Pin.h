#ifndef PIN_H
#define PIN_H

#include <GL/glut.h>
#include "GameObject.h"

class Pin : public GameObject {
private:
    float x, z;
    bool down;
    bool counted;

public:
    Pin(float x, float z) : x(x), z(z), down(false), counted(false) {}

    void draw() override {
        if (down) return;

        glPushMatrix();
        glTranslatef(x, 20, z);
        glColor3f(1, 1, 1);
        glutSolidCone(10, 40, 10, 10);
        glPopMatrix();
    }


    float getX() const { return x; }
    float getZ() const { return z; }
    bool isDown() const { return down; }
    bool isCounted() const { return counted; }

    void knockDown() { down = true; }

    void markCounted() { counted = true; }

    void reset() {
        down = false;
        counted = false;
    }
};

#endif // PIN_H