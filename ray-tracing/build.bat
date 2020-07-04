@echo off

rem `-g` flag to generate obj files, use to debug.
rem `-Wall` warning all.

set main_file=src/main.cpp
set compiler_flags=-Wall -g -O3
set output_file=-o bin/ray_traser.exe

g++ %main_file% %compiler_flags% %output_file%

echo Compilation Done
popd