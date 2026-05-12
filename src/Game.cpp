#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game()
    : state(GameState::OPERATION_MENU),
      diffConfig(getDifficultyConfig(Difficulty::EASY)),
      score(0), total(0),
      showResult(false), wasCorrect(false), resultTimer(0.0f),
      selectedOperation(0), selectedDifficulty(Difficulty::EASY) {
    srand(static_cast<unsigned>(time(nullptr)));
}

// ---------------------------------------------------------------------------
// Main loop: dispatch to the current state handler each frame
// ---------------------------------------------------------------------------

void Game::run() {
    while (!renderer.shouldClose()) {
        switch (state) {
            case GameState::OPERATION_MENU:  handleOperationMenu();  break;
            case GameState::DIFFICULTY_MENU: handleDifficultyMenu(); break;
            case GameState::PLAYING:         handlePlaying();        break;
        }
    }
}

// ---------------------------------------------------------------------------
// State: operation menu
// ---------------------------------------------------------------------------

void Game::handleOperationMenu() {
    renderer.beginFrame();
    std::vector<std::string> names;
    for (int i = 0; i < generator.getOperationCount(); ++i)
        names.push_back(std::string(1, generator.getOperation(i)->getSymbol()));
    renderer.drawOperationMenu(names);
    renderer.endFrame();

    int clicked = renderer.getClickedOperation(names);
    if (clicked >= 0) {
        selectedOperation = clicked;
        generator.setOperationIndex(clicked);
        state = GameState::DIFFICULTY_MENU;
    }
}

// ---------------------------------------------------------------------------
// State: difficulty menu
// ---------------------------------------------------------------------------

void Game::handleDifficultyMenu() {
    renderer.beginFrame();
    renderer.drawDifficultyMenu();
    renderer.endFrame();

    // ESC goes back to operation selection
    if (renderer.isBackPressed()) {
        state = GameState::OPERATION_MENU;
        return;
    }

    int clicked = renderer.getClickedDifficulty();
    if (clicked >= 0) {
        selectedDifficulty = static_cast<Difficulty>(clicked);
        diffConfig = getDifficultyConfig(selectedDifficulty);
        score = 0;
        total = 0;
        current = generator.generate(diffConfig);
        showResult = false;
        state = GameState::PLAYING;
    }
}

// ---------------------------------------------------------------------------
// State: playing
// ---------------------------------------------------------------------------

void Game::handlePlaying() {
    // ESC returns to the main menu
    if (renderer.isBackPressed()) {
        state = GameState::OPERATION_MENU;
        return;
    }

    // Input: pick an answer (only when not already showing the result)
    if (!showResult) {
        int clicked = renderer.getClickedOption();
        if (clicked >= 0) {
            ++total;
            wasCorrect = (clicked == current.correctIndex);
            if (wasCorrect) ++score;
            showResult  = true;
            resultTimer = 1.0f;
        }
    }

    // Update: advance the result display timer
    if (showResult) {
        resultTimer -= renderer.getFrameTime();
        if (resultTimer <= 0.0f) {
            showResult = false;
            current = generator.generate(diffConfig);
        }
    }

    // Draw
    renderer.beginFrame();
    renderer.drawTitle();
    renderer.drawScore(score, total);
    renderer.drawQuestion(current);
    renderer.drawOptions(current, showResult);
    if (showResult) renderer.drawResult(wasCorrect);
    renderer.endFrame();
}

