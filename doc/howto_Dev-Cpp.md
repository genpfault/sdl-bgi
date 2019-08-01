How to compile SDL_bgi programs with Dev-C++
============================================

These instructions show how to compile an `SDL_bgi` program using
Dev-C++; we will use `fern.c`, provided in the `test/`
directory.

Let's assume that Dev-C++ is installed in `C:\Dev-Cpp`, and that the
SDL2 development libraries (MinGW) are installed in `C:\SDL2-2.0.10`.
If you installed the software in different directories, change the
following instructions accordingly.

The procedure was tested with Orwell Dev-Cpp 5.11, TDM-GCC 5.1.0-3,
and SDL2-devel-2.0.10-mingw.

Links:

<https://sourceforge.net/projects/orwelldevcpp/>

<http://tdm-gcc.tdragon.net/download>

<https://libsdl.org/download-2.0.php>


Upgrading Dev-C++'s Compiler
----------------------------

By default, Dev-C++ ships with TDM-GCC 4.9.2; this is an old version
that is not compatible with current SDL2 development libraries.
To compile `SDL_bgi` programs, you must upgrade TDM-GCC to the
latest release.

- remove the directory `C:\Dev-Cpp\MinGW64`, or rename it as\
  `C:\Dev-Cpp\MinGW64_old`

- install TDM-GCC (32 bit) in `C:\Dev-Cpp\MinGW64`

- start Dev-C++. A window will appear, informing that a suitable
  compiler was not found. Click on `No`

- from menu `Tools/Compiler Options.../Compiler set to configure`, 
  click on the `++` icon (Find and automatically configure compilers)

- when asked to `Clear current compiler list?`, click on `Yes`

- select "TDM-GCC 5.1.0 64-bit release" as the "Compiler set to
  configure"

- restart Dev-C++. A window will appear, informing that `gdb` was not
  found. From menu `Tools/Compiler Options...` select the tab
  `Programs` and replace `gdb.exe` with `gdb32.exe`

- test Dev-C++ and make sure that you can compile programs.


Compiling `SDL_bgi` Programs
----------------------------

- copy `SDL_bgi.dll` to `C:\Dev-Cpp\MinGW64\lib`

- copy `SDL_bgi.h` to\
  `C:\SDL2-2.0.10\i686-w64-mingw32\include\SDL2`

- copy `graphics.h` to `C:\SDL2-2.0.10\i686-w64-mingw32\include`

- Run Dev-C++ and start an empty project called `Fern`: menu
  `File/New/Project...`, tick "C Project", select "Empty Project",
  insert the name "Fern", click on Ok, save the project file

- in the left-hand pane, click on tab `Project`, remove `Untitled`,
  right click on `Fern`, click on `Add to project...`, pick `fern.c`

- menu `Project/Project options...`, tab `Parameters`, tab `Linker`:
  add the string `-lmingw32 -lSDL_bgi -lSDL2main -lSDL2 -mwindows`

- tab `Directories`, tab `Include Directories`: add\
  `C:\SDL2-2.0.10\i686-w64-mingw32\include\SDL2` and\
  `C:\SDL2-2.0.10\i686-w64-mingw32\include`

- tab `Library Directories`: add `C:\SDL2-2.0.10\i686-w64-mingw32\lib`
  then click on Ok

- compile the program: menu `Execute/Compile`

- copy `SDL2.dll` and `SDL_bgi.dll` to the same directory as `fern.exe`

- run it: menu `Execute/Run`.

**Note 1**: if you release your compiled program, you must
also provide `SDL2.dll` and `SDL_bgi.dll` in the same directory.

**Note 2**: if your code uses functions provided by `stdio.h` (e.g.
`printf()`, `scanf()`, etc.) you must also run a terminal. Remove the
option `-mwindows` from the linker parameters.
