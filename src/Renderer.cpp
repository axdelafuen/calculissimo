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

void Renderer::drawTimer(float timeLeft, float timeTotal) const {
    if (timeTotal <= 0.0f) return;   // unlimited mode — nothing to show

    const int barX = 20, barY = 20, barH = 16;
    const int barW = GAME_AREA_W - 40;

    float ratio = (timeLeft > 0.0f) ? timeLeft / timeTotal : 0.0f;
    int filled   = static_cast<int>(barW * ratio);

    // Background track
    DrawRectangle(barX, barY, barW, barH, LIGHTGRAY);
    // Filled portion — green → orange → red as time runs out
    Color fill = (ratio > 0.5f) ? GREEN : (ratio > 0.25f) ? ORANGE : RED;
    DrawRectangle(barX, barY, filled, barH, fill);
    DrawRectangleLinesEx({(float)barX, (float)barY, (float)barW, (float)barH}, 2, DARKGRAY);

    // Remaining seconds
    std::string label = std::to_string(static_cast<int>(timeLeft)) + "s";
    DrawText(label.c_str(), barX + barW + 6, barY, 16, DARKGRAY);
}

// ---------------------------------------------------------------------------
// Results screen
// ---------------------------------------------------------------------------

void Renderer::drawResults(int score, int total, const char* diffLabel,
                            int allTimeCorrect, int allTimeTotal) const {
    drawMenuTitle("Session over!");

    std::string sub = std::string("Difficulty: ") + diffLabel;
    DrawText(sub.c_str(),
             SCREEN_W / 2 - MeasureText(sub.c_str(), 22) / 2,
             120, 22, DARKGRAY);

    std::string scoreStr = std::to_string(score) + " / " + std::to_string(total) + " correct";
    DrawText(scoreStr.c_str(),
             SCREEN_W / 2 - MeasureText(scoreStr.c_str(), 48) / 2,
             210, 48, BLACK);

    float pct = (total > 0) ? (float)score / total : 0.0f;
    const char* feedback;
    Color fbColor;
    if      (pct >= 0.9f) { feedback = "Excellent!";  fbColor = DARKGREEN; }
    else if (pct >= 0.7f) { feedback = "Good job!";   fbColor = GREEN;     }
    else if (pct >= 0.5f) { feedback = "Keep going!"; fbColor = ORANGE;    }
    else                  { feedback = "Practice more!"; fbColor = RED;     }

    DrawText(feedback,
             SCREEN_W / 2 - MeasureText(feedback, 32) / 2,
             280, 32, fbColor);

    // "Play again" button
    Rectangle btn = {(float)(SCREEN_W / 2 - 110), 370.0f, 220.0f, 55.0f};
    drawButton(btn, "Play again", 26, SKYBLUE, ColorBrightness(SKYBLUE, 0.3f), BLACK);

    // All-time stats from history
    if (allTimeTotal > 0) {
        std::string hist = "All-time: " + std::to_string(allTimeCorrect)
                         + " / " + std::to_string(allTimeTotal) + " correct";
        DrawText(hist.c_str(),
                 SCREEN_W / 2 - MeasureText(hist.c_str(), 18) / 2,
                 445, 18, GRAY);
    }
}

bool Renderer::getClickedPlayAgain() const {
    Rectangle btn = {(float)(SCREEN_W / 2 - 110), 370.0f, 220.0f, 55.0f};
    return isClicked(btn);
}

void Renderer::drawBadgeNotification(const char* name, const char* desc) const {
    // Toast in the bottom-left of the game area
    const int toastW = 340, toastH = 60;
    const int toastX = 10, toastY = SCREEN_H - toastH - 10;
    DrawRectangle(toastX, toastY, toastW, toastH, {255, 220, 50, 230});
    DrawRectangleLinesEx({(float)toastX, (float)toastY, (float)toastW, (float)toastH}, 2, GOLD);
    std::string header = std::string("Badge unlocked: ") + name;
    DrawText(header.c_str(), toastX + 8, toastY + 8,  16, BLACK);
    DrawText(desc,            toastX + 8, toastY + 30, 14, DARKGRAY);
}

void Renderer::drawBadgeList(const std::vector<Badge>& badges) const {
    // Small badge grid below the main results content
    const int startY = 480, cols = 3;
    const int cellW = 220, cellH = 36, padX = 20;
    int totalW = cols * cellW + (cols - 1) * padX;
    int startX = (GAME_AREA_W - totalW) / 2;

    for (int i = 0; i < static_cast<int>(badges.size()); ++i) {
        int col = i % cols;
        int row = i / cols;
        int x = startX + col * (cellW + padX);
        int y = startY + row * (cellH + 6);

        Color bg  = badges[i].unlocked ? (Color){180, 240, 180, 255} : (Color){220, 220, 220, 180};
        Color txt = badges[i].unlocked ? BLACK : GRAY;
        DrawRectangle(x, y, cellW, cellH, bg);
        DrawRectangleLinesEx({(float)x, (float)y, (float)cellW, (float)cellH}, 1, DARKGRAY);

        std::string label = std::string(badges[i].unlocked ? "★ " : "○ ") + badges[i].name;
        DrawText(label.c_str(), x + 6, y + (cellH - 16) / 2, 16, txt);
    }
}

// ---------------------------------------------------------------------------
// Mascot menu
// ---------------------------------------------------------------------------
#include "MascotDef.h"

void Renderer::drawMascotMenu(int selectedIdx, bool visible) const {
    drawMenuTitle("Choose your mascot");

    const int btnW = 160, btnH = 50, spacing = 20;
    int totalW = MASCOT_COUNT * btnW + (MASCOT_COUNT - 1) * spacing;
    int startX = (SCREEN_W - totalW) / 2;

    for (int i = 0; i < MASCOT_COUNT; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            160.0f, static_cast<float>(btnW), static_cast<float>(btnH)
        };
        Color idle = (i == selectedIdx) ? GOLD : LIGHTGRAY;
        drawButton(btn, MASCOTS[i].name, 22, idle, SKYBLUE, BLACK);
    }

    // Toggle visibility button
    Rectangle toggleBtn = {(float)(SCREEN_W / 2 - 120), 240.0f, 240.0f, 50.0f};
    const char* toggleLabel = visible ? "Hide mascot" : "Show mascot";
    drawButton(toggleBtn, toggleLabel, 22, LIGHTGRAY, ORANGE, BLACK);
}

int Renderer::getClickedMascot() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;
    const int btnW = 160, btnH = 50, spacing = 20;
    int totalW = MASCOT_COUNT * btnW + (MASCOT_COUNT - 1) * spacing;
    int startX = (SCREEN_W - totalW) / 2;
    for (int i = 0; i < MASCOT_COUNT; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            160.0f, static_cast<float>(btnW), static_cast<float>(btnH)
        };
        if (CheckCollisionPointRec(GetMousePosition(), btn)) return i;
    }
    return -1;
}

bool Renderer::getClickedToggleMascot() const {
    Rectangle btn = {(float)(SCREEN_W / 2 - 120), 240.0f, 240.0f, 50.0f};
    return isClicked(btn);
}

