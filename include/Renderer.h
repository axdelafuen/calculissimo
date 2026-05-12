#pragma once

#include "Question.h"
#include <raylib.h>
#include <vector>
#include <string>

// Renderer owns the window and all drawing calls.
// It is intentionally kept separate from game logic: it receives plain data
// (scores, questions, labels…) and returns which UI element was clicked.
// Swapping the graphical back-end only requires touching this class.
class Renderer {
    public:
        // Fixed window dimensions; the right 300 px are reserved for the mascot panel.
        static constexpr int SCREEN_W      = 1100;
        static constexpr int SCREEN_H      = 700;
        static constexpr int GAME_AREA_W   = 800;   // usable left area
        static constexpr int MASCOT_PANEL_X = 800;
        static constexpr int MASCOT_PANEL_W = 300;

        Renderer();
        ~Renderer();

        bool shouldClose() const;
        float getFrameTime() const;
        void beginFrame();
        void endFrame();

        // ---- Operation menu ----
        void drawOperationMenu(const std::vector<std::string>& names) const;
        int  getClickedOperation(const std::vector<std::string>& names) const;

        // ---- Difficulty menu ----
        void drawDifficultyMenu() const;
        int  getClickedDifficulty() const;   // 0=Easy 1=Medium 2=Hard, -1=none

        // ---- Playing screen ----
        void drawTitle() const;
        void drawScore(int score, int total) const;
        void drawQuestion(const Question& q) const;
        void drawOptions(const Question& q, bool showResult) const;
        void drawResult(bool wasCorrect) const;
        int  getClickedOption() const;

        // ESC is checked by the game loop directly via raylib; the renderer
        // just exposes an "is back pressed" helper for consistency.
        bool isBackPressed() const;

    private:
        static bool isHovered(Rectangle btn);
        static bool isClicked(Rectangle btn);
        void drawButton(Rectangle btn, const char* text, int fontSize,
                        Color idle, Color hover, Color textColor) const;
        void drawMenuTitle(const char* text) const;

        // Returns the rectangle for the i-th answer button (0,1,2)
        Rectangle optionButton(int index) const;

        static constexpr int BTN_W       = 180;
        static constexpr int BTN_H       = 60;
        static constexpr int BTN_Y       = 310;
        static constexpr int BTN_SPACING = 30;
};
