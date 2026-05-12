#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game()
    : state(GameState::OPERATION_MENU),
      diffConfig(getDifficultyConfig(Difficulty::EASY)),
      score(0), total(0),
      showResult(false), wasCorrect(false), resultTimer(0.0f),
      sessionTimer(0.0f), sessionOver(false),
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
            case GameState::RESULTS:         handleResults();        break;
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
        sessionTimer = diffConfig.timeLimit;   // 0 means unlimited
        sessionOver  = false;
        current = generator.generate(diffConfig);
        showResult = false;
        state = GameState::PLAYING;
    }
}

// ---------------------------------------------------------------------------
// State: playing
// ---------------------------------------------------------------------------

void Game::handlePlaying() {
    float dt = renderer.getFrameTime();

    // ESC returns to the main menu
    if (renderer.isBackPressed()) {
        state = GameState::OPERATION_MENU;
        return;
    }

    // Count-down timer (skip when timeLimit == 0 → unlimited)
    if (diffConfig.timeLimit > 0.0f && !sessionOver) {
        sessionTimer -= dt;
        if (sessionTimer <= 0.0f) {
            sessionTimer = 0.0f;
            state = GameState::RESULTS;
            return;
        }
    }

    // Session ends when the question quota is reached
    if (diffConfig.questionCount > 0 && total >= diffConfig.questionCount && !showResult) {
        state = GameState::RESULTS;
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
        resultTimer -= dt;
        if (resultTimer <= 0.0f) {
            showResult = false;
            // Move to results if quota reached after this answer
            if (diffConfig.questionCount > 0 && total >= diffConfig.questionCount) {
                state = GameState::RESULTS;
                return;
            }
            current = generator.generate(diffConfig);
        }
    }

    // Draw
    renderer.beginFrame();
    renderer.drawTimer(sessionTimer, diffConfig.timeLimit);
    renderer.drawTitle();
    renderer.drawScore(score, total);
    renderer.drawQuestion(current);
    renderer.drawOptions(current, showResult);
    if (showResult) renderer.drawResult(wasCorrect);
    renderer.endFrame();
}

// ---------------------------------------------------------------------------
// State: results
// ---------------------------------------------------------------------------

void Game::handleResults() {
    renderer.beginFrame();
    renderer.drawResults(score, total, diffConfig.label);
    renderer.endFrame();

    if (renderer.getClickedPlayAgain()) {
        state = GameState::OPERATION_MENU;
    }
}

