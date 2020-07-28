@echo off
pushd build
@echo on

rem `height | samples | is_debug | num_of_threads`

if not exist "result" mkdir result

ray_tracer.exe 384 100 0 16 > result/image.ppm

@echo off
echo The ray_tracer.exe returned status code: %errorlevel%
popd
@echo on
