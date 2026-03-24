#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game()
    : renderer(800, 450), score(0), total(0),
      showResult(false), wasCorrect(false), resultTimer(0.0f) {
    srand(static_cast<unsigned>(time(nullptr)));
    current = generator.generate();
}

void Game::run() {
    while (!renderer.shouldClose()) {
        handleInput();
        update();

        renderer.beginFrame();
        renderer.drawTitle();
        renderer.drawScore(score, total);
        renderer.drawQuestion(current);
        renderer.drawOptions(current, showResult);
        if (showResult) renderer.drawResult(wasCorrect);
        renderer.endFrame();
    }
}

void Game::handleInput() {
    if (showResult) return;

    int clicked = renderer.getClickedOption();
    if (clicked >= 0) {
        total++;
        wasCorrect = (clicked == current.correctIndex);
        if (wasCorrect) score++;
        showResult = true;
        resultTimer = 1.0f;
    }
}

void Game::update() {
    if (!showResult) return;

    resultTimer -= renderer.getFrameTime();
    if (resultTimer <= 0.0f) {
        showResult = false;
        current = generator.generate();
    }
}
