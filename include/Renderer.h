#pragma once

#include "Question.h"
#include <raylib.h>
#include <vector>
#include <string>
#include <string>

class Renderer {
    public:
        Renderer(int width, int height);
        ~Renderer();

        bool shouldClose() const;
        float getFrameTime() const;

        void beginFrame();
        void endFrame();

        void drawTitle();
        void drawScore(int score, int total);
        void drawQuestion(const Question& q);
        void drawOptions(const Question& q, bool showResult);
        void drawResult(bool wasCorrect);
        int getClickedOption() const;

        void drawOperationMenu(const std::vector<std::string>& opNames, int selected) const;
        int getClickedOperation(const std::vector<std::string>& opNames) const;

    private:
        int screenWidth;
        int screenHeight;

        static constexpr int btnWidth = 180;
        static constexpr int btnHeight = 60;
        static constexpr int btnY = 280;
        static constexpr int btnSpacing = 30;
        int btnStartX;
};
