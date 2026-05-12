#pragma once
#include <string>

// A badge is a one-time achievement that the player unlocks by meeting
// a specific condition.  Conditions are checked after every answered
// question so the player gets immediate feedback.
//
// Each badge has:
//   - id:          unique identifier used for persistence
//   - name:        short display name
//   - description: explains how it is earned
//   - unlocked:    true once the condition has been met at least once
struct Badge {
    const char* id;
    const char* name;
    const char* description;
    bool        unlocked = false;
};
