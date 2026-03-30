#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>

Game::Game()
        : renderer(800, 450), score(0), total(0),
            showResult(false), wasCorrect(false), resultTimer(0.0f), selectingOperation(true) {
        srand(static_cast<unsigned>(time(nullptr)));
}

void Game::run() {
    std::vector<std::string> opNames;
    for (int i = 0; i < generator.getOperationCount(); ++i) {
        opNames.push_back(std::string(1, generator.getOperation(i)->getSymbol()));
    }
    while (!renderer.shouldClose()) {
        if (selectingOperation) {
            renderer.beginFrame();
            renderer.drawTitle();
            renderer.drawOperationMenu(opNames, generator.getOperationCount() > 0 ? 0 : -1);
            renderer.endFrame();
            int clicked = renderer.getClickedOperation(opNames);
            if (clicked >= 0) {
                generator.setOperationIndex(clicked);
                current = generator.generate();
                selectingOperation = false;
            }
            continue;
        }
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
