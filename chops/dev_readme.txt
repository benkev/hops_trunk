CHOPS:

This is a project to chop up the code in hops/fourfit into
logically separate libraries.

Eventually, all the various C source files should satisfy the following:

1) Be linked into shared libraries, with a tree-like dependency hierarchy (no cycles)
2) Have a corresponding header '.h' file (with #ifdef guards) for each '.c' file or logical collection of files.
3) Not require any global state/variables, all necessary data should be passed by pointer/value.
4) Duplicate files/functionality should be consolidated into single files/libraries.

Also,

5) Each library should get a collection of well defined unit tests, but
in the meantime every effort should be made to keep the existing regression tests working.

6) Where possible, external dependencies should be made optional. Namely, PGPLOT, and possibly
FFTW. This requires the calculation of various output quantities and the plotting routines to be separated.

7) This code base will necessarily diverge from the HOPS trunk, but it should track
the individual source files as closely as possible, at the moment this is done using 
the dumb 'import_xx.sh' scripts after defining the environmental variables pointing
to the (hops trunk) HOPS_SRC_DIR and (top level chops directory) CHOPS_SRC_DIR.
This should probably be made more automatic, but any automatic solution will
grow more difficult as the directory structure changes.

There are two build systems at the moment, cmake and automake.

To configure with defaults and compile with cmake, run:

$ cd ./chops
$ mkdir build
$ cd ./build 
$ cmake ../
$ make && make install

To change the default options or if a library is not automatically found, use the command line 
interface ccmake (cmake-curses-gui) in place of cmake to configure.


To compile with automake (important to use a --prefix flag during configure! as a default install
directory is not deduced). Also check, distcheck, and dist targets do not work yet, only make and make install.

$ cd ./chops
$ ./autogen.sh
$ install_dir="$PWD"/install
$ mkdir build
$ cd build
$ ../configure --prefix=$install_dir 
$ make && make install

After compiling (with either cmake or autotools) you can run a small set of regression tests with:

$ cd <install directory>/bin
$ source ./chopsenv.sh
$ ./hopstestsuite.py


TODO LIST:

- Fix fourfit default config file stuff, text directories...still needs work
- Clean up environmental variable names (check for collisions with hops)
- Improve import system from HOPS 
- Fourfit refactoring
- Import other applications/libs/utils: CorAsc2, fringex, etc. etc.
- vhelp, conversion to man pages?
