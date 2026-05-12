#include "Game.h"
#include "HistoryEntry.h"
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game()
    : state(GameState::OPERATION_MENU),
      diffConfig(getDifficultyConfig(Difficulty::EASY)),
      score(0), total(0),
      showResult(false), wasCorrect(false), resultTimer(0.0f),
      sessionTimer(0.0f), sessionOver(false),
      selectedOperation(0), selectedDifficulty(Difficulty::EASY),
      stateBeforeMascot(GameState::OPERATION_MENU) {
    srand(static_cast<unsigned>(time(nullptr)));
    // Load the first mascot by default so the player immediately sees one
    mascot.load(0);
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
            case GameState::MASCOT_MENU:     handleMascotMenu();     break;
        }
    }
}

// ---------------------------------------------------------------------------
// State: operation menu
// ---------------------------------------------------------------------------

void Game::handleOperationMenu() {
    mascot.update(renderer.getFrameTime());

    // Press M to open mascot selection
    if (IsKeyPressed(KEY_M)) {
        stateBeforeMascot = state;
        state = GameState::MASCOT_MENU;
        return;
    }

    renderer.beginFrame();
    std::vector<std::string> names;
    for (int i = 0; i < generator.getOperationCount(); ++i)
        names.push_back(std::string(1, generator.getOperation(i)->getSymbol()));
    renderer.drawOperationMenu(names);
    // Small hint
    DrawText("Press M to change mascot",
             Renderer::GAME_AREA_W / 2 - MeasureText("Press M to change mascot", 16) / 2,
             560, 16, GRAY);
    mascot.draw(Renderer::MASCOT_PANEL_X, Renderer::MASCOT_PANEL_W, Renderer::SCREEN_H);
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
    mascot.update(renderer.getFrameTime());

    renderer.beginFrame();
    renderer.drawDifficultyMenu();
    mascot.draw(Renderer::MASCOT_PANEL_X, Renderer::MASCOT_PANEL_W, Renderer::SCREEN_H);
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
    mascot.update(dt);

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

            // Record this attempt in the persistent history
            HistoryEntry entry;
            entry.question      = std::to_string(current.a) + " " + current.op
                                  + " " + std::to_string(current.b) + " = ?";
            entry.givenAnswer   = current.options[clicked];
            entry.correctAnswer = current.correctAnswer;
            entry.wasCorrect    = wasCorrect;
            entry.timestamp     = std::time(nullptr);
            history.record(entry);

            // Evaluate badge conditions after recording
            auto newBadges = badges.evaluate(history.entries(), score, total);
            for (auto* b : newBadges)
                pendingBadges.push_back(b);
            if (!pendingBadges.empty()) badgeNotifTimer = 2.5f;

            // Trigger the matching mascot animation
            if (wasCorrect) mascot.playCorrect();
            else            mascot.playWrong();

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
    // Badge notification toast (displayed on top, fades after timer)
    if (badgeNotifTimer > 0.0f && !pendingBadges.empty()) {
        renderer.drawBadgeNotification(pendingBadges.back()->name,
                                       pendingBadges.back()->description);
        badgeNotifTimer -= dt;
        if (badgeNotifTimer <= 0.0f) {
            pendingBadges.pop_back();
            if (!pendingBadges.empty()) badgeNotifTimer = 2.5f;
        }
    }
    mascot.draw(Renderer::MASCOT_PANEL_X, Renderer::MASCOT_PANEL_W, Renderer::SCREEN_H);
    renderer.endFrame();
}

// ---------------------------------------------------------------------------
// State: results
// ---------------------------------------------------------------------------

void Game::handleResults() {
    mascot.update(renderer.getFrameTime());

    int allCorrect = 0, allTotal = 0;
    for (const auto& e : history.entries()) {
        ++allTotal;
        if (e.wasCorrect) ++allCorrect;
    }

    renderer.beginFrame();
    renderer.drawResults(score, total, diffConfig.label, allCorrect, allTotal);
    renderer.drawBadgeList(badges.badges());
    mascot.draw(Renderer::MASCOT_PANEL_X, Renderer::MASCOT_PANEL_W, Renderer::SCREEN_H);
    renderer.endFrame();

    if (renderer.getClickedPlayAgain()) {
        state = GameState::OPERATION_MENU;
    }
}

// ---------------------------------------------------------------------------
// State: mascot selection menu
// ---------------------------------------------------------------------------

void Game::handleMascotMenu() {
    mascot.update(renderer.getFrameTime());

    renderer.beginFrame();
    renderer.drawMascotMenu(mascot.currentIndex(), mascot.isVisible());
    mascot.draw(Renderer::MASCOT_PANEL_X, Renderer::MASCOT_PANEL_W, Renderer::SCREEN_H);
    renderer.endFrame();

    // ESC or M closes the menu
    if (renderer.isBackPressed() || IsKeyPressed(KEY_M)) {
        state = stateBeforeMascot;
        return;
    }

    int clicked = renderer.getClickedMascot();
    if (clicked >= 0) mascot.load(clicked);

    if (renderer.getClickedToggleMascot()) mascot.toggleVisible();
}

