#pragma once
#include "badge/Badge.h"
#include "history/HistoryEntry.h"
#include <vector>
#include <string>

// BadgeManager owns all badge definitions and checks them against the
// player's history after each answer.
//
// Persistence: unlocked badges are saved to a simple text file (one id
// per line) so they survive between sessions.
class BadgeManager {
public:
    explicit BadgeManager(const std::string& filePath = "badges.dat");

    // Call after every answered question to evaluate conditions.
    // Returns the badges newly unlocked during this call (may be empty).
    std::vector<const Badge*> evaluate(const std::vector<HistoryEntry>& history,
                                       int sessionScore, int sessionTotal);

    const std::vector<Badge>& badges() const { return badges_; }

    void save() const;
    void load();

private:
    std::vector<Badge> badges_;
    std::string        filePath_;

    bool unlock(Badge& badge);
};
