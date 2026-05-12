#include "Mascot.h"
#include <rlgl.h>
#include <cstring>
#include <cmath>

static constexpr float ANIM_FPS = 30.0f;

Mascot::Mascot() = default;

Mascot::~Mascot() {
    unload();
}

void Mascot::unload() {
    if (!loaded_) return;
    if (anims_ && animCount_ > 0)
        UnloadModelAnimations(anims_, animCount_);
    UnloadModel(model_);
    anims_     = nullptr;
    animCount_ = 0;
    loaded_    = false;
}

// Base colors to use when the GLTF textures are unavailable (the asset files
// reference absolute Windows-style paths that don't exist in this repo).
// Each color matches the character's original appearance as closely as possible.
static const Color MASCOT_COLORS[] = {
    {90,  60,  110, 255},  // Deathy — dark purple (death-themed murloc)
    {40,  80,  160, 255},  // Gurgl  — deep blue   (murlocbabyblueblack)
    {210, 110, 160, 255},  // Gurky  — pink        (murlocbabypink)
    {185, 210, 225, 255},  // Lurky  — pale white  (murlocbabywhite)
};

void Mascot::load(int index) {
    unload();
    if (index < 0 || index >= MASCOT_COUNT) return;

    const MascotDef& def = MASCOTS[index];
    model_        = LoadModel(def.gltfPath);
    anims_        = LoadModelAnimations(def.gltfPath, &animCount_);
    currentIndex_ = index;
    loaded_       = true;

    // Apply a characteristic color to every material so the model is not
    // rendered as a featureless white blob when textures are missing.
    Color tint = MASCOT_COLORS[index];
    for (int i = 0; i < model_.materialCount; ++i)
        model_.materials[i].maps[MATERIAL_MAP_ALBEDO].color = tint;

    playAnim(def.idleAnim, true);
}

const char* Mascot::name() const {
    if (currentIndex_ < 0 || currentIndex_ >= MASCOT_COUNT) return "None";
    return MASCOTS[currentIndex_].name;
}

// ---------------------------------------------------------------------------
// Animation helpers
// ---------------------------------------------------------------------------

void Mascot::playAnim(int animIndex, bool loop) {
    if (!loaded_ || animIndex < 0 || animIndex >= animCount_) return;
    playingAnimIndex_ = animIndex;
    loopAnim_         = loop;
    animFrame_        = 0.0f;
}

void Mascot::playCorrect() {
    if (!loaded_) return;
    playAnim(MASCOTS[currentIndex_].correctAnim, false);
}

void Mascot::playWrong() {
    if (!loaded_) return;
    playAnim(MASCOTS[currentIndex_].wrongAnim, false);
}

void Mascot::update(float dt) {
    if (!loaded_ || !visible_ || animCount_ == 0) return;

    const ModelAnimation& anim = anims_[playingAnimIndex_];
    animFrame_ += dt * ANIM_FPS;

    if (static_cast<int>(animFrame_) >= anim.frameCount) {
        if (loopAnim_) {
            // Wrap around for looping animations
            animFrame_ = fmod(animFrame_, static_cast<float>(anim.frameCount));
        } else {
            // One-shot: clamp at last frame then return to idle
            animFrame_ = static_cast<float>(anim.frameCount - 1);
            playAnim(MASCOTS[currentIndex_].idleAnim, true);
        }
    }

    UpdateModelAnimation(model_, anim, static_cast<int>(animFrame_));
}

// ---------------------------------------------------------------------------
// Rendering
// ---------------------------------------------------------------------------

void Mascot::draw(int panelX, int panelW, int screenH) const {
    if (!loaded_ || !visible_) return;

    // Constrain drawing to the mascot panel using scissor test
    BeginScissorMode(panelX, 0, panelW, screenH);

    // Panel background
    DrawRectangle(panelX, 0, panelW, screenH, {230, 230, 240, 255});

    // 3D scene
    Camera3D cam = {};
    cam.position   = {0.0f, 1.5f, 3.5f};
    cam.target     = {0.0f, 0.8f, 0.0f};
    cam.up         = {0.0f, 1.0f, 0.0f};
    cam.fovy       = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    // Shift the viewport so the 3D scene is centered in the panel.
    // We use rlViewport to move the OpenGL viewport into the panel area.
    rlViewport(panelX, 0, panelW, screenH);

    BeginMode3D(cam);
        DrawModel(model_, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        DrawGrid(4, 0.5f);
    EndMode3D();

    // Restore full viewport for subsequent 2D drawing
    rlViewport(0, 0, panelX + panelW, screenH);

    // Mascot name label
    const char* lbl = name();
    int lw = MeasureText(lbl, 18);
    DrawText(lbl, panelX + (panelW - lw) / 2, screenH - 30, 18, DARKGRAY);

    EndScissorMode();
}
