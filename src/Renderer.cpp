#include "Renderer.h"

Renderer::Renderer(int width, int height)
    : screenWidth(width), screenHeight(height) {
    int totalBtnsWidth = 3 * btnWidth + 2 * btnSpacing;
    btnStartX = (screenWidth - totalBtnsWidth) / 2;
    InitWindow(screenWidth, screenHeight, "Calculissimo");
    SetTargetFPS(60);
}

Renderer::~Renderer() {
    CloseWindow();
}

bool Renderer::shouldClose() const {
    return WindowShouldClose();
}

float Renderer::getFrameTime() const {
    return GetFrameTime();
}

void Renderer::beginFrame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
}

void Renderer::endFrame() {
    EndDrawing();
}

void Renderer::drawTitle() {
    DrawText("CALCULISSIMO",
             screenWidth / 2 - MeasureText("CALCULISSIMO", 30) / 2,
             20, 30, DARKGRAY);
}

void Renderer::drawScore(int score, int total) {
    std::string text = "Score: " + std::to_string(score) + " / " + std::to_string(total);
    DrawText(text.c_str(),
             screenWidth / 2 - MeasureText(text.c_str(), 20) / 2,
             65, 20, GRAY);
}

void Renderer::drawQuestion(const Question& q) {
    std::string text = std::to_string(q.a) + " " + q.op + " " + std::to_string(q.b) + " = ?";
    DrawText(text.c_str(),
             screenWidth / 2 - MeasureText(text.c_str(), 50) / 2,
             140, 50, BLACK);
}

void Renderer::drawOptions(const Question& q, bool showResult) {
    for (int i = 0; i < 3; i++) {
        Rectangle btn = {
            static_cast<float>(btnStartX + i * (btnWidth + btnSpacing)),
            static_cast<float>(btnY),
            static_cast<float>(btnWidth),
            static_cast<float>(btnHeight)
        };

        Color btnColor = LIGHTGRAY;
        if (showResult) {
            btnColor = (i == q.correctIndex) ? GREEN : MAROON;
        } else {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, btn)) btnColor = SKYBLUE;
        }

        DrawRectangleRec(btn, btnColor);
        DrawRectangleLinesEx(btn, 2, DARKGRAY);

        std::string text = std::to_string(q.options[i]);
        DrawText(text.c_str(),
                 static_cast<int>(btn.x) + btnWidth / 2 - MeasureText(text.c_str(), 30) / 2,
                 btnY + btnHeight / 2 - 15,
                 30, BLACK);
    }
}

void Renderer::drawResult(bool wasCorrect) {
    const char* msg = wasCorrect ? "Correct !" : "Faux !";
    Color color = wasCorrect ? DARKGREEN : RED;
    DrawText(msg,
             screenWidth / 2 - MeasureText(msg, 40) / 2,
             370, 40, color);
}

int Renderer::getClickedOption() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;

    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < 3; i++) {
        Rectangle btn = {
            static_cast<float>(btnStartX + i * (btnWidth + btnSpacing)),
            static_cast<float>(btnY),
            static_cast<float>(btnWidth),
            static_cast<float>(btnHeight)
        };
        if (CheckCollisionPointRec(mouse, btn)) return i;
    }
    return -1;
}

void Renderer::drawOperationMenu(const std::vector<std::string>& opNames, int selected) const {
    int menuY = 120;
    int menuBtnWidth = 160;
    int menuBtnHeight = 50;
    int menuSpacing = 30;
    int totalWidth = opNames.size() * menuBtnWidth + (opNames.size() - 1) * menuSpacing;
    int startX = (screenWidth - totalWidth) / 2;
    for (size_t i = 0; i < opNames.size(); ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (menuBtnWidth + menuSpacing)),
            static_cast<float>(menuY),
            static_cast<float>(menuBtnWidth),
            static_cast<float>(menuBtnHeight)
        };
        Color btnColor = LIGHTGRAY;
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, btn)) 
            btnColor = SKYBLUE;
            
        DrawRectangleRec(btn, btnColor);
        DrawRectangleLinesEx(btn, 2, DARKGRAY);
        DrawText(opNames[i].c_str(),
                 static_cast<int>(btn.x) + menuBtnWidth / 2 - MeasureText(opNames[i].c_str(), 28) / 2,
                 menuY + menuBtnHeight / 2 - 14,
                 28, BLACK);
    }
}

int Renderer::getClickedOperation(const std::vector<std::string>& opNames) const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;
    int menuY = 120;
    int menuBtnWidth = 160;
    int menuBtnHeight = 50;
    int menuSpacing = 30;
    int totalWidth = opNames.size() * menuBtnWidth + (opNames.size() - 1) * menuSpacing;
    int startX = (screenWidth - totalWidth) / 2;
    Vector2 mouse = GetMousePosition();
    for (size_t i = 0; i < opNames.size(); ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (menuBtnWidth + menuSpacing)),
            static_cast<float>(menuY),
            static_cast<float>(menuBtnWidth),
            static_cast<float>(menuBtnHeight)
        };
        if (CheckCollisionPointRec(mouse, btn)) return static_cast<int>(i);
    }
    return -1;
}
