# DVD_Raylib
A "simple" project which utilises Raylib to make a bouncing DVD window thingy.

# Other Branches
- [`main`][mbranch] - The same project, made with SDL3.
  - [x] Less SFX
  - [x] **NO** FPS Counter
  - [x] **MORE** lines of code
  - [x] A genuinely awful experience

![img.png](img.png)

# Features
- [x] Bouncing capabilities
  - [x] Colour-changing ability
  - [x] Bounce SFX
  - [x] SFX for when you hit a corner very near-perfectly
  - [x] No randomness so it follows the same path over and over until you resize the window to set it on a new path lol

# Building
This should (*hopefully*) be cross-platform on anything that Raylib supports.
## Linux
1. Install CMake
2. Do CMake stuff to build
3. You probably need Ninja, idk

   # texture_width = static_cast<float>(texture->w) * (TEXTURE_SIZE / static_cast<float>(texture->h));