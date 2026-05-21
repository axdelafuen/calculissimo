#include "history/History.h"
#include <fstream>
#include <sstream>
#include <algorithm>

History::History(const std::string& filePath) : filePath_(filePath) {
    load();
}

void History::record(const HistoryEntry& entry) {
    entries_.push_back(entry);
    save();   // persist immediately so data survives crashes
}

void History::save() const {
    std::ofstream out(filePath_);
    if (!out.is_open()) return;

    out << "timestamp,question,givenAnswer,correctAnswer,wasCorrect\n";
    for (const auto& e : entries_) {
        out << static_cast<long long>(e.timestamp) << ","
            << escapeCsv(e.question) << ","
            << e.givenAnswer << ","
            << e.correctAnswer << ","
            << (e.wasCorrect ? "1" : "0") << "\n";
    }
}

void History::load() {
    entries_.clear();
    std::ifstream in(filePath_);
    if (!in.is_open()) return;

    std::string line;
    std::getline(in, line);   // skip header

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;
        HistoryEntry e;

        std::getline(ss, token, ','); e.timestamp     = static_cast<std::time_t>(std::stoll(token));
        std::getline(ss, token, ','); e.question       = unescapeCsv(token);
        std::getline(ss, token, ','); e.givenAnswer    = std::stoi(token);
        std::getline(ss, token, ','); e.correctAnswer  = std::stoi(token);
        std::getline(ss, token, ','); e.wasCorrect     = (token == "1");

        entries_.push_back(e);
    }
}

void History::clear() {
    entries_.clear();
    save();
}

std::string History::escapeCsv(const std::string& s) {
    // Wrap in quotes if the string contains commas or quotes
    if (s.find_first_of(",\"") == std::string::npos) return s;
    std::string out = "\"";
    for (char c : s) {
        if (c == '"') out += "\"\"";
        else          out += c;
    }
    out += "\"";
    return out;
}

std::string History::unescapeCsv(const std::string& s) {
    if (s.size() < 2 || s.front() != '"') return s;
    std::string out;
    for (std::size_t i = 1; i + 1 < s.size(); ++i) {
        if (s[i] == '"' && s[i + 1] == '"') { out += '"'; ++i; }
        else out += s[i];
    }
    return out;
}
