#pragma once
#include "mascot/MascotDef.h"
#include <raylib.h>

class Mascot {
public:
    Mascot();
    ~Mascot();

    Mascot(const Mascot&) = delete;
    Mascot& operator=(const Mascot&) = delete;

    void load(int index);
    void unload();

    bool isLoaded()  const { return loaded_; }
    bool isVisible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }
    void toggleVisible()    { visible_ = !visible_; }

    int  currentIndex() const { return currentIndex_; }
    const char* name()  const;

    void playCorrect();
    void playWrong();

    void update(float dt);
    void draw(int panelX, int panelW, int screenH);

private:
    void playAnim(int animIndex, bool loop);
    void ensureTarget(int w, int h);

    Model model_{};
    ModelAnimation* anims_ = nullptr;
    int animCount_ = 0;

    int  currentIndex_ = -1;
    bool loaded_  = false;
    bool visible_ = true;

    int   playingAnimIndex_ = 0;
    bool  loopAnim_ = true;
    int   animFrame_ = 0;

    RenderTexture2D target_{};
    int tgtW_ = 0, tgtH_ = 0;
};
