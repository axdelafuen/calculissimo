#pragma once
#include <string>
#include <ctime>

// One recorded answer attempt.
struct HistoryEntry {
    std::string  question;       // human-readable, e.g. "3 x 7 = ?"
    int          givenAnswer;
    int          correctAnswer;
    bool         wasCorrect;
    std::time_t  timestamp;
};
