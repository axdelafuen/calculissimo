#pragma once

struct MascotDef {
    const char* name;
    const char* modelPath;
    int idleAnim;
    int correctAnim;
    int wrongAnim;
};

inline constexpr MascotDef MASCOTS[] = {
    {"Gurky",  "assets/glb/Gurky.glb",  0, 16, 6},
    {"Lurky",  "assets/glb/Lurky.glb",  0, 16, 6},
    {"Murky",  "assets/glb/Murky.glb",  0, 16, 6},
    {"Deathy", "assets/glb/Deathy.glb", 1, 18, 16},
};

inline constexpr int MASCOT_COUNT = static_cast<int>(sizeof(MASCOTS) / sizeof(MASCOTS[0]));
