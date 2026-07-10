# Compile

Firstly clone SDL3 `git clone https://github.com/libsdl-org/SDL.git vendored/SDL`.
and SDL3_ttf: `git clone https://github.com/libsdl-org/SDL_ttf.git vendored/SDL_ttf`

Then, compile with CMake:

```
cmake -S . -B build
cmake --build build
```
