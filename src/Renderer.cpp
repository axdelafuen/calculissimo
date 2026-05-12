#include "Renderer.h"
#include <string>

// ---------------------------------------------------------------------------
// Window lifecycle
// ---------------------------------------------------------------------------

Renderer::Renderer() {
    InitWindow(SCREEN_W, SCREEN_H, "Calculissimo");
    SetTargetFPS(60);
}

Renderer::~Renderer() {
    CloseWindow();
}

bool Renderer::shouldClose() const { return WindowShouldClose(); }
float Renderer::getFrameTime() const { return GetFrameTime(); }
void Renderer::beginFrame() { BeginDrawing(); ClearBackground(RAYWHITE); }
void Renderer::endFrame()   { EndDrawing(); }
bool Renderer::isBackPressed() const { return IsKeyPressed(KEY_ESCAPE); }

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

bool Renderer::isHovered(Rectangle btn) {
    return CheckCollisionPointRec(GetMousePosition(), btn);
}

bool Renderer::isClicked(Rectangle btn) {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isHovered(btn);
}

void Renderer::drawButton(Rectangle btn, const char* text, int fontSize,
                           Color idle, Color hover, Color textColor) const {
    Color fill = isHovered(btn) ? hover : idle;
    DrawRectangleRec(btn, fill);
    DrawRectangleLinesEx(btn, 2, DARKGRAY);
    int tw = MeasureText(text, fontSize);
    DrawText(text,
             static_cast<int>(btn.x) + (static_cast<int>(btn.width)  - tw) / 2,
             static_cast<int>(btn.y) + (static_cast<int>(btn.height) - fontSize) / 2,
             fontSize, textColor);
}

void Renderer::drawMenuTitle(const char* text) const {
    int fs = 36;
    DrawText(text, SCREEN_W / 2 - MeasureText(text, fs) / 2, 60, fs, DARKBLUE);
}

Rectangle Renderer::optionButton(int index) const {
    int totalW = 3 * BTN_W + 2 * BTN_SPACING;
    int startX = (GAME_AREA_W - totalW) / 2;
    return {
        static_cast<float>(startX + index * (BTN_W + BTN_SPACING)),
        static_cast<float>(BTN_Y),
        static_cast<float>(BTN_W),
        static_cast<float>(BTN_H)
    };
}

// ---------------------------------------------------------------------------
// Operation menu
// ---------------------------------------------------------------------------

void Renderer::drawOperationMenu(const std::vector<std::string>& names) const {
    drawMenuTitle("Choose an operation");

    const int btnW = 160, btnH = 55, spacing = 25;
    int totalW = static_cast<int>(names.size()) * btnW +
                 (static_cast<int>(names.size()) - 1) * spacing;
    int startX = (SCREEN_W - totalW) / 2;

    for (int i = 0; i < static_cast<int>(names.size()); ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            160.0f,
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        drawButton(btn, names[i].c_str(), 28, LIGHTGRAY, SKYBLUE, BLACK);
    }
}

int Renderer::getClickedOperation(const std::vector<std::string>& names) const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;

    const int btnW = 160, btnH = 55, spacing = 25;
    int totalW = static_cast<int>(names.size()) * btnW +
                 (static_cast<int>(names.size()) - 1) * spacing;
    int startX = (SCREEN_W - totalW) / 2;

    for (int i = 0; i < static_cast<int>(names.size()); ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            160.0f,
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        if (CheckCollisionPointRec(GetMousePosition(), btn)) return i;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Difficulty menu
// ---------------------------------------------------------------------------

static const char* DIFF_LABELS[] = {"Easy", "Medium", "Hard"};
static Color DIFF_COLORS[]       = {GREEN, ORANGE, RED};

void Renderer::drawDifficultyMenu() const {
    drawMenuTitle("Choose difficulty");

    const char* subtitles[] = {
        "Small numbers  |  8 questions  |  No time limit",
        "Normal numbers  |  10 questions  |  90 seconds",
        "Large numbers  |  15 questions  |  60 seconds"
    };

    const int btnW = 200, btnH = 60, spacing = 40;
    int totalW = 3 * btnW + 2 * spacing;
    int startX = (SCREEN_W - totalW) / 2;

    for (int i = 0; i < 3; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            160.0f,
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        drawButton(btn, DIFF_LABELS[i], 26, DIFF_COLORS[i],
                   ColorBrightness(DIFF_COLORS[i], 0.3f), BLACK);

        // Sub-label below each button
        int sw = MeasureText(subtitles[i], 14);
        DrawText(subtitles[i],
                 static_cast<int>(btn.x) + (btnW - sw) / 2,
                 static_cast<int>(btn.y) + btnH + 8,
                 14, DARKGRAY);
    }
}

int Renderer::getClickedDifficulty() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;

    const int btnW = 200, btnH = 60, spacing = 40;
    int totalW = 3 * btnW + 2 * spacing;
    int startX = (SCREEN_W - totalW) / 2;

    for (int i = 0; i < 3; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            160.0f,
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        if (CheckCollisionPointRec(GetMousePosition(), btn)) return i;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Playing screen
// ---------------------------------------------------------------------------

void Renderer::drawTitle() const {
    const char* title = "CALCULISSIMO";
    DrawText(title,
             GAME_AREA_W / 2 - MeasureText(title, 30) / 2,
             20, 30, DARKBLUE);
}

void Renderer::drawScore(int score, int total) const {
    std::string text = "Score: " + std::to_string(score) + " / " + std::to_string(total);
    DrawText(text.c_str(),
             GAME_AREA_W / 2 - MeasureText(text.c_str(), 22) / 2,
             62, 22, GRAY);
}

void Renderer::drawQuestion(const Question& q) const {
    std::string text = std::to_string(q.a) + " " + q.op + " " + std::to_string(q.b) + " = ?";
    DrawText(text.c_str(),
             GAME_AREA_W / 2 - MeasureText(text.c_str(), 52) / 2,
             180, 52, BLACK);
}

void Renderer::drawOptions(const Question& q, bool showResult) const {
    for (int i = 0; i < 3; i++) {
        Rectangle btn = optionButton(i);
        Color fill;
        if (showResult) {
            fill = (i == q.correctIndex) ? GREEN : MAROON;
        } else {
            fill = isHovered(btn) ? SKYBLUE : LIGHTGRAY;
        }
        DrawRectangleRec(btn, fill);
        DrawRectangleLinesEx(btn, 2, DARKGRAY);
        std::string text = std::to_string(q.options[i]);
        int tw = MeasureText(text.c_str(), 30);
        DrawText(text.c_str(),
                 static_cast<int>(btn.x) + (BTN_W - tw) / 2,
                 static_cast<int>(btn.y) + (BTN_H - 30) / 2,
                 30, BLACK);
    }
}

void Renderer::drawResult(bool wasCorrect) const {
    const char* msg = wasCorrect ? "Correct!" : "Wrong!";
    Color color = wasCorrect ? DARKGREEN : RED;
    DrawText(msg,
             GAME_AREA_W / 2 - MeasureText(msg, 40) / 2,
             400, 40, color);
}

int Renderer::getClickedOption() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;
    for (int i = 0; i < 3; i++) {
        if (CheckCollisionPointRec(GetMousePosition(), optionButton(i))) return i;
    }
    return -1;
}

