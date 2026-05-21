#include "mascot/Mascot.h"

Mascot::Mascot() = default;

Mascot::~Mascot() {
    unload();
    if (tgtW_ > 0) UnloadRenderTexture(target_);
}

void Mascot::unload() {
    if (!loaded_) return;
    if (anims_ && animCount_ > 0)
        UnloadModelAnimations(anims_, animCount_);
    UnloadModel(model_);
    anims_ = nullptr;
    animCount_ = 0;
    loaded_ = false;
}

void Mascot::load(int index) {
    unload();
    if (index < 0 || index >= MASCOT_COUNT) return;

    const MascotDef& def = MASCOTS[index];
    model_ = LoadModel(def.modelPath);
    anims_ = LoadModelAnimations(def.modelPath, &animCount_);
    currentIndex_ = index;
    loaded_ = true;

    playAnim(def.idleAnim, true);
}

const char* Mascot::name() const {
    if (currentIndex_ < 0 || currentIndex_ >= MASCOT_COUNT) return "None";
    return MASCOTS[currentIndex_].name;
}

void Mascot::playAnim(int animIndex, bool loop) {
    if (!loaded_ || animIndex < 0 || animIndex >= animCount_) return;
    playingAnimIndex_ = animIndex;
    loopAnim_ = loop;
    animFrame_ = 0;
}

void Mascot::playCorrect() {
    if (!loaded_) return;
    playAnim(MASCOTS[currentIndex_].correctAnim, false);
}

void Mascot::playWrong() {
    if (!loaded_) return;
    playAnim(MASCOTS[currentIndex_].wrongAnim, false);
}

void Mascot::update(float /*dt*/) {
    // Animations disabled: WoW GLB exports not compatible with raylib's UpdateModelAnimation
}

void Mascot::ensureTarget(int w, int h) {
    if (tgtW_ == w && tgtH_ == h) return;
    if (tgtW_ > 0) UnloadRenderTexture(target_);
    target_ = LoadRenderTexture(w, h);
    tgtW_ = w;
    tgtH_ = h;
}

void Mascot::draw(int panelX, int panelW, int screenH) {
    if (!loaded_ || !visible_) return;

    int h = screenH / 2;
    ensureTarget(panelW, h);

    Camera3D cam = {};
    cam.position   = {5.0f, 4.0f, -5.0f};
    cam.target     = {0.0f, 1.0f, 0.0f};
    cam.up         = {0.0f, 1.0f, 0.0f};
    cam.fovy       = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    BeginTextureMode(target_);
        ClearBackground(BLANK);
        BeginMode3D(cam);
            DrawModel(model_, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        EndMode3D();
    EndTextureMode();

    // Draw render texture in bottom-right (flip Y for correct orientation)
    Rectangle src = {0, 0, (float)tgtW_, -(float)tgtH_};
    Rectangle dst = {(float)panelX, (float)(screenH - h), (float)panelW, (float)h};
    DrawTexturePro(target_.texture, src, dst, {0, 0}, 0.0f, WHITE);

    const char* lbl = name();
    int lw = MeasureText(lbl, 18);
    DrawText(lbl, panelX + (panelW - lw) / 2, screenH - 30, 18, DARKGRAY);
}
