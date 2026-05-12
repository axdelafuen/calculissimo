# Calculissimo

A mental-arithmetic trainer built in C++17 with [raylib](https://www.raylib.com/).

---

## How to compile

**Requirements:** CMake ≥ 3.10, a C++17 compiler, OpenGL, an internet connection on the first build (raylib is fetched automatically via CMake FetchContent).

```bash
cmake -B build
cmake --build build
```

Or use the provided helper script which configures, builds, and runs in one step:

```bash
./autorun.sh
```

To clean the build:

```bash
./autorun.sh clean
```

---

## How to run

```bash
./build/calculissimo
```

The binary must be run from the project root so that the `assets/` folder is found correctly.

---

## Features

### Core gameplay
The player is presented with an arithmetic question and must pick the correct answer from three options. Four operations are available: **+**, **−**, **×**, **÷**. Division always produces integer results.

### 1 · Difficulty levels
Three levels selectable before each session:

| Level  | Operand range | Questions | Time limit |
|--------|---------------|-----------|------------|
| Easy   | 1 – 10        | 8         | none       |
| Medium | 1 – 20        | 10        | 90 s       |
| Hard   | 5 – 50        | 15        | 60 s       |

The difficulty also controls how far wrong answers are from the correct one.

### 2 · Timed sessions
Medium and Hard difficulties have a countdown timer shown as a colour-coded bar (green → orange → red). The session ends when either the question quota is reached or the timer runs out. A **Results** screen then shows the score, a performance rating, and all-time statistics.

### 3 · History tracking
Every answered question is appended to `history.csv` with its timestamp, question text, given answer, correct answer, and whether it was right. The all-time correct/total ratio is displayed on the Results screen.

### 4 · 3D Mascot
A 3D character is displayed in the right panel throughout the game. Press **M** at any time to open the mascot menu where you can:
- Switch between the four mascots: **Deathy**, **Gurgl**, **Gurky**, **Lurky**.
- Toggle mascot visibility on/off.

The mascot reacts to gameplay: it plays a celebration animation on a correct answer and a wound animation on a wrong one, then returns to its idle loop.

### 5 · Badge system (gamification)
Badges are unlocked by meeting cumulative conditions. A toast notification appears when a badge is earned. The full badge list (locked/unlocked) is shown on the Results screen. Unlocked badges persist in `badges.dat`.

| Badge          | Condition                              |
|----------------|----------------------------------------|
| First Blood    | Answer first question correctly        |
| On a Roll      | 3 correct answers in a row             |
| Hot Streak     | 5 correct answers in a row             |
| Unstoppable    | 10 correct answers in a row            |
| Apprentice     | 10 questions answered in total         |
| Practitioner   | 50 questions answered in total         |
| Master         | 100 questions answered in total        |
| Flawless Five  | Perfect score on a session of 5+       |
| Divider        | Correct answer on a division question  |

---

## Project structure

```
calculissimo/
├── assets/               # 3D mascot models (GLTF + BIN)
│   ├── deathy/
│   ├── gurgl/
│   ├── gurky/
│   └── lurky/
├── include/              # All header files
│   ├── Badge.h           # Badge data structure
│   ├── BadgeManager.h    # Badge evaluation & persistence
│   ├── Difficulty.h      # Difficulty enum + config
│   ├── Game.h            # Game state machine
│   ├── History.h / HistoryEntry.h
│   ├── Mascot.h / MascotDef.h
│   ├── Operation.h       # Abstract operation interface
│   ├── Operations.h      # +, -, x, / implementations
│   ├── Question.h        # Question data structure
│   ├── QuestionGenerator.h
│   └── Renderer.h        # All drawing logic
├── src/                  # Implementation files
│   ├── BadgeManager.cpp
│   ├── Game.cpp
│   ├── History.cpp
│   ├── main.cpp
│   ├── Mascot.cpp
│   ├── QuestionGenerator.cpp
│   └── Renderer.cpp
├── tests/                # Unit test entry point
├── CMakeLists.txt
└── autorun.sh
```

### Key architectural decisions

- **State machine in `Game`** — each screen is a dedicated `handle*()` method; adding a new screen means adding one enum value and one method.
- **`Operation` polymorphism** — every arithmetic operation is a subclass; `generateOperands()` is virtual so `Division` can enforce integer results without polluting the generator.
- **`Renderer` isolation** — the renderer only receives plain data and returns clicked indices; swapping the graphics back-end only touches `Renderer`.
- **`Mascot` sub-viewport** — the 3D mascot uses `rlViewport` + scissor mode to draw into a fixed panel without disturbing the 2D layout.
- **`BadgeManager` data-driven** — the badge table is a plain array; adding a new badge is a single-line edit with no other code changes required.
