#pragma once

#include "question/Question.h"
#include "badge/Badge.h"
#include <raylib.h>
#include <vector>
#include <string>

// Renderer owns the window and all drawing calls.
// It is intentionally kept separate from game logic: it receives plain data
// (scores, questions, labels…) and returns which UI element was clicked.
// Swapping the graphical back-end only requires touching this class.
//
// Layout model
// ┌────────────── sw() ──────────────┐
// │   game area gw()   │ mascot 300  │
// └────────────────────┴─────────────┘
// All Y positions are scaled from a 700 px design height using scaled().
class Renderer {
    public:
        // The mascot side panel is always 300 px wide regardless of resolution.
        static constexpr int MASCOT_PANEL_W = 300;

        // Minimum window size to keep the UI usable.
        static constexpr int MIN_W = 880;
        static constexpr int MIN_H = 560;

        // Design resolution used as the base for scaled().
        static constexpr int DESIGN_H = 700;

        Renderer();
        ~Renderer();

        bool shouldClose() const;
        float getFrameTime() const;
        void beginFrame();
        void endFrame();

        // Current window dimensions (change when user resizes).
        int sw() const;   // screen width
        int sh() const;   // screen height
        int gw() const;   // game area width  = sw() - MASCOT_PANEL_W
        int mx() const;   // mascot panel x   = sw() - MASCOT_PANEL_W

        // Scale a design value (designed for DESIGN_H) to the current height.
        int scaled(int v) const;

        // ---- Operation menu ----
        void drawOperationMenu(const std::vector<std::string>& names) const;
        int  getClickedOperation(const std::vector<std::string>& names) const;

        // ---- Difficulty menu ----
        void drawDifficultyMenu() const;
        int  getClickedDifficulty() const;

        // ---- Playing screen ----
        void drawTitle() const;
        void drawScore(int score, int total) const;
        void drawQuestion(const Question& q) const;
        void drawOptions(const Question& q, bool showResult) const;
        void drawResult(bool wasCorrect) const;
        void drawTimer(float timeLeft, float timeTotal) const;
        int  getClickedOption() const;

        // ---- Results screen ----
        void drawResults(int score, int total, const char* diffLabel,
                         int allTimeCorrect, int allTimeTotal) const;
        bool getClickedPlayAgain() const;

        // ---- Badge notifications ----
        void drawBadgeNotification(const char* name, const char* desc) const;
        void drawBadgeList(const std::vector<Badge>& badges) const;

        // ---- Mascot panel overlay ----
        void drawMascotMenu(int selectedIdx, bool visible) const;
        int  getClickedMascot() const;
        bool getClickedToggleMascot() const;

        bool isBackPressed() const;

    private:
        static bool isHovered(Rectangle btn);
        static bool isClicked(Rectangle btn);
        void drawButton(Rectangle btn, const char* text, int fontSize,
                        Color idle, Color hover, Color textColor) const;
        void drawMenuTitle(const char* text) const;

        // Returns the rectangle for the i-th answer button (0,1,2).
        // Uses scaled() so hit boxes match drawn positions at any resolution.
        Rectangle optionButton(int index) const;
};
