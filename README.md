# Wii-Tac-Toe

[![Actions Status](https://github.com/Crayon2000/Wii-Tac-Toe/actions/workflows/ci.yml/badge.svg)](https://github.com/Crayon2000/Wii-Tac-Toe/actions)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ab38c1f3adee4d629c72d285efee6a73)](https://www.codacy.com/app/Crayon2000/Wii-Tac-Toe?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Crayon2000/Wii-Tac-Toe&amp;utm_campaign=Badge_Grade)

Wii-Tac-Toe is a Tic-Tac-Toe game for the Nintendo Wii. It was programmed in C++ using devkitPro along with GRRLIB.

The game can be played alone vs the CPU or with a friend using one Wii Remote controller. The game AI does not always do the same moves, so the fun never stop.

To win the game, you have to place three marks (O or X) in a horizontal, vertical or diagonal row. If the grid is filled completely and no one has won, well, it's a tie game. Start over and try to beat your opponent.

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
  * libjpeg-turbo

To compile:

```text
pacman --sync --needed --noconfirm ppc-mxml ppc-libpng ppc-freetype ppc-libjpeg-turbo
mkdir --parents build && cd build
/opt/devkitpro/portlibs/wii/bin/powerpc-eabi-cmake ..
cmake --build .
```
