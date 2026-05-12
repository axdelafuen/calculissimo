#pragma once
#include "MascotDef.h"
#include <raylib.h>

// Mascot owns the loaded 3D model, its animations, and tracks which
// animation is currently playing.
//
// Design choice: the mascot is rendered inside a raylib sub-viewport
// (using ScissorMode + a dedicated Camera3D) so it never interferes with
// the 2D game area on the left.  The Renderer calls Mascot::draw() after
// finishing its 2D pass.
class Mascot {
public:
    Mascot();
    ~Mascot();

    // Mascot is non-copyable because it owns GPU resources.
    Mascot(const Mascot&) = delete;
    Mascot& operator=(const Mascot&) = delete;

    // Load a mascot by index (0..MASCOT_COUNT-1). Unloads any previous one.
    void load(int index);
    void unload();

    bool isLoaded()  const { return loaded_; }
    bool isVisible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }
    void toggleVisible()    { visible_ = !visible_; }

    int  currentIndex() const { return currentIndex_; }
    const char* name()  const;

    // Trigger a one-shot animation; returns to idle when it finishes.
    void playCorrect();
    void playWrong();

    // Update animation frame (call every game frame).
    void update(float dt);

    // Draw the mascot into the right panel [panelX .. panelX+panelW].
    void draw(int panelX, int panelW, int screenH) const;

private:
    void playAnim(int animIndex, bool loop);

    Model      model_;
    ModelAnimation* anims_    = nullptr;
    int             animCount_ = 0;

    int  currentIndex_ = -1;
    bool loaded_       = false;
    bool visible_      = true;

    // Current animation playback state
    int   playingAnimIndex_ = 0;
    bool  loopAnim_         = true;
    float animFrame_        = 0.0f;   // fractional frame counter
};
