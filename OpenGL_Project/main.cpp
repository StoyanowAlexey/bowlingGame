#include <GL/freeglut.h>
#include <string>
#include <ctime>
#include <cstdlib>

#include "Game.h"

// глобальний об'єкт гри
Game game;

// локальні (це нормально залишити тут)
float scale = 1.0f;
float rotateX = 20.0f;
float rotateY = 0.0f;

// --- TEXT ---
void drawText(float x, float y, const std::string& text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 700);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// --- DISPLAY ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // PROJECTION
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1200.0 / 700.0, 1.0, 3000.0);

    // MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!game.isStarted()) {
        drawText(500, 350, "PRESS ANY KEY TO START");
        glutSwapBuffers();
        return;
    }

    // камера
    static float camX = 450;
    camX += (game.getBall().getX() - camX) * 0.05f;

    gluLookAt(camX, 300, 400,
              game.getBall().getX(), 0, -800,
              0, 1, 0);

    // трансформації
    glScalef(scale, scale, scale);
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);

    // доріжка
    glColor3f(0.4f, 0.25f, 0.1f);
    glBegin(GL_QUADS);
        glVertex3f(200, 0, 200);
        glVertex3f(700, 0, 200);
        glVertex3f(700, 0, -1200);
        glVertex3f(200, 0, -1200);
    glEnd();

    // 🔥 тепер усе малює Game
    game.draw();

    // HUD
    glColor3f(1, 1, 1);

    if (game.isGameOver()) {
        drawText(500, 400, "GAME OVER! SCORE: " + std::to_string(game.getScore()));
        drawText(500, 370, "PRESS 'R' TO RESTART");
    } else {
        drawText(50, 650, "Round: " + std::to_string(game.getRound()));
        drawText(50, 620, "Attempt: " + std::to_string(game.getAttempt()));
        drawText(50, 590, "Score: " + std::to_string(game.getScore()));
    }

    glutSwapBuffers();
}

// --- TIMER ---
void timer(int v) {
    game.update();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// --- KEYBOARD ---
void keyboard(unsigned char key, int x, int y) {

    if (!game.isStarted()) {
        game.start();
        return;
    }

    if (key == '+') scale += 0.1f;
    if (key == '-') scale -= 0.1f;
    if (scale < 0.2f) scale = 0.2f;

    if (key == 'r' || key == 'R')
        game.init();

    if (key == 27)
        exit(0);
}

// --- SPECIAL KEYS ---
void special(int key, int x, int y) {
    float step = 5.0f;

    if (key == GLUT_KEY_LEFT)  rotateY -= step;
    if (key == GLUT_KEY_RIGHT) rotateY += step;
    if (key == GLUT_KEY_UP)    rotateX -= step;
    if (key == GLUT_KEY_DOWN)  rotateX += step;
}

// --- MOUSE MOVE ---
void mousePassive(int x, int y) {
    if (!game.isStarted()) return;

    Ball& ball = game.getBall();

    if (!ball.isShooting()) {
        float newX = 200.0f + ((float)x / 1200.0f) * 500.0f;
        ball.setX(newX);
    }
}

// --- MOUSE CLICK ---
void mouseClick(int button, int state, int x, int y) {
    if (!game.isStarted()) return;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Ball& ball = game.getBall();

        if (!ball.isShooting() && !game.isGameOver()) {
            ball.shoot();
        }
    }
}

// --- MAIN ---
int main(int argc, char** argv) {
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 700);
    glutCreateWindow("Bowling OOP");

    glEnable(GL_DEPTH_TEST);

    game.init();

    glutDisplayFunc(display);
    glutTimerFunc(16, timer, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutPassiveMotionFunc(mousePassive);
    glutMouseFunc(mouseClick);

    glutMainLoop();
    return 0;
}