# Wii-Tac-Toe

[![Actions Status](https://github.com/Crayon2000/Wii-Tac-Toe/actions/workflows/ci.yml/badge.svg)](https://github.com/Crayon2000/Wii-Tac-Toe/actions)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/ab38c1f3adee4d629c72d285efee6a73)](https://app.codacy.com/gh/Crayon2000/Wii-Tac-Toe/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

Wii-Tac-Toe is a Tic-Tac-Toe game for the Nintendo Wii, programmed in C++ using devkitPro and GRRLIB.

This game features both solo play against the CPU and a two-player mode, either by sharing one Wii Remote or using two. The game’s AI varies its moves, ensuring that games feel unique and challenging.

To win, align three of your marks (O or X) in a horizontal, vertical, or diagonal row. If all spaces are filled without a winner, it’s a tie. Start a new game and “Wii”-ignite the fun!

![http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png](http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png)

## Build

Prerequisites:

* devkitPro
  * devkitPPC (release 45 or later)
  * libogc
  * libfat
* Port libs
  * Mini-XML
  * libpng
  * FreeType

To compile:

```text
pacman --sync --needed --noconfirm ppc-mxml ppc-libpng ppc-freetype
mkdir --parents build && cd build
/opt/devkitpro/portlibs/wii/bin/powerpc-eabi-cmake ..
cmake --build .
```
