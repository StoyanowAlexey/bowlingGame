#pragma once
#include "Lane.h"

class Game {
public:
    Lane lane;
    int totalScore;
    int turn;

    Game() {
        totalScore = 0;
        turn = 0;
    }

    void startTurn() {
        lane.ball.reset();
    }

    void shoot(float power) {
        lane.ball.active = true;
        lane.ball.speed = power;
    }

    void update() {
        lane.update();
    }

    void nextTurn() {
        totalScore += lane.score();
        lane.initPins();
        turn++;
    }
};