#pragma once

#include "QuestionGenerator.h"
#include "Renderer.h"

class Game {
public:
    Game();
    void run();

private:
    void handleInput();
    void update();

    QuestionGenerator generator;
    Renderer renderer;
    Question current;
    int score;
    int total;
    bool showResult;
    bool wasCorrect;
    float resultTimer;
};
