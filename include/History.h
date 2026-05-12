#pragma once
#include "HistoryEntry.h"
#include <vector>
#include <string>

// History keeps the full list of answered questions for the current run and
// can persist/reload them from a CSV file so the player's record survives
// between sessions.
//
// CSV format (one line per entry):
//   timestamp,question,givenAnswer,correctAnswer,wasCorrect
class History {
public:
    explicit History(const std::string& filePath = "history.csv");

    void record(const HistoryEntry& entry);
    void save() const;
    void load();
    void clear();

    const std::vector<HistoryEntry>& entries() const { return entries_; }

private:
    std::vector<HistoryEntry> entries_;
    std::string filePath_;

    // Escapes commas/quotes inside a CSV field.
    static std::string escapeCsv(const std::string& s);
    static std::string unescapeCsv(const std::string& s);
};
