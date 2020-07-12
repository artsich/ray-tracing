@echo off
pushd bin
@echo on

rem `height | samples | is_debug | num_of_threads`

ray_tracer.exe 1920 100 0 16 > image.ppm

echo The ray_tracer.exe returned status code: %errorlevel%

@echo off
popd
@echo on
