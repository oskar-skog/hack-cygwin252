## Hacked Cygwin 2.5.2

This is a version of newlib-cygwin with (yet to be added) debug logging
and maybe some hacks to improve a few things (like fork).


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
