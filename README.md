# GameEngine
[![Engine Test](https://github.com/nanchen2483/GameEngine/actions/workflows/engine-test.yml/badge.svg)](https://github.com/nanchen2483/GameEngine/actions/workflows/engine-test.yml)

A game engine written in C++ with OpenGL

## Development

### Getting started

Clone repository with submodules (with version 2.13 of [Git](https://git-scm.com) and later)
```powershell
git clone --recurse-submodules -j8 https://github.com/nanchen2483/GameEngine.git
```
For already cloned repos, use:
```powershell
cd GameEngine
git submodule update --init --recursive
```
### Prerequist
- [Visual Studio 2022](https://docs.microsoft.com/en-us/visualstudio/releases/2022/compatibility)
    - [Desktop development with C++](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170)
        - [C++ CMake tools for Windows](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
### How to build
```powershell
cd Scripts
build.bat
```
