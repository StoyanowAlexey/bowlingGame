#ifndef PINMANAGER_H
#define PINMANAGER_H

#include "Pin.h"
#include "Ball.h"
#include <vector>
#include <cmath>

class PinManager {
private:
    std::vector<Pin> pins;

public:
    void reset() {
        pins.clear();

        float spacingX = 32.0f;
        float spacingZ = 40.0f;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j <= i; j++) {
                pins.emplace_back(
                    450 + (j * spacingX) - (i * spacingX / 2.0f),
                    -750 - (i * spacingZ)
                );
            }
        }
    }

    void draw() {
        for (auto& p : pins)
            p.draw();
    }

    int checkCollisions(Ball& ball) {
        int downed = 0;

        for (auto& p : pins) {
            if (p.isDown()) continue;

            float dx = p.getX() - ball.getX();
            float dz = p.getZ() - ball.getZ();
            float dist = sqrt(dx * dx + dz * dz);

            if (dist < 35.0f) {
                p.knockDown();
                downed++;

                float nx = dx / dist;
                
                float newVelX = ball.getVelX() * 0.85f - nx * 2.0f;
                float newVelZ = ball.getVelZ() * 0.98f;

                ball.setVelocity(newVelX, newVelZ);
            }
        }

        return downed;
    }

    int countDowned() {
        int count = 0;

        for (auto& p : pins) {
            if (p.isDown() && !p.isCounted()) {
                count++;
                p.markCounted();
            }
        }

        return count;
    }
};

#endif // PINMANAGER_H