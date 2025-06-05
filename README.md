## Make vs automake vs CMake vs IMake

## automake
* `/Volumes/git/github/2024/docker_images/bison_helloworld_automake/`
* for legacy gnu tools (CMake is more modern)
* Good to understand how all those projects are built

```
+--------------+     +------------+     +-------------+     +-----------+     +----------+     +------+     +--------------+     +----------+
| Makefile.am  | --> |  automake  | --> | Makefile.in | --> | configure | --> | Makefile | --> | make | --> | object files | --> | binaries |
+--------------+     +------------+     +-------------+     +-----------+     +----------+     +------+     +--------------+     +----------+
                                                              ^
                                                              |
                                                              |
+--------------+     +------------+                           |
| configure.ac | --> |  autoconf  | --------------------------+
+--------------+     +------------+
  |
  |
  v
+--------------+     +------------+
|   aclocal    | --> | aclocal.m4 |
+--------------+     +------------+

```

## CMake
* see /Volumes/git/github/2024/docker_images/bison_helloworld_cmake/src
* A build system generator, not a build system itself.
* Generates Makefiles, Ninja, or Visual Studio project files.
* Uses CMakeLists.txt files.
* Integrates with IDEs, compilers, and cross-platform projects.
* Cross-platform and widely adopted (especially in C++ and open source).
* Easier management of large codebases.


## IMake - obsolete
* Use cmake
* was used by x11
* I think objectivity used this for Cross Platform
