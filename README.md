# Wii-Tac-Toe

[![Actions Status](https://github.com/Crayon2000/Wii-Tac-Toe/actions/workflows/ci.yml/badge.svg)](https://github.com/Crayon2000/Wii-Tac-Toe/actions)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/ab38c1f3adee4d629c72d285efee6a73)](https://app.codacy.com/gh/Crayon2000/Wii-Tac-Toe/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

Wii-Tac-Toe is a Tic-Tac-Toe game for the Nintendo Wii. It was programmed in C++ using devkitPro along with GRRLIB.

The game can be played alone vs the CPU or with a friend using one Wii Remote controller. The game AI does not always do the same moves, so the fun never stop.

To win the game, you have to place three marks (O or X) in a horizontal, vertical or diagonal row. If the grid is filled completely and no one has won, well, it's a tie game. Start over and try to beat your opponent.

![http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png](http://wiibrew.org/w/images/e/e7/Wii-Tac-Toe-screenshot.png)

## Build

Prerequisites:

* devkitPro
  * devkitPPC (release 46 or later)
  * libogc
  * libfat
* Port libs
  * Mini-XML
  * libpng
  * FreeType

To compile:

```text
pacman --sync --needed --noconfirm ppc-mxml ppc-libpng ppc-freetype
cmake --workflow wii
```
