# DVD_SDL3
A "simple" project I made which allows you to have an SDL3 bouncing DVD window thingy.

![img.png](img.png)

# Features
- [x] Bouncing capabilities
  - [x] Colour-changing ability
  - [x] Annoying airhorn sfx when hitting a corner near-perfectly
  - [x] No randomness so it follows the same path over and over until you resize the window to set it on a new path lol

# Building
This should (*hopefully*) be cross-platform on anything that SDL3 supports.
## Linux
1. Install CMake
2. Do CMake stuff to build
3. You probably need Ninja, idk

# Story
Basically, I saw [this][yt] YouTube video, where the guy was talking about SDL3's cool stuff. I went to the release they showed in the video, and saw [the SDL3 examples website][sdl3ex].
On this site, I found [this][clipdemo] demo which demonstrated a bouncing clipping box. I'm sure you can see where this is going. Anyway—I tried experimenting with it just being a bouncing DVD logo, then I added colour changing and audio, and then... boom! We're here!

[yt]: https://www.youtube.com/watch?v=PLZXJVPxDTI
[sdl3ex]: https://examples.libsdl.org/SDL3/
[clipdemo]: https://examples.libsdl.org/SDL3/renderer/15-cliprect/