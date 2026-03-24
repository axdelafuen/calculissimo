#include <raylib.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Question {
    int a;
    int b;
    char op;
    int correctAnswer;
    int options[3];
    int correctIndex;
};

Question generateQuestion() {
    Question q;
    q.a = rand() % 20 + 1;
    q.b = rand() % 20 + 1;

    int opType = rand() % 3;
    switch (opType) {
        case 0: q.op = '+'; q.correctAnswer = q.a + q.b; break;
        case 1: q.op = '-'; q.correctAnswer = q.a - q.b; break;
        case 2: q.op = 'x'; q.correctAnswer = q.a * q.b; break;
    }

    q.correctIndex = rand() % 3;

    for (int i = 0; i < 3; i++) {
        if (i == q.correctIndex) {
            q.options[i] = q.correctAnswer;
        } else {
            int wrong;
            do {
                wrong = q.correctAnswer + (rand() % 11) - 5;
            } while (wrong == q.correctAnswer);
            q.options[i] = wrong;
        }
    }

    return q;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Calculissimo");
    SetTargetFPS(60);

    Question current = generateQuestion();
    int score = 0;
    int total = 0;
    bool showResult = false;
    bool wasCorrect = false;
    float resultTimer = 0.0f;

    const int btnWidth = 180;
    const int btnHeight = 60;
    const int btnY = 280;
    const int btnSpacing = 30;
    const int totalBtnsWidth = 3 * btnWidth + 2 * btnSpacing;
    const int btnStartX = (screenWidth - totalBtnsWidth) / 2;

    while (!WindowShouldClose()) {
        if (showResult) {
            resultTimer -= GetFrameTime();
            if (resultTimer <= 0.0f) {
                showResult = false;
                current = generateQuestion();
            }
        }

        if (!showResult && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 3; i++) {
                Rectangle btn = {
                    static_cast<float>(btnStartX + i * (btnWidth + btnSpacing)),
                    static_cast<float>(btnY),
                    static_cast<float>(btnWidth),
                    static_cast<float>(btnHeight)
                };
                if (CheckCollisionPointRec(mouse, btn)) {
                    total++;
                    wasCorrect = (i == current.correctIndex);
                    if (wasCorrect) score++;
                    showResult = true;
                    resultTimer = 1.0f;
                    break;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Title
        DrawText("CALCULISSIMO", screenWidth / 2 - MeasureText("CALCULISSIMO", 30) / 2, 20, 30, DARKGRAY);

        // Score
        std::string scoreText = "Score: " + std::to_string(score) + " / " + std::to_string(total);
        DrawText(scoreText.c_str(), screenWidth / 2 - MeasureText(scoreText.c_str(), 20) / 2, 65, 20, GRAY);

        // Question
        std::string questionText = std::to_string(current.a) + " " + current.op + " " + std::to_string(current.b) + " = ?";
        DrawText(questionText.c_str(), screenWidth / 2 - MeasureText(questionText.c_str(), 50) / 2, 140, 50, BLACK);

        // Buttons
        for (int i = 0; i < 3; i++) {
            Rectangle btn = {
                static_cast<float>(btnStartX + i * (btnWidth + btnSpacing)),
                static_cast<float>(btnY),
                static_cast<float>(btnWidth),
                static_cast<float>(btnHeight)
            };

            Color btnColor = LIGHTGRAY;
            if (showResult) {
                if (i == current.correctIndex) btnColor = GREEN;
                else btnColor = MAROON;
            } else {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, btn)) btnColor = SKYBLUE;
            }

            DrawRectangleRec(btn, btnColor);
            DrawRectangleLinesEx(btn, 2, DARKGRAY);

            std::string optText = std::to_string(current.options[i]);
            DrawText(optText.c_str(),
                     static_cast<int>(btn.x) + btnWidth / 2 - MeasureText(optText.c_str(), 30) / 2,
                     btnY + btnHeight / 2 - 15,
                     30, BLACK);
        }

        // Result feedback
        if (showResult) {
            const char* msg = wasCorrect ? "Correct !" : "Faux !";
            Color msgColor = wasCorrect ? DARKGREEN : RED;
            DrawText(msg, screenWidth / 2 - MeasureText(msg, 40) / 2, 370, 40, msgColor);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
