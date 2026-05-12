#pragma once

#include "QuestionGenerator.h"
#include "Renderer.h"
#include "Difficulty.h"

// All possible screens / phases of the application.
// The state machine is the backbone of Game::run(); each state maps to a
// dedicated handle*() method so every screen has a single responsibility.
enum class GameState {
    OPERATION_MENU,   // step 1: choose arithmetic operation
    DIFFICULTY_MENU,  // step 2: choose difficulty level
    PLAYING,          // active gameplay
};

class Game {
    public:
        Game();
        void run();

    private:
        // ---- Subsystems ----
        Renderer renderer;
        QuestionGenerator generator;

        // ---- Game state ----
        GameState state;
        DifficultyConfig diffConfig;
        Question current;
        int score;
        int total;
        bool showResult;
        bool wasCorrect;
        float resultTimer;

        // ---- Menu selections ----
        int selectedOperation;
        Difficulty selectedDifficulty;

        // ---- State handlers ----
        void handleOperationMenu();
        void handleDifficultyMenu();
        void handlePlaying();
};
