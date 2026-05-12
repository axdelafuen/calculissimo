#include "BadgeManager.h"
#include <fstream>
#include <algorithm>

// ---------------------------------------------------------------------------
// Badge definitions — add new badges here only; no other file needs changes.
// ---------------------------------------------------------------------------
static Badge BADGE_DEFS[] = {
    {"first_blood",   "First Blood",   "Answer your first question correctly", false},
    {"streak_3",      "On a Roll",     "3 correct answers in a row",           false},
    {"streak_5",      "Hot Streak",    "5 correct answers in a row",           false},
    {"streak_10",     "Unstoppable",   "10 correct answers in a row",          false},
    {"total_10",      "Apprentice",    "Answer 10 questions in total",         false},
    {"total_50",      "Practitioner",  "Answer 50 questions in total",         false},
    {"total_100",     "Master",        "Answer 100 questions in total",        false},
    {"perfect_5",     "Flawless Five", "Finish a session of 5+ with 100%",     false},
    {"correct_div",   "Divider",       "Answer a division question correctly", false},
};

BadgeManager::BadgeManager(const std::string& filePath) : filePath_(filePath) {
    // Copy static definitions into the mutable vector
    for (const auto& b : BADGE_DEFS)
        badges_.push_back(b);
    load();
}

bool BadgeManager::unlock(Badge& badge) {
    if (badge.unlocked) return false;
    badge.unlocked = true;
    return true;
}

std::vector<const Badge*> BadgeManager::evaluate(
        const std::vector<HistoryEntry>& history,
        int sessionScore, int sessionTotal) {

    std::vector<const Badge*> newlyUnlocked;

    if (history.empty()) return newlyUnlocked;

    const HistoryEntry& latest = history.back();

    // Helper: find a badge by id
    auto find = [&](const char* id) -> Badge* {
        for (auto& b : badges_)
            if (std::string(b.id) == id) return &b;
        return nullptr;
    };

    auto tryUnlock = [&](const char* id) {
        Badge* b = find(id);
        if (b && unlock(*b)) {
            newlyUnlocked.push_back(b);
        }
    };

    // --- first_blood ---
    if (latest.wasCorrect)
        tryUnlock("first_blood");

    // --- correct_div ---
    if (latest.wasCorrect && latest.question.find('/') != std::string::npos)
        tryUnlock("correct_div");

    // --- streak badges (count consecutive correct at end of history) ---
    int streak = 0;
    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        if (!it->wasCorrect) break;
        ++streak;
    }
    if (streak >= 3)  tryUnlock("streak_3");
    if (streak >= 5)  tryUnlock("streak_5");
    if (streak >= 10) tryUnlock("streak_10");

    // --- total questions ---
    int totalAnswered = static_cast<int>(history.size());
    if (totalAnswered >= 10)  tryUnlock("total_10");
    if (totalAnswered >= 50)  tryUnlock("total_50");
    if (totalAnswered >= 100) tryUnlock("total_100");

    // --- perfect session ---
    if (sessionTotal >= 5 && sessionScore == sessionTotal)
        tryUnlock("perfect_5");

    if (!newlyUnlocked.empty()) save();
    return newlyUnlocked;
}

void BadgeManager::save() const {
    std::ofstream out(filePath_);
    for (const auto& b : badges_)
        if (b.unlocked) out << b.id << "\n";
}

void BadgeManager::load() {
    std::ifstream in(filePath_);
    if (!in.is_open()) return;

    std::string id;
    while (std::getline(in, id)) {
        for (auto& b : badges_)
            if (id == b.id) b.unlocked = true;
    }
}
