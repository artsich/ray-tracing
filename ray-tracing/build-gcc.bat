@echo off

rem `-g` flag to generate obj files, use to debug.
rem `-Wall` warning all.

set compiled_files=src/main.cpp src/platform/win32/system/cuncurrency/thread.cpp
set output_file=-o bin/ray_tracer.exe

set included_dirs=src/

set precompiler_option=-I %included_dirs%
set linker_option=-std=c++17
set compiler_flags=-Wall -g -O3 -fpermissive

g++ %precompiler_option% %linker_option% %compiled_files% %compiler_flags% %output_file%

if errorlevel 1 (
   echo Failure Reason Given is %errorlevel%
   exit /b %errorlevel%
)

echo Compilation Done
popd