## Hacked Cygwin 2.5.2

This is a version of newlib-cygwin with (yet to be added) debug logging to
figure out why things are broken on ReactOS, and maybe some hacks to improve
a few things (like a less unreliable fork).

This is based on the tag `cygwin-2_5_2-release` from
git://cygwin.com/git/newlib-cygwin.git


## Build environent

Cygwin 2.5.2 on Windows Server 2003, may also work on XP but has not been
tested.

You need to install the following packages:
- gcc-g++
- make
- automake
- autoconf
- git
- perl
- gettext-devel
- libiconv-devel
- zlib-devel
- cocom
- mingw64-i686-gcc-g++
- mingw64-i686-zlib
- mingw64-x86_64-gcc-g++
- mingw64-x86_64-zlib


The list of build dependencies above and build instructions below are based
on [current build instructions](https://cygwin.com/faq/faq.html#faq.programming.building-cygwin).


## Build instructions

```
git clone git@github.com:oskar-skog/hack-cygwin252.git
cd hack-cygwin252

mkdir build
mkdir installroot

cd winsup
./autogen.sh        # Will appear to have some errors

cd ../installroot
prefix=$(pwd)

cd ../build
../configure "--prefix=$prefix"
make
make install
```

`cygwin1.dll` will be created in `install/bin`.
To install it you need to end all Cygwin applications and copy it to
`C:\cygwin\bin` (or wherever your Cygwin /bin happens to be).


## Hacking

```C++
#include "hack.h"
void hack_print(const char * format, ...);
void hack_utf16_to_utf8(char * dest, size_t bufsize, LPWSTR src);
```

`winsup/cygwin` contains all the things of interest.

`winsup/cygwin/hack.h` header for configuring stuff, make sure to `touch` all
files that include it as needed to ensure they actually get compiled if you
change some defines in it.

`dll_crt0_1` in `winsup/cygwin/dcrt0.cc` calls `main` of the program,
`hack_init` is called immediately before.  `do_exit` in `dcrt0.cc`
calls `hack_end`.
