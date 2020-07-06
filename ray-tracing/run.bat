@echo off
pushd bin
@echo on

rem `height | samples | is_debug | num_of_threads`

ray_tracer.exe 384 100 0 16 > image.ppm

@echo off
popd
@echo on