#pragma once
#include <string>

// Describes one mascot: its display name, the path to its GLTF file,
// and animation indices for each game event.
// Animation indices refer to the order they appear in the GLTF file.
// They were determined by inspecting each .gltf file directly.
struct MascotDef {
    const char* name;
    const char* gltfPath;
    int idleAnim;    // Stand
    int correctAnim; // celebration on correct answer
    int wrongAnim;   // reaction on wrong answer
};

// The four available mascots.
// Indices are stable because we use the .gltf files (not .glb).
inline constexpr MascotDef MASCOTS[] = {
    // Deathy: idx 1=Stand, 8=AttackUnarmed, 13=CombatWound
    {"Deathy", "assets/deathy/Deathy.gltf", 1, 8, 13},
    // Gurgl:  idx 0=Stand, 4=AttackUnarmed, 9=CombatWound
    {"Gurgl",  "assets/gurgl/Gurgl.gltf",   0, 4,  9},
    // Gurky:  idx 0=Stand, 15=EmoteDance,  20=CombatWound
    {"Gurky",  "assets/gurky/Gurky.gltf",   0, 15, 20},
    // Lurky:  idx 0=Stand, 15=EmoteDance,  20=CombatWound
    {"Lurky",  "assets/lurky/Lurky.gltf",   0, 15, 20},
};

inline constexpr int MASCOT_COUNT = static_cast<int>(sizeof(MASCOTS) / sizeof(MASCOTS[0]));
