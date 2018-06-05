README
------

This is `SDL_bgi`, a graphics library based on SDL2. `SDL_bgi` provides
graphics primitives, and is much easier to use than pure SDL2. It
should be especially useful for beginners, i.e. in high school
programming courses.

`SDL_bgi` is nearly fully compatible with BGI (`graphics.h`), the
Borland Graphics Interface that was the de facto standard in PC
graphics back in DOS days. For instance, it compiles and runs the
original `bgidemo.c` almost perfectly. `SDL_bgi` is pretty fast,
very easy to use, and it also provides extensions for ARGB colours and
mouse support. It should run on any platform supported by SDL2; it was
tested in GNU/Linux, Windows (MSYS2 + Mingw-w64, CodeBlocks, Dev-C++),
and Mac OS X Yosemite.

I wrote `SDL_bgi` because I wanted a simple but fast graphics library
for my experiments with fractals and cellular automata, using the BGI
syntax I'm used to. Obviously, native SDL2 functions can be used in
`SDL_bgi` programs.

A few examples are provided in the `test/` directory.

If you don't need BGI compatibility, I suggest that you check out
SDL_gfx, a graphics library more complete and more powerful than
`SDL_bgi`.

Links:

0. The SDL library:
   <https://www.libsdl.org/>

1. WinBGIm, another BGI Windows port:
   <http://www.cs.colorado.edu/~main/cs1300/doc/bgi/>

2. SDL_gfx, graphics drawing primitives and more:
   <http://cms.ferzkopp.net/index.php/software/13-sdl-gfx>

3. BGI on Wikipedia:
   <https://en.wikipedia.org/wiki/Borland_Graphics_Interface>

4. Xbgi, a BGI XLib port:
   <http://libxbgi.sourceforge.net/>

5. Borland Turbo C 2.01 online emulator:
   <https://archive.org/details/msdos_borland_turbo_c_2.01>

This library is licensed under the Zlib license; please see the
enclosed file LICENSE. The 8x8 font comes from A. Schiffler's SDL_gfx.

Brought to you by Guido Gonzato, PhD <guido.gonzato at gmail.com>
