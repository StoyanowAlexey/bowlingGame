#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>

// Константи гри
const int TOTAL_ROUNDS = 5;
const int PINS_COUNT = 10;

// Глобальний стан
int currentRound = 1;
int currentAttempt = 1;
int totalScore = 0;
bool isGameOver = false;
float scale = 1.0f;
float rotateX = 20.0f, rotateY = 0.0f;
float ballX = 450.0f, ballZ = 0.0f;
bool isShooting = false;
time_t startTime;
float velX = 0.0f;
float velZ = -15.0f;
int firstThrowPins = 0;
bool gameStarted = false;

struct Pin {
    float x, z;
    bool down;
    bool counted;
};
std::vector<Pin> pins;

void resetPins() {
    pins.clear();
    float spacingX = 32.0f; // Ще ближче
    float spacingZ = 40.0f; // Ще ближче
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) {
            pins.push_back({450.0f + (j * spacingX) - (i * (spacingX / 2.0f)),
                            -750.0f - (i * spacingZ),
                            false,
                            false});
        }
    }
}
void saveStats() {
    std::ofstream file("stats.txt", std::ios::app);
    if (file.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        file << "Date: " << dt << "Final Score: " << totalScore << "\n----------\n";
        file.close();
    }
}

void initGame() {
    currentRound = 1;
    currentAttempt = 1;
    totalScore = 0;
    isGameOver = false;

    gameStarted = false;
    ballX = 450.0f;
    ballZ = 0.0f;
    velX = 0.0f;
    velZ = -15.0f;
    isShooting = false;

    startTime = time(NULL);
    resetPins();
}

void drawText(float x, float y, std::string text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glRasterPos2f(x, y);
    for (char c : text) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 🔥 PROJECTION
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1200.0 / 700.0, 1.0, 3000.0);

    // 🔥 MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!gameStarted)
    {
        drawText(500, 350, "PRESS ANY KEY TO START");
        glutSwapBuffers();
        return;
    }
    static float camX = 450;
    camX += (ballX - camX) * 0.05f;
    gluLookAt(camX, 300, 400, ballX, 0, -800, 0, 1, 0);

    // Масштабування та Обертання
    glScalef(scale, scale, scale);
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);

    // Доріжка
    glColor3f(0.4f, 0.25f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(200, 0, 200); glVertex3f(700, 0, 200);
    glVertex3f(700, 0, -1200); glVertex3f(200, 0, -1200);
    glEnd();

    // Кеглі
    for (auto& p : pins) {
        if (!p.down) {
            glPushMatrix();
            glTranslatef(p.x, 20, p.z);
            glColor3f(1, 1, 1);
            glutSolidCone(10, 40, 10, 10); // Більш схоже на кеглі
            glPopMatrix();
        }
    }

    // Куля
    glPushMatrix();
    glTranslatef(ballX, 20, ballZ);
    glColor3f(0.1f, 0.6f, 0.1f);
    glutSolidSphere(18, 32, 32);
    glPopMatrix();

    // HUD (Інтерфейс)
    glColor3f(1, 1, 1);
    if (isGameOver) {
        drawText(500, 400, "GAME OVER! SCORE: " + std::to_string(totalScore));
        drawText(500, 370, "PRESS 'R' TO RESTART");
    } else {
        drawText(50, 650, "Round: " + std::to_string(currentRound) + "/" + std::to_string(TOTAL_ROUNDS));
        drawText(50, 620, "Attempt: " + std::to_string(currentAttempt));
        drawText(50, 590, "Total Score: " + std::to_string(totalScore));
        drawText(900, 650, "Scale: " + std::to_string(scale));
    }

    glutSwapBuffers();
}

void nextStep() {
    int downedThisTurn = 0;
    for (auto &p : pins)
    {
        if (p.down && !p.counted)
        {
            downedThisTurn++;
            p.counted = true; // щоб вдруге не рахувалась
        }
    }

    // Логіка очок за правилами професійного Strike
    if (currentAttempt == 1)
    {
        if (downedThisTurn == 10)
        {
            int strikeBonus = (rand() % 10) + 1;
            totalScore += 10 + strikeBonus;

            currentRound++;
            resetPins();
        }
        else
        {
            firstThrowPins = downedThisTurn; // 🔥 запам’ятали
            currentAttempt = 2;
        }
    }
    else if (currentAttempt == 2)
    {
        totalScore += firstThrowPins + downedThisTurn; // 🔥 СУМА

        currentRound++;
        currentAttempt = 1;
        resetPins();
    }

    if (currentRound > TOTAL_ROUNDS) {
        isGameOver = true;
        saveStats();
    }
    ballZ = 0;
    isShooting = false;
    velX = 0;
    velZ = -15.0f;
}

// ... (всі глобальні змінні залишаються ті самі)

void timer(int v) {
    if (!gameStarted)
    {
        glutPostRedisplay();
        glutTimerFunc(16, timer, 0);
        return;
    }
    if (isShooting && !isGameOver) {
        ballX += velX;
        ballZ += velZ;

        if (ballX < 200) ballX = 200;
        if (ballX > 700) ballX = 700;

        velZ *= 0.995f;
        velX *= 0.99f;

        // 🔥 ОЦЕ СТАВИ ТУТ (після тертя)
        if (fabs(velZ) < 0.1f)
            velZ = 0;
        if (fabs(velX) < 0.05f)
            velX = 0;

        for (int i = 0; i < pins.size(); i++) {
            if (!pins[i].down) {
                float dx = pins[i].x - ballX;
                float dz = pins[i].z - ballZ;
                float dist = sqrt(dx * dx + dz * dz);

                // Збільшуємо хітбокс кулі трохи, щоб легше було зачепити кеглю
                if (dist < 35.0f) { 
                    pins[i].down = true;

                    // Фізика кулі: відхиляється, але не втрачає всю швидкість
                    float nx = dx / dist;
                    velX = velX * 0.85f - nx * 2.0f; 
                    velZ *= 0.98f; 

                    // --- ЕФЕКТ ВИБУХОВОГО СТРАЙКУ ---
                    for (int j = 0; j < pins.size(); j++) {
                        if (!pins[j].down) {
                            float kx = pins[j].x - pins[i].x;
                            float kz = pins[j].z - pins[i].z;
                            float kDist = sqrt(kx * kx + kz * kz);

                            // Збільшуємо радіус до 85 одиниць для "ланцюгової реакції"
                            if (kDist < 85.0f) {
                                // Шанс збиття сусіда (чим ближче, тим вище)
                                // Додаємо бонус, якщо це перші ряди
                                float probability = (85.0f - kDist) / 85.0f * 100.0f;
                                if ((rand() % 100) < (probability + 20)) { 
                                    pins[j].down = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (ballX < 205) velX = std::abs(velX) * 0.5f;
        if (ballX > 695) velX = -std::abs(velX) * 0.5f;

        if (ballZ < -1150) nextStep();
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
        if (!gameStarted)
        {
            gameStarted = true;
            return;
        }
    if (key == '+' || key == '=') scale += 0.1f;
    if (key == '-' || key == '_') scale -= 0.1f;
    if (scale < 0.2f) scale = 0.2f;
    if (key == 'r' || key == 'R') initGame();
    if (key == 27) exit(0);
}

void special(int key, int x, int y) {
    float step = 5.0f;
    if (key == GLUT_KEY_LEFT) rotateY -= step;
    if (key == GLUT_KEY_RIGHT) rotateY += step;
    if (key == GLUT_KEY_UP) rotateX -= step;
    if (key == GLUT_KEY_DOWN) rotateX += step;
}

void mousePassive(int x, int y) {
    if (!gameStarted) return;
    if (!isShooting) 
        ballX = 200.0f + ((float)x / 1200.0f) * 500.0f;
}

void mouseClick(int button, int state, int x, int y) {
    if (isShooting) return;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isShooting && !isGameOver)
	{
		isShooting = true;

		velZ = -15.0f;

		// напрям залежить від позиції кулі
		velX = (ballX - 450.0f) * 0.05f;
	}
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 700);
    glutCreateWindow("Bowling System - NTU KhPI");

    glEnable(GL_DEPTH_TEST);
    initGame();

    glutDisplayFunc(display);
    glutPassiveMotionFunc(mousePassive);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}