# DVD_Raylib
A "simple" project which utilises Raylib to make a bouncing DVD window thingy.

![img.png](img.png)

# Features
- [x] Bouncing capabilities
  - [x] Colour-changing ability
  - [x] Annoying airhorn sfx when hitting a corner near-perfectly
  - [x] No randomness so it follows the same path over and over until you resize the window to set it on a new path lol

# Building
This should (*hopefully*) be cross-platform on anything that Raylib supports.
## Linux
1. Install CMake
2. Do CMake stuff to build
3. You probably need Ninja, idk

   # texture_width = static_cast<float>(texture->w) * (TEXTURE_SIZE / static_cast<float>(texture->h));