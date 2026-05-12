#pragma once

#include "QuestionGenerator.h"
#include "Renderer.h"
#include "Difficulty.h"
#include "History.h"

// All possible screens / phases of the application.
// The state machine is the backbone of Game::run(); each state maps to a
// dedicated handle*() method so every screen has a single responsibility.
enum class GameState {
    OPERATION_MENU,   // step 1: choose arithmetic operation
    DIFFICULTY_MENU,  // step 2: choose difficulty level
    PLAYING,          // active gameplay
    RESULTS,          // end-of-session summary
};

class Game {
    public:
        Game();
        void run();

    private:
        // ---- Subsystems ----
        Renderer renderer;
        QuestionGenerator generator;
        History history;

        // ---- Game state ----
        GameState state;
        DifficultyConfig diffConfig;
        Question current;
        int score;
        int total;
        bool showResult;
        bool wasCorrect;
        float resultTimer;

        // ---- Timed session ----
        float sessionTimer;   // seconds remaining (0 = unlimited)
        bool  sessionOver;    // true once time or question count is exhausted

        // ---- Menu selections ----
        int selectedOperation;
        Difficulty selectedDifficulty;

        // ---- State handlers ----
        void handleOperationMenu();
        void handleDifficultyMenu();
        void handlePlaying();
        void handleResults();
};
