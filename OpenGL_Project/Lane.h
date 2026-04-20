#pragma once
#include "Pin.h"
#include "Ball.h"
#include <vector>

class Lane {
public:
    std::vector<Pin> pins;
    Ball ball;

    Lane() {
        initPins();
    }

    void initPins() {
        pins.clear();

        // 6 pins like your old code
        pins.push_back(Pin(-20, 0, -200));
        pins.push_back(Pin(0, 0, -200));
        pins.push_back(Pin(20, 0, -200));
        pins.push_back(Pin(-10, 0, -240));
        pins.push_back(Pin(10, 0, -240));
        pins.push_back(Pin(0, 0, -280));
    }

    void update() {
        ball.move();

        for (auto& p : pins)
            p.checkHit(ball);
    }

    int score() {
        int s = 0;
        for (auto& p : pins)
            if (p.knocked) s++;
        return s;
    }

    void draw() {
        ball.draw();
        for (auto& p : pins)
            p.draw();
    }
};