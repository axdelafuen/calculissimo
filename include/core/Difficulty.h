#pragma once

// Difficulty controls operand ranges, wrong-answer spread, and session length.
// Adding a new difficulty only requires adding an enum value and a row in
// getDifficultyConfig() — no other code needs to change.

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

struct DifficultyConfig {
    int minValue;          // smallest operand value generated
    int maxValue;          // largest operand value generated
    int wrongAnswerRange;  // maximum offset of a wrong answer from the correct one
    int questionCount;     // questions per session
    float timeLimit;       // session time limit in seconds (0 = unlimited)
    const char* label;
};

inline DifficultyConfig getDifficultyConfig(Difficulty d) {
    switch (d) {
        case Difficulty::EASY:   return {1, 10, 3,  8,   0.0f, "Easy"};
        case Difficulty::MEDIUM: return {1, 20, 5, 10,  90.0f, "Medium"};
        case Difficulty::HARD:   return {5, 50, 8, 15,  60.0f, "Hard"};
    }
    return {1, 10, 3, 8, 0.0f, "Easy"};
}
