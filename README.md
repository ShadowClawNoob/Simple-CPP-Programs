# Learning Projects

A public log of my journey learning C++ and systems programming. Each project here is
something I built to understand a concept end to end — written by me, following along
with a tutorial or working from a reference where noted. The point isn't polished
software; it's visible progression. Known gaps are listed honestly per project.

I write all the code myself rather than copy-pasting, so each project reflects my own
formatting, naming, and structuring decisions even where I'm following someone else's
build.

---

## Projects

### 1. Console Tetris

A playable Tetris clone running entirely in the Windows console via the console screen
buffer. My first C++ program.

**Built following** [javidx9 (OneLoneCoder)](https://www.youtube.com/javidx9)'s console
Tetris tutorial. The core design — the rotation formula, the collision-fit scan, and the
`WriteConsoleOutputCharacter` rendering approach — follows his build. All formatting,
naming, and structuring decisions are my own; the code was written by hand, not copied.

- Original author: David Barr (javidx9) — https://www.onelonecoder.com
- Source/reference: https://github.com/OneLoneCoder/Javidx9

**Platform:** Windows only. Uses `<Windows.h>` and the Windows console API
(`CreateConsoleScreenBuffer`, `WriteConsoleOutputCharacter`, `GetAsyncKeyState`), so it
will not build on Linux or macOS.

**Build:**
- Visual Studio 2022: open the `.cpp`, build x64, run.
- Or from a Developer Command Prompt: `cl /EHsc /std:c++17 tetris.cpp`

**Controls:** Left / Right / Down arrows to move, `Z` to rotate.

**Known issues / not yet done** (learning log, not oversights):
- RNG is not seeded yet, so the piece sequence is currently fixed per build. Planned fix:
  seed with `srand` short-term, migrate to `<random>` (`std::mt19937` +
  `uniform_int_distribution`) once I've covered it.
- The two heap buffers (`pointerField`, `screen`) are `new[]`'d and not `delete[]`'d —
  they leak until process exit. Revisiting after I reach dynamic memory / RAII in
  learncpp; intend to move them to `std::vector` so lifetime is owned automatically.

---

<!--
### 2. <Next project title>

One-line description of what it is.

**Built following** <author / course / book>, link.
All code written by hand; formatting and structure my own.

**Platform:** <platform / cross-platform>

**Build:** <how to build and run>

**Known issues / not yet done:**
- ...
-->

## About

I'm learning C++ and systems programming with the long-term goal of building my own
game engine. These projects are the practice reps along the way. Feedback and issues
welcome.

## License

[MIT](LICENSE) — for the code I wrote in this repository. Tutorial-derived projects
credit their original authors in each project's section above; please respect the
original authors' work and licenses if you reuse their material directly.
