@echo off
pushd %~dp0\..\
call 3rdParty\premake5.exe vs2022
popd
PAUSE