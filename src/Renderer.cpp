#include "Renderer.h"
#include "MascotDef.h"
#include <string>

// ---------------------------------------------------------------------------
// Window lifecycle
// ---------------------------------------------------------------------------

Renderer::Renderer() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1100, 700, "Calculissimo");
    SetWindowMinSize(MIN_W, MIN_H);
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
// Dynamic layout helpers
// ---------------------------------------------------------------------------

int Renderer::sw() const { return GetScreenWidth(); }
int Renderer::sh() const { return GetScreenHeight(); }
int Renderer::gw() const { return sw() - MASCOT_PANEL_W; }
int Renderer::mx() const { return sw() - MASCOT_PANEL_W; }

// Scale a value designed for DESIGN_H (700 px) to the current window height.
int Renderer::scaled(int v) const {
    int result = static_cast<int>(v * GetScreenHeight() / static_cast<float>(DESIGN_H));
    return result > 0 ? result : 1;
}

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

// Menu title centred in the game area (gw), not the full window.
void Renderer::drawMenuTitle(const char* text) const {
    const int fs = 36;
    DrawText(text, gw() / 2 - MeasureText(text, fs) / 2, scaled(60), fs, DARKBLUE);
}

// Returns the rectangle for the i-th answer option button.
Rectangle Renderer::optionButton(int index) const {
    const int btnW = scaled(180), btnH = scaled(60), spacing = scaled(30);
    int totalW = 3 * btnW + 2 * spacing;
    int startX = (gw() - totalW) / 2;
    return {
        static_cast<float>(startX + index * (btnW + spacing)),
        static_cast<float>(scaled(310)),
        static_cast<float>(btnW),
        static_cast<float>(btnH)
    };
}

// ---------------------------------------------------------------------------
// Operation menu
// ---------------------------------------------------------------------------

void Renderer::drawOperationMenu(const std::vector<std::string>& names) const {
    drawMenuTitle("Choose an operation");

    const int btnW = scaled(160), btnH = scaled(55), spacing = scaled(25);
    int n      = static_cast<int>(names.size());
    int totalW = n * btnW + (n - 1) * spacing;
    int startX = (gw() - totalW) / 2;  // centre in game area, not full screen

    for (int i = 0; i < n; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            static_cast<float>(scaled(160)),
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        drawButton(btn, names[i].c_str(), 28, LIGHTGRAY, SKYBLUE, BLACK);
    }
}

int Renderer::getClickedOperation(const std::vector<std::string>& names) const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;

    const int btnW = scaled(160), btnH = scaled(55), spacing = scaled(25);
    int n      = static_cast<int>(names.size());
    int totalW = n * btnW + (n - 1) * spacing;
    int startX = (gw() - totalW) / 2;

    for (int i = 0; i < n; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            static_cast<float>(scaled(160)),
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
        "Small numbers | 8 questions | No limit",
        "Normal numbers | 10 questions | 90 s",
        "Large numbers | 15 questions | 60 s"
    };

    const int btnW = scaled(200), btnH = scaled(60), spacing = scaled(30);
    int totalW = 3 * btnW + 2 * spacing;
    int startX = (gw() - totalW) / 2;   // centre in game area

    for (int i = 0; i < 3; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            static_cast<float>(scaled(160)),
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        drawButton(btn, DIFF_LABELS[i], 26, DIFF_COLORS[i],
                   ColorBrightness(DIFF_COLORS[i], 0.3f), BLACK);

        int sw_ = MeasureText(subtitles[i], 13);
        DrawText(subtitles[i],
                 static_cast<int>(btn.x) + (btnW - sw_) / 2,
                 static_cast<int>(btn.y + btn.height) + scaled(8),
                 13, DARKGRAY);
    }
}

int Renderer::getClickedDifficulty() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;

    const int btnW = scaled(200), btnH = scaled(60), spacing = scaled(30);
    int totalW = 3 * btnW + 2 * spacing;
    int startX = (gw() - totalW) / 2;

    for (int i = 0; i < 3; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            static_cast<float>(scaled(160)),
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
             gw() / 2 - MeasureText(title, 30) / 2,
             scaled(20), 30, DARKBLUE);
}

void Renderer::drawScore(int score, int total) const {
    std::string text = "Score: " + std::to_string(score) + " / " + std::to_string(total);
    DrawText(text.c_str(),
             gw() / 2 - MeasureText(text.c_str(), 22) / 2,
             scaled(62), 22, GRAY);
}

void Renderer::drawQuestion(const Question& q) const {
    std::string text = std::to_string(q.a) + " " + q.op + " " + std::to_string(q.b) + " = ?";
    const int fs = 52;
    DrawText(text.c_str(),
             gw() / 2 - MeasureText(text.c_str(), fs) / 2,
             scaled(180), fs, BLACK);
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
                 static_cast<int>(btn.x) + (static_cast<int>(btn.width)  - tw) / 2,
                 static_cast<int>(btn.y) + (static_cast<int>(btn.height) - 30) / 2,
                 30, BLACK);
    }
}

void Renderer::drawResult(bool wasCorrect) const {
    const char* msg = wasCorrect ? "Correct!" : "Wrong!";
    Color color = wasCorrect ? DARKGREEN : RED;
    DrawText(msg,
             gw() / 2 - MeasureText(msg, 40) / 2,
             scaled(400), 40, color);
}

int Renderer::getClickedOption() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;
    for (int i = 0; i < 3; i++) {
        if (CheckCollisionPointRec(GetMousePosition(), optionButton(i))) return i;
    }
    return -1;
}

void Renderer::drawTimer(float timeLeft, float timeTotal) const {
    if (timeTotal <= 0.0f) return;

    const int barX = scaled(20), barY = scaled(20), barH = scaled(16);
    const int barW = gw() - 2 * barX;

    float ratio  = (timeLeft > 0.0f) ? timeLeft / timeTotal : 0.0f;
    int   filled = static_cast<int>(barW * ratio);

    DrawRectangle(barX, barY, barW, barH, LIGHTGRAY);
    Color fill = (ratio > 0.5f) ? GREEN : (ratio > 0.25f) ? ORANGE : RED;
    DrawRectangle(barX, barY, filled, barH, fill);
    DrawRectangleLinesEx({(float)barX, (float)barY, (float)barW, (float)barH}, 2, DARKGRAY);

    std::string label = std::to_string(static_cast<int>(timeLeft)) + "s";
    DrawText(label.c_str(), barX + barW + scaled(6), barY, 16, DARKGRAY);
}

// ---------------------------------------------------------------------------
// Results screen
// ---------------------------------------------------------------------------

void Renderer::drawResults(int score, int total, const char* diffLabel,
                            int allTimeCorrect, int allTimeTotal) const {
    drawMenuTitle("Session over!");

    std::string sub = std::string("Difficulty: ") + diffLabel;
    DrawText(sub.c_str(),
             gw() / 2 - MeasureText(sub.c_str(), 22) / 2,
             scaled(120), 22, DARKGRAY);

    std::string scoreStr = std::to_string(score) + " / " + std::to_string(total) + " correct";
    const int bigFs = 48;
    DrawText(scoreStr.c_str(),
             gw() / 2 - MeasureText(scoreStr.c_str(), bigFs) / 2,
             scaled(200), bigFs, BLACK);

    float pct = (total > 0) ? static_cast<float>(score) / total : 0.0f;
    const char* feedback;
    Color fbColor;
    if      (pct >= 0.9f) { feedback = "Excellent!";     fbColor = DARKGREEN; }
    else if (pct >= 0.7f) { feedback = "Good job!";      fbColor = GREEN;     }
    else if (pct >= 0.5f) { feedback = "Keep going!";    fbColor = ORANGE;    }
    else                  { feedback = "Practice more!"; fbColor = RED;       }

    DrawText(feedback,
             gw() / 2 - MeasureText(feedback, 32) / 2,
             scaled(270), 32, fbColor);

    // All-time stats
    if (allTimeTotal > 0) {
        std::string hist = "All-time: " + std::to_string(allTimeCorrect)
                         + " / " + std::to_string(allTimeTotal) + " correct";
        DrawText(hist.c_str(),
                 gw() / 2 - MeasureText(hist.c_str(), 18) / 2,
                 scaled(320), 18, GRAY);
    }

    // "Play again" button
    const int btnW = scaled(220), btnH = scaled(55);
    Rectangle btn = {
        static_cast<float>(gw() / 2 - btnW / 2),
        static_cast<float>(scaled(360)),
        static_cast<float>(btnW),
        static_cast<float>(btnH)
    };
    drawButton(btn, "Play again", 26, SKYBLUE, ColorBrightness(SKYBLUE, 0.3f), BLACK);
}

bool Renderer::getClickedPlayAgain() const {
    const int btnW = scaled(220), btnH = scaled(55);
    Rectangle btn = {
        static_cast<float>(gw() / 2 - btnW / 2),
        static_cast<float>(scaled(360)),
        static_cast<float>(btnW),
        static_cast<float>(btnH)
    };
    return isClicked(btn);
}

// ---------------------------------------------------------------------------
// Badge notifications & list
// ---------------------------------------------------------------------------

void Renderer::drawBadgeNotification(const char* name, const char* desc) const {
    const int toastW = scaled(360), toastH = scaled(60);
    const int toastX = scaled(10),  toastY = sh() - toastH - scaled(10);
    DrawRectangle(toastX, toastY, toastW, toastH, {255, 220, 50, 230});
    DrawRectangleLinesEx({(float)toastX, (float)toastY,
                          (float)toastW, (float)toastH}, 2, GOLD);
    std::string header = std::string("Badge unlocked: ") + name;
    DrawText(header.c_str(), toastX + scaled(8), toastY + scaled(8),  16, BLACK);
    DrawText(desc,            toastX + scaled(8), toastY + scaled(34), 13, DARKGRAY);
}

void Renderer::drawBadgeList(const std::vector<Badge>& badges) const {
    const int cols  = 3;
    const int cellW = scaled(210), cellH = scaled(34), padX = scaled(16);
    int totalW = cols * cellW + (cols - 1) * padX;
    int startX = (gw() - totalW) / 2;
    int startY = scaled(440);

    for (int i = 0; i < static_cast<int>(badges.size()); ++i) {
        int col = i % cols;
        int row = i / cols;
        int x = startX + col * (cellW + padX);
        int y = startY + row * (cellH + scaled(5));

        if (y + cellH > sh()) break;   // don't draw off-screen at small heights

        Color bg  = badges[i].unlocked ? (Color){180, 240, 180, 255}
                                       : (Color){220, 220, 220, 180};
        Color txt = badges[i].unlocked ? BLACK : GRAY;
        DrawRectangle(x, y, cellW, cellH, bg);
        DrawRectangleLinesEx({(float)x, (float)y, (float)cellW, (float)cellH},
                             1, DARKGRAY);

        std::string label = std::string(badges[i].unlocked ? "* " : "o ") + badges[i].name;
        DrawText(label.c_str(), x + scaled(6), y + (cellH - 15) / 2, 15, txt);
    }
}

// ---------------------------------------------------------------------------
// Mascot menu
// ---------------------------------------------------------------------------

void Renderer::drawMascotMenu(int selectedIdx, bool visible) const {
    drawMenuTitle("Choose your mascot");

    const int btnW = scaled(160), btnH = scaled(50), spacing = scaled(20);
    int totalW = MASCOT_COUNT * btnW + (MASCOT_COUNT - 1) * spacing;
    int startX = (gw() - totalW) / 2;

    for (int i = 0; i < MASCOT_COUNT; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            static_cast<float>(scaled(160)),
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        Color idle = (i == selectedIdx) ? GOLD : LIGHTGRAY;
        drawButton(btn, MASCOTS[i].name, 22, idle, SKYBLUE, BLACK);
    }

    const int togW = scaled(240), togH = scaled(50);
    Rectangle toggleBtn = {
        static_cast<float>(gw() / 2 - togW / 2),
        static_cast<float>(scaled(230)),
        static_cast<float>(togW),
        static_cast<float>(togH)
    };
    const char* toggleLabel = visible ? "Hide mascot" : "Show mascot";
    drawButton(toggleBtn, toggleLabel, 22, LIGHTGRAY, ORANGE, BLACK);

    DrawText("Press M or ESC to close",
             gw() / 2 - MeasureText("Press M or ESC to close", 15) / 2,
             scaled(300), 15, GRAY);
}

int Renderer::getClickedMascot() const {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return -1;
    const int btnW = scaled(160), btnH = scaled(50), spacing = scaled(20);
    int totalW = MASCOT_COUNT * btnW + (MASCOT_COUNT - 1) * spacing;
    int startX = (gw() - totalW) / 2;
    for (int i = 0; i < MASCOT_COUNT; ++i) {
        Rectangle btn = {
            static_cast<float>(startX + i * (btnW + spacing)),
            static_cast<float>(scaled(160)),
            static_cast<float>(btnW),
            static_cast<float>(btnH)
        };
        if (CheckCollisionPointRec(GetMousePosition(), btn)) return i;
    }
    return -1;
}

bool Renderer::getClickedToggleMascot() const {
    const int togW = scaled(240), togH = scaled(50);
    Rectangle btn = {
        static_cast<float>(gw() / 2 - togW / 2),
        static_cast<float>(scaled(230)),
        static_cast<float>(togW),
        static_cast<float>(togH)
    };
    return isClicked(btn);
}
