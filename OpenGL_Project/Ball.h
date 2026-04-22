#ifndef BALL_H
#define BALL_H

#include <GL/glut.h>
#include <cmath>

class Ball {
private:
    float x;
    float z;
    float velX;
    float velZ;
    bool shooting;

public:
    Ball() {
        reset();
    }

    void update() {
        if (!shooting) return;

        x += velX;
        z += velZ;

        velZ *= 0.995f;
        velX *= 0.99f;

        if (fabs(velZ) < 0.1f) velZ = 0;
        if (fabs(velX) < 0.05f) velX = 0;
    }

    void draw() {
        glPushMatrix();
        glTranslatef(x, 20, z);
        glColor3f(0.1f, 0.6f, 0.1f);
        glutSolidSphere(18, 32, 32);
        glPopMatrix();
    }

    void shoot() {
        if (shooting) return;

        shooting = true;
        velZ = -15.0f;
        velX = (x - 450.0f) * 0.05f;
    }

    void reset() {
        x = 450.0f;
        z = 0.0f;
        velX = 0.0f;
        velZ = -15.0f;
        shooting = false;
    }

    // 🔥 ГЕТТЕРИ
    float getX() const { return x; }
    float getZ() const { return z; }

    // 🔥 СЕТТЕР (контрольований)
    void setX(float newX) {
        if (!shooting) // щоб не рухати кулю під час кидка
            x = newX;
    }

    // 🔥 СТАН
    bool isShooting() const { return shooting; }

    // 🔥 доступ до швидкості (якщо треба для фізики)
    float getVelX() const { return velX; }
    float getVelZ() const { return velZ; }

    void setVelocity(float vx, float vz) {
        velX = vx;
        velZ = vz;
    }
};

#endif // BALL_H