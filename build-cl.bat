@echo off

set output_folder=build

if not exist %output_folder% mkdir %output_folder%
pushd %output_folder%

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64

set compiled_files=../ray-tracing/src/main.cpp ../ray-tracing/src/platform/win32/system/cuncurrency/thread.cpp

set included_dirs=/I ../ray-tracing/src/
set warning_options=/W4 /wd4100 /wd4201
set compiler_options= %included_dirs% /Zi /favor:INTEL64 /O2 /Oi /nologo /EHa /fp:fast /Gm- /GR- /FC %warning_options%
set linker_options=-incremental:no -opt:ref user32.lib winmm.lib /out:ray_tracer.exe

cl %compiler_options% %compiled_files% /link %linker_options%

if errorlevel 1 (
   echo Failure Reason Given is %errorlevel%
   exit /b %errorlevel%
)

popd