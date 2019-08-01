# Installing `SDL_bgi`

To install `SDL_bgi`, you normally compile it from sources; Windows
users may use the provided binaries.


## Installing the MSYS2 Mingw-w64 Binaries

Let's assume you installed the native package `mingw-w64-x86_64-SDL2`.

Start the MSYS2 shell, unzip the archive, then copy 
`src/graphics.h` to `/mingw64/include/`, `src/SDL_bgi.h`
to `/mingw64/include/SDL2/`, and `bin/Mingw64/SDL_bgi.dll`
to `/mingw64/bin`.

Please note that to run a program compiled with `SDL_bgi` outside
of the MSYS2 environment, you will need `SDL_bgi.dll` and `SDL2.dll`
in the same directory as the program.


## Installing the Code::Blocks Binaries

Let's assume that you installed CodeBlocks in `C:\CodeBlocks` and SDL2
in `C:\SDL2-2.0.9`. Change the following instructions as needed for
your installation.

Copy `src/graphics.h` to `C:\CodeBlocks\MinGW\include`,
`src/SDL_bgi.h` to `C:\SDL2-2.0.9\i686-w64-mingw32\include\SDL2`,
and `bin/CodeBlocks/SDL_bgi.dll` to `C:\CodeBlocks\MinGW\bin`.

To see how to compile programs, please see `howto_CodeBlocks.md`.


## Installing the Dev-C++ Binaries

Let's assume that you installed Dev-C++ `C:\DevCpp` and SDL2
in `C:\SDL2-2.0.9`. Change the following instructions as needed for
your installation.

Copy `src/graphics.h` to `C:\SDL2-2.0.9\i686-w64-mingw32\include`,
`src/SDL_bgi.h` to `C:\SDL2-2.0.9\i686-w64-mingw32\include\SDL2`,
and `bin/DevCpp/SDL_bgi.dll` to `C:\DevCpp\MinGW64\bin`.

To see how to compile programs, please see `howto_DevCpp.md`.


# Compiling from sources

The main `SDL_bgi` development platform is GNU/Linux, but Windows
(MSYS2 + Mingw-w64 toolchain, CodeBlocks, Dev-C++) and Mac OS X are
also supported.

Please make sure you have SDL2 development packages. On Debian, Mint,
Ubuntu and similar distributions, please install `libsdl2-dev` and its
dependencies; in Fedora, it's `SDL2-devel`. Obviously, you'll also
need a C compiler; `gcc` or `clang` are fine.

Mingw-w64 users should install `make`, `mingw-w64-x86_64-SDL2`, and
`mingw-w64-x86_64-gcc` (*not* plain `gcc`). Mac OS X users should
install the SDL2 library from the latest `.dmg` available.


## With `cmake`, GNU/Linux

To compile the SDL_bgi library, and make `.deb` or `.rpm` packages,
unpack the `.tar.gz` archive, move to the newly created directory, then
run:

    $ ./build.sh

which creates a subdirectory called `build/`, cd's to it, runs
`cmake`, then `cpack`. In a few seconds, in directory `build/` will
find a package that should be suitable for your system (`.deb` or
`.rpm`). You'll want to install that.

If you prefer the usual way:

    $ mkdir build && cd build
    $ cmake ..
    $ make && sudo make install 


## Without `cmake`

If you don't want to use `cmake`, then do:

    $ cd src
    $ make
    $ sudo make install

Files will be installed in appropriate directories, depending on the OS.

GNU/Linux:

`SDL_bgi.h`        -> `/usr/include/SDL2/`
`graphics.h`       -> `/usr/include/`
`libSDL_bgi.so`    -> `/usr/lib/`.

Mingw-w64:

`SDL_bgi_.h`       -> `/mingw64/include/SDL2/`
`graphics.h`       -> `/mingw64/include/`
`SDL_bgi.dll`      -> `/mingw64/bin/`

Mac OS X:

`SDL_bgi.h`        -> `/library/Frameworks/SDL2.framework/Headers`
`graphics.h`       -> `/usr/include/`
`libSDL_bgi.so`    -> `/usr/lib/`


## Test Programs

Once `SDL_bgi` is installed, you can compile some test programs you
can find in the `test/` directory. Type:

    $ cd test && make

Please see the `using.md` file in the `doc/` directory.

Enjoy!

Guido =8-)

