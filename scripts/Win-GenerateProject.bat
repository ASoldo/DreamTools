@echo off
pushd ..\
call vendor\bin\premake5\premake5.exe vs2019
popd 
PAUSE