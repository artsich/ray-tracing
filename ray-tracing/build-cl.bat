@echo on

if not exist "./bin" mkdir bin
pushd bin

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64

set compiled_files=../src/main.cpp ../src/platform/win32/system/cuncurrency/thread.cpp

set included_dirs=/I ../src/
set compiler_options= %included_dirs% /favor:INTEL64 /O2 /Oi /nologo /EHa /fp:fast /Gm- /GR- /FC 
set linker_options=-incremental:no -opt:ref user32.lib gdi32.lib winmm.lib /out:ray_tracer.exe

cl %compiler_options% %compiled_files% /link %linker_options%

if errorlevel 1 (
   echo Failure Reason Given is %errorlevel%
   exit /b %errorlevel%
)

popd