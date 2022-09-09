@echo off

:: root MUST end with \
::
set root=C:\

echo About to install Cygwin to %root%cygwin
echo.
echo All required packages will be selected automatically.
echo Make sure to add your favorite editor when installing
echo (press the view button to search and add more packages)
echo.
pause

set setup_src=https://web.archive.org/web/20160820100148/http://cygwin.com/setup-x86.exe
set setup_dst=setup-x86-2.874.exe

:: dwnl is not available on MS Windows, and bitsadmin isn't pre-installed on
:: Windows 2003
dwnl %setup_src% %setup_dst% || (
    echo Unable to download %setup_src%
    echo Please download and save as %setup_dst% in the current working directory
    pause
)

%setup_dst% ^
    -X -O -s http://ctm.crouchingtigerhiddenfruitbat.org/pub/cygwin/circa/2016/08/30/104223/ ^
    -R %root%cygwin ^
    -l %root%cygcache ^
    -P gcc-g++,make,automake,autoconf,git,perl,gettext-devel,libiconv-devel,zlib-devel,cocom,mingw64-i686-gcc-g++,mingw64-i686-zlib,mingw64-x86_64-gcc-g++,mingw64-x86_64-zlib
