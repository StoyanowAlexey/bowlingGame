#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "PinManager.h"

class Game {
private:
    Ball ball;
    PinManager pins;

    int round;
    int attempt;
    int totalScore;
    int firstThrow;

    bool gameOver;
    bool started;

public:
    Game() { init(); }

    void init() {
        round = 1;
        attempt = 1;
        totalScore = 0;
        gameOver = false;
        started = false;

        ball.reset();
        pins.reset();
    }

    void start() { started = true; }

    void update() {
        if (!started || gameOver) return;

        ball.update();
        pins.checkCollisions(ball);

        if (ball.getZ() < -1150)
            nextStep();
    }

    void nextStep() {
        int down = pins.countDowned();

        if (attempt == 1) {
            if (down == 10) {
                totalScore += 10 + (rand() % 10 + 1);
                round++;
                pins.reset();
            } else {
                firstThrow = down;
                attempt = 2;
            }
        } else {
            totalScore += firstThrow + down;
            round++;
            attempt = 1;
            pins.reset();
        }

        if (round > 5)
            gameOver = true;

        ball.reset();
    }

    void draw() {
        pins.draw();
        ball.draw();
    }

    int getScore() const { return totalScore; }
    int getRound() const { return round; }
    int getAttempt() const { return attempt; }
    bool isGameOver() const { return gameOver; }
    bool isStarted() const { return started; }

    Ball& getBall() { return ball; } 
};

#endif